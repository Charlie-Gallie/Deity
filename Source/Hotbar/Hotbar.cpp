#include "Hotbar.hpp"

#include "Deity/Deity.hpp"

namespace dty {
	void Hotbar::Update() {
		olc::PixelGameEngine& pge = deity.getPGE();

		pge.FillRect(position, size, olc::RED);
	}
}