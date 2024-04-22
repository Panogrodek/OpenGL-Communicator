#include "pch.h"
#include "Application.hpp"
#include "SPDLOG/spdlog.h"

#include "freetype/ft2build.h"
#include FT_FREETYPE_H  

int main() {
	Application app;
	app.Run();
	return 0;
}