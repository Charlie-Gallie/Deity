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

			tile->ground->SetCurrentSprite((SpriteName)prop[0], deity.spriteLoader);
			tile->object->SetCurrentSprite((SpriteName)prop[1], deity.spriteLoader);

			tile->ground->SetGridPosition({ (uint32_t)x, (uint32_t)y });
			tile->ground->size = {
				(uint32_t)tile->ground->olcDecal->sprite->width,
				(uint32_t)tile->ground->olcDecal->sprite->height
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
		std::cout << "[IsometricPlane] Use InitializeWorld() rather than Initialize()!" << std::endl;
		// Todo: Load blank world if the world file cannot be loaded correctly
	}

	void IsometricPlane::Update() {
		olc::PixelGameEngine& pge = deity.getPGE();

		Vector2i mousePosition = {
			(int32_t)pge.GetMouseX(),
			(int32_t)pge.GetMouseY()
		};

		pge.SetDrawTarget(deity.renderLayers[1]);
		pge.SetPixelMode(olc::Pixel::ALPHA);

		pge.Clear(deity.config.skyColor);

		hoverSprite->SetCurrentSprite(SpriteName::NO_SPRITE, deity.spriteLoader);
		for (Ref<Tile> tile : tiles) {
			if ( // Check if the cursor is over the tile - Such a bodge
				(int32_t)((mousePosition.x - 16) - gridOffset.x > tile->ground->position.x) &&
				(int32_t)((mousePosition.x - 16) - gridOffset.x < tile->ground->position.x + (deity.config.baseTileSize.x * 0.5)) &&
				(int32_t)((mousePosition.y - 16) - gridOffset.y > tile->ground->position.y) &&
				(int32_t)((mousePosition.y - 16) - gridOffset.y < tile->ground->position.y + deity.config.baseTileSize.y)
			) {
				HoveringOverTile(tile); // Pass the tile the user is hovering
			}

			DrawTile(tile); // Draw tile
		}
		
		DrawHoverSprite(); // Draw the hovering sprite to indicate which tile you're over

		pge.SetPixelMode(olc::Pixel::NORMAL);
		pge.SetDrawTarget(deity.renderLayers[0]);
	}

	void IsometricPlane::DrawTile(Ref<Tile> _tile) {
		olc::PixelGameEngine& pge = deity.getPGE();

		double viewPadding = 96; // Todo: Find a better way than just adding a buffer
		if ( // If the tile can be seen (Culling)
			_tile->ground->position.x + gridOffset.x + position.x >= (int32_t)(position.x - viewPadding) &&
			_tile->ground->position.x + gridOffset.x - position.x <= (int32_t)(position.x + size.y + viewPadding) &&
			_tile->ground->position.y + gridOffset.y + position.y >= (int32_t)(position.y - viewPadding) &&
			_tile->ground->position.y + gridOffset.y + position.y <= (int32_t)(position.y + size.y + viewPadding)
		) {
			Vector2 spritePos = _tile->ground->position + gridOffset;

			_tile->ground->Draw(pge, spritePos);
			_tile->object->Draw(pge, spritePos);
		}
	}

	double saveTimer = 0;
	void IsometricPlane::DrawHoverSprite() {
		olc::PixelGameEngine& pge = deity.getPGE();

		pge.SetDrawTarget(deity.renderLayers[1]);
		hoverSprite->Draw(pge, hoverSprite->position + gridOffset);
		pge.SetDrawTarget(deity.renderLayers[0]);

		if (saveTimer > 0) {
			pge.DrawString({ (int32_t)position.x + 10, (int32_t)position.y + 10 }, "Saved!", olc::DARK_BLUE, 2U);
			saveTimer -= pge.GetElapsedTime();
		}
	}

	void IsometricPlane::HoveringOverTile(Ref<Tile> _tile) {
		olc::PixelGameEngine& pge = deity.getPGE();

		hoverSprite->SetGridPosition(_tile->ground->GetGridPosition());

		hoverSprite->SetCurrentSprite(SpriteName::HOVERED, deity.spriteLoader);

		if (pge.GetMouse(olc::Mouse::LEFT).bPressed) {
			SetTileGround(_tile, SpriteName::GRASS_2);
			worldFile->SetTileElements(
				_tile->ground->GetGridPosition(),
				{
					(int32_t)_tile->ground->GetCurrentSprite(),
					(int32_t)_tile->object->GetCurrentSprite(),
					0
				}
			);
		}

		if (pge.GetMouse(olc::Mouse::RIGHT).bPressed) {
			SetTileObject(_tile, SpriteName::TREE_1);

			worldFile->SetTileElements(
				_tile->ground->GetGridPosition(),
				{
					(int32_t)_tile->ground->GetCurrentSprite(),
					(int32_t)_tile->object->GetCurrentSprite(),
					0
				}
			);
		}

		if (pge.GetKey(olc::ENTER).bPressed) {
			worldFile->Save();
			saveTimer = 1.5;
		}
	}

	// Todo: Write new tile to WorldFileIO object
	// E.g., worldFile->SetTileElements(_tile->ground->GetGridPosition(), { (int32_t)_tile->ground->GetCurrentSprite(), 1, 0 });
	void IsometricPlane::SetTileGround(Ref<Tile> _tile, SpriteName _spriteName) {
		_tile->ground->SetCurrentSprite(_spriteName, deity.spriteLoader);
	}

	void IsometricPlane::SetTileObject(Ref<Tile> _tile, SpriteName _spriteName) {
		_tile->object->SetCurrentSprite(_spriteName, deity.spriteLoader);
	}
}