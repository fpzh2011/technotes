#include "HowEasy.h"
#include "gtest/gtest.h"

TEST (HowEasyTest, Basic) {
	HowEasy how;
	EXPECT_EQ(500, how.pointVal("this is a problem statement."));
}

TEST (HowEasyTest, singleWord) {
	HowEasy how;
	EXPECT_EQ(250, how.pointVal("a"));
	EXPECT_EQ(250, how.pointVal("ab"));
	EXPECT_EQ(250, how.pointVal("."));
	EXPECT_EQ(250, how.pointVal("a."));
	EXPECT_EQ(250, how.pointVal("ab."));
	EXPECT_EQ(250, how.pointVal("a.."));
	EXPECT_EQ(250, how.pointVal("ab.."));
}

TEST (HowEasyTest, multiWord) {
	HowEasy how;
	EXPECT_EQ(500, how.pointVal("This is a problem statement"));
	EXPECT_EQ(500, how.pointVal("Implement a class H5 which contains some method."));
	EXPECT_EQ(1000, how.pointVal("Implementaclass"));
}
