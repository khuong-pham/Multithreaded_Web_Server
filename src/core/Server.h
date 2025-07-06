#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <string>
#include <cstring>
#include <cerrno>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdexcept>
#include "HttpRequest.h"
#include "HttpParser.h"
#include "FileHandler.h"
#include "ResponseGenerator.h"
#include "ThreadPool.h"

class Server {
private:
    int server_socket;
    int port;
    struct sockaddr_in server_addr;
    bool running;
    FileHandler file_handler;  // Add file handler
    std::unique_ptr<ThreadPool> thread_pool;  // Thread pool for handling requests

    
    // Helper methods
    void setupSocket();
    void bindSocket();
    void startListening();
    void handleClient(int client_socket);
    std::string routeRequest(const HttpRequest& request);

public:
    /*Constructor & Destructor*/
    Server(int port = 8080);
    ~Server();
    
    void start();
    void stop();
    int getPort() const { return port; }
    bool isRunning() const { return running; }
};

#endif // SERVER_H