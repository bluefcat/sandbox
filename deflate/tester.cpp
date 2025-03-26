#include "gtest/gtest.h"
#include "include/lz77.h"

testing::AssertionResult compress(
	std::string target
){
	Deflate::LZ77<std::string> lz{10, 20};
	lz.encode(target);
	std::string result = lz.decode();
	if(result != target)
		return testing::AssertionFailure() << lz.decode() << " != " << target;
	return testing::AssertionSuccess();
}

TEST(SimpleStringTest, HandleVariableValue){
	EXPECT_TRUE(compress("AAAA"));
	EXPECT_TRUE(compress("HELLO, WORLD!"));
}

int main(int argc, char** argv){
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
