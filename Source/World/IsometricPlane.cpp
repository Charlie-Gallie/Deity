#include "IsometricPlane.hpp"

#include "Deity/Deity.hpp"

#include <iostream>
#include <fstream>
#include <random>
#include <regex>

namespace dty {
	void IsometricPlane::InitializeWorld(Ref<WorldFileIO> _worldFile) {
		worldFile = _worldFile;

		PropertyArray worldSize = worldFile->GetProperty("sS", 2);
		gridSize = {
			(uint32_t)worldSize[0][0],
			(uint32_t)worldSize[0][1]
		};

		std::cout << gridSize.x << " : " << gridSize.y << std::endl;

		uint32_t x = 0, y = 0;
		PropertyArray properties = worldFile->GetProperty("gG", 1);
		for (const auto& prop : properties) {
			Ref<Tile> tile = CreateRef<Tile>();

			tile->ground->olcDecal = deity.spriteLoader->GetOLCDecal((SpriteName)prop[0]);

			Vector2u spriteSize = {
				(uint32_t)tile->ground->olcDecal->sprite->width,
				(uint32_t)tile->ground->olcDecal->sprite->height
			};

			tile->ground->position = {
				(double)(x * 0.5 * spriteSize.x + y * -0.5 * spriteSize.x),
				(double)(x * 0.5 * deity.config.baseTileSize.y + y * 0.5 * deity.config.baseTileSize.y)
			};

			if (x++ >= gridSize.x) {
				x = 0;
				++y;
			}

			tiles.push_back(tile);
		}
	}

	void IsometricPlane::Initialize() {
		
	}

	void IsometricPlane::Update() {
		olc::PixelGameEngine& pge = deity.getPGE();

		pge.SetPixelMode(olc::Pixel::ALPHA);

		double viewPadding = 128; // Todo: Find a better way than just adding a buffer
		for (Ref<Tile> sprite : tiles) {
			if (
				sprite->ground->position.x + gridOffset.x + position.x >= (int32_t)(position.x - viewPadding) &&
				sprite->ground->position.x + gridOffset.x - position.x <= (int32_t)(position.x + size.y + viewPadding) &&
				sprite->ground->position.y + gridOffset.y + position.y >= (int32_t)(position.y - viewPadding) &&
				sprite->ground->position.y + gridOffset.y + position.y <= (int32_t)(position.y + size.y + viewPadding)
			) {
				pge.SetDrawTarget(deity.renderLayers[1]);
				pge.DrawDecal(sprite->ground->position + gridOffset, sprite->ground->olcDecal.get());

				if(sprite->object->olcDecal)
					pge.DrawDecal(sprite->object->position + gridOffset, sprite->object->olcDecal.get());


				pge.SetDrawTarget(deity.renderLayers[0]);
			}
		}

		pge.SetPixelMode(olc::Pixel::NORMAL);
	}
}