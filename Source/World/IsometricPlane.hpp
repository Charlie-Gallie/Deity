#pragma once

#include <deque>

#include "GUI/Animation.hpp"

#include "Miscellaneous/WorldFileIO.hpp"
#include "Miscellaneous/Matrix.hpp"
#include "Miscellaneous/Memory.hpp"
#include "GUI/Animation.hpp"
#include "GUI/GUIBase.hpp"
#include "GUI/Sprite.hpp"

namespace dty {
	class Deity;
	class Tile {
	public:
		Tile(Deity& _deity);

		Vector2i position = 0;

		Ref<Animation>
			ground,
			object,
			effect;

	private:
		Deity& deity;
	};

	class IsometricPlane : public GUIBase {
	public:
		using GUIBase::GUIBase;

		// Alter the grid size prior to calling Initialize()
		Vector2u gridSize = { 100, 100 };
		Vector2 gridOffset = 0;
		
		void InitializeWorld(Ref<WorldFileIO> _worldFile);
		void Initialize() override;
		void Update() override;
	private:
		Ref<WorldFileIO> worldFile;
		std::deque<Ref<Tile>> tiles;
		Ref<Sprite> hoverSprite = CreateRef<Sprite>();

		void IgniteTile(Ref<Tile>);
		void SetTileGround(Ref<Tile>, SpriteName);
		void SetTileObject(Ref<Tile>, SpriteName);
		void SetTileEffect(Ref<Tile>, SpriteName);

		void DrawTile(Ref<Tile>);
		void DrawHoverSprite();
		void HoveringOverTile(Ref<Tile>);
	};
}