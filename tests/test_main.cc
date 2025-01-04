#include <gtest/gtest.h>

// Google Test 실행을 위한 메인 함수
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);  // Google Test 초기화
    return RUN_ALL_TESTS();                 // 모든 테스트 실행
}