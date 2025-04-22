#include "ContinuousKnapsackProblem/continuous_item.h"
#include "ContinuousKnapsackProblem/continuous_knapsack.h"
#include <gtest/gtest.h>
#include <vector>
#include <cmath>

const double EPSILON = 1e-4;  ///< Error aceptable para comparación de doubles

/// @brief Calcula el beneficio total de una solución
double computeTotalValue(const std::vector<ContinuousItem>& items, const std::vector<std::pair<int, double>>& result)
{
    double total = 0.0;
    for (const auto& [index, amount] : result)
        total += items[index].getValueIndex() * amount;
    return total;
}

/// @brief Calcula el peso total usado en una solución
double computeTotalWeight(const std::vector<ContinuousItem>& items, const std::vector<std::pair<int, double>>& result)
{
    double total = 0.0;
    for (const auto& [index, amount] : result)
        total += amount;
    return total;
}

TEST(ContinuousKnapsackProblemTest, ConstructorSetsFields)
{
    std::vector<ContinuousItem> items = {
        ContinuousItem(3.0, 10.0),
        ContinuousItem(2.0, 5.0)
    };
    double capacity = 15.0;

    ContinuousKnapsack problem(capacity, items);

    EXPECT_DOUBLE_EQ(problem.getCapacity(), capacity);
    EXPECT_EQ(problem.getItems().size(), items.size());
}

TEST(ContinuousKnapsackProblemTest, Example1)
{
    std::vector<ContinuousItem> items = {
        ContinuousItem(3.0, 20.0),   // value = 60
        ContinuousItem(2.0, 50.0),   // value = 100
        ContinuousItem(4.0, 30.0)    // value = 120
    };
    double capacity = 50.0;

    ContinuousKnapsack problem(capacity, items);
    auto result = problem.solve();

    EXPECT_NEAR(computeTotalValue(items, result), 180.0, EPSILON);
}

TEST(ContinuousKnapsackProblemTest, Example2)
{
    std::vector<ContinuousItem> items = {
        ContinuousItem(500.0 / 30.0, 30.0) // value = 500
    };
    double capacity = 10.0;

    ContinuousKnapsack problem(capacity, items);
    auto result = problem.solve();

    EXPECT_NEAR(computeTotalValue(items, result), 166.6667, EPSILON);
}

TEST(ContinuousKnapsackProblemTest, GreedySelectionWithMultipleItems)
{
    std::vector<ContinuousItem> items = {
        ContinuousItem(6.0, 1.0),
        ContinuousItem(4.5, 2.0),
        ContinuousItem(3.0, 1.0),
        ContinuousItem(1.78, 9.0),
        ContinuousItem(1.0, 5.0),
        ContinuousItem(0.64, 11.0),
        ContinuousItem(0.33, 6.0)
    };
    
    double capacity = 28.0;

    ContinuousKnapsack problem(capacity, items);
    auto result = problem.solve();

    EXPECT_NEAR(computeTotalValue(items, result), 45.37, 0.05);
}

TEST(ContinuousKnapsackProblemTest, ClassicExampleWithExpectedValue230)
{
    std::vector<ContinuousItem> items = {
        ContinuousItem(6.0, 5.0),   // Item 1: 30 / 5
        ContinuousItem(4.0, 10.0),  // Item 2: 40 / 10
        ContinuousItem(3.0, 15.0),  // Item 3: 45 / 15
        ContinuousItem(3.5, 22.0),  // Item 4: 77 / 22
        ContinuousItem(3.6, 25.0)   // Item 5: 90 / 25
    };

    double capacity = 60.0;

    ContinuousKnapsack problem(capacity, items);
    auto result = problem.solve();

    EXPECT_NEAR(computeTotalValue(items, result), 230.0, EPSILON);
}
