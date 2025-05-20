#include <gtest/gtest.h>
#include "TransportProblem/cflp_tansport_problem.h"
#include <random>

namespace {

// Utilidades para generar datos aleatorios
std::vector<int> generateRandomVector(size_t size, int minValue = 1, int maxValue = 30) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(minValue, maxValue);

    std::vector<int> vec(size);
    for (auto &val : vec)
        val = dist(gen);
    return vec;
}

std::vector<std::vector<int>> generateRandomMatrix(size_t rows, size_t cols, int minValue = 1, int maxValue = 100) {
    std::vector<std::vector<int>> matrix(rows);
    for (auto &row : matrix)
        row = generateRandomVector(cols, minValue, maxValue);
    return matrix;
}

} // namespace

// --------- CONSTRUCTOR VALIDATION ---------

TEST(CFLPTransportSubproblemTest, ThrowsOnMismatchedCostMatrixAndCapacities) {
    auto costMatrix = generateRandomMatrix(4, 5); // 4 facilities
    auto capacities = generateRandomVector(3);    // should be 4
    auto demands = generateRandomVector(5);
    auto openFacilities = std::vector<int>(4, 1);

    EXPECT_THROW({
        CFLPTransportSubproblem sub(costMatrix, capacities, demands, openFacilities);
    }, std::invalid_argument);
}

TEST(CFLPTransportSubproblemTest, ThrowsOnMismatchedOpenFacilitiesSize) {
    auto costMatrix = generateRandomMatrix(4, 5);
    auto capacities = generateRandomVector(4);
    auto demands = generateRandomVector(5);
    auto openFacilities = std::vector<int>(3, 1); // should be 4

    EXPECT_THROW({
        CFLPTransportSubproblem sub(costMatrix, capacities, demands, openFacilities);
    }, std::invalid_argument);
}

TEST(CFLPTransportSubproblemTest, ThrowsOnMismatchedDemandSize) {
    auto costMatrix = generateRandomMatrix(3, 5);
    auto capacities = generateRandomVector(3);
    auto demands = generateRandomVector(4); // should be 5
    auto openFacilities = std::vector<int>(3, 1);

    EXPECT_THROW({
        CFLPTransportSubproblem sub(costMatrix, capacities, demands, openFacilities);
    }, std::invalid_argument);
}

// --------- FUNCTIONAL TESTS ---------
/**
TEST(CFLPTransportSubproblemTest, BuildsTransportProblemFromOpenFacilities) {
    auto costMatrix = generateRandomMatrix(4, 5);
    auto capacities = generateRandomVector(4);
    auto demands = generateRandomVector(5);
    std::vector<bool> openFacilities = {1, 0, 1, 0};

    CFLPTransportSubproblem sub(costMatrix, capacities, demands, openFacilities);
    const TransportationProblem &tp = sub.getTransportProblem();

    EXPECT_EQ(tp.getSupply().size(), 2); // Only 2 facilities are open
    EXPECT_EQ(tp.getDemand().size(), 5);
    EXPECT_EQ(tp.getCostMatrix().size(), 2);
    for (const auto &row : tp.getCostMatrix()) {
        EXPECT_EQ(row.size(), 5);
    }
}

TEST(CFLPTransportSubproblemTest, ToggleFacilityUpdatesSubproblem) {
    auto costMatrix = generateRandomMatrix(3, 4);
    auto capacities = generateRandomVector(3);
    auto demands = generateRandomVector(4);
    std::vector<bool> openFacilities = {1 , 0, 0}; 

    CFLPTransportSubproblem sub(costMatrix, capacities, demands, openFacilities);

    // Initially only 1 facility open
    EXPECT_EQ(sub.getTransportProblem().getSupply().size(), 1);

    // Toggle one more facility to open
    sub.toggleFacility(1);
    EXPECT_EQ(sub.getOpenFacilities()[1], true);
    EXPECT_EQ(sub.getTransportProblem().getSupply().size(), 2);

    // Toggle it back to closed
    sub.toggleFacility(1);
    EXPECT_EQ(sub.getOpenFacilities()[1], false);
    EXPECT_EQ(sub.getTransportProblem().getSupply().size(), 1);
}

TEST(CFLPTransportSubproblemTest, ToggleFacilityOutOfRangeThrows) {
    auto costMatrix = generateRandomMatrix(2, 3);
    auto capacities = generateRandomVector(2);
    auto demands = generateRandomVector(3);
    std::vector<bool> openFacilities = {0,1};

    CFLPTransportSubproblem sub(costMatrix, capacities, demands, openFacilities);

    EXPECT_THROW(sub.toggleFacility(-1), std::out_of_range);
    EXPECT_THROW(sub.toggleFacility(2), std::out_of_range);
}*/