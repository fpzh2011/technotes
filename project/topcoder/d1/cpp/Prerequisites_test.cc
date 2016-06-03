#include "Prerequisites.h"
#include "gtest/gtest.h"
#include <string>
bool cmpclass (std::string class1, std::string class2); 
TEST (Prerequisites, Basic) {
	//std::string a[] = {"ABC123"};
	std::string s = "ABC123";
	bool b = cmpclass(s, s);
	EXPECT_EQ(true, b);
}
