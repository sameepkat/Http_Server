# C++ HTTP Server

A simple, lightweight HTTP server built from scratch in C++.

## Description

This project is a basic HTTP/1.1 server implementation in C++. It's designed to be a learning tool for understanding the fundamentals of network programming and the HTTP protocol. The server can handle basic GET requests, serve static files, and has a simple routing mechanism.

## Features

*   Handles GET requests
*   Serves static files from the `public` directory
*   Basic routing for `/`, `/health`, and a 404 page
*   Non-blocking I/O using `poll()` for handling multiple connections

## Getting Started

### Prerequisites

*   A C++ compiler that supports C++20 (e.g., `clang++` or `g++`)
*   `make`

### Building and Running

1.  **Clone the repository:**
    ```bash
    git clone <repository-url>
    cd http-server
    ```

2.  **Build the server:**
    ```bash
    make
    ```
    This will create an executable named `server` in the root directory.

3.  **Run the server:**
    ```bash
    ./server
    ```
    The server will start and listen on port 8080.

## Usage

Once the server is running, you can interact with it using a web browser or a tool like `curl`.

*   **Root:** `http://localhost:8080/` - Serves the `index.html` file.
*   **Health Check:** `http://localhost:8080/health` - Returns a "200 OK" status.
*   **Static Files:** `http://localhost:8080/styles.css` - Serves the `styles.css` file.
*   **Not Found:** `http://localhost:8080/foo` - Returns a "404 Not Found" error.

## Project Structure

```
.
├── Makefile
├── include
│   ├── http_types.hpp
│   ├── http_utils.hpp
│   ├── request_handler.hpp
│   └── route_handler.hpp
├── public
│   ├── index.html
│   └── styles.css
└── src
    ├── http_utils.cpp
    ├── request_handler.cpp
    ├── route_handler.cpp
    └── server.cpp
```

### Key Files

*   `src/server.cpp`: The main entry point and server logic.
*   `src/request_handler.cpp`: Handles incoming HTTP requests and dispatches them to the appropriate handlers.
*   `src/route_handler.cpp`: Maps routes to specific handler functions.
*   `include/http_types.hpp`: Defines the `Request` and `Response` structs.
*   `public/`: Contains the static files (HTML, CSS, etc.) that the server can serve.
*   `Makefile`: Contains the build instructions for the project.
