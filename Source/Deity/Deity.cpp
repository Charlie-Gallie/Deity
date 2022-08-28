#define OLC_PGE_APPLICATION
#include "Deity.hpp"

namespace dty {
	Deity::Deity() {
		guiLayers[GUILayer::WORLD] = CreateRef<World>(*this);
	}

	Deity::~Deity() {}

	bool Deity::OnUserCreate() {
		sAppName = config.title;

		return true;
	}

	bool Deity::OnUserUpdate(float deltaTime) {
		Clear(config.clearColor);

		for (const auto& guiLayerPair : guiLayers) {
			Ref<GUI> guiLayer = guiLayerPair.second;
			guiLayer->Update();
		}

		// Update game space
		// Update ui

		return true;
	}

	void Deity::Lifetime() {
		this->Construct(
			config.window.width,	// Window width
			config.window.height,	// Window height
			1,						// Pixel X scale
			1,						// Pixel Y scale
			false,					// Fullscreen enabled
			true,					// VSync enabled
			true					// Maintain aspect ratio enabled
		);

		this->Start();
	}

	olc::PixelGameEngine& Deity::getPGE() {
		return static_cast<olc::PixelGameEngine&>(*this);
	}
}