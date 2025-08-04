#include "../include/route_handler.hpp"
#include "../include/request_handler.hpp"
#include "../include/file_server.hpp"
#include <functional>
#include <unordered_map>

using RouteHandler = std::function<Response(const Request& )>;

std::unordered_map<std::string, RouteHandler> get_routes = {
    {"/", handle_root},
    {"/health", handle_health},
    {"/index.html", serve_file},
    {"/styles.css", serve_file}
};

Response route_handler(const Request& req){
    if(req.method == "GET"){
        auto it = get_routes.find(req.path);
        if(it != get_routes.end()){
            return it->second(req);
        }
    }
    return handle_404(req);
}
