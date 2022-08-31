#pragma once

#include <deque>

#include "Miscellaneous/Matrix.hpp"
#include "IsometricPlane.hpp"

namespace dty {
	class World : public GUIBase {
	public:
		using GUIBase::GUIBase;

		void Update() override;
		void Initialize() override;

		Ref<IsometricPlane> isometricPlane;
	private:
		Ref<WorldFileIO> worldFile;
		double worldMoveSpeed = 250.0;
	};
}