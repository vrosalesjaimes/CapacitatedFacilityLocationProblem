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
                                         std::vector<int>& supply, std::vector<int>& demand)
    {
        supply = generateRandomVector(supplySize);
        demand = generateRandomVector(demandSize);

        int totalSupply = std::accumulate(supply.begin(), supply.end(), 0);
        int totalDemand = std::accumulate(demand.begin(), demand.end(), 0);

        // Si la demanda es mayor que la oferta, ajustamos la última demanda
        if (totalDemand > totalSupply) {
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
    generateSupplyAndDemand(3, 3, supply, demand);

    auto costMatrix = generateRandomMatrix(3, 3);
    TransportationProblem tp(supply, demand, costMatrix);
    tp.calculateTotalSupplyAndDemand();
    tp.balance();

    EXPECT_NO_THROW(tp.solveHungarianMethod());
}

TEST(TransportationProblemMethodTest, SolveHungarianMethodBalanced)
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