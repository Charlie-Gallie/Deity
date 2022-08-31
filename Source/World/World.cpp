#include "World.hpp"

#include <cmath>
#include <string>
#include <regex>

#include "Miscellaneous/WorldFileIO.hpp"
#include "GUI/SpriteLoader.hpp"
#include "Deity/Deity.hpp"

namespace dty {
	void World::Initialize() {
		isometricPlane = CreateRef<IsometricPlane>(deity);
		isometricPlane->position = position;
		isometricPlane->size = size;

		std::string worldPath = deity.config.assetsFolder + deity.config.worldFile;

		worldFile = CreateRef<WorldFileIO>();
		worldFile->LoadTiles("t"); // Load tiles by token "t"
		if (!worldFile->Open(worldPath))
			std::cout << "Cannot open file" << std::endl;

		isometricPlane->InitializeWorld(worldFile);
	}

	void World::Update() {
		olc::PixelGameEngine& pge = deity.getPGE();

		double deltaTime = deity.GetElapsedTime();

		double moveSpeedThisFrame = worldMoveSpeed * (deltaTime * 2);
		if (pge.GetKey(olc::W).bHeld) isometricPlane->gridOffset.y += moveSpeedThisFrame;
		if (pge.GetKey(olc::S).bHeld) isometricPlane->gridOffset.y -= moveSpeedThisFrame;
		if (pge.GetKey(olc::A).bHeld) isometricPlane->gridOffset.x += moveSpeedThisFrame;
		if (pge.GetKey(olc::D).bHeld) isometricPlane->gridOffset.x -= moveSpeedThisFrame;

		isometricPlane->Update();
	}
}