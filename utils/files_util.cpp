#include "../include/files_util.hpp"
#include <fstream>
#include <sstream>
#include <sys/stat.h>
#include <unordered_map>


bool file_exists(const std::string& file_path) {
    struct stat file;
    return (stat(file_path.c_str(), &file) == 0);
}

std::string read_file(const std::string& file_path){
    std::ifstream file(file_path,std::ios::binary);
    std::ostringstream oss;
    oss << file.rdbuf();
    return oss.str();
}


std::string get_mime_type(const std::string& file_path){
    std::unordered_map<std::string, std::string> mimeTypes = {
        {".html", "text/html"},
        {".css", "text/css"},
        {".js", "application/javascript"},
        {".png", "image/png"},
        {".jpg", "image/jpeg"},
        {".svg", "text/svg+xml"},
        {".ico", "text/x-icon"},
    };

    size_t dot_pos = file_path.rfind('.');
    if(dot_pos == std::string::npos){
        return "application/octet-stream" ;
    }

    std::string file_ext = file_path.substr(dot_pos);

    auto it = mimeTypes.find(file_ext);
    if(it != mimeTypes.end()){
        return it->second;
    }
    return "application/octet-stream";
}
