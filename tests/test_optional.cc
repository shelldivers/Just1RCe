
#include <gtest/gtest.h>
#include <string>
#include <vector>
#include "../includes/ft_optional.h"

TEST(FTOptionalTest, StringConstruction) {
    ft::optional<std::string> test("test construction");
    EXPECT_TRUE(test.has_value());
    EXPECT_EQ(test.value(), "test construction");
}

TEST(FTOptionalTest, DefaultInitialization) {
    ft::optional<std::string> test1;
    EXPECT_FALSE(test1.has_value());

    ft::optional<std::string> test2("hello world");
    EXPECT_TRUE(test2.has_value());
    EXPECT_EQ(test2.value(), "hello world");
    EXPECT_EQ(*test2, "hello world");
}

TEST(FTOptionalTest, ConstReferenceAccess) {
    ft::optional<std::string> test1("hello world");
    const ft::optional<std::string>& ref_test1 = test1;

    EXPECT_TRUE(ref_test1.has_value());
    EXPECT_EQ(ref_test1.value(), "hello world");
    EXPECT_EQ(*ref_test1, "hello world");
}

TEST(FTOptionalTest, MutabilityTest) {
    ft::optional<std::string> test1("hello world");
    *test1 = "goodbye world";

    EXPECT_TRUE(test1.has_value());
    EXPECT_EQ(test1.value(), "goodbye world");
    EXPECT_EQ(*test1, "goodbye world");
}

TEST(FTOptionalTest, PrimitiveTypeHandling) {
    ft::optional<int> test2(0);
    EXPECT_TRUE(test2.has_value());
    EXPECT_EQ(test2.value(), 0);
}

TEST(FTOptionalTest, VectorStringHandling) {
    ft::optional<std::vector<std::string>> test3((std::vector<std::string>()));
    (*test3).push_back("hello");

    EXPECT_TRUE(test3.has_value());
    EXPECT_EQ((*test3)[0], "hello");
}

// Google Test 실행을 위한 메인 함수
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);  // Google Test 초기화
    return RUN_ALL_TESTS();                 // 모든 테스트 실행
}

