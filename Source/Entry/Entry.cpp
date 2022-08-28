#include <iostream>
#include <cstdint>

#include "Deity/Deity.hpp"

int32_t main() {
	dty::Deity* deity = new dty::Deity();
	deity->Lifetime();
	delete deity;

	return 0;
}