#include "../include/request_handler.hpp"
#include <string>
#include <sys/socket.h>

Response handle_root(const Request& req){
    std::string body = "Welcome to the root path";
    return Response {
        "HTTP/1.1",
        200,
        "OK",
        {{
                "Content-Type", {"text/plain"}},
         {"Content-Length", {std::to_string(body.size())}}},
        body
    };
}

Response handle_health(const Request& req){
    std::string body = R"({"health": "ok"})";

  return Response{
      "HTTP/1.1",
      200,
      "OK",
      {
          {"Content-Type", {"application/json"}},
          {"Content-Length", {std::to_string(body.size())}}
      },
      body
  };
}

Response handle_404(const Request& req){
  std::string body = "Not Found";

  return Response{
      "HTTP/1.1",
      400,
      "Not Found",
      {
          {"Content-Type", {"text/plain"}},
          {"Content-Length", {std::to_string(body.size())}}
      },
      body
  };
}
