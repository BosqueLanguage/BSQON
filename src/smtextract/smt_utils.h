#pragma once

#include "../../build/include/z3/z3++.h"
#include <cstdio>
#include <optional>

// Utils
void badArgs(const char* msg);
bool validPath(const char* filepath, const char* extension);
