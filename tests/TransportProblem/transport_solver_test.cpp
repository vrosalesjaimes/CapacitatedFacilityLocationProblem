#include "TransportProblem/transport_solver.h"
#include <gtest/gtest.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <random>

using namespace std;

namespace InitialSolution
{
    vector<vector<int>> PROBLEM_4_MODI = {{50, 20, 0, 0}, {0, 40, 50, 0}, {0, 0, 20, 95}};

    vector<vector<int>> PROBLEM_5_MODI = {{4, 0, 0, 4}, {0, 0, 6, 4}, {0, 7, 0, 4}};

    vector<vector<int>> PROBLEM_6_MODI = {{5, 5, 30}, {50, 0, 0}, {0, 40, 0}};
}

namespace Demands
{
    vector<int> PROBLEM_1_BALANCED_VAM = {5, 15, 15, 15};

    vector<int> PROBLEM_2_BALANCED_VAM = {70, 40, 70, 35};

    vector<int> PROBLEM_3_UNBALANCED_VAM = {45, 20, 30};

    vector<int> PROBLEM_4_MODI = {50, 60, 70, 95};

    vector<int> PROBLEM_5_MODI = {4, 7, 6, 12};

    vector<int> PROBLEM_6_MODI = {55, 45, 30};

    vector<int> PROBLEM_7_MODI = {4, 7, 6, 12};

    vector<int> PROBLEM_8_MODI = {55, 45, 30};

    vector<int> PROBLEM_9_UNBALANCED_VAM = {30,20,15,10,20,22};
}

namespace Supplies
{
    vector<int> PROBLEM_1_BALANCED_VAM = {15, 25, 10};

    vector<int> PROBLEM_2_BALANCED_VAM = {80, 30, 70, 45};

    vector<int> PROBLEM_3_UNBALANCED_VAM = {35, 50, 40};

    vector<int> PROBLEM_4_MODI = {70, 90, 115};

    vector<int> PROBLEM_5_MODI = {8, 10, 11};

    vector<int> PROBLEM_6_MODI = {40, 50, 40};

    vector<int> PROBLEM_7_MODI = {8, 10, 11};

    vector<int> PROBLEM_8_MODI = {40, 50, 40};

    vector<int> PROBLEM_9_UNBALANCED_VAM = {15,25,42,35};
}

namespace Costs
{
    int PROBLEM_1_BALANCED_VAM = 475;

    int PROBLEM_2_BALANCED_VAM = 620;

    int PROBLEM_3_UNBALANCED_VAM = 1000;

    int PROBLEM_4_MODI = 4185;

    int PROBLEM_5_MODI = 412;

    int PROBLEM_6_MODI = 1320;

    int PROBLEM_7_PMSJRA = 412;

    int PROBLEM_8_PMSJRA = 1200;

    int PROBLEM_9_UNBALANCED_VAM = 374;
}

namespace CostMatrices
{
    vector<vector<int>> PROBLEM_1_BALANCED_VAM = {{10, 2, 20, 11}, {12, 7, 9, 20}, {4, 14, 16, 18}};

    vector<vector<int>> PROBLEM_2_BALANCED_VAM = {{5, 2, 7, 3}, {3, 6, 6, 1}, {6, 1, 2, 4}, {4, 3, 6, 6}};

    vector<vector<int>> PROBLEM_3_UNBALANCED_VAM = {{8, 15, 10}, {10, 12, 14}, {14, 9, 15}, {0, 0, 0}};

    vector<vector<int>> PROBLEM_4_MODI = {{17, 20, 13, 12}, {15, 21, 26, 25}, {15, 14, 15, 17}};

    vector<vector<int>> PROBLEM_5_MODI = {{13, 18, 30, 8}, {55, 20, 25, 40}, {30, 6, 50, 10}};

    vector<vector<int>> PROBLEM_6_MODI = {{11, 9, 6}, {12, 14, 11}, {10, 8, 10}};

    vector<vector<int>> PROBLEM_7_PMSJRA = {{13, 18, 30, 8}, {55, 20, 25, 40}, {30, 6, 50, 10}};

    vector<vector<int>> PROBLEM_8_PMSJRA = {{11, 9, 6}, {12, 14, 11}, {10, 8, 10}};

    vector<vector<int>> PROBLEM_9_UNBALANCED_VAM = {{5, 1, 8, 7, 5, 0}, {3, 9, 6, 7, 8, 0}, {4, 2, 7, 6, 5, 0}, {7, 11, 10, 4, 9, 0}};
}

namespace
{
    tuple<vector<vector<int>>, vector<int>, vector<int>, vector<vector<int>>, int, int> generateRandomTransportSolver(int numFacilities, int numCustomers)
    {
        vector<vector<int>> costs(numFacilities, vector<int>(numCustomers));
        vector<int> customerDemands(numCustomers);
        vector<int> facilityCapacities(numFacilities);
        vector<vector<int>> assignments(numFacilities, vector<int>(numCustomers, 0));
        int totalCapacity = 0;
        int totalDemand = 0;

        // Seed for random number generation
        srand(time(nullptr));

        // Generate random costs
        for (int i = 0; i < numFacilities; ++i)
        {
            for (int j = 0; j < numCustomers; ++j)
            {
                costs[i][j] = rand() % 100 + 1; // Costs between 1 and 100
            }
        }

        // Generate random customer demands
        for (int j = 0; j < numCustomers; ++j)
        {
            customerDemands[j] = rand() % 50 + 1; // Demands between 1 and 50
        }

        // Generate random facility capacities
        for (int i = 0; i < numFacilities; ++i)
        {
            facilityCapacities[i] = rand() % 100 + 1; // Capacities between 1 and 100
        }

        // Calculate total capacity
        totalCapacity = accumulate(facilityCapacities.begin(), facilityCapacities.end(), 0);

        // Calculate total demand
        totalDemand = accumulate(customerDemands.begin(), customerDemands.end(), 0);

        return {costs, facilityCapacities, customerDemands, assignments, totalCapacity, totalDemand};
    }

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(100, 200);

    int randomFacilities = dis(gen);
    int randomCustomers = dis(gen);

    auto inputs = generateRandomTransportSolver(randomFacilities, randomCustomers);

    TransportSolver randomSolver(get<0>(inputs), get<1>(inputs), get<2>(inputs), get<3>(inputs), get<4>(inputs), get<5>(inputs));

    vector<vector<int>> assignments(randomFacilities, vector<int>(randomCustomers, 0));

    int totalSupply1 = accumulate(Supplies::PROBLEM_1_BALANCED_VAM.begin(), Supplies::PROBLEM_1_BALANCED_VAM.end(), 0);
    int totalDemand1 = accumulate(Demands::PROBLEM_1_BALANCED_VAM.begin(), Demands::PROBLEM_1_BALANCED_VAM.end(), 0);

    TransportSolver problem1(CostMatrices::PROBLEM_1_BALANCED_VAM, Supplies::PROBLEM_1_BALANCED_VAM, Demands::PROBLEM_1_BALANCED_VAM, assignments, totalSupply1, totalDemand1);

    int totalSupply2 = accumulate(Supplies::PROBLEM_2_BALANCED_VAM.begin(), Supplies::PROBLEM_2_BALANCED_VAM.end(), 0);
    int totalDemand2 = accumulate(Demands::PROBLEM_2_BALANCED_VAM.begin(), Demands::PROBLEM_2_BALANCED_VAM.end(), 0);

    TransportSolver problem2(CostMatrices::PROBLEM_2_BALANCED_VAM, Supplies::PROBLEM_2_BALANCED_VAM, Demands::PROBLEM_2_BALANCED_VAM, assignments, totalSupply2, totalDemand2);

    int totalSupply3 = accumulate(Supplies::PROBLEM_3_UNBALANCED_VAM.begin(), Supplies::PROBLEM_3_UNBALANCED_VAM.end(), 0);
    int totalDemand3 = accumulate(Demands::PROBLEM_3_UNBALANCED_VAM.begin(), Demands::PROBLEM_3_UNBALANCED_VAM.end(), 0);

    TransportSolver problem3(CostMatrices::PROBLEM_3_UNBALANCED_VAM, Supplies::PROBLEM_3_UNBALANCED_VAM, Demands::PROBLEM_3_UNBALANCED_VAM, assignments, totalSupply3, totalDemand3);

    int totalSupply4 = accumulate(Supplies::PROBLEM_4_MODI.begin(), Supplies::PROBLEM_4_MODI.end(), 0);
    int totalDemand4 = accumulate(Demands::PROBLEM_4_MODI.begin(), Demands::PROBLEM_4_MODI.end(), 0);

    TransportSolver problem4(CostMatrices::PROBLEM_4_MODI, Supplies::PROBLEM_4_MODI, Demands::PROBLEM_4_MODI, InitialSolution::PROBLEM_4_MODI, totalSupply4, totalDemand4);

    int totalSupply5 = accumulate(Supplies::PROBLEM_5_MODI.begin(), Supplies::PROBLEM_5_MODI.end(), 0);
    int totalDemand5 = accumulate(Demands::PROBLEM_5_MODI.begin(), Demands::PROBLEM_5_MODI.end(), 0);

    TransportSolver problem5(CostMatrices::PROBLEM_5_MODI, Supplies::PROBLEM_5_MODI, Demands::PROBLEM_5_MODI, InitialSolution::PROBLEM_5_MODI, totalSupply5, totalDemand5);

    int totalSupply6 = accumulate(Supplies::PROBLEM_6_MODI.begin(), Supplies::PROBLEM_6_MODI.end(), 0);
    int totalDemand6 = accumulate(Demands::PROBLEM_6_MODI.begin(), Demands::PROBLEM_6_MODI.end(), 0);

    TransportSolver problem6(CostMatrices::PROBLEM_6_MODI, Supplies::PROBLEM_6_MODI, Demands::PROBLEM_6_MODI, InitialSolution::PROBLEM_6_MODI, totalSupply6, totalDemand6);

    int totalSupply7 = accumulate(Supplies::PROBLEM_7_MODI.begin(), Supplies::PROBLEM_7_MODI.end(), 0);
    int totalDemand7 = accumulate(Demands::PROBLEM_7_MODI.begin(), Demands::PROBLEM_7_MODI.end(), 0);

    TransportSolver problem7(CostMatrices::PROBLEM_7_PMSJRA, Supplies::PROBLEM_7_MODI, Demands::PROBLEM_7_MODI, assignments, totalSupply7, totalDemand7);

    int totalSupply8 = accumulate(Supplies::PROBLEM_8_MODI.begin(), Supplies::PROBLEM_8_MODI.end(), 0);
    int totalDemand8 = accumulate(Demands::PROBLEM_8_MODI.begin(), Demands::PROBLEM_8_MODI.end(), 0);

    TransportSolver problem8(CostMatrices::PROBLEM_8_PMSJRA, Supplies::PROBLEM_8_MODI, Demands::PROBLEM_8_MODI, assignments, totalSupply8, totalDemand8);

    int totalSupply9 = accumulate(Supplies::PROBLEM_9_UNBALANCED_VAM.begin(), Supplies::PROBLEM_9_UNBALANCED_VAM.end(), 0);
    int totalDemand9 = accumulate(Demands::PROBLEM_9_UNBALANCED_VAM.begin(), Demands::PROBLEM_9_UNBALANCED_VAM.end(), 0);

    TransportSolver problem9(CostMatrices::PROBLEM_9_UNBALANCED_VAM, Supplies::PROBLEM_9_UNBALANCED_VAM, Demands::PROBLEM_9_UNBALANCED_VAM, assignments, totalSupply9, totalDemand9);

}

TEST(TransportSolverTest, RandomConstructor)
{
    ASSERT_EQ(randomSolver.getCostMatrix(), get<0>(inputs));
    ASSERT_EQ(randomSolver.getSupply(), get<1>(inputs));
    ASSERT_EQ(randomSolver.getDemand(), get<2>(inputs));
    ASSERT_EQ(randomSolver.getAssignamentMatrix(), get<3>(inputs));
    ASSERT_EQ(randomSolver.getTotalSupply(), get<4>(inputs));
    ASSERT_EQ(randomSolver.getTotalDemand(), get<5>(inputs));

    EXPECT_NO_THROW(TransportSolver solver(get<0>(inputs), get<1>(inputs), get<2>(inputs), get<3>(inputs), get<4>(inputs), get<5>(inputs)));
}

TEST(TransportSolverTest, RandomSetAndGetSupply)
{
    vector<int> newSupply(randomFacilities);

    for (int i = 0; i < randomFacilities; ++i)
    {
        newSupply[i] = rand() % 100 + 1;
    }
    randomSolver.setSupply(newSupply);

    ASSERT_EQ(randomSolver.getSupply(), newSupply);
}

TEST(TransportSolverTest, RandomSetAndGetDemand)
{
    vector<int> newDemand(randomCustomers);

    for (int i = 0; i < randomCustomers; ++i)
    {
        newDemand[i] = rand() % 100 + 1;
    }
    randomSolver.setDemand(newDemand);

    ASSERT_EQ(randomSolver.getDemand(), newDemand);
}

TEST(TransportSolverTest, RandomSetAndGetTotalSupply)
{
    int newTotalSupply = rand() % 100 + 1;

    randomSolver.setTotalSupply(newTotalSupply);

    ASSERT_EQ(randomSolver.getTotalSupply(), newTotalSupply);
}

TEST(TransportSolverTest, RandomSetAndGetTotalDemand)
{
    int newTotalDemand = rand() % 100 + 1;

    randomSolver.setTotalDemand(newTotalDemand);

    ASSERT_EQ(randomSolver.getTotalDemand(), newTotalDemand);
}

TEST(TransportSolverTest, RandomSetAndGetAssignamentMatrix)
{
    vector<vector<int>> newAssignamentMatrix(randomFacilities, vector<int>(randomCustomers, 1));

    randomSolver.setAssignamentMatrix(newAssignamentMatrix);

    ASSERT_EQ(randomSolver.getAssignamentMatrix(), newAssignamentMatrix);
}

TEST(TransportSolverTest, RandomSetAndGetTotalCost)
{
    ASSERT_EQ(randomSolver.getTotalCost(), 0);
    randomSolver.setTotalCost(100);
    ASSERT_EQ(randomSolver.getTotalCost(), 100);
}

TEST(TransportSolverTest, Constructor)
{
    ASSERT_EQ(problem1.getCostMatrix(), CostMatrices::PROBLEM_1_BALANCED_VAM);
    ASSERT_EQ(problem1.getSupply(), Supplies::PROBLEM_1_BALANCED_VAM);
    ASSERT_EQ(problem1.getDemand(), Demands::PROBLEM_1_BALANCED_VAM);
    ASSERT_EQ(problem1.getAssignamentMatrix(), assignments);
    ASSERT_EQ(problem1.getTotalSupply(), totalSupply1);
    ASSERT_EQ(problem1.getTotalDemand(), totalDemand1);

    EXPECT_NO_THROW(TransportSolver solver(CostMatrices::PROBLEM_1_BALANCED_VAM, Supplies::PROBLEM_1_BALANCED_VAM, Demands::PROBLEM_1_BALANCED_VAM, assignments, totalSupply1, totalDemand1));

    ASSERT_EQ(problem2.getCostMatrix(), CostMatrices::PROBLEM_2_BALANCED_VAM);
    ASSERT_EQ(problem2.getSupply(), Supplies::PROBLEM_2_BALANCED_VAM);
    ASSERT_EQ(problem2.getDemand(), Demands::PROBLEM_2_BALANCED_VAM);
    ASSERT_EQ(problem2.getAssignamentMatrix(), assignments);
    ASSERT_EQ(problem2.getTotalSupply(), totalSupply2);
    ASSERT_EQ(problem2.getTotalDemand(), totalDemand2);

    EXPECT_NO_THROW(TransportSolver solver(CostMatrices::PROBLEM_2_BALANCED_VAM, Supplies::PROBLEM_2_BALANCED_VAM, Demands::PROBLEM_2_BALANCED_VAM, assignments, totalSupply2, totalDemand2));

    ASSERT_EQ(problem3.getCostMatrix(), CostMatrices::PROBLEM_3_UNBALANCED_VAM);
    ASSERT_EQ(problem3.getSupply(), Supplies::PROBLEM_3_UNBALANCED_VAM);
    ASSERT_EQ(problem3.getDemand(), Demands::PROBLEM_3_UNBALANCED_VAM);
    ASSERT_EQ(problem3.getAssignamentMatrix(), assignments);
    ASSERT_EQ(problem3.getTotalSupply(), totalSupply3);
    ASSERT_EQ(problem3.getTotalDemand(), totalDemand3);

    EXPECT_NO_THROW(TransportSolver solver(CostMatrices::PROBLEM_3_UNBALANCED_VAM, Supplies::PROBLEM_3_UNBALANCED_VAM, Demands::PROBLEM_3_UNBALANCED_VAM, assignments, totalSupply3, totalDemand3));

    ASSERT_EQ(problem4.getCostMatrix(), CostMatrices::PROBLEM_4_MODI);
    ASSERT_EQ(problem4.getSupply(), Supplies::PROBLEM_4_MODI);
    ASSERT_EQ(problem4.getDemand(), Demands::PROBLEM_4_MODI);
    ASSERT_EQ(problem4.getAssignamentMatrix(), InitialSolution::PROBLEM_4_MODI);
    ASSERT_EQ(problem4.getTotalSupply(), totalSupply4);
    ASSERT_EQ(problem4.getTotalDemand(), totalDemand4);

    EXPECT_NO_THROW(TransportSolver solver(CostMatrices::PROBLEM_4_MODI, Supplies::PROBLEM_4_MODI, Demands::PROBLEM_4_MODI, assignments, totalSupply4, totalDemand4));

    ASSERT_EQ(problem5.getCostMatrix(), CostMatrices::PROBLEM_5_MODI);
    ASSERT_EQ(problem5.getSupply(), Supplies::PROBLEM_5_MODI);
    ASSERT_EQ(problem5.getDemand(), Demands::PROBLEM_5_MODI);
    ASSERT_EQ(problem5.getAssignamentMatrix(), InitialSolution::PROBLEM_5_MODI);
    ASSERT_EQ(problem5.getTotalSupply(), totalSupply5);
    ASSERT_EQ(problem5.getTotalDemand(), totalDemand5);

    EXPECT_NO_THROW(TransportSolver solver(CostMatrices::PROBLEM_5_MODI, Supplies::PROBLEM_5_MODI, Demands::PROBLEM_5_MODI, assignments, totalSupply5, totalDemand5));

    ASSERT_EQ(problem6.getCostMatrix(), CostMatrices::PROBLEM_6_MODI);
    ASSERT_EQ(problem6.getSupply(), Supplies::PROBLEM_6_MODI);
    ASSERT_EQ(problem6.getDemand(), Demands::PROBLEM_6_MODI);
    ASSERT_EQ(problem6.getAssignamentMatrix(), InitialSolution::PROBLEM_6_MODI);
    ASSERT_EQ(problem6.getTotalSupply(), totalSupply6);
    ASSERT_EQ(problem6.getTotalDemand(), totalDemand6);

    EXPECT_NO_THROW(TransportSolver solver(CostMatrices::PROBLEM_6_MODI, Supplies::PROBLEM_6_MODI, Demands::PROBLEM_6_MODI, assignments, totalSupply6, totalDemand6));

    ASSERT_EQ(problem7.getCostMatrix(), CostMatrices::PROBLEM_7_PMSJRA);
    ASSERT_EQ(problem7.getSupply(), Supplies::PROBLEM_7_MODI);
    ASSERT_EQ(problem7.getDemand(), Demands::PROBLEM_7_MODI);
    ASSERT_EQ(problem7.getAssignamentMatrix(), assignments);
    ASSERT_EQ(problem7.getTotalSupply(), totalSupply7);
    ASSERT_EQ(problem7.getTotalDemand(), totalDemand7);

    EXPECT_NO_THROW(TransportSolver solver(CostMatrices::PROBLEM_7_PMSJRA, Supplies::PROBLEM_7_MODI, Demands::PROBLEM_7_MODI, assignments, totalSupply7, totalDemand7));

    ASSERT_EQ(problem8.getCostMatrix(), CostMatrices::PROBLEM_8_PMSJRA);
    ASSERT_EQ(problem8.getSupply(), Supplies::PROBLEM_8_MODI);
    ASSERT_EQ(problem8.getDemand(), Demands::PROBLEM_8_MODI);
    ASSERT_EQ(problem8.getAssignamentMatrix(), assignments);
    ASSERT_EQ(problem8.getTotalSupply(), totalSupply8);
    ASSERT_EQ(problem8.getTotalDemand(), totalDemand8);

    EXPECT_NO_THROW(TransportSolver solver(CostMatrices::PROBLEM_8_PMSJRA, Supplies::PROBLEM_8_MODI, Demands::PROBLEM_8_MODI, assignments, totalSupply8, totalDemand8));
}

TEST(TransportSolverTest, SetAndGetSupply)
{
    vector<int> newSupply(randomFacilities);

    for (int i = 0; i < randomFacilities; ++i)
    {
        newSupply[i] = rand() % 100 + 1;
    }

    problem1.setSupply(newSupply);
    ASSERT_EQ(problem1.getSupply(), newSupply);

    problem2.setSupply(newSupply);
    ASSERT_EQ(problem2.getSupply(), newSupply);

    problem3.setSupply(newSupply);
    ASSERT_EQ(problem3.getSupply(), newSupply);

    problem4.setSupply(newSupply);
    ASSERT_EQ(problem4.getSupply(), newSupply);

    problem5.setSupply(newSupply);
    ASSERT_EQ(problem5.getSupply(), newSupply);

    problem6.setSupply(newSupply);
    ASSERT_EQ(problem6.getSupply(), newSupply);

    problem7.setSupply(newSupply);
    ASSERT_EQ(problem7.getSupply(), newSupply);

    problem8.setSupply(newSupply);
    ASSERT_EQ(problem8.getSupply(), newSupply);
}

TEST(TransportSolverTest, SetAndGetDemand)
{
    vector<int> newDemand(randomCustomers);

    for (int i = 0; i < randomCustomers; ++i)
    {
        newDemand[i] = rand() % 100 + 1;
    }

    problem1.setDemand(newDemand);
    ASSERT_EQ(problem1.getDemand(), newDemand);

    problem2.setDemand(newDemand);
    ASSERT_EQ(problem2.getDemand(), newDemand);

    problem3.setDemand(newDemand);
    ASSERT_EQ(problem3.getDemand(), newDemand);

    problem4.setDemand(newDemand);
    ASSERT_EQ(problem4.getDemand(), newDemand);

    problem5.setDemand(newDemand);
    ASSERT_EQ(problem5.getDemand(), newDemand);

    problem6.setDemand(newDemand);
    ASSERT_EQ(problem6.getDemand(), newDemand);

    problem7.setDemand(newDemand);
    ASSERT_EQ(problem7.getDemand(), newDemand);

    problem8.setDemand(newDemand);
    ASSERT_EQ(problem8.getDemand(), newDemand);
}

TEST(TransportSolverTest, SetAndGetTotalSupply)
{
    int newTotalSupply = rand() % 100 + 1;

    problem1.setTotalSupply(newTotalSupply);
    ASSERT_EQ(problem1.getTotalSupply(), newTotalSupply);

    problem2.setTotalSupply(newTotalSupply);
    ASSERT_EQ(problem2.getTotalSupply(), newTotalSupply);

    problem3.setTotalSupply(newTotalSupply);
    ASSERT_EQ(problem3.getTotalSupply(), newTotalSupply);

    problem4.setTotalSupply(newTotalSupply);
    ASSERT_EQ(problem4.getTotalSupply(), newTotalSupply);

    problem5.setTotalSupply(newTotalSupply);
    ASSERT_EQ(problem5.getTotalSupply(), newTotalSupply);

    problem6.setTotalSupply(newTotalSupply);
    ASSERT_EQ(problem6.getTotalSupply(), newTotalSupply);

    problem7.setTotalSupply(newTotalSupply);
    ASSERT_EQ(problem7.getTotalSupply(), newTotalSupply);

    problem8.setTotalSupply(newTotalSupply);
    ASSERT_EQ(problem8.getTotalSupply(), newTotalSupply);
}

TEST(TransportSolverTest, SetAndGetTotalDemand)
{
    int newTotalDemand = rand() % 100 + 1;

    problem1.setTotalDemand(newTotalDemand);
    ASSERT_EQ(problem1.getTotalDemand(), newTotalDemand);

    problem2.setTotalDemand(newTotalDemand);
    ASSERT_EQ(problem2.getTotalDemand(), newTotalDemand);

    problem3.setTotalDemand(newTotalDemand);
    ASSERT_EQ(problem3.getTotalDemand(), newTotalDemand);

    problem4.setTotalDemand(newTotalDemand);
    ASSERT_EQ(problem4.getTotalDemand(), newTotalDemand);

    problem5.setTotalDemand(newTotalDemand);
    ASSERT_EQ(problem5.getTotalDemand(), newTotalDemand);

    problem6.setTotalDemand(newTotalDemand);
    ASSERT_EQ(problem6.getTotalDemand(), newTotalDemand);

    problem7.setTotalDemand(newTotalDemand);
    ASSERT_EQ(problem7.getTotalDemand(), newTotalDemand);

    problem8.setTotalDemand(newTotalDemand);
    ASSERT_EQ(problem8.getTotalDemand(), newTotalDemand);
}

TEST(TransportSolverTest, SetAndGetAssignamentMatrix)
{
    vector<vector<int>> newAssignamentMatrix(randomFacilities, vector<int>(randomCustomers, 1));

    problem1.setAssignamentMatrix(newAssignamentMatrix);
    ASSERT_EQ(problem1.getAssignamentMatrix(), newAssignamentMatrix);

    problem2.setAssignamentMatrix(newAssignamentMatrix);
    ASSERT_EQ(problem2.getAssignamentMatrix(), newAssignamentMatrix);

    problem3.setAssignamentMatrix(newAssignamentMatrix);
    ASSERT_EQ(problem3.getAssignamentMatrix(), newAssignamentMatrix);

    problem4.setAssignamentMatrix(newAssignamentMatrix);
    ASSERT_EQ(problem4.getAssignamentMatrix(), InitialSolution::PROBLEM_4_MODI);

    problem5.setAssignamentMatrix(newAssignamentMatrix);
    ASSERT_EQ(problem5.getAssignamentMatrix(), InitialSolution::PROBLEM_5_MODI);

    problem6.setAssignamentMatrix(newAssignamentMatrix);
    ASSERT_EQ(problem6.getAssignamentMatrix(), InitialSolution::PROBLEM_6_MODI);

    problem7.setAssignamentMatrix(newAssignamentMatrix);
    ASSERT_EQ(problem7.getAssignamentMatrix(), newAssignamentMatrix);

    problem8.setAssignamentMatrix(newAssignamentMatrix);
    ASSERT_EQ(problem8.getAssignamentMatrix(), newAssignamentMatrix);
}

TEST(TransportSolverTest, SetAndGetTotalCost)
{
    ASSERT_EQ(problem1.getTotalCost(), 0);
    problem1.setTotalCost(100);
    ASSERT_EQ(problem1.getTotalCost(), 100);

    ASSERT_EQ(problem2.getTotalCost(), 0);
    problem2.setTotalCost(100);
    ASSERT_EQ(problem2.getTotalCost(), 100);

    ASSERT_EQ(problem3.getTotalCost(), 0);
    problem3.setTotalCost(100);
    ASSERT_EQ(problem3.getTotalCost(), 100);

    ASSERT_EQ(problem4.getTotalCost(), 0);
    problem4.setTotalCost(100);
    ASSERT_EQ(problem4.getTotalCost(), 100);

    ASSERT_EQ(problem5.getTotalCost(), 0);
    problem5.setTotalCost(100);
    ASSERT_EQ(problem5.getTotalCost(), 100);

    ASSERT_EQ(problem6.getTotalCost(), 0);
    problem6.setTotalCost(100);
    ASSERT_EQ(problem6.getTotalCost(), 100);

    ASSERT_EQ(problem7.getTotalCost(), 0);
    problem7.setTotalCost(100);
    ASSERT_EQ(problem7.getTotalCost(), 100);

    ASSERT_EQ(problem8.getTotalCost(), 0);
    problem8.setTotalCost(100);
    ASSERT_EQ(problem8.getTotalCost(), 100);
}

TEST(TransportSolverTest, VogelsApproximationMethodBalanced)
{
    ASSERT_EQ(problem1.VogelsApproximationMethod(), Costs::PROBLEM_1_BALANCED_VAM);
    ASSERT_EQ(problem2.VogelsApproximationMethod(), Costs::PROBLEM_2_BALANCED_VAM);
}

TEST(TransportSolverTest, VogelsApproximationMethodUnbalanced)
{
    ASSERT_EQ(problem3.VogelsApproximationMethod(), Costs::PROBLEM_3_UNBALANCED_VAM);
    ASSERT_EQ(problem9.VogelsApproximationMethod(), Costs::PROBLEM_9_UNBALANCED_VAM);
}

TEST(TransportSolverTest, MODIBalanced)
{
    ASSERT_EQ(problem4.MODI(), Costs::PROBLEM_4_MODI);
    ASSERT_EQ(problem5.MODI(), Costs::PROBLEM_5_MODI);
    ASSERT_EQ(problem6.MODI(), Costs::PROBLEM_6_MODI);
}

TEST(TransportSolverTest, PSR)
{
    ASSERT_EQ(problem7.PSR(), Costs::PROBLEM_7_PMSJRA);
    ASSERT_EQ(problem8.PSR(), Costs::PROBLEM_8_PMSJRA);
}