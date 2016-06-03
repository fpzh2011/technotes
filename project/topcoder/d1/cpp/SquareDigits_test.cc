#include "SquareDigits.h"
#include "gtest/gtest.h"

TEST (HowEasyTest, Basic) {
	SquareDigits s;
	EXPECT_EQ(0, s.smallestResult(0));
	EXPECT_EQ(1, s.smallestResult(1));
	EXPECT_EQ(133, s.smallestResult(19));
	//EXPECT_EQ(0, s.smallestResult(123));
	EXPECT_EQ(2, s.smallestResult(37));
	EXPECT_EQ(5, s.smallestResult(85));
	EXPECT_EQ(2666, s.smallestResult(112));
}
