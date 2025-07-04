#include "Server.h"

Server::Server(int port) : port(port), running(false), server_socket(-1), file_handler("./public") {
    std::cout << "[Server] Initializing server on port " << port << std::endl;
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
            
            /* Handle client request synchronous */
            handleClient(client_socket);
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
            
            // Route request based on path
            std::string response = routeRequest(request);
            
            // Send response to client
            send(client_socket, response.c_str(), response.length(), 0);
            std::cout << "[Server] Response sent to client" << std::endl;
            
        } else {
            std::cout << "[Server] Invalid HTTP request received" << std::endl;
            
            // Send 400 Bad Request
            std::string error_response = createErrorResponse(400, "Bad Request");
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

    if (file_handler.canServeFile(path)) {
        std::cout << "[Server] Serving file for path: " << path << std::endl;
        return file_handler.serveFile(path);
    }
    
    // Fallback to hardcoded routes for special pages
    if (path == "/about") {
        return createAboutPageResponse(); 
    } else if (path == "/") {
        return createHomePageResponse();
    } else {
        return createErrorResponse(404, "Not Found");
    }
}

std::string Server::createHomePageResponse() 
{
    std::string html_body = R"(
<!DOCTYPE html>
<html>
<head>
    <title>Home - Multithreaded Web Server</title>
    <style>
        body { font-family: Arial, sans-serif; margin: 40px; background: #f4f4f4; }
        .container { background: white; padding: 30px; border-radius: 8px; box-shadow: 0 2px 10px rgba(0,0,0,0.1); }
        h1 { color: #333; border-bottom: 2px solid #4CAF50; padding-bottom: 10px; }
        .info { background: #e8f5e8; padding: 15px; border-radius: 4px; margin: 20px 0; }
        .nav { margin: 20px 0; }
        .nav a { color: #4CAF50; text-decoration: none; margin-right: 20px; }
    </style>
</head>
<body>
    <div class="container">
        <h1> Home Page</h1>
        <div class="nav">
            <a href="/">Home</a>
            <a href="/about">About</a>
        </div>
        <div class="info">
            <h3> HTTP Parser Working!</h3>
            <p>Request successfully parsed and routed!</p>
            <p><strong>Port:</strong> )" + std::to_string(port) + R"(</p>
        </div>
    </div>
</body>
</html>)";
    
    return createSimpleResponse(html_body);
}

std::string Server::createAboutPageResponse() {
    std::string html_body = R"(
<!DOCTYPE html>
<html>
<head>
    <title>About - Multithreaded Web Server</title>
    <style>
        body { font-family: Arial, sans-serif; margin: 40px; background: #f4f4f4; }
        .container { background: white; padding: 30px; border-radius: 8px; box-shadow: 0 2px 10px rgba(0,0,0,0.1); }
        h1 { color: #333; border-bottom: 2px solid #2196F3; padding-bottom: 10px; }
        .tech { background: #f0f8ff; padding: 15px; border-radius: 4px; margin: 20px 0; }
        .nav { margin: 20px 0; }
        .nav a { color: #2196F3; text-decoration: none; margin-right: 20px; }
    </style>
</head>
<body>
    <div class="container">
        <h1> About Page</h1>
        <div class="nav">
            <a href="/">Home</a>
            <a href="/about">About</a>
        </div>
        <div class="tech">
            <h3> Technical Features</h3>
            <ul>
                <li> TCP Socket Programming</li>
                <li> HTTP Request Parsing</li>
                <li> URL Routing</li>
                <li> Static File Serving (Next)</li>
                <li> Multithreading (Coming)</li>
            </ul>
        </div>
    </div>
</body>
</html>)";
    
    return createSimpleResponse(html_body);
}

std::string Server::createErrorResponse(int status_code, const std::string& message) {
    std::string status_line;
    std::string html_body;
    
    switch (status_code) {
        case 404:
            status_line = "HTTP/1.1 404 Not Found\r\n";
            html_body = R"(
<!DOCTYPE html>
<html>
<head><title>404 Not Found</title></head>
<body>
    <h1>404 - Page Not Found</h1>
    <p>The requested page could not be found.</p>
    <a href="/">Back to Home</a>
</body>
</html>)";
            break;
        case 400:
            status_line = "HTTP/1.1 400 Bad Request\r\n";
            html_body = R"(
<!DOCTYPE html>
<html>
<head><title>400 Bad Request</title></head>
<body>
    <h1>400 - Bad Request</h1>
    <p>The request could not be understood.</p>
</body>
</html>)";
            break;
        default:
            status_line = "HTTP/1.1 500 Internal Server Error\r\n";
            html_body = "<h1>500 - Internal Server Error</h1>";
    }
    
    std::string response = status_line;
    response += "Content-Type: text/html\r\n";
    response += "Content-Length: " + std::to_string(html_body.length()) + "\r\n";
    response += "Server: CustomHTTPServer/1.0\r\n";
    response += "Connection: close\r\n";
    response += "\r\n";
    response += html_body;
    
    return response;
}


std::string Server::createSimpleResponse(const std::string& body) {
    std::string response;
    
    response += "HTTP/1.1 200 OK\r\n";
    
    /* Header */
    response += "Content-Type: text/html\r\n";
    response += "Content-Length: " + std::to_string(body.length()) + "\r\n";
    response += "Server: CustomHTTPServer/1.0\r\n";
    response += "Connection: close\r\n";
    response += "\r\n";      
    
    /* Body */ 
    response += body;
    
    return response;
}

void Server::stop() {
    if (running) {
        running = false;
        std::cout << "[Server] Stopping server..." << std::endl;
    }
    
    if (server_socket != -1) {
        close(server_socket);
        server_socket = -1;
        std::cout << "[Server] Socket closed" << std::endl;
    }
}