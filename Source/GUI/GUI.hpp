#pragma once

#include <olcPixelGameEngine.h>

namespace dty {
	class Deity;

	/*
		This class may only exist as an inherited class.
		The constructor is left public for ease-of-use
		in subclasses.

		Make sure to include the Deity header if you want
		to use it, it's only forward declared in here.
	*/
	class GUI {
	public:
		GUI(Deity& _deity) : deity(_deity) {};

		virtual void Update() {}
	protected:
		virtual ~GUI() {};

		Deity& deity;
	};
}