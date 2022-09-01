#include "SpriteLoader.hpp"

#include "GUI/Sprite.hpp"

namespace dty {
	void SpriteLoader::Initalize() {
		spritePaths[SpriteName::NO_SPRITE] = "Assets/No_Sprite.png"; // Should not be seen - If see it then there's an issue
		spritePaths[SpriteName::HOVERED] = "Assets/Hovered.png";
		spritePaths[SpriteName::GRASS_1] = "Assets/Grass_1.png";
		spritePaths[SpriteName::GRASS_2] = "Assets/Grass_2.png";
		spritePaths[SpriteName::WATER_1] = "Assets/Water_1.png";
		spritePaths[SpriteName::TREE_1] = "Assets/Tree_1.png";

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
}