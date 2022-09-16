#ifndef ARG_PARSER
#define ARG_PARSER
#pragma once
#include "config.h"
#include <string>

namespace sig {

bool parse_command_line(int argc, const char** argv, Config&);

} // namespace sig

#endif