#pragma once

#include <olcPixelGameEngine.h>
#include <string>

#include "GUI/SpriteLoader.hpp"
#include "Miscellaneous/Vector2.hpp"
#include "Miscellaneous/Memory.hpp"

namespace dty {
	class Sprite {
	public:
		Sprite() = default;
		Sprite(std::string _path) : path(_path) {};

		Ref<olc::Decal> olcDecal;
		Ref<olc::Sprite> olcSprite;

		Vector2 position = 0;
		Vector2u size = 0;
		SpriteName spriteName = SpriteName::NO_SPRITE;

		std::string path;

		void SetGridPosition(Vector2u _position) {
			gridPosition = _position;

			position = {
				(double)(_position.x * 0.5 * 64 + _position.y * -0.5 * 64),
				(double)(_position.x * 0.5 * 32 + _position.y * 0.5 * 32)
			};
		}

		void Draw(olc::PixelGameEngine& _pge, Vector2 _position) {
			if (olcDecal.get() && spriteName != SpriteName::NO_SPRITE) // fix
				_pge.DrawDecal(_position, olcDecal.get());
			else
				std::cout << "No valid decal" << std::endl;
		}

		Vector2u GetGridPosition() {
			return gridPosition;
		}

	private:
		Vector2u gridPosition = 0;
	};
}