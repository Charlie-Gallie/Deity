#include "SpriteLoader.hpp"

#include "GUI/Sprite.hpp"

namespace dty {
	void SpriteLoader::Initalize() {
		spritePaths[SpriteName::NO_SPRITE] = "Assets/No_Sprite.png"; // Should not be seen - If see then there's an issue
		spritePaths[SpriteName::HOVERED] = "Assets/Hovered.png";

		spritePaths[SpriteName::GRASS_L0_1] = "Assets/Grass_L0_1.png";
		spritePaths[SpriteName::GRASS_L0_2] = "Assets/Grass_L0_2.png";
		spritePaths[SpriteName::GRASS_L0_3] = "Assets/Grass_L0_3.png";
		spritePaths[SpriteName::GRASS_L0_4] = "Assets/Grass_L0_4.png";

		spritePaths[SpriteName::GRASS_L1_1] = "Assets/Grass_L1_4.png";
		spritePaths[SpriteName::GRASS_L1_2] = "Assets/Grass_L1_4.png";
		spritePaths[SpriteName::GRASS_L1_3] = "Assets/Grass_L1_4.png";
		spritePaths[SpriteName::GRASS_L1_4] = "Assets/Grass_L1_4.png";
		
		spritePaths[SpriteName::DIRT_L0_1] = "Assets/Dirt_L0_1.png";
		spritePaths[SpriteName::SAND_L0_1] = "Assets/Sand_L0_1.png";
		spritePaths[SpriteName::GRAVEL_L0_1] = "Assets/Gravel_L0_1.png";

		spritePaths[SpriteName::WATER_L0_1] = "Assets/Water_L0_1.png";

		spritePaths[SpriteName::TREE_1] = "Assets/Tree_1.png";
		spritePaths[SpriteName::TREE_2] = "Assets/Tree_2.png";

		spritePaths[SpriteName::DEAD_TREE_1] = "Assets/Dead_Tree_1.png";
		spritePaths[SpriteName::DEAD_TREE_2] = "Assets/Dead_Tree_2.png";

		spritePaths[SpriteName::BUSH_1] = "Assets/Bush_1.png";
		spritePaths[SpriteName::SHRUB_1] = "Assets/Shrub_1.png";

		spritePaths[SpriteName::WEED_1] = "Assets/Weed_1.png";
		spritePaths[SpriteName::WEED_2] = "Assets/Weed_2.png";
		spritePaths[SpriteName::WEED_3] = "Assets/Weed_3.png";

		spritePaths[SpriteName::FIRE_1] = "Assets/Fire_1.png";
		spritePaths[SpriteName::FIRE_2] = "Assets/Fire_2.png";
		spritePaths[SpriteName::FIRE_3] = "Assets/Fire_3.png";
		spritePaths[SpriteName::FIRE_4] = "Assets/Fire_4.png";

		spritePaths[SpriteName::LIGHTNING_1] = "Assets/Lightning_1.png";
		spritePaths[SpriteName::LIGHTNING_2] = "Assets/Lightning_2.png";
		spritePaths[SpriteName::LIGHTNING_3] = "Assets/Lightning_3.png";

		for (const auto& spriteNamePair : spritePaths) {
			const std::string& name = spriteNamePair.second;
			SpriteDecalPair spriteDecalPair;

			spriteDecalPair.sprite = CreateRef<olc::Sprite>(name);
			spriteDecalPair.decal = CreateRef<olc::Decal>(spriteDecalPair.sprite.get());
			spriteDecalPair.decal->UpdateSprite();

			sprites[spriteNamePair.first] = spriteDecalPair;
		}
	}

	Ref<olc::Decal> SpriteLoader::GetOLCDecal(SpriteName _spriteName) {
		return sprites[_spriteName].decal;
	}

	void SpriteLoader::LoadIntoDecal(Ref<olc::Decal>& _decal, SpriteName _spriteName) {
		_decal = sprites[_spriteName].decal;
	}

	std::string SpriteLoader::GetSpritePath(SpriteName _spriteName) {
		return spritePaths[_spriteName];
	}
}