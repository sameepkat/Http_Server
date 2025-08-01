#include "../include/http_utils.hpp"
#include <sstream>

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
