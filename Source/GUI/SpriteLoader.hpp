#pragma once

#include <olcPixelGameEngine.h>
#include <unordered_map>
#include <iostream>
#include <stdio.h>

#include "Miscellaneous/Memory.hpp"

namespace dty {
	class Sprite;

	enum class SpriteName {
		NO_SPRITE = 0, // A real tile but should not be rendered

		HOVERED,

		GRASS_L0_1,
		GRASS_L0_2,
		GRASS_L0_3,
		GRASS_L0_4,

		GRASS_L1_1,
		GRASS_L1_2,
		GRASS_L1_3,
		GRASS_L1_4,

		DIRT_L0_1,
		SAND_L0_1,
		GRAVEL_L0_1,

		WATER_L0_1,

		TREE_1,
		TREE_2,

		DEAD_TREE_1,
		DEAD_TREE_2,

		BUSH_1,
		SHRUB_1,

		WEED_1,
		WEED_2,
		WEED_3,

		FIRE_1,
		FIRE_2,
		FIRE_3,
		FIRE_4,

		LIGHTNING_1,
		LIGHTNING_2,
		LIGHTNING_3,

		SIZE // Not a real tile, it's for enum arithmetic
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
		std::string GetSpritePath(SpriteName);
	private:
		std::unordered_map<SpriteName, SpriteDecalPair> sprites;

		// Stores the path with the enum name
		std::unordered_map<SpriteName, std::string> spritePaths;
	};
}