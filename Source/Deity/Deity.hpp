#pragma once

#include <olcPixelGameEngine.h>
#include <unordered_map>

#include "Miscellaneous/Configuration.hpp"
#include "Miscellaneous/Memory.hpp"
#include "World/World.hpp"

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

		olc::PixelGameEngine& getPGE();

		Configuration config;
	private:
		/*
			A GUI layer is any class which inherits from GUI.
			A GUILayer is just a key to signify a specific layer.
		*/
		std::unordered_map<GUILayer, Ref<GUI>> guiLayers;
	};
}