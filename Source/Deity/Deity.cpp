#define OLC_PGE_APPLICATION
#include "Deity.hpp"

#include "World/World.hpp"

namespace dty {
	Deity::Deity() {}
	Deity::~Deity() {}

	bool Deity::OnUserCreate() {
		sAppName = config.title;

		Initialize();

		renderLayers.push_back(CreateLayer());

		for (uint32_t layer : renderLayers)
			EnableLayer(layer, true);

		return true;
	}

	bool Deity::OnUserUpdate(float deltaTime) {
		Clear(config.clearColor);

		for (const auto& guiLayerPair : guiLayers) {
			Ref<GUIBase> guiLayer = guiLayerPair.second;
			guiLayer->Update();
		}

		return true;
	}

	void Deity::Initialize() {
		spriteLoader = CreateRef<SpriteLoader>();
		spriteLoader->Initalize(); // This will load all of the sprites from file

		Ref<World> world = CreateRef<World>(*this);

		// Configure world
		world->position = {
			0,
			0
		};
		world->size = {
			config.windowSize.width,
			config.windowSize.height
		};

		// There was going to be a hotbar for different weather options but I didn't have time
		// 
		/*
			Ref<Hotbar> hotbar = CreateRef<Hotbar>(*this); 

			hotbar->Initialize();
			hotbar->position = 0;
			hotbar->size = {
				(uint32_t)((config.windowSize.width / 16) * 3),
				config.windowSize.height
			};

			guiLayers[GUILayer::HOTBAR] = static_cast<Ref<GUIBase>>(hotbar);
			hotabr->Initialize();
		*/

		guiLayers[GUILayer::WORLD] = static_cast<Ref<GUIBase>>(world);
		world->Initialize();
	}

	void Deity::Lifetime() {
		this->Construct(
			config.windowSize.width,	// Window width
			config.windowSize.height,	// Window height
			1,							// Pixel X scale
			1,							// Pixel Y scale
			false,						// Fullscreen enabled
			true,						// VSync enabled // Todo: Enable this at production
			false						// Maintain size enabled
		);

		this->Start();
	}

	olc::PixelGameEngine& Deity::getPGE() {
		return static_cast<olc::PixelGameEngine&>(*this);
	}

	std::unordered_map<GUILayer, Ref<GUIBase>> Deity::GetGUILayers() {
		return guiLayers;
	}
}