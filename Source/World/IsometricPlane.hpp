#pragma once

#include <deque>

#include "Miscellaneous/WorldFileIO.hpp"
#include "Miscellaneous/Matrix.hpp"
#include "Miscellaneous/Memory.hpp"
#include "GUI/GUIBase.hpp"
#include "GUI/Sprite.hpp"

namespace dty {
	class Tile {
	public:
		Ref<Sprite> ground = CreateRef<Sprite>();
		Ref<Sprite> object = CreateRef<Sprite>();
		// Animation object layer
		// Effect layer
	};

	class IsometricPlane : public GUIBase {
	public:
		using GUIBase::GUIBase;

		// Alter the grid size prior to calling Initialize()
		Vector2u gridSize = { 100, 100 };
		Vector2 gridOffset;

		void InitializeWorld(Ref<WorldFileIO> _worldFile);
		void Initialize() override;
		void Update() override;
	private:
		Ref<WorldFileIO> worldFile;
		std::deque<Ref<Tile>> tiles;
	};
}