cmake_minimum_required(VERSION 3.10)
project(webserver)

# C++17 standard
set(CMAKE_CXX_STANDARD 17)

include_directories(src)
include_directories(src/core)
include_directories(src/http)

# All source files
add_executable(webserver
    src/core/main.cpp
    src/core/Server.cpp
    src/http/HttpRequest.cpp
    src/http/HttpParser.cpp
)

# Link pthread
find_package(Threads REQUIRED)
target_link_libraries(webserver Threads::Threads)