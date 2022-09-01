#pragma once

#include <olcPixelGameEngine.h>
#include <unordered_map>
#include <iostream>
#include <stdio.h>

#include "Miscellaneous/Memory.hpp"

namespace dty {
	class Sprite;

	enum class SpriteName {
		NO_SPRITE = 0,

		HOVERED,

		GRASS_1,
		GRASS_2,

		WATER_1,

		TREE_1,

		SIZE
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
		void LoadIntoDecal(Ref<olc::Decal>& decal, SpriteName spriteName);
	private:
		std::unordered_map<SpriteName, SpriteDecalPair> sprites;

		// Stores the path with the enum name
		std::unordered_map<SpriteName, std::string> spritePaths;
	};
}