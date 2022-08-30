#pragma once

#include <olcPixelGameEngine.h>
#include <string>

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

		std::string path;
	};
}