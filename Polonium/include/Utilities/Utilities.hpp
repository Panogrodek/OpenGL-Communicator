#pragma once
#include <string>
#include "AABB2D.hpp"

inline std::string ErasePart(char symbol, std::string& line)
{
	if (line.find(symbol) == std::string::npos)
		return "";

	int index = line.find(symbol) + 1;
	std::string part = line.substr(0, index);
	line.erase(0, index);

	return part;
}