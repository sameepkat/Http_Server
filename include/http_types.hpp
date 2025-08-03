#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include <sstream>

struct Request{
    std::string method;
    std::string path;
    std::string version;
    std::unordered_map<std::string ,std::vector<std::string>> headers;
    std::string body;
} ;


struct Response{
    std::string version = "HTTP/1.1";
    int status_code = 200;
    std::string reason = "OK";
    std::unordered_map<std::string, std::vector<std::string>> headers;
    std::string body;

    std::string to_string() const {
        std::ostringstream oss;
        oss << version << " " << std::to_string(status_code) << " " << reason << "\r\n";
        for (const auto& [key, values] : headers) {
           for(const auto& value: values) {
               oss << key << ": " << value << "\r\n";
           }
        }
        oss << "\r\n" << body;
        return oss.str();
    }
};
