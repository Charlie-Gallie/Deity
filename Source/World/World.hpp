#pragma once

#include "GUI/GUI.hpp"

namespace dty {
	class World : public GUI {
	public:
		using GUI::GUI;

		void Update() override;
	private:

	};
}