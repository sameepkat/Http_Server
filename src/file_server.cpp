// Check if the file exists, return the function that returns the files content
#include "../include/file_server.hpp"
#include "../include/files_util.hpp"


Response serve_file(const Request& req){
    std::string file_path = "public" + req.path;

    if(!file_exists(file_path)){
        std::string body = "File not found";

        return Response{
      "HTTP/1.1",
      404,
      "Not Found",
      {
          {"Content-Type", {"text/plain"}},
          {"Content-Length", {std::to_string(body.size())}}
      },
      body
        };
    }
    std::string buffer = read_file(file_path);
    std::string mimeType = get_mime_type(file_path);

    return Response {
      "HTTP/1.1",
      200,
      "OK",
      {
          {"Content-Type", {mimeType}},
          {"Content-Length", {std::to_string(buffer.size())}}
      },
      buffer
    };
}
