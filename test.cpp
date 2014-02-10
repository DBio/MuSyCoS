/*
 * Copyright (c) 2014 - Adam Streck 
 * MuSyCoS - Multivalued Synchronous Networks Constraint Solver 
 * MuSyCoS is a free software: you can redistribute it and/or modify it under the terms of the GNU General Public License version 3. 
 * MuSyCoS is released without any warranty. See the GNU General Public License for more details. <http://www.gnu.org/licenses/>. 
 * For affiliations see <http://www.mi.fu-berlin.de/en/math/groups/dibimath> 
 */
#include "test/parsing_test.hpp"

int main(int argc, char* argv[]) {
	::testing::InitGoogleTest(&argc, argv);
	int result = RUN_ALL_TESTS();

#if (_MSC_VER == 1800)
	cin.get();
#endif 
	return result;
}
