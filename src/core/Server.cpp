#include "Server.h"

Server::Server(int port) : port(port), running(false), server_socket(-1), file_handler("./public") {
    std::cout << "[Server] Initializing server on port " << port << std::endl;

    // Initialize thread pool with hardware concurrency size;
    size_t thread_count = std::thread::hardware_concurrency();
    if(thread_count == 0 ) thread_count = 4;

    thread_pool = std::make_unique<ThreadPool>(thread_count);
    std::cout << "[Server] Thread pool initialized with " << thread_count << " threads" << std::endl;
}

Server::~Server() {
    stop();
}

void Server::setupSocket() {
    /* Create Socket with type SOCK_STREAM*/
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("socket() failed");
        throw std::runtime_error("Failed to create socket: " + std::string(strerror(errno)));
    }
    
    std::cout << "[Server] Socket created successfully (fd=" << server_socket << ")" << std::endl;
    
    /* Set REUSEADDR option to avoid "Address aldready in use"*/
    int opt = 1;
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt() failed");
        close(server_socket);
        throw std::runtime_error("Failed to set socket options: " + std::string(strerror(errno)));
    }
    
    std::cout << "[Server] Socket options set successfully" << std::endl;
}

void Server::bindSocket() {
    /* Init and set parameters */
    memset(&server_addr, 0, sizeof(server_addr));  
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;  // Accept connections from any IP
    server_addr.sin_port = htons(port);        // Convert to network byte order
    
    std::cout << "[Server] Attempting to bind to port " << port << std::endl;
    
    /* Bind socket to address */ 
    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind() failed");
        close(server_socket);
        throw std::runtime_error("Failed to bind socket to port " + std::to_string(port) + 
                                ": " + std::string(strerror(errno)));
    }
    
    std::cout << "[Server] Socket bound to port " << port << " successfully" << std::endl;
}

void Server::startListening() {
    /* Start listening for connections (backlog = 10) */
    if (listen(server_socket, 10) < 0) {
        close(server_socket);
        throw std::runtime_error("Failed to listen on socket");
    }
    
    std::cout << "[Server] Listening for connections..." << std::endl;
}

void Server::start() {
    try {
        setupSocket();
        bindSocket();
        startListening();
        
        running = true;
        std::cout << "[Server] Server started successfully on http://localhost:" << port << std::endl;
        
        /* Main loop at Server side */
        while (running) {
            struct sockaddr_in client_addr;
            socklen_t client_len = sizeof(client_addr);
            
            // Accept incoming connection
            int client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_len);
            
            if (client_socket < 0) {
                if (running) {  // Only log error if server is still supposed to be running
                    std::cerr << "[Server] Failed to accept client connection" << std::endl;
                }
                continue;
            }
            
            /* Get client IP */
            char client_ip[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, INET_ADDRSTRLEN);
            std::cout << "[Server] New connection from " << client_ip << std::endl;
            
            // Handle client request using thread pool (NON BLOCKING)
            thread_pool->enqueue([this, client_socket]() {
                try {
                    handleClient(client_socket);
                } catch (const std::exception& e) {
                    std::cerr << "[Server] Error handling client: " << e.what() << std::endl;
                    close(client_socket);  // Ensure client socket is closed on error
                }
            });
        }
        
    } catch (const std::exception& e) {
        std::cerr << "[Server] Error: " << e.what() << std::endl;
        stop();
    }
}

void Server::handleClient(int client_socket) {
    char buffer[4096] = {0};
    
    // Read request from client
    ssize_t bytes_read = read(client_socket, buffer, sizeof(buffer) - 1);
    
    if (bytes_read > 0) {
        std::string raw_request(buffer);
        std::cout << "[Server] Received request (" << bytes_read << " bytes)" << std::endl;
        
        // Parse HTTP request
        HttpRequest request = HttpParser::parse(raw_request);
        
        if (request.isValid()) {
            std::cout << "[Server] Valid HTTP request parsed:" << std::endl;
            std::cout << request.toString() << std::endl;
            
            try {
                // Route request and generate response
                std::string response = routeRequest(request);
                
                // Send response to client
                send(client_socket, response.c_str(), response.length(), 0);
                std::cout << "[Server] Response sent successfully" << std::endl;
                
            } catch (const std::exception& e) {
                std::cerr << "[Server] Error processing request: " << e.what() << std::endl;
                
                // Send 500 Internal Server Error
                std::string error_response = ResponseGenerator::create500Response();
                send(client_socket, error_response.c_str(), error_response.length(), 0);
            }
            
        } else {
            std::cout << "[Server] Invalid HTTP request received" << std::endl;
            
            // Send 400 Bad Request
            std::string error_response = ResponseGenerator::create400Response();
            send(client_socket, error_response.c_str(), error_response.length(), 0);
        }
    }
    else {
        std::cout << "[Server] No data received from client" << std::endl;
    }

    // Close client connection
    close(client_socket);
    std::cout << "[Server] Client connection closed" << std::endl;
}

std::string Server::routeRequest(const HttpRequest& request)
{
    std::string path = request.getPath();
    std::cout <<"[Server] Routing request to path: " << path << std::endl;  

    // Try to serve static files for everything else
    if (file_handler.canServeFile(path)) {
        std::cout << "[Server] Serving static file: " << path << std::endl;
        return file_handler.serveFile(path);
    }

    if (path == "/about") {
        std::cout << "[Server] Serving about page" << std::endl;
        return ResponseGenerator::createAboutPageResponse(); 
    } 
    else if (path == "/status") {
        std::cout << "[Server] Serving status page" << std::endl;
        return ResponseGenerator::createStatusPageResponse(port);
    }

    

    std::cout << "[Server] Path not found: " << path << std::endl;
    return ResponseGenerator::create404Response();
}

void Server::stop() {
    if (running) {
        running = false;
        std::cout << "[Server] Stopping server..." << std::endl;
        
        // Show thread pool statistics
        if (thread_pool) {
            thread_pool->printStatus();
        }
    }
    
    if (server_socket != -1) {
        close(server_socket);
        server_socket = -1;
        std::cout << "[Server] Socket closed" << std::endl;
    }
    
    // Shutdown thread pool
    if (thread_pool) {
        std::cout << "[Server] Shutting down thread pool..." << std::endl;
        thread_pool->shutdown();
        std::cout << "[Server] Thread pool shut down complete" << std::endl;
    }
}