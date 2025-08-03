#ifndef HTTP_UTILS
#define HTTP_UTILS

#include <string>
#include <unistd.h>
#include "../include/http_types.hpp"

Request headerParser(const std::string& request);

#endif
