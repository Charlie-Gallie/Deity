#pragma once

#include <olcPixelGameEngine.h>

#include "Miscellaneous/Vector2.hpp"

namespace dty {
	class Deity;

	/*
		This class may only exist as an inherited class.
		The constructor is left public for ease-of-use
		in subclasses.

		Make sure to include the Deity header if you want
		to use it, it's only forward declared in here.
	*/
	class GUIBase {
	public:
		GUIBase(Deity& _deity) : deity(_deity) {};

		virtual void Update() {}
		virtual void Initialize() {};

		Vector2u
			position,
			size;
	protected:
		virtual ~GUIBase() {};

		Deity& deity;
	};
}