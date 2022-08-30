#pragma once

#include <olcPixelGameEngine.h>
#include <unordered_map>
#include <iostream>
#include <stdio.h>

#include "Miscellaneous/Memory.hpp"
#include "GUI/Sprite.hpp"

namespace dty {
	enum class SpriteName {
		GRASS_1,
		GRASS_2,

		TREE_1
	};

	struct SpriteDecalPair {
		Ref<olc::Sprite> sprite;
		Ref<olc::Decal> decal;
	};

	class SpriteLoader {
	public:
		SpriteLoader() = default;
		
		void Initalize();
		Ref<olc::Decal> GetOLCDecal(SpriteName spriteName);
	private:
		std::unordered_map<SpriteName, SpriteDecalPair> sprites;

		// Stores the path with the enum name
		std::unordered_map<SpriteName, std::string> spritePaths;
	};
}