#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <sys/types.h>
#include <stdexcept>

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

    // Creating the server address
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

    std:: cout << "Server listening on port 8000" << std::endl;

    sockaddr_in client_address;
    socklen_t client_address_len = sizeof(client_address);

    
    int client_sockfd = accept(socket_fd, (sockaddr *)&client_address, &client_address_len);
    if(client_sockfd == -1){
        std::cerr << "Failed to accept connections " << std::endl;
        close(socket_fd);
        return 1;
    }

    // Print client information
    char client_ip_string[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &client_address.sin_addr, client_ip_string, INET_ADDRSTRLEN);
    std::cout << "Accepted connection from"<< client_ip_string << ":" << ntohs(client_address.sin_port) << std::endl;



    close(client_sockfd);
    close(socket_fd);
         
}
