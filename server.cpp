#include <bits/types/struct_timeval.h>
#include <cerrno>
#include <cstddef>
#include <cstdio>
#include <iostream>
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

int main(){
    // Creating the socket
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    if (socket_fd == -1){
        std::cerr << "failed to connet to socket." << strerror(errno) << std::endl;
    }

    std::cout << "Socket created with fd = " << socket_fd << std::endl;

    int reuse = 1;
    if(setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) == -1){
        std::cerr << "Error setting SO_REUSEADDR: " << strerror(errno) << std::endl;
        close(socket_fd);
        return 1;
    }

    // creating the server address
    sockaddr_in server_address{};
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(8000);
    server_address.sin_addr.s_addr = INADDR_ANY;

    // Binding to that sockaddr
    if(bind(socket_fd, (sockaddr*)&server_address, sizeof(server_address)) == -1){
        std::cerr << "failed to bind to server"<< strerror(errno)  << std::endl;
    close(socket_fd);
    }

    std::cout << "Socket bound to port 8000" << std::endl;

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

    std::vector<int> client_sockets;
    int max_sd = socket_fd;

    while(true) {
        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(socket_fd, &readfds);
        

        for(int client_sd: client_sockets){
            FD_SET(client_sd, &readfds);
            max_sd = std::max(max_sd, client_sd);
        }

        timeval timeout;
        timeout.tv_sec = 5 ;
        timeout.tv_usec = 0;

        int activity = select(max_sd + 1, &readfds, NULL, NULL, &timeout);

        if((activity < 0 ) && (errno != EINTR)){
            perror("select error");
            break;
        }

        // Incoming connection on the server socket
        int new_socket;
        socklen_t addrlen = sizeof(server_address);
        char buffer[1024] = {0};

        
        if(FD_ISSET(socket_fd, &readfds)){
            // Why server_address instead of client_address
            if((new_socket = accept(socket_fd, (sockaddr *)&server_address, &addrlen)) < 0) {
                perror("accept");
                break;
            }
            std::cout << "New connection, socket fd is " << new_socket << " , ip is : " << inet_ntoa(server_address.sin_addr) << ", port : " << ntohs(server_address.sin_port) << std::endl;

            flags = fcntl(new_socket, F_GETFL, 0);
            if(fcntl(new_socket, F_SETFL, flags | O_NONBLOCK) < 0){
                perror("fcntl failed");
                close(new_socket);
                continue;
            }

            // Add new socket to the list of sockets
            client_sockets.push_back(new_socket);
        }
            // Handle data from client sockets
            for(auto it = client_sockets.begin(); it != client_sockets.end();) {
                int sd = *it;
                if(FD_ISSET(sd, &readfds)){
                    ssize_t bytes_received = read(sd, buffer, 1024);
                    if(bytes_received < 0){
                        // No data received yet, try again later
                        if (errno == EAGAIN || errno == EWOULDBLOCK) {
                          ++it;
                          continue;
                        }else{
                            perror("read failed");
                            close(sd);
                            it = client_sockets.erase(it);
                            continue;
                        }

                    }
                    else if(bytes_received == 0){
                        // Somebody disconnected, get his retails and print
                        // getpeername(sd, (sockaddr *)&server_address, &addrlen);
                        std::cout << "Host disconnected, ip " << inet_ntoa(server_address.sin_addr) << " , port " << ntohs(server_address.sin_port) << std::endl;

                        // Close the socket and remove from list
                        close(sd);
                        it = client_sockets.erase(it);
                        continue;
                    }else {
                        // Echo back the messaeg that came in
                        buffer[bytes_received] = '\0';
                        std::cout << buffer << std::endl;
                        send(sd, buffer, strlen(buffer), 0);
                        it++;
                    }
                }else{
                    it++;
                }
            }
        }

        // Close all client sockets
        for(int client_sd: client_sockets){
            close(client_sd);
        }
    close(socket_fd);
    return 0;
}
