cmake_minimum_required(VERSION 3.10)
project(webserver)

# C++17 standard
set(CMAKE_CXX_STANDARD 17)

include_directories(src)
include_directories(src/core)
include_directories(src/http)
include_directories(src/handlers)
include_directories(src/threading)

file(GLOB_RECURSE HEADERS
    src/core/*.h
    src/http/*.h
    src/handlers/*.h
    src/threading/*.h
)

# All source files
add_executable(webserver
    src/core/main.cpp
    src/core/Server.cpp
    src/http/HttpRequest.cpp
    src/http/HttpParser.cpp
    src/handlers/FileHandler.cpp
    src/handlers/ResponseGenerator.cpp
    src/threading/ThreadPool.cpp
)
 
# Link pthread
find_package(Threads REQUIRED) 
target_link_libraries(webserver Threads::Threads)

# Install binary
install(TARGETS webserver
    RUNTIME DESTINATION bin
)

# Copy public static files to build directory for testing/dev
add_custom_command(TARGET webserver POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/src/public $<TARGET_FILE_DIR:webserver>/public
)