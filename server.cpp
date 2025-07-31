#include <algorithm>
#include <bits/types/struct_timeval.h>
#include <cerrno>
#include <cstddef>
#include <cstdio>
#include <iostream>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <sys/types.h>
#include <cstring>
#include <vector>
#include <fcntl.h>
#include <poll.h>
#include <vector>
#include <unordered_map>
#include <sstream>
#include <algorithm>

std::unordered_map<std::string, std::vector<std::string>> headerParser(const std::string& request){
    std::unordered_map<std::string, std::vector<std::string>> headers;
    std::stringstream requestStream(request);
    std::string line;

    // Skip the request line (e.g., "GET /HTTP/1.1")
    std::getline(requestStream, line);

    // Read headers until an empty line is encountered
    while(std::getline(requestStream, line) && !line.empty() && line != "\r"){
        if(!line.empty() && line.back() == '\r'){
            line.pop_back();
        }

        size_t colonPos = line.find(':');
        if (colonPos != std::string::npos) {
            std::string name = line.substr(0, colonPos);
            std::string value = line.substr(colonPos + 1);

            // Trim leading/ trailing whitespace from name
            size_t start = name.find_first_not_of(" \t");
            name = (start == std::string::npos) ? "" : name.substr(start);
            size_t end = name.find_last_not_of(" \t");
            name = (end == std::string::npos) ? "" : name.substr(0, end+1);

            // Trim leading/ trailing whitespace from value
            start = value.find_first_not_of(" \t");
            value = (start == std::string::npos) ? "" : value.substr(start);
            end = value.find_last_not_of(" \t");
            value = (end == std::string::npos) ? "" : value.substr(0, end+1);

            headers[name].push_back(value);
        }
    }

    return headers;
}

int main(){

    std::vector<pollfd> poll_fds;
    std::vector<int> client_sockets;

    // Creating the socket
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (socket_fd == -1){
        std::cerr << "failed to connet to socket." << strerror(errno) << std::endl;
    }

    // std::cout << "Socket created with fd = " << socket_fd << std::endl;

    int reuse = 1;
    if(setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) == -1){
        std::cerr << "Error setting SO_REUSEADDR: " << strerror(errno) << std::endl;
        close(socket_fd);
        return 1;
    }

    // creating the server address
    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_port = htons(8080);
    address.sin_addr.s_addr = INADDR_ANY;

    // Binding to that sockaddr
    if(bind(socket_fd, (sockaddr*)&address, sizeof(address)) == -1){
        std::cerr << "failed to bind to server"<< strerror(errno)  << std::endl;
    close(socket_fd);
    }

    // std::cout << "Socket bound to port 8080" << std::endl;

    if(listen(socket_fd, 10) == -1){
        std::cerr << "Error listening to requests " << strerror(errno) << std::endl;
        close(socket_fd);
        return 1;
    }

    // Set the server socket to non-blockign mode
    int flags = fcntl(socket_fd, F_GETFL, 0);
    if(fcntl(socket_fd, F_SETFL, flags | O_NONBLOCK) < 0) {
        perror("fcntl failed");
        return -1;
    }

    pollfd server_poll_fd;
    server_poll_fd.fd = socket_fd;
    server_poll_fd.events = POLLIN;
    poll_fds.push_back(server_poll_fd);


    while(true) {
        int activity = poll(poll_fds.data(), poll_fds.size(), -1); // -1 means indefinite timeout

        if((activity < 0 ) && (errno != EINTR)){
            perror("select error");
            break;
        }

        // Incoming connection on the server socket
        int new_socket;
        socklen_t addrlen = sizeof(address);
        char buffer[1024] = {0};


        if(poll_fds[0].revents & POLLIN){
            // Why server_address instead of client_address

            if((new_socket = accept(socket_fd, (sockaddr *)&address, &addrlen)) < 0) {
                perror("accept failed");
                continue;
            }
            // std::cout << "New connection, socket fd is " << new_socket << " , ip is : " << inet_ntoa(address.sin_addr) << ", port : " << ntohs(address.sin_port) << std::endl;

            flags = fcntl(new_socket, F_GETFL, 0);
            if(fcntl(new_socket, F_SETFL, flags | O_NONBLOCK) < 0){
                perror("fcntl failed");
                close(new_socket);
                continue;
            }


            pollfd new_poll_fd;
            new_poll_fd.fd = new_socket;
            new_poll_fd.events = POLLIN;
            poll_fds.push_back(new_poll_fd);
            // Add new socket to the list of sockets
            client_sockets.push_back(new_socket);
        }
        // std::cout << "New connection, socket fd is " << new_socket << std::endl;
            // Check for activity on other sockets
        for(size_t i=1; i< poll_fds.size(); ++i) {
            if(poll_fds[i].revents & POLLIN){
                int sd = poll_fds[i].fd;
                ssize_t bytes_received = read(sd, buffer, 1024);
                if(bytes_received < 0){
                    // No data received yet, try again later
                    if (errno == EAGAIN || errno == EWOULDBLOCK) {
                        continue;
                    }else{
                        perror("read failed");
                        close(sd);
                        poll_fds.erase(poll_fds.begin() + i);
                        auto it = std::remove(client_sockets.begin(), client_sockets.end(), sd);
                        client_sockets.erase(it, client_sockets.end());
                        --i;
                        continue;
                    }

                }
                else if(bytes_received == 0){
                    // Somebody disconnected, get his retails and print
                    // getpeername(sd, (sockaddr *)&address, &addrlen);
                     // std::cout << "Host disconnected, ip " << inet_ntoa(address.sin_addr) << " , port " << ntohs(address.sin_port) << std::endl;

                    // Close the socket and remove from list
                    close(sd);
                    poll_fds.erase(poll_fds.begin() + i);
                    client_sockets.erase(std::remove(client_sockets.begin(), client_sockets.end(), sd), client_sockets.end());
                    continue;
                }else {
                    // Echo back the messaeg that came in
                    buffer[bytes_received] = '\0';
                    std::cout << buffer << std::endl;
                    std::unordered_map<std::string, std::vector<std::string>> parsedData = headerParser(buffer);
                    for(const auto& pair: parsedData) {
                        std::cout << pair.first << " = ";

                        for(const auto& value: pair.second) {
                            std::cout << value << " ";
                        }
                        std::cout << std::endl;
                    }

                    std::string body = "Hello, World!";
                    std::string response = "HTTP/1.1 200 OK\r\n";
                    response += "Content-Type: text/plain\r\n";
                    response +="Content-Length: " + std::to_string(body.size()) + "\r\n";
                    response += "\r\n";
                    response += body;

                    send(sd, response.c_str(), response.size(), 0);
                }
            }
            }
        }

        // Close all client sockets
        for(int client_sd: client_sockets){
            close(client_sd);
        }
    close(socket_fd);
    poll_fds.erase(poll_fds.begin() +1);
    return 0;
}
