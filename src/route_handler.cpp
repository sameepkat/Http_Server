#include "../include/route_handler.hpp"
#include "../include/request_handler.hpp"


Response route_handler(const Request& req){
    if(req.method == "GET" && req.path == "/"){
        return handle_root(req);
    }else if (req.method == "GET" && req.path == "/health"){
        return handle_health(req);
    }else {
        return handle_404(req);
    }
}
