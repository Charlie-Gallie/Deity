#include "Deity/Deity.hpp"

int main() {
	dty::Deity* deity = new dty::Deity();
	deity->Lifetime();
	delete deity;

	return 0;
}