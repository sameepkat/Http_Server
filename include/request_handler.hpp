#pragma once
#include <string>

struct Request{
    std::string method;
    std::string path;
    std::string version;
} ;

std::string request_handler(const Request& req, const int sd);
