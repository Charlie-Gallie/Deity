#pragma once

#include <olcPixelGameEngine.h>
#include <unordered_map>
#include <vector>

#include "Miscellaneous/Configuration.hpp"
#include "Miscellaneous/Memory.hpp"
#include "GUI/SpriteLoader.hpp"
#include "GUI/GUIBase.hpp"

namespace dty {
	enum class GUILayer {
		HOTBAR,
		WORLD
	};

	/*
		Members of this class may forward declare this class
		as a reference of this is passed into the member.

		This is done so the member may draw to the window, take
		inputs, modify values, etc.
	*/
	class Deity : public olc::PixelGameEngine {
	public:
		Deity();
		~Deity();

		bool OnUserUpdate(float) override;
		bool OnUserCreate() override;
		void Lifetime();
		void Initialize();

		olc::PixelGameEngine& getPGE();

		Configuration config;
		Ref<SpriteLoader> spriteLoader;
		std::vector<uint32_t> renderLayers = { 0 };
	private:
		/*
			A GUI layer is any class which inherits from GUI.
			A GUILayer is just a key to signify a specific layer.
		*/
		std::unordered_map<GUILayer, Ref<GUIBase>> guiLayers;


		// TODO: Create a game state. This will hold the currently selected power,
		//       loaded sprites, and whatever else.
	};
}