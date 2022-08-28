#pragma once

#include <cstdint>
#include <olcPixelGameEngine.h>

#include "Vector2.hpp"

namespace dty {
	struct Configuration {
		// Meta window stuff:
		const Vector2ui window = { 1280, 720 };			// 16 : 9
		olc::Pixel clearColor = { 50, 100, 200 };		// Should not be seen
		const char* title = "Deity [olc::CodeJam 2022]";
	};
}