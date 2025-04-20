#include "PLQT/power_of_two.h"
#include <gtest/gtest.h>
#include <random>

void checkFiveRandomPowers(const PowerOfTwo& potencias, size_t count) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, static_cast<int>(count - 1));

    for (int i = 0; i < 5; ++i) {
        size_t idx = dis(gen);
        uint64_t expected = 1ULL << idx;
        uint64_t actual = potencias.getPower(idx);
        EXPECT_EQ(actual, expected) << "Mismatch at index " << idx;
    }
}

TEST(PowerOfTwoTest, ConstructorAndGetters_Small) {
    size_t count = 8;
    PowerOfTwo potencias(count);
    
    EXPECT_EQ(potencias.getNumPowers(), count);
    checkFiveRandomPowers(potencias, count);
}

TEST(PowerOfTwoTest, ConstructorAndGetters_Medium) {
    size_t count = 32;
    PowerOfTwo potencias(count);

    EXPECT_EQ(potencias.getNumPowers(), count);
    checkFiveRandomPowers(potencias, count);
}

TEST(PowerOfTwoTest, ConstructorAndGetters_Large) {
    size_t count = 64;
    PowerOfTwo potencias(count);

    EXPECT_EQ(potencias.getNumPowers(), count);
    checkFiveRandomPowers(potencias, count);
}

TEST(PowerOfTwoTest, ThrowsOnInvalidSize) {
    EXPECT_THROW(PowerOfTwo potencias(65), std::out_of_range);
}

TEST(PowerOfTwoTest, ThrowsOnOutOfBoundsAccess) {
    PowerOfTwo potencias(16);
    EXPECT_THROW(potencias.getPower(16), std::out_of_range);
}
