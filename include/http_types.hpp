#pragma once
#include <string>
#include <unordered_map>
#include <vector>

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
        std::string res = version + " " + std::to_string(status_code) + " " + reason + "\r\n";
        for (const auto& [key, values] : headers) {
           for(const auto& value: values) {
               res += key + ": " + value + "\r\n";
           }
        }
        res += "\r\n" + body;
        return res;
    }
};
