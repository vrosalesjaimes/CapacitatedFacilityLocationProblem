#include <gtest/gtest.h>
#include "TransportProblem/transport_problem.h"
#include <random>
#include <numeric>

namespace
{

    // Generador de vector aleatorio para oferta y demanda, asegurando que la oferta >= demanda
    std::vector<int> generateRandomVector(size_t size, int minValue = 1, int maxValue = 50)
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dist(minValue, maxValue);

        std::vector<int> vec(size);
        for (auto &val : vec)
        {
            val = dist(gen);
        }
        return vec;
    }

    // Generador de matriz aleatoria, asegurando que la oferta sea mayor o igual a la demanda
    std::vector<std::vector<int>> generateRandomMatrix(size_t rows, size_t cols, int minValue = 1, int maxValue = 100)
    {
        std::vector<std::vector<int>> matrix(rows);
        for (auto &row : matrix)
        {
            row = generateRandomVector(cols, minValue, maxValue);
        }
        return matrix;
    }

    void generateSupplyAndDemand(size_t supplySize, size_t demandSize,
                                 std::vector<int> &supply, std::vector<int> &demand)
    {
        supply = generateRandomVector(supplySize);
        demand = generateRandomVector(demandSize);

        int totalSupply = std::accumulate(supply.begin(), supply.end(), 0);
        int totalDemand = std::accumulate(demand.begin(), demand.end(), 0);

        // Si la demanda es mayor que la oferta, ajustamos la última demanda
        if (totalDemand > totalSupply)
        {
            supply[supply.size() - 1] += (totalDemand - totalSupply);
        }
    }

}

TEST(TransportationProblemTest, ValidConstructionAndAccessors)
{
    size_t supplySize = 5;
    size_t demandSize = 6;

    std::vector<int> supply, demand;
    generateSupplyAndDemand(supplySize, demandSize, supply, demand);

    auto costMatrix = generateRandomMatrix(supplySize, demandSize);

    TransportationProblem tp(supply, demand, costMatrix);
    tp.calculateTotalSupplyAndDemand();

    EXPECT_EQ(tp.getSupply(), supply);
    EXPECT_EQ(tp.getDemand(), demand);
    EXPECT_EQ(tp.getCostMatrix(), costMatrix);

    auto assignment = tp.getAssignmentMatrix();
    EXPECT_EQ(assignment.size(), supplySize);
    for (const auto &row : assignment)
    {
        EXPECT_EQ(row.size(), demandSize);
    }

    EXPECT_EQ(tp.getTotalCost(), 0);
}

TEST(TransportationProblemTest, ThrowsIfRowCountDoesNotMatchSupply)
{
    std::vector<int> supply = {10, 20, 30};
    std::vector<int> demand = {5, 5};
    auto costMatrix = generateRandomMatrix(2, 2);
    EXPECT_THROW({ TransportationProblem tp(supply, demand, costMatrix); }, std::invalid_argument);
}

TEST(TransportationProblemTest, ThrowsIfColumnCountDoesNotMatchDemand)
{
    std::vector<int> supply = {10, 20};
    std::vector<int> demand = {5, 5, 5};
    std::vector<std::vector<int>> costMatrix = {
        {1, 2},
        {3, 4}};

    EXPECT_THROW({ TransportationProblem tp(supply, demand, costMatrix); }, std::invalid_argument);
}

TEST(TransportationProblemTest, SettersWorkCorrectly)
{
    std::vector<int> supply, demand;
    generateSupplyAndDemand(3, 4, supply, demand);

    auto costMatrix = generateRandomMatrix(3, 4);
    TransportationProblem tp(supply, demand, costMatrix);
    tp.calculateTotalSupplyAndDemand();

    auto newSupply = generateRandomVector(3);
    auto newDemand = generateRandomVector(4);
    auto newCost = generateRandomMatrix(3, 4);

    tp.setSupply(newSupply);
    EXPECT_EQ(tp.getSupply(), newSupply);

    tp.setDemand(newDemand);
    EXPECT_EQ(tp.getDemand(), newDemand);

    tp.setCostMatrix(newCost);
    EXPECT_EQ(tp.getCostMatrix(), newCost);
}

TEST(TransportationProblemTest, SettersThrowOnInvalidSize)
{
    std::vector<int> supply = generateRandomVector(2);
    std::vector<int> demand = generateRandomVector(2);
    auto costMatrix = generateRandomMatrix(2, 2);
    TransportationProblem tp(supply, demand, costMatrix);
    tp.calculateTotalSupplyAndDemand();

    auto wrongCostMatrix = generateRandomMatrix(3, 2);
    EXPECT_THROW(tp.setCostMatrix(wrongCostMatrix), std::invalid_argument);

    wrongCostMatrix = generateRandomMatrix(2, 3);
    EXPECT_THROW(tp.setCostMatrix(wrongCostMatrix), std::invalid_argument);

    auto wrongSupply = generateRandomVector(3);
    EXPECT_THROW(tp.setSupply(wrongSupply), std::invalid_argument);

    auto wrongDemand = generateRandomVector(3);
    EXPECT_THROW(tp.setDemand(wrongDemand), std::invalid_argument);
}

TEST(TransportationProblemTest, PlaceholderMethodsDoNotThrow)
{
    std::vector<int> supply, demand;
    generateSupplyAndDemand(3, 3, supply, demand);

    auto costMatrix = generateRandomMatrix(3, 3);
    TransportationProblem tp(supply, demand, costMatrix);
    tp.calculateTotalSupplyAndDemand();
    tp.balance();

    EXPECT_NO_THROW(tp.solveHungarianMethod());
}

TEST(TransportationProblemTest, HungarianMethod)
{
    std::vector<int> supply, demand;
    generateSupplyAndDemand(200, 200, supply, demand);

    auto costMatrix = generateRandomMatrix(200, 200);
    TransportationProblem tp(supply, demand, costMatrix);
    tp.calculateTotalSupplyAndDemand();
    tp.balance();

    EXPECT_NO_THROW(tp.solveHungarianMethod());
}

TEST(TransportationProblemMethodTest, SolveHungarianMethodBalanced3x5)
{
    TransportationProblem problem(
        {5, 4, 6},
        {2, 3, 3, 5, 2},
        {{5, 3, 4, 5, 6},
         {2, 6, 5, 3, 2},
         {6, 4, 3, 4, 4}});

    problem.calculateTotalSupplyAndDemand();
    problem.solveHungarianMethod();

    EXPECT_EQ(problem.getTotalCost(), 48);
}

TEST(TransportationProblemMethodTest, SolveHungarianMethodBalanced10x14)
{
    TransportationProblem problem(
        std::vector<int>{2, 1, 10, 7, 6, 10, 8, 8, 9, 7},
        std::vector<int>{3, 4, 10, 7, 3, 3, 3, 3, 10, 3, 6, 2, 9, 2},
        std::vector<std::vector<int>>{
            {73, 69, 32, 75, 50, 79, 29, 60, 66, 46, 87, 21, 23, 21},
            {28, 51, 89, 11, 85, 17, 62, 18, 18, 25, 64, 23, 73, 28},
            {30, 35, 77, 91, 70, 41, 74, 92, 53, 29, 22, 71, 52, 37},
            {88, 36, 40, 33, 22, 57, 37, 93, 50, 98, 55, 61, 99, 49},
            {17, 47, 28, 79, 17, 30, 36, 95, 83, 31, 82, 34, 64, 70},
            {69, 27, 79, 44, 45, 32, 65, 44, 20, 74, 42, 75, 61, 39},
            {16, 83, 20, 27, 30, 76, 96, 75, 86, 67, 74, 66, 49, 87},
            {40, 19, 64, 29, 22, 30, 99, 45, 74, 34, 92, 81, 83, 82},
            {28, 19, 94, 85, 45, 32, 39, 13, 74, 39, 68, 26, 44, 11},
            {53, 31, 35, 16, 84, 48, 55, 45, 57, 93, 98, 26, 51, 64}});

    problem.calculateTotalSupplyAndDemand();
    problem.solveHungarianMethod();

    EXPECT_EQ(problem.getTotalCost(), 1651);
}

TEST(TransportationProblemMethodTest, SolveHungarianMethodBalanced20x17)
{
    TransportationProblem problem(
        std::vector<int>{
            4, 4, 7, 6, 1, 1, 10, 7, 4, 1,
            9, 5, 3, 10, 3, 5, 2, 3, 10, 5},
        std::vector<int>{
            3, 5, 8, 10, 5, 4, 9, 2, 8, 1,
            4, 9, 9, 6, 5, 5, 7},
        std::vector<std::vector<int>>{
            {97, 87, 10, 42, 70, 30, 56, 33, 42, 12, 99, 77, 28, 52, 13, 87, 85},
            {75, 91, 30, 87, 65, 83, 78, 74, 98, 27, 76, 76, 30, 62, 98, 66, 10},
            {78, 23, 83, 20, 21, 93, 82, 31, 91, 42, 52, 49, 99, 62, 56, 11, 67},
            {43, 77, 48, 19, 41, 31, 73, 62, 10, 41, 44, 78, 55, 17, 31, 56, 96},
            {82, 35, 10, 20, 46, 97, 33, 57, 22, 62, 74, 77, 69, 48, 78, 95, 70},
            {33, 62, 78, 92, 76, 20, 88, 42, 75, 94, 52, 24, 50, 41, 88, 72, 57},
            {33, 64, 71, 20, 45, 21, 33, 58, 65, 12, 30, 56, 32, 47, 32, 35, 85},
            {48, 43, 92, 78, 44, 98, 33, 84, 60, 50, 30, 33, 97, 54, 98, 84, 56},
            {64, 96, 87, 92, 38, 16, 40, 32, 36, 43, 40, 17, 33, 62, 67, 22, 98},
            {35, 48, 69, 47, 82, 47, 22, 70, 84, 81, 81, 35, 55, 87, 56, 39, 17},
            {91, 54, 78, 39, 51, 63, 54, 77, 91, 10, 39, 39, 78, 60, 74, 50, 83},
            {53, 76, 24, 71, 32, 64, 59, 54, 85, 14, 24, 56, 82, 19, 76, 15, 40},
            {32, 34, 44, 95, 78, 63, 45, 32, 63, 24, 15, 73, 39, 39, 66, 89, 51},
            {39, 11, 61, 60, 92, 14, 84, 41, 94, 51, 48, 17, 62, 93, 40, 67, 28},
            {17, 85, 76, 98, 74, 87, 83, 32, 11, 86, 11, 19, 54, 76, 32, 20, 99},
            {16, 65, 74, 33, 42, 84, 41, 51, 48, 16, 78, 73, 95, 73, 67, 46, 53},
            {71, 86, 35, 78, 29, 67, 73, 54, 57, 69, 77, 79, 99, 87, 48, 49, 31},
            {20, 10, 18, 31, 76, 42, 67, 71, 69, 90, 12, 30, 36, 38, 83, 33, 16},
            {84, 75, 21, 48, 51, 15, 20, 71, 28, 55, 20, 49, 92, 60, 73, 44, 35},
            {85, 68, 81, 81, 55, 40, 25, 75, 79, 81, 20, 42, 89, 92, 45, 24, 16}});

    problem.calculateTotalSupplyAndDemand();
    problem.solveHungarianMethod();

    EXPECT_EQ(problem.getTotalCost(), 2102);
}

TEST(TransportationProblemMethodTest, SolveHungarianMethodUnBalanced)
{
    TransportationProblem problem(
        {5, 4, 6},
        {2, 3, 3, 5, 1},
        {{5, 3, 4, 5, 6},
         {2, 6, 5, 3, 2},
         {6, 4, 3, 4, 4}});

    problem.calculateTotalSupplyAndDemand();
    EXPECT_THROW(problem.solveHungarianMethod(), std::logic_error);

    problem.balance();
    problem.solveHungarianMethod();

    EXPECT_EQ(problem.getTotalCost(), 44);
}

TEST(TransportationProblemBalanceTest, AlreadyBalanced)
{
    size_t m = 5, n = 4;
    std::vector<int> supply, demand;
    generateSupplyAndDemand(m, n, supply, demand);

    auto costMatrix = generateRandomMatrix(m, n);
    TransportationProblem tp(supply, demand, costMatrix);
    tp.calculateTotalSupplyAndDemand();

    EXPECT_EQ(tp.getSupply().size(), m);
    EXPECT_EQ(tp.getDemand().size(), n);
    EXPECT_EQ(tp.getCostMatrix()[0].size(), n);
}

TEST(TransportationProblemBalanceTest, SupplyGreaterThanDemand)
{
    size_t m = 4, n = 4;
    std::vector<int> supply, demand;
    generateSupplyAndDemand(m, n, supply, demand);

    demand[n - 1] -= demand[n - 1];

    auto costMatrix = generateRandomMatrix(m, n);
    TransportationProblem tp(supply, demand, costMatrix);
    tp.calculateTotalSupplyAndDemand();
    tp.balance();

    EXPECT_EQ(tp.getDemand().size(), n + 1);
    EXPECT_EQ(tp.getCostMatrix()[0].size(), n + 1);
    for (const auto &row : tp.getCostMatrix())
    {
        EXPECT_EQ(row.back(), 0);
    }
}