#include "../include/request_handler.hpp"
#include <sys/socket.h>
#include <vector>

Response request_handler(const Request &req){
    Response res;
    std::string body;
    if(req.method == "GET"){
        body = "Handling GET Request\n";
    }else{
        body+= "Sorry. That method is not allowed as of now";
    }
    body += "The requested path is: " + req.path;

    // std::string response = "HTTP/1.1 200 OK\r\n";
    // response += "Content-Type: text/plain\r\n";
    // response +="Content-Length: " + std::to_string(body.size()) + "\r\n";
    // response += "\r\n";
    // response += body;

    std::unordered_map<std::string, std::vector<std::string>> headers = {
        {"Content-Type", {"text/plain"}},
        {"Content-Length", {std::to_string(body.size())}}
    };


    res = {
        "HTTP/1.1",
        200,
        "OK",
        headers,
        body,
    };

    return res;
}
