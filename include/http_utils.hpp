#ifndef HTTP_UTILS
#define HTTP_UTILS

#include <string>
#include <vector>
#include <unistd.h>
#include <unordered_map>

std::unordered_map<std::string, std::vector<std::string>> headerParser(const std::string& request);

#endif
