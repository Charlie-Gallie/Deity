#include "IsometricPlane.hpp"

#include "Deity/Deity.hpp"

#include <iostream>
#include <fstream>
#include <random>
#include <regex>

namespace dty {
	void IsometricPlane::InitializeWorld(Ref<WorldFileIO> _worldFile) {
		worldFile = _worldFile;

		PropertyArray worldSize = worldFile->GetPropertyArray("s", 2);
		gridSize = {
			(uint32_t)worldSize[0][0],
			(uint32_t)worldSize[0][1]
		};

		uint32_t x = 0, y = 0;
		PropertyArray properties = worldFile->GetPropertyArray("t", 3);
		for (const auto& prop : properties) {
			Ref<Tile> tile = CreateRef<Tile>();

			tile->ground->spriteName = (SpriteName)prop[0];
			tile->object->spriteName = (SpriteName)prop[1];

			deity.spriteLoader->LoadIntoDecal(tile->ground->olcDecal, tile->ground->spriteName);
			deity.spriteLoader->LoadIntoDecal(tile->object->olcDecal, tile->object->spriteName);

			tile->ground->olcDecal = deity.spriteLoader->GetOLCDecal((SpriteName)prop[0]);

			tile->ground->SetGridPosition({ (uint32_t)x, (uint32_t)y });
			tile->ground->size = {
				deity.config.baseTileSize.x,
				deity.config.baseTileSize.y
			};

			tile->object->SetGridPosition({ (uint32_t)x, (uint32_t)y });
			tile->object->size = {
				deity.config.baseTileSize.x,
				deity.config.baseTileSize.y
			};

			++x;
			if (x >= gridSize.x) {
				x = 0;
				++y;
			}

			tiles.push_back(tile);
		}

		PropertyArray worldPosition = worldFile->GetPropertyArray("p", 2);
		if (worldPosition.size())
			gridOffset = {
				(double)worldPosition[0][0],
				(double)worldPosition[0][1]
			};
	}

	void IsometricPlane::Initialize() {
		
	}

	void IsometricPlane::Update() {
		olc::PixelGameEngine& pge = deity.getPGE();

		pge.SetPixelMode(olc::Pixel::ALPHA);

		Vector2i mousePosition = {
			(int32_t)pge.GetMouseX(),
			(int32_t)pge.GetMouseY()
		};

		double viewPadding = 96; // Todo: Find a better way than just adding a buffer
		for (Ref<Tile> sprite : tiles) {
			// Get coord of current tile
			if ( // Get current tile being hovered over - Such a bodge
				(int32_t)((mousePosition.x - 16) - gridOffset.x > sprite->ground->position.x) &&
				(int32_t)((mousePosition.x - 16) - gridOffset.x < sprite->ground->position.x + (deity.config.baseTileSize.x * 0.5)) &&
				(int32_t)((mousePosition.y - 16) - gridOffset.y > sprite->ground->position.y) &&
				(int32_t)((mousePosition.y - 16) - gridOffset.y < sprite->ground->position.y + deity.config.baseTileSize.y)
				) {
				if (pge.GetMouse(olc::Mouse::LEFT).bPressed) {
					SpriteName tempSprite = sprite->object->spriteName;

					// vvv Does not work
					tempSprite = tempSprite == SpriteName::TREE_1 ? SpriteName::NO_SPRITE : SpriteName::TREE_1;

					deity.spriteLoader->LoadIntoDecal(sprite->object->olcDecal, tempSprite);
				}
				//std::cout << (olc::vi2d)sprite->ground->GetGridPosition() << std::endl;
			}

			if ( // Cull tiles which cannot be seen
				sprite->ground->position.x + gridOffset.x + position.x >= (int32_t)(position.x - viewPadding) &&
				sprite->ground->position.x + gridOffset.x - position.x <= (int32_t)(position.x + size.y + viewPadding) &&
				sprite->ground->position.y + gridOffset.y + position.y >= (int32_t)(position.y - viewPadding) &&
				sprite->ground->position.y + gridOffset.y + position.y <= (int32_t)(position.y + size.y + viewPadding)
			) {
				Vector2 spritePos = sprite->ground->position + gridOffset;

				pge.SetDrawTarget(deity.renderLayers[1]);

				sprite->ground->Draw(pge, spritePos);
				sprite->object->Draw(pge, spritePos);
				
				pge.SetDrawTarget(deity.renderLayers[0]);
			}
		}

		pge.SetPixelMode(olc::Pixel::NORMAL);
	}
}