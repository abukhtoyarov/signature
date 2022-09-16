#ifndef MD5_H
#define MD5_H
#pragma once
#include <string>

namespace sig {

std::string calcHash(const char* data, size_t sz);
std::string calcHash(const std::string& data);

} // namespace sig;

#endif
