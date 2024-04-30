#pragma once
#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <GLM/glm.hpp>
#include <SPDLOG/spdlog.h>
#include "Utilities/Utilities.hpp"

#ifdef DEBUG
#define ASSERT(x) {if(x) __debugbreak();}
#else
#define ASSERT(x, ...)
#endif