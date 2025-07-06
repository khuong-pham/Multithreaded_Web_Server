#include "ResponseGenerator.h"

std::string ResponseGenerator::createHttpResponse(const std::string& body, 
                                                const std::string& content_type,
                                                int status_code,
                                                const std::string& status_text) {
    std::string response;
    response += "HTTP/1.1 " + std::to_string(status_code) + " " + status_text + "\r\n";
    response += "Content-Type: " + content_type + "\r\n";
    response += "Content-Length: " + std::to_string(body.length()) + "\r\n";
    response += "Server: CustomHTTPServer/1.0\r\n";
    response += "Connection: close\r\n";
    response += "\r\n";
    response += body;
    return response;
}

std::string ResponseGenerator::createHomePageResponse(int port) {
    std::string html_body = R"(<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Home - Multithreaded Web Server</title>
    <style>
        body { 
            font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, Oxygen, Ubuntu, Cantarell, sans-serif;
            margin: 40px; 
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            color: #333;
            line-height: 1.6;
        }
        .container { 
            background: white; 
            padding: 30px; 
            border-radius: 12px; 
            box-shadow: 0 10px 30px rgba(0,0,0,0.1); 
            max-width: 800px;
            margin: 0 auto;
        }
        h1 { 
            color: #333; 
            border-bottom: 3px solid #4CAF50; 
            padding-bottom: 15px;
            font-size: 2.2em;
            margin-bottom: 25px;
        }
        .info { 
            background: linear-gradient(135deg, #e8f5e8, #d4edda);
            padding: 20px; 
            border-radius: 8px; 
            margin: 25px 0;
            border-left: 5px solid #4CAF50;
        }
        .nav { 
            margin: 25px 0;
            text-align: center;
        }
        .nav a { 
            color: #4CAF50; 
            text-decoration: none; 
            margin: 0 20px;
            padding: 10px 20px;
            border-radius: 25px;
            transition: all 0.3s ease;
            border: 2px solid transparent;
        }
        .nav a:hover {
            background: #4CAF50;
            color: white;
            border-color: #4CAF50;
        }
        .threading { 
            background: linear-gradient(135deg, #e3f2fd, #bbdefb);
            padding: 20px; 
            border-radius: 8px; 
            border-left: 5px solid #2196F3;
            margin: 25px 0;
        }
        .threading h3 {
            color: #1976D2;
            margin-bottom: 15px;
        }
        .threading ul {
            margin: 10px 0;
            padding-left: 20px;
        }
        .threading li {
            margin: 8px 0;
        }
        .status {
            display: inline-block;
            background: #28a745;
            color: white;
            padding: 5px 12px;
            border-radius: 15px;
            font-size: 0.9em;
            font-weight: bold;
        }
    </style>
</head>
<body>
    <div class="container">
        <h1>Home Page - Multithreaded Server</h1>
        <div class="nav">
            <a href="/">Home</a>
            <a href="/about">About</a>
            <a href="/status">Status</a>
        </div>
        <div class="info">
            <h3><span class="status">ACTIVE</span> Multithreading Enabled!</h3>
            <p>Server now handles concurrent requests using ThreadPool architecture</p>
            <p><strong>Port:</strong> )" + std::to_string(port) + R"(</p>
            <p><strong>Status:</strong> Online and Processing Requests</p>
        </div>
        <div class="threading">
            <h3>Threading Features</h3>
            <ul>
                <li><strong>Worker Thread Pool:</strong> Concurrent request processing</li>
                <li><strong>Non-blocking I/O:</strong> Immediate response handling</li>
                <li><strong>Thread Safety:</strong> Mutex-protected operations</li>
                <li><strong>Load Balancing:</strong> Automatic task distribution</li>
                <li><strong>Scalable Design:</strong> Ready for high-traffic loads</li>
            </ul>
        </div>
    </div>
</body>
</html>)";
    
    return createHttpResponse(html_body);
}

std::string ResponseGenerator::createAboutPageResponse() {
    std::string html_body = R"(<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>About - Multithreaded Web Server</title>
    <style>
        body { 
            font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, Oxygen, Ubuntu, Cantarell, sans-serif;
            margin: 40px; 
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            color: #333;
            line-height: 1.6;
        }
        .container { 
            background: white; 
            padding: 30px; 
            border-radius: 12px; 
            box-shadow: 0 10px 30px rgba(0,0,0,0.1); 
            max-width: 800px;
            margin: 0 auto;
        }
        h1 { 
            color: #333; 
            border-bottom: 3px solid #2196F3; 
            padding-bottom: 15px;
            font-size: 2.2em;
            margin-bottom: 25px;
        }
        .tech { 
            background: linear-gradient(135deg, #f0f8ff, #e3f2fd);
            padding: 20px; 
            border-radius: 8px; 
            margin: 25px 0;
            border-left: 5px solid #2196F3;
        }
        .nav { 
            margin: 25px 0;
            text-align: center;
        }
        .nav a { 
            color: #2196F3; 
            text-decoration: none; 
            margin: 0 20px;
            padding: 10px 20px;
            border-radius: 25px;
            transition: all 0.3s ease;
            border: 2px solid transparent;
        }
        .nav a:hover {
            background: #2196F3;
            color: white;
            border-color: #2196F3;
        }
        .feature { 
            background: white;
            padding: 15px; 
            margin: 15px 0; 
            border-radius: 8px;
            border-left: 4px solid #FF9800;
            box-shadow: 0 2px 5px rgba(0,0,0,0.1);
        }
        .feature.completed {
            border-left-color: #4CAF50;
        }
        .feature.planned {
            border-left-color: #FFC107;
        }
        .status {
            display: inline-block;
            padding: 3px 8px;
            border-radius: 12px;
            font-size: 0.8em;
            font-weight: bold;
            margin-left: 10px;
        }
        .status.done {
            background: #4CAF50;
            color: white;
        }
        .status.next {
            background: #FFC107;
            color: #333;
        }
    </style>
</head>
<body>
    <div class="container">
        <h1>About - C++ Web Server</h1>
        <div class="nav">
            <a href="/">Home</a>
            <a href="/about">About</a>
            <a href="/status">Status</a>
        </div>
        <div class="tech">
            <h3>Technical Implementation</h3>
            <div class="feature completed">
                <strong>HTTP Parser:</strong> Request parsing and validation
                <span class="status done">DONE</span>
            </div>
            <div class="feature completed">
                <strong>Static Files:</strong> HTML, CSS, JS, images serving
                <span class="status done">DONE</span>
            </div>
            <div class="feature completed">
                <strong>Thread Pool:</strong> Concurrent request processing
                <span class="status done">DONE</span>
            </div>
            <div class="feature completed">
                <strong>MIME Types:</strong> Automatic content-type detection
                <span class="status done">DONE</span>
            </div>
            <div class="feature completed">
                <strong>Error Handling:</strong> Comprehensive HTTP status codes
                <span class="status done">DONE</span>
            </div>
            <div class="feature planned">
                <strong>Keep-Alive:</strong> Connection reuse
                <span class="status next">PLANNED</span>
            </div>
            <div class="feature planned">
                <strong>Caching:</strong> Response caching
                <span class="status next">PLANNED</span>
            </div>
        </div>
        
        <div class="tech">
            <h3>Architecture Details</h3>
            <p><strong>Language:</strong> C++17 with modern features</p>
            <p><strong>Build System:</strong> CMake cross-platform</p>
            <p><strong>Threading:</strong> std::thread with custom ThreadPool</p>
            <p><strong>Networking:</strong> POSIX sockets (low-level)</p>
            <p><strong>Design Patterns:</strong> Factory, Singleton, Observer, Producer-Consumer</p>
            <p><strong>Performance:</strong> Multi-threaded concurrent processing</p>
        </div>
    </div>
</body>
</html>)";
    
    return createHttpResponse(html_body);
}

std::string ResponseGenerator::createStatusPageResponse(int port) {
    std::string html_body = R"(<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Server Status</title>
    <style>
        body { 
            font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, Oxygen, Ubuntu, Cantarell, sans-serif;
            margin: 40px; 
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            color: #333;
            line-height: 1.6;
        }
        .container { 
            background: white; 
            padding: 30px; 
            border-radius: 12px; 
            box-shadow: 0 10px 30px rgba(0,0,0,0.1); 
            max-width: 800px;
            margin: 0 auto;
        }
        h1 { 
            color: #333; 
            border-bottom: 3px solid #FF9800; 
            padding-bottom: 15px;
            font-size: 2.2em;
            margin-bottom: 25px;
        }
        .status { 
            background: linear-gradient(135deg, #e8f5e8, #d4edda);
            padding: 20px; 
            border-radius: 8px; 
            margin: 25px 0;
            border-left: 5px solid #4CAF50;
        }
        .nav { 
            margin: 25px 0;
            text-align: center;
        }
        .nav a { 
            color: #FF9800; 
            text-decoration: none; 
            margin: 0 20px;
            padding: 10px 20px;
            border-radius: 25px;
            transition: all 0.3s ease;
            border: 2px solid transparent;
        }
        .nav a:hover {
            background: #FF9800;
            color: white;
            border-color: #FF9800;
        }
        .metric { 
            display: flex; 
            justify-content: space-between; 
            padding: 12px 0; 
            border-bottom: 2px solid #f0f0f0;
            font-size: 1.1em;
        }
        .metric:last-child {
            border-bottom: none;
        }
        .metric-label {
            font-weight: 600;
            color: #555;
        }
        .metric-value {
            color: #2196F3;
            font-weight: bold;
        }
        .online { 
            color: #4CAF50; 
            font-weight: bold;
            font-size: 1.3em;
        }
        .status-indicator {
            display: inline-block;
            width: 12px;
            height: 12px;
            background: #4CAF50;
            border-radius: 50%;
            margin-right: 8px;
            animation: pulse 2s infinite;
        }
        @keyframes pulse {
            0% { opacity: 1; }
            50% { opacity: 0.5; }
            100% { opacity: 1; }
        }
    </style>
</head>
<body>
    <div class="container">
        <h1>Server Status Dashboard</h1>
        <div class="nav">
            <a href="/">Home</a>
            <a href="/about">About</a>
            <a href="/status">Status</a>
        </div>
        <div class="status">
            <h3 class="online">
                <span class="status-indicator"></span>Server Online
            </h3>
            <div class="metric">
                <span class="metric-label">Port:</span>
                <span class="metric-value">)" + std::to_string(port) + R"(</span>
            </div>
            <div class="metric">
                <span class="metric-label">Protocol:</span>
                <span class="metric-value">HTTP/1.1</span>
            </div>
            <div class="metric">
                <span class="metric-label">Threading:</span>
                <span class="metric-value">Multi-threaded (Thread Pool)</span>
            </div>
            <div class="metric">
                <span class="metric-label">Static Files:</span>
                <span class="metric-value">Enabled</span>
            </div>
            <div class="metric">
                <span class="metric-label">MIME Detection:</span>
                <span class="metric-value">Active</span>
            </div>
            <div class="metric">
                <span class="metric-label">Build:</span>
                <span class="metric-value">C++17 Release</span>
            </div>
        </div>
        
        <div class="status">
            <h3>Threading Status</h3>
            <p><strong>Worker Pool:</strong> Multi-threaded server with worker pool active</p>
            <p><strong>Concurrency:</strong> Concurrent request processing enabled</p>
            <p><strong>Performance:</strong> Non-blocking I/O operations</p>
            <p><strong>Scalability:</strong> Ready for high-traffic loads</p>
        </div>
    </div>
</body>
</html>)";
    
    return createHttpResponse(html_body);
}

std::string ResponseGenerator::createErrorResponse(int status_code, const std::string& message) {
    std::string title = getStatusText(status_code);
    std::string html_body = createErrorHtml(status_code, title, message);
    return createHttpResponse(html_body, "text/html", status_code, title);
}

std::string ResponseGenerator::create404Response() {
    return createErrorResponse(404, "The requested page could not be found.");
}

std::string ResponseGenerator::create400Response() {
    return createErrorResponse(400, "The request could not be understood by the server.");
}

std::string ResponseGenerator::create500Response() {
    return createErrorResponse(500, "An internal server error occurred.");
}

std::string ResponseGenerator::getStatusText(int status_code) {
    switch (status_code) {
        case 200: return "OK";
        case 400: return "Bad Request";
        case 404: return "Not Found";
        case 500: return "Internal Server Error";
        default: return "Unknown Status";
    }
}

std::string ResponseGenerator::createErrorHtml(int status_code, const std::string& title, const std::string& message) {
    return R"(
<!DOCTYPE html>
<html>
<head>
    <title>)" + std::to_string(status_code) + " " + title + R"(</title>
    <style>
        body { font-family: Arial, sans-serif; margin: 40px; text-align: center; background: #f4f4f4; }
        .error-container { background: white; padding: 40px; border-radius: 8px; box-shadow: 0 2px 10px rgba(0,0,0,0.1); max-width: 600px; margin: 0 auto; }
        h1 { color: #e74c3c; font-size: 3em; margin-bottom: 20px; }
        p { color: #666; margin: 20px 0; }
        a { color: #3498db; text-decoration: none; }
        .back-btn { background: #3498db; color: white; padding: 10px 20px; border-radius: 4px; display: inline-block; margin-top: 20px; }
    </style>
</head>
<body>
    <div class="error-container">
        <h1>)" + std::to_string(status_code) + R"(</h1>
        <h2>)" + title + R"(</h2>
        <p>)" + message + R"(</p>
        <a href="/" class="back-btn">← Back to Home</a>
    </div>
</body>
</html>)";
}