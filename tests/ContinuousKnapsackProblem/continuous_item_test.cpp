#include "ContinuousKnapsackProblem/continuous_item.h"
#include <gtest/gtest.h>
#include <random>

/**
 * @brief Generates a random double in the given range.
 */
double getRandomDouble(double min, double max)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(min, max);
    return dis(gen);
}

TEST(ContinuousItemTest, ConstructorSetsFields)
{
    double randomValueIndex = getRandomDouble(1.0, 10.0);
    double randomWeight = getRandomDouble(1.0, 20.0);

    ContinuousItem item(randomValueIndex, randomWeight);

    EXPECT_DOUBLE_EQ(item.getValueIndex(), randomValueIndex);
    EXPECT_DOUBLE_EQ(item.getWeight(), randomWeight);
}

TEST(ContinuousItemTest, GettersReturnCorrectValues)
{
    double valueIndex = getRandomDouble(1.0, 15.0);
    double weight = getRandomDouble(5.0, 25.0);

    ContinuousItem item(valueIndex, weight);

    EXPECT_DOUBLE_EQ(item.getValueIndex(), valueIndex);
    EXPECT_DOUBLE_EQ(item.getWeight(), weight);
}

TEST(ContinuousItemTest, SettersModifyValues)
{
    ContinuousItem item(1.0, 1.0);

    double newValueIndex = getRandomDouble(10.0, 20.0);
    double newWeight = getRandomDouble(30.0, 40.0);

    item.setValueIndex(newValueIndex);
    item.setWeight(newWeight);

    EXPECT_DOUBLE_EQ(item.getValueIndex(), newValueIndex);
    EXPECT_DOUBLE_EQ(item.getWeight(), newWeight);
}
