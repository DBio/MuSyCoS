/*
* Copyright (C) 2012-2014 - Adam Streck
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
