#include "World.hpp"

#include <cmath>
#include <string>
#include <regex>

#include "Miscellaneous/WorldFileIO.hpp"
#include "GUI/SpriteLoader.hpp"
#include "Deity/Deity.hpp"

namespace dty {
	void World::Initialize() {
		std::string worldPath = deity.config.assetsFolder + deity.config.worldFile;

		worldFile = CreateRef<WorldFileIO>();
		if (!worldFile->Open(worldPath))
			std::cout << "Cannot open file" << std::endl;
		worldFile->LoadTiles("t"); // Load tiles by token "t"

		isometricPlane = CreateRef<IsometricPlane>(deity);
		isometricPlane->position = position;
		isometricPlane->size = size;

		isometricPlane->InitializeWorld(worldFile);
	}

	void World::Update() {
		olc::PixelGameEngine& pge = deity.getPGE();

		double deltaTime = deity.GetElapsedTime();

		Vector2 mousePosition = { (double)pge.GetMouseX(), (double)pge.GetMouseY() };

		/*		World Movement		*/

		// Mouse
		if (pge.GetMouse(olc::Mouse::MIDDLE).bPressed)
			mouseDragStartPosition = mousePosition;
		
		if (pge.GetMouse(olc::Mouse::MIDDLE).bHeld) {
			Vector2 mouseDragDifference = mousePosition - mouseDragStartPosition;

			isometricPlane->gridOffset += mouseDragDifference;
			mouseDragStartPosition = mousePosition;
		}

		if (pge.GetMouse(olc::Mouse::MIDDLE).bReleased)
			mouseDragStartPosition = 0;

		// Keyboard
		double moveSpeedThisFrame = worldMoveSpeed * (deltaTime * 2);
		if (pge.GetKey(olc::W).bHeld) isometricPlane->gridOffset.y += moveSpeedThisFrame;
		if (pge.GetKey(olc::S).bHeld) isometricPlane->gridOffset.y -= moveSpeedThisFrame;
		if (pge.GetKey(olc::A).bHeld) isometricPlane->gridOffset.x += moveSpeedThisFrame;
		if (pge.GetKey(olc::D).bHeld) isometricPlane->gridOffset.x -= moveSpeedThisFrame;

		isometricPlane->Update();
	}
}