#pragma once

#include <cstdint>
#include <olcPixelGameEngine.h>

#include "Vector2.hpp"

namespace dty {
	struct Configuration {
		// Meta window stuff:
		const Vector2u windowSize = { 1280, 720 };	// 16 : 9
		olc::Pixel clearColor = { 255, 0, 170, 0 };	// Ensure it's transparent for lower layers (zero alpha)
		const char* title = "Deity [olc::CodeJam 2022]";

		// Game info
		Vector2u baseTileSize = { 64, 32 };
		std::string
			assetsFolder = "Assets/",
			worldFile = "World.dty";
	};
}