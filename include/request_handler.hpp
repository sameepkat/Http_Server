#pragma once
#include "../include/http_types.hpp"

Response handle_root(const Request& req);
Response handle_health(const Request& req);
Response handle_404(const Request& req);
