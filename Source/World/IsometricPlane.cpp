#include "IsometricPlane.hpp"

#include "Deity/Deity.hpp"
#include "Deity/Deity.hpp"
#include "World/World.hpp"

#include <iostream>
#include <fstream>
#include <random>
#include <regex>

namespace dty {
	Tile::Tile(Deity& _deity) : deity(_deity) {
		Ref<IsometricPlane> isoPlane = ((World*)(deity.GetGUILayers()[GUILayer::WORLD].get()))->isometricPlane;

		ground = CreateRef<Animation>(isoPlane);
		object = CreateRef<Animation>(isoPlane);
		effect = CreateRef<Animation>(isoPlane);
	}

	void IsometricPlane::InitializeWorld(Ref<WorldFileIO> _worldFile) {
		worldFile = _worldFile;

		PropertyArray worldSize = worldFile->GetPropertyArray("s", 2);
		gridSize = {
			(uint32_t)worldSize[0][0],
			(uint32_t)worldSize[0][1]
		};

		uint32_t x = 0, y = 0;
		PropertyArray properties = worldFile->GetPropertyArray("t", 3);
		for (auto& prop : properties) {
			Ref<Tile> tile = CreateRef<Tile>(deity);

			tile->ground->AppendSprite((SpriteName)prop[0], deity.spriteLoader);
			tile->object->AppendSprite((SpriteName)prop[1], deity.spriteLoader);
			tile->effect->AppendSprite((SpriteName)prop[2], deity.spriteLoader);

			tile->ground->SetGridPositions({ (uint32_t)x, (uint32_t)y });
			tile->ground->SetSizes({
				(uint32_t)tile->ground->GetCurrentSprite()->olcDecal->sprite->width,
				(uint32_t)tile->ground->GetCurrentSprite()->olcDecal->sprite->height
				});

			tile->object->SetGridPositions({ (uint32_t)x, (uint32_t)y });
			tile->object->SetSizes({
				deity.config.baseTileSize.x,
				deity.config.baseTileSize.y
			});

			tile->effect->SetGridPositions({ (uint32_t)x, (uint32_t)y});
			tile->effect->SetSizes({
				64,
				128
			});

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

		std::random_device randomDevice;
		std::mt19937 mersenne(randomDevice());
		std::uniform_int_distribution<int> dist(1, 1000);

		if (pge.GetKey(olc::F2).bPressed)
			gridOffset = { 700, -1200 };

		hoverSprite->SetCurrentSprite(SpriteName::NO_SPRITE, deity.spriteLoader);
		for (Ref<Tile> tile : tiles) {
			if ( // Check if the cursor is over the tile - Such a bodge
				(int32_t)((mousePosition.x - 16) - gridOffset.x > tile->ground->GetCurrentSprite()->position.x) &&
				(int32_t)((mousePosition.x - 16) - gridOffset.x < tile->ground->GetCurrentSprite()->position.x + (deity.config.baseTileSize.x * 0.5)) &&
				(int32_t)((mousePosition.y - 16) - gridOffset.y > tile->ground->GetCurrentSprite()->position.y) &&
				(int32_t)((mousePosition.y - 16) - gridOffset.y < tile->ground->GetCurrentSprite()->position.y + deity.config.baseTileSize.y)
			) {
				HoveringOverTile(tile); // Pass the tile the user is hovering
			}

			DrawTile(tile); // Draw tile

			// Fire spread
			if (tile->object->GetSprites().size()) {
				if (tile->object->GetCurrentSprite()->GetCurrentSprite() >= SpriteName::FIRE_1 && tile->ground->GetCurrentSprite()->GetCurrentSprite() <= SpriteName::FIRE_4) {
					Vector2u tilePosition = tile->ground->GetSprites()[0]->GetGridPosition();
					for (int32_t x = tilePosition.x - 1; x <= (int32_t)tilePosition.x + 1; ++x) {
						for (int32_t y = tilePosition.y - 1; y <= (int32_t)tilePosition.y + 1; ++y) {
							if (x < 0 || y < 0)
								continue;

							if (x > 99 || y > 99)
								continue;

							if (x == (int32_t)tilePosition.x && y == (int32_t)tilePosition.y)
								continue;

							int random = dist(mersenne);
							if (random == 1) {
								Ref<Tile> tileToIgnite = tiles[y * 100 + x];
								IgniteTile(tileToIgnite);
							}
						}
					}
				}
			}
		}
		
		DrawHoverSprite(); // Draw the hovering sprite to indicate which tile you're over

		pge.SetPixelMode(olc::Pixel::NORMAL);
		pge.SetDrawTarget(deity.renderLayers[0]);
	}

	void IsometricPlane::DrawTile(Ref<Tile> _tile) {
		olc::PixelGameEngine& pge = deity.getPGE();

		double viewBuffer = deity.config.baseTileSize.x;
		if ( // If the tile can be seen (Culling)
			_tile->ground->GetCurrentSprite()->position.x + gridOffset.x + position.x > (int32_t)(position.x - viewBuffer) &&
			_tile->ground->GetCurrentSprite()->position.x + gridOffset.x - position.x < (int32_t)(position.x + size.x + viewBuffer) &&
			_tile->ground->GetCurrentSprite()->position.y + gridOffset.y + position.y > (int32_t)(position.y - viewBuffer) &&
			_tile->ground->GetCurrentSprite()->position.y + gridOffset.y - position.y < (int32_t)(position.y + size.y + viewBuffer)
		) {
			Vector2 spritePos = _tile->ground->GetCurrentSprite()->position + gridOffset;
			const float& deltaTime = pge.GetElapsedTime();

			_tile->ground->Update(deltaTime);
			_tile->object->Update(deltaTime);
			_tile->effect->Update(deltaTime);

			_tile->ground->GetCurrentSprite()->Draw(pge, spritePos);

			if (_tile->object->GetCurrentSprite().get())
				_tile->object->GetCurrentSprite()->Draw(pge, spritePos);

			if(_tile->effect->GetCurrentSprite().get())
				_tile->effect->GetCurrentSprite()->Draw(pge, spritePos + Vector2(0, -32)); // Hack ;-;
		}
	}

	int32_t currentSprite = 0;
	double saveTimer = 0;
	void IsometricPlane::DrawHoverSprite() {
		olc::PixelGameEngine& pge = deity.getPGE();

		pge.SetDrawTarget(deity.renderLayers[1]);
		hoverSprite->Draw(pge, hoverSprite->position + gridOffset);
		pge.SetDrawTarget(deity.renderLayers[0]);

		if (saveTimer > 0) {
			pge.DrawString({ (int32_t)position.x + 10, (int32_t)position.y + 30 }, "Saved!", olc::DARK_BLUE, 2U);
			saveTimer -= pge.GetElapsedTime();
		}

		pge.DrawString({ (int32_t)position.x + 10, (int32_t)position.y + 10 }, deity.spriteLoader->GetSpritePath((SpriteName)currentSprite), olc::DARK_BLUE, 2U);
	}

	void IsometricPlane::HoveringOverTile(Ref<Tile> _tile) {
		olc::PixelGameEngine& pge = deity.getPGE();

		hoverSprite->SetGridPosition(_tile->ground->GetCurrentSprite()->GetGridPosition());
		hoverSprite->SetCurrentSprite(SpriteName::HOVERED, deity.spriteLoader);

		if (int32_t mouseWheelDelta = pge.GetMouseWheel()) {
			if(mouseWheelDelta > 0)
				currentSprite -= (mouseWheelDelta / mouseWheelDelta);
			else
				currentSprite += (mouseWheelDelta / mouseWheelDelta);


			if (currentSprite < (int32_t)SpriteName::NO_SPRITE) {
				currentSprite = (int32_t)SpriteName::SIZE - 1;
			}
			else if (currentSprite > (int32_t)SpriteName::SIZE - 1) {
				currentSprite = (int32_t)SpriteName::NO_SPRITE;
			}
		}

		if (pge.GetMouse(olc::Mouse::LEFT).bPressed) {
			if (_tile->ground->GetCurrentSprite()->GetCurrentSprite() != SpriteName::WATER_L0_1)
				SetTileEffect(_tile, SpriteName::LIGHTNING_1);
		}

		/*
		*	Edit mode: Change ground tiles
		* 
			if (pge.GetKey(olc::Key::SHIFT).bHeld ? pge.GetMouse(olc::Mouse::LEFT).bHeld : pge.GetMouse(olc::Mouse::LEFT).bPressed) {
				SetTileGround(_tile, (SpriteName)currentSprite);
				worldFile->SetTileElements(
					_tile->ground->GetCurrentSprite()->GetGridPosition(),
					{
						(int32_t)_tile->ground->GetCurrentSprite()->GetCurrentSprite(),
						(int32_t)_tile->object->GetCurrentSprite()->GetCurrentSprite(),
						0
					}
				);
			}
		*/

		/*
		*	Edit mode: Change object tile
		* 
			if (pge.GetKey(olc::Key::SHIFT).bHeld ? pge.GetMouse(olc::Mouse::RIGHT).bHeld : pge.GetMouse(olc::Mouse::RIGHT).bPressed) {
				SetTileObject(_tile, (SpriteName)currentSprite);

				worldFile->SetTileElements(
					_tile->ground->GetCurrentSprite()->GetGridPosition(),
					{
						(int32_t)_tile->ground->GetCurrentSprite()->GetCurrentSprite(),
						(int32_t)_tile->object->GetCurrentSprite()->GetCurrentSprite(),
						0
					}
				);
			}
		*/

		/*
		*	Edit mode: Save world
		* 
			if (pge.GetKey(olc::ENTER).bPressed) {
				worldFile->Save();
				saveTimer = 1.5;
			}
		*/
	}

	void IsometricPlane::SetTileGround(Ref<Tile> _tile, SpriteName _spriteName) {
		_tile->ground->Clear();
		_tile->ground->AppendSprite(_spriteName, deity.spriteLoader);

		worldFile->SetTileElements(_tile->ground->GetCurrentSprite()->GetGridPosition(),
			{
				(int32_t)_tile->ground->GetCurrentSprite()->GetCurrentSprite(),
				(int32_t)_tile->ground->GetCurrentSprite()->GetCurrentSprite(),
				0
			}
		);
	}

	void IsometricPlane::SetTileObject(Ref<Tile> _tile, SpriteName _spriteName) {
		_tile->object->Clear();

		if ((int32_t)_spriteName >= (int32_t)SpriteName::FIRE_1 && (int32_t)_spriteName <= (int32_t)SpriteName::FIRE_4) {
			_tile->object->AppendSprite(SpriteName::FIRE_1, deity.spriteLoader);
			_tile->object->AppendSprite(SpriteName::FIRE_2, deity.spriteLoader);
			_tile->object->AppendSprite(SpriteName::FIRE_3, deity.spriteLoader);
			_tile->object->AppendSprite(SpriteName::FIRE_4, deity.spriteLoader);

			_tile->object->SetLifetimeLimit(15.0f);
		}
		else
			_tile->object->AppendSprite(_spriteName, deity.spriteLoader);

		worldFile->SetTileElements(
			_tile->object->GetCurrentSprite()->GetGridPosition(),
			{
				(int32_t)_tile->object->GetCurrentSprite()->GetCurrentSprite(),
				(int32_t)_tile->object->GetCurrentSprite()->GetCurrentSprite(),
				0
			}
		);
	}

	void IsometricPlane::SetTileEffect(Ref<Tile> _tile, SpriteName _spriteName) {
		_tile->effect->Clear();

		if ((int32_t)_spriteName >= (int32_t)SpriteName::LIGHTNING_1 && (int32_t)_spriteName <= (int32_t)SpriteName::LIGHTNING_3) {
			_tile->effect->AppendSprite(SpriteName::LIGHTNING_1, deity.spriteLoader);
			_tile->effect->AppendSprite(SpriteName::LIGHTNING_2, deity.spriteLoader);
			_tile->effect->AppendSprite(SpriteName::LIGHTNING_3, deity.spriteLoader);

			SetTileObject(_tile, SpriteName::FIRE_1);
		}
		else
			_tile->effect->AppendSprite(_spriteName, deity.spriteLoader);

		worldFile->SetTileElements(
			_tile->effect->GetCurrentSprite()->GetGridPosition(),
			{
				(int32_t)_tile->effect->GetCurrentSprite()->GetCurrentSprite(),
				(int32_t)_tile->effect->GetCurrentSprite()->GetCurrentSprite(),
				0
			}
		);

		_tile->effect->SetLifetimeLimit(0.5f);
	}

	void IsometricPlane::IgniteTile(Ref<Tile> _tile) {
		bool isBanned = false;
		std::vector<SpriteName> bannedFireTiles = {
			SpriteName::WATER_L0_1,
			SpriteName::GRAVEL_L0_1
		};

		for (SpriteName bannedTile : bannedFireTiles)
			if (_tile->ground->GetCurrentSprite()->GetCurrentSprite() == bannedTile)
				isBanned = true;

		if (!isBanned) {
			SetTileObject(_tile, SpriteName::FIRE_1);

			if (
				_tile->ground->GetCurrentSprite()->GetCurrentSprite() >= SpriteName::GRASS_L0_1 &&
				_tile->ground->GetCurrentSprite()->GetCurrentSprite() <= SpriteName::GRASS_L0_4
			) {
				// Set the tile to dirt
			}
		}
	}
}