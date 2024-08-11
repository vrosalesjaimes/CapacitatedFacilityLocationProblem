#include "TransportProblem/transport_problem.h"
#include <gtest/gtest.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <random>

using namespace std;

namespace
{
    tuple<vector<vector<int>>, vector<int>, vector<int>, vector<uint8_t>> generateRandomTransportProblem(int numFacilities, int numCustomers)
    {
        vector<vector<int>> costs(numFacilities, vector<int>(numCustomers));
        vector<int> customerDemands(numCustomers);
        vector<int> facilityCapacities(numFacilities);
        vector<uint8_t> facilityStatus(numFacilities);

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

        // Randomly set facility status (open/closed)
        for (int i = 0; i < numFacilities; ++i)
        {
            facilityStatus[i] = rand() % 2; // 0 or 1
        }

        return {costs, customerDemands, facilityCapacities, facilityStatus};
    }

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(100, 200);

    int randomFacilities = dis(gen);
    int randomCustomers = dis(gen);

    auto inputs = generateRandomTransportProblem(randomFacilities, randomCustomers);

    TransportProblem randomTransportProblem(get<0>(inputs), get<1>(inputs), get<2>(inputs), get<3>(inputs));
}

namespace Demands
{
    const vector<int> PROBLEM_1_BALANCED_VAM = {5, 15, 15, 15};

    const vector<int> PROBLEM_2_BALANCED_VAM = {70, 40, 70, 35};

    const vector<int> PROBLEM_3_UNBALANCED_VAM = {45, 20, 30};

    const vector<int> PROBLEM_4_MODI = {50, 60, 70, 95};

    const vector<int> PROBLEM_5_MODI = {4, 7, 6, 12};

    const vector<int> PROBLEM_6_MODI = {55, 45, 30};

    const vector<int> PROBLEM_7_MODI = {4, 7, 6, 12};

    const vector<int> PROBLEM_8_MODI = {55, 45, 30};
}

namespace Supplies
{
    const vector<int> PROBLEM_1_BALANCED_VAM = {15, 25, 10};

    const vector<int> PROBLEM_2_BALANCED_VAM = {80, 30, 70, 45};

    const vector<int> PROBLEM_3_UNBALANCED_VAM = {35, 50, 40};

    const vector<int> PROBLEM_4_MODI = {70, 90, 115};

    const vector<int> PROBLEM_5_MODI = {8, 10, 11};

    const vector<int> PROBLEM_6_MODI = {40, 50, 40};

    const vector<int> PROBLEM_7_MODI = {8, 10, 11};

    const vector<int> PROBLEM_8_MODI = {40, 50, 40};
}

namespace Costs
{
    const int PROBLEM_1_BALANCED_VAM = 475;

    const int PROBLEM_2_BALANCED_VAM = 620;

    const int PROBLEM_3_UNBALANCED_VAM = 1000;

    const int PROBLEM_4_MODI = 4185;

    const int PROBLEM_5_MODI = 412;

    const int PROBLEM_6_MODI = 1320;

    const int PROBLEM_7_PMSJRA = 412;

    const int PROBLEM_8_PMSJRA = 1200;
}

namespace CostMatrices
{
    const vector<vector<int>> PROBLEM_1_BALANCED_VAM = {{10, 2, 20, 11}, {12, 7, 9, 20}, {4, 14, 16, 18}};

    const vector<vector<int>> PROBLEM_2_BALANCED_VAM = {{5, 2, 7, 3}, {3, 6, 6, 1}, {6, 1, 2, 4}, {4, 3, 6, 6}};

    const vector<vector<int>> PROBLEM_3_UNBALANCED_VAM = {{8, 15, 10}, {10, 12, 14}, {14, 9, 15}};

    const vector<vector<int>> PROBLEM_4_MODI = {{17, 20, 13, 12}, {15, 21, 26, 25}, {15, 14, 15, 17}};

    const vector<vector<int>> PROBLEM_5_MODI = {{13, 18, 30, 8}, {55, 20, 25, 40}, {30, 6, 50, 10}};

    const vector<vector<int>> PROBLEM_6_MODI = {{11, 9, 6}, {12, 14, 11}, {10, 8, 10}};

    const vector<vector<int>> PROBLEM_7_PMSJRA = {{13, 18, 30, 8}, {55, 20, 25, 40}, {30, 6, 50, 10}};

    const vector<vector<int>> PROBLEM_8_PMSJRA = {{11, 9, 6}, {12, 14, 11}, {10, 8, 10}};
}

TEST(TransportProblemTest, RandomConstructor)
{
    EXPECT_NO_THROW(TransportProblem randomTransportProblem(get<0>(inputs), get<1>(inputs), get<2>(inputs), get<3>(inputs)));
}

TEST(TransportProblemTest, RandomGetCost)
{
    EXPECT_EQ(get<0>(inputs), randomTransportProblem.getCosts());
}

TEST(TransportProblemTest, RandomInitialGetTotalCapacityAndDemandAndCost)
{
    EXPECT_EQ(0, randomTransportProblem.getTotalCapacity());
    EXPECT_EQ(0, randomTransportProblem.getTotalDemand());
    EXPECT_EQ(0, randomTransportProblem.getTotalCost());
}

TEST(TransportProblemTest, RandomGetCostMatrix)
{
    vector<vector<int>> costMatrix = get<0>(inputs);

    for (vector<int> &v : costMatrix)
    {
        v.push_back(0);
    }

    EXPECT_EQ(costMatrix, randomTransportProblem.getCostMatrix());
}

TEST(TransportProblemTest, RandomCalculateTotalCapacity)
{
    randomTransportProblem.balanceProblem();
    int totalCapacity = 0;
    for (size_t i = 0; i < get<2>(inputs).size(); ++i)
    {
        if (get<3>(inputs)[i] == 1)
        {
            totalCapacity += get<2>(inputs)[i];
        }
    }
    EXPECT_EQ(totalCapacity, randomTransportProblem.getTotalCapacity());
}

TEST(TransportProblemTest, RandomCalculateTotalDemand)
{
    int totalDemand = accumulate(get<1>(inputs).begin(), get<1>(inputs).end(), 0);
    EXPECT_EQ(totalDemand, randomTransportProblem.getTotalDemand());
}

TEST(TransportProblemTest, RandomDemandWithDummy)
{
    vector<int> demandsWithDummy = randomTransportProblem.getCustomerDemandsWithDummyDemand();
    int totalDemand = accumulate(get<1>(inputs).begin(), get<1>(inputs).end(), 0);
    int totalCapacity = 0;
    for (size_t i = 0; i < get<2>(inputs).size(); ++i)
    {
        if (get<3>(inputs)[i] == 1)
        {
            totalCapacity += get<2>(inputs)[i];
        }
    }
    int slackDemand = totalCapacity - totalDemand;
    EXPECT_EQ(demandsWithDummy.back(), slackDemand);
}

TEST(TransportProblemTest, Problem1BalancedVAM)
{
    vector<vector<int>> costs = CostMatrices::PROBLEM_1_BALANCED_VAM;
    vector<int> demands = Demands::PROBLEM_1_BALANCED_VAM;
    vector<int> supplies = Supplies::PROBLEM_1_BALANCED_VAM;
    vector<uint8_t> status = {1, 1, 1}; // Asumiendo que todos están abiertos para este ejemplo

    TransportProblem tp(costs, demands, supplies, status);

    EXPECT_EQ(demands, tp.getCustomerDemands());
    EXPECT_EQ(supplies, tp.getFacilityCapacities());
    EXPECT_EQ(costs, tp.getCosts());

    tp.balanceProblem();

    int totalCapacity = 0;
    for (size_t i = 0; i < tp.getFacilityCapacities().size(); ++i)
    {
        if (tp.getFacilityStatus()[i] == 1)
        {
            totalCapacity += tp.getFacilityCapacities()[i];
        }
    }
    EXPECT_EQ(totalCapacity, tp.getTotalCapacity());

    int totalDemand = accumulate(tp.getCustomerDemands().begin(), tp.getCustomerDemands().end(), 0);
    EXPECT_EQ(totalDemand, tp.getTotalDemand());

    vector<int> demandsWithDummy = tp.getCustomerDemandsWithDummyDemand();
    int slackDemand = totalCapacity - totalDemand;
    EXPECT_EQ(demandsWithDummy.back(), slackDemand);

    vector<vector<int>> costMatrix = tp.getCosts();

    for (vector<int> &v : costMatrix)
    {
        v.push_back(0);
    }

    EXPECT_EQ(costMatrix, tp.getCostMatrix());
}

TEST(TransportProblemTest, Problem2BalancedVAM)
{
    vector<vector<int>> costs = CostMatrices::PROBLEM_2_BALANCED_VAM;
    vector<int> demands = Demands::PROBLEM_2_BALANCED_VAM;
    vector<int> supplies = Supplies::PROBLEM_2_BALANCED_VAM;
    vector<uint8_t> status = {1, 1, 1, 1}; // Asumiendo que todos están abiertos para este ejemplo

    TransportProblem tp(costs, demands, supplies, status);

    EXPECT_EQ(demands, tp.getCustomerDemands());
    EXPECT_EQ(supplies, tp.getFacilityCapacities());
    EXPECT_EQ(costs, tp.getCosts());

    tp.balanceProblem();

    int totalCapacity = 0;
    for (size_t i = 0; i < tp.getFacilityCapacities().size(); ++i)
    {
        if (tp.getFacilityStatus()[i] == 1)
        {
            totalCapacity += tp.getFacilityCapacities()[i];
        }
    }
    EXPECT_EQ(totalCapacity, tp.getTotalCapacity());

    int totalDemand = accumulate(tp.getCustomerDemands().begin(), tp.getCustomerDemands().end(), 0);
    EXPECT_EQ(totalDemand, tp.getTotalDemand());

    vector<int> demandsWithDummy = tp.getCustomerDemandsWithDummyDemand();
    int slackDemand = totalCapacity - totalDemand;
    EXPECT_EQ(demandsWithDummy.back(), slackDemand);

    vector<vector<int>> costMatrix = tp.getCosts();

    for (vector<int> &v : costMatrix)
    {
        v.push_back(0);
    }

    EXPECT_EQ(costMatrix, tp.getCostMatrix());
}

TEST(TransportProblemTest, Problem3UnbalancedVAM)
{
    vector<vector<int>> costs = CostMatrices::PROBLEM_3_UNBALANCED_VAM;
    vector<int> demands = Demands::PROBLEM_3_UNBALANCED_VAM;
    vector<int> supplies = Supplies::PROBLEM_3_UNBALANCED_VAM;
    vector<uint8_t> status = {1, 1, 1}; // Asumiendo que todos están abiertos para este ejemplo

    TransportProblem tp(costs, demands, supplies, status);

    EXPECT_EQ(demands, tp.getCustomerDemands());
    EXPECT_EQ(supplies, tp.getFacilityCapacities());
    EXPECT_EQ(costs, tp.getCosts());

    tp.balanceProblem();

    int totalCapacity = 0;
    for (size_t i = 0; i < tp.getFacilityCapacities().size(); ++i)
    {
        if (tp.getFacilityStatus()[i] == 1)
        {
            totalCapacity += tp.getFacilityCapacities()[i];
        }
    }
    EXPECT_EQ(totalCapacity, tp.getTotalCapacity());

    int totalDemand = accumulate(tp.getCustomerDemands().begin(), tp.getCustomerDemands().end(), 0);
    EXPECT_EQ(totalDemand, tp.getTotalDemand());

    vector<int> demandsWithDummy = tp.getCustomerDemandsWithDummyDemand();
    int slackDemand = totalCapacity - totalDemand;
    EXPECT_EQ(demandsWithDummy.back(), slackDemand);

    vector<vector<int>> costMatrix = tp.getCosts();

    for (vector<int> &v : costMatrix)
    {
        v.push_back(0);
    }

    EXPECT_EQ(costMatrix, tp.getCostMatrix());
}

TEST(TransportProblemTest, Problem4MODI)
{
    vector<vector<int>> costs = CostMatrices::PROBLEM_4_MODI;
    vector<int> demands = Demands::PROBLEM_4_MODI;
    vector<int> supplies = Supplies::PROBLEM_4_MODI;
    vector<uint8_t> status = {1, 1, 1}; // Asumiendo que todos están abiertos para este ejemplo

    TransportProblem tp(costs, demands, supplies, status);

    EXPECT_EQ(demands, tp.getCustomerDemands());
    EXPECT_EQ(supplies, tp.getFacilityCapacities());
    EXPECT_EQ(costs, tp.getCosts());

    tp.balanceProblem();

    int totalCapacity = 0;
    for (size_t i = 0; i < tp.getFacilityCapacities().size(); ++i)
    {
        if (tp.getFacilityStatus()[i] == 1)
        {
            totalCapacity += tp.getFacilityCapacities()[i];
        }
    }
    EXPECT_EQ(totalCapacity, tp.getTotalCapacity());

    int totalDemand = accumulate(tp.getCustomerDemands().begin(), tp.getCustomerDemands().end(), 0);
    EXPECT_EQ(totalDemand, tp.getTotalDemand());

    vector<int> demandsWithDummy = tp.getCustomerDemandsWithDummyDemand();
    int slackDemand = totalCapacity - totalDemand;
    EXPECT_EQ(demandsWithDummy.back(), slackDemand);

    vector<vector<int>> costMatrix = tp.getCosts();

    for (vector<int> &v : costMatrix)
    {
        v.push_back(0);
    }

    EXPECT_EQ(costMatrix, tp.getCostMatrix());
}

TEST(TransportProblemTest, Problem5MODI)
{
    vector<vector<int>> costs = CostMatrices::PROBLEM_5_MODI;
    vector<int> demands = Demands::PROBLEM_5_MODI;
    vector<int> supplies = Supplies::PROBLEM_5_MODI;
    vector<uint8_t> status = {1, 1, 1}; // Asumiendo que todos están abiertos para este ejemplo

    TransportProblem tp(costs, demands, supplies, status);

    EXPECT_EQ(demands, tp.getCustomerDemands());
    EXPECT_EQ(supplies, tp.getFacilityCapacities());
    EXPECT_EQ(costs, tp.getCosts());

    tp.balanceProblem();

    int totalCapacity = 0;
    for (size_t i = 0; i < tp.getFacilityCapacities().size(); ++i)
    {
        if (tp.getFacilityStatus()[i] == 1)
        {
            totalCapacity += tp.getFacilityCapacities()[i];
        }
    }
    EXPECT_EQ(totalCapacity, tp.getTotalCapacity());

    int totalDemand = accumulate(tp.getCustomerDemands().begin(), tp.getCustomerDemands().end(), 0);
    EXPECT_EQ(totalDemand, tp.getTotalDemand());

    vector<int> demandsWithDummy = tp.getCustomerDemandsWithDummyDemand();
    int slackDemand = totalCapacity - totalDemand;
    EXPECT_EQ(demandsWithDummy.back(), slackDemand);

    vector<vector<int>> costMatrix = tp.getCosts();

    for (vector<int> &v : costMatrix)
    {
        v.push_back(0);
    }

    EXPECT_EQ(costMatrix, tp.getCostMatrix());
}

TEST(TransportProblemTest, Problem6MODI)
{
    vector<vector<int>> costs = CostMatrices::PROBLEM_6_MODI;
    vector<int> demands = Demands::PROBLEM_6_MODI;
    vector<int> supplies = Supplies::PROBLEM_6_MODI;
    vector<uint8_t> status = {1, 1, 1}; // Asumiendo que todos están abiertos para este ejemplo

    TransportProblem tp(costs, demands, supplies, status);

    EXPECT_EQ(demands, tp.getCustomerDemands());
    EXPECT_EQ(supplies, tp.getFacilityCapacities());
    EXPECT_EQ(costs, tp.getCosts());

    tp.balanceProblem();

    int totalCapacity = 0;
    for (size_t i = 0; i < tp.getFacilityCapacities().size(); ++i)
    {
        if (tp.getFacilityStatus()[i] == 1)
        {
            totalCapacity += tp.getFacilityCapacities()[i];
        }
    }
    EXPECT_EQ(totalCapacity, tp.getTotalCapacity());

    int totalDemand = accumulate(tp.getCustomerDemands().begin(), tp.getCustomerDemands().end(), 0);
    EXPECT_EQ(totalDemand, tp.getTotalDemand());

    vector<int> demandsWithDummy = tp.getCustomerDemandsWithDummyDemand();
    int slackDemand = totalCapacity - totalDemand;
    EXPECT_EQ(demandsWithDummy.back(), slackDemand);

    vector<vector<int>> costMatrix = tp.getCosts();

    for (vector<int> &v : costMatrix)
    {
        v.push_back(0);
    }

    EXPECT_EQ(costMatrix, tp.getCostMatrix());
}

TEST(TransportProblemTest, Problem7PMSJRA)
{
    vector<vector<int>> costs = CostMatrices::PROBLEM_7_PMSJRA;
    vector<int> demands = Demands::PROBLEM_7_MODI;
    vector<int> supplies = Supplies::PROBLEM_7_MODI;
    vector<uint8_t> status = {1, 1, 1}; // Asumiendo que todos están abiertos para este ejemplo

    TransportProblem tp(costs, demands, supplies, status);

    EXPECT_EQ(demands, tp.getCustomerDemands());
    EXPECT_EQ(supplies, tp.getFacilityCapacities());
    EXPECT_EQ(costs, tp.getCosts());

    tp.balanceProblem();

    int totalCapacity = 0;
    for (size_t i = 0; i < tp.getFacilityCapacities().size(); ++i)
    {
        if (tp.getFacilityStatus()[i] == 1)
        {
            totalCapacity += tp.getFacilityCapacities()[i];
        }
    }
    EXPECT_EQ(totalCapacity, tp.getTotalCapacity());

    int totalDemand = accumulate(tp.getCustomerDemands().begin(), tp.getCustomerDemands().end(), 0);
    EXPECT_EQ(totalDemand, tp.getTotalDemand());

    vector<int> demandsWithDummy = tp.getCustomerDemandsWithDummyDemand();
    int slackDemand = totalCapacity - totalDemand;
    EXPECT_EQ(demandsWithDummy.back(), slackDemand);

    vector<vector<int>> costMatrix = tp.getCosts();

    for (vector<int> &v : costMatrix)
    {
        v.push_back(0);
    }

    EXPECT_EQ(costMatrix, tp.getCostMatrix());
}

TEST(TransportProblemTest, Problem8PMSJRA)
{
    vector<vector<int>> costs = CostMatrices::PROBLEM_8_PMSJRA;
    vector<int> demands = Demands::PROBLEM_8_MODI;
    vector<int> supplies = Supplies::PROBLEM_8_MODI;
    vector<uint8_t> status = {1, 1, 1}; // Asumiendo que todos están abiertos para este ejemplo

    TransportProblem tp(costs, demands, supplies, status);

    EXPECT_EQ(demands, tp.getCustomerDemands());
    EXPECT_EQ(supplies, tp.getFacilityCapacities());
    EXPECT_EQ(costs, tp.getCosts());

    tp.balanceProblem();

    int totalCapacity = 0;
    for (size_t i = 0; i < tp.getFacilityCapacities().size(); ++i)
    {
        if (tp.getFacilityStatus()[i] == 1)
        {
            totalCapacity += tp.getFacilityCapacities()[i];
        }
    }
    EXPECT_EQ(totalCapacity, tp.getTotalCapacity());

    int totalDemand = accumulate(tp.getCustomerDemands().begin(), tp.getCustomerDemands().end(), 0);
    EXPECT_EQ(totalDemand, tp.getTotalDemand());

    vector<int> demandsWithDummy = tp.getCustomerDemandsWithDummyDemand();
    int slackDemand = totalCapacity - totalDemand;
    EXPECT_EQ(demandsWithDummy.back(), slackDemand);

    vector<vector<int>> costMatrix = tp.getCosts();

    for (vector<int> &v : costMatrix)
    {
        v.push_back(0);
    }

    EXPECT_EQ(costMatrix, tp.getCostMatrix());
}
