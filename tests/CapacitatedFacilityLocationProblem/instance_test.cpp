#include <gtest/gtest.h>
#include <random>

#include "CapacitatedFacilityLocationProblem/instance.h"

using namespace std;

namespace {
    // Función para generar un número aleatorio en un rango dado
    int randomInt(int min, int max) {
        static std::mt19937 rng(std::random_device{}());
        std::uniform_int_distribution<int> dist(min, max);
        return dist(rng);
    }

    double randomDouble(double min, double max) {
        static std::mt19937 rng(std::random_device{}());
        std::uniform_real_distribution<double> dist(min, max);
        return dist(rng);
    }

    // Función para crear una instancia de forma aleatoria
    Instance createRandomInstance() {
        int numFacilities = randomInt(5, 10);
        int numCustomers = randomInt(5, 10);

        vector<int> facilityCapacities(numFacilities);
        vector<double> openingCosts(numFacilities);
        vector<int> customerDemands(numCustomers);
        vector<vector<double>> transportationCosts(numFacilities, vector<double>(numCustomers));

        for (int i = 0; i < numFacilities; ++i) {
            facilityCapacities[i] = randomInt(50, 200);
            openingCosts[i] = randomDouble(1000.0, 5000.0);
        }

        for (int j = 0; j < numCustomers; ++j) {
            customerDemands[j] = randomInt(10, 100);
            for (int i = 0; i < numFacilities; ++i) {
                transportationCosts[i][j] = randomDouble(10.0, 50.0);
            }
        }

        vector<bool> y(numFacilities);
        vector<vector<int>> x(numFacilities, vector<int>(numCustomers));

        Solution bestSolution(0.0, y, x);

        return Instance(numFacilities, numCustomers, facilityCapacities, customerDemands, openingCosts, transportationCosts, bestSolution);
    }

    // Instancia global aleatoria
    Instance randomInstance = createRandomInstance();
}

// Test del constructor
TEST(InstanceTest, ConstructorTest) {
    EXPECT_NO_THROW(Instance instance = createRandomInstance());
}

// Test del getter getNumFacilities
TEST(InstanceTest, GetNumFacilitiesTest) {
    EXPECT_EQ(randomInstance.getNumFacilities(), randomInstance.getFacilityCapacities().size());
}

// Test del getter getNumCustomers
TEST(InstanceTest, GetNumCustomersTest) {
    EXPECT_EQ(randomInstance.getNumCustomers(), randomInstance.getCustomerDemands().size());
}

// Test del getter getFacilityCapacities
TEST(InstanceTest, GetFacilityCapacitiesTest) {
    const auto& capacities = randomInstance.getFacilityCapacities();
    EXPECT_EQ(capacities.size(), randomInstance.getNumFacilities());
    for (int cap : capacities) {
        EXPECT_GE(cap, 50);
        EXPECT_LE(cap, 200);
    }
}

// Test del getter getCustomerDemands
TEST(InstanceTest, GetCustomerDemandsTest) {
    const auto& demands = randomInstance.getCustomerDemands();
    EXPECT_EQ(demands.size(), randomInstance.getNumCustomers());
    for (int demand : demands) {
        EXPECT_GE(demand, 10);
        EXPECT_LE(demand, 100);
    }
}

// Test del getter getOpeningCosts
TEST(InstanceTest, GetOpeningCostsTest) {
    const auto& costs = randomInstance.getOpeningCosts();
    EXPECT_EQ(costs.size(), randomInstance.getNumFacilities());
    for (double cost : costs) {
        EXPECT_GE(cost, 1000.0);
        EXPECT_LE(cost, 5000.0);
    }
}

// Test del getter getTransportationCosts
TEST(InstanceTest, GetTransportationCostsTest) {
    const auto& costs = randomInstance.getTransportationCosts();
    EXPECT_EQ(costs.size(), randomInstance.getNumFacilities());
    for (const auto& facilityCosts : costs) {
        EXPECT_EQ(facilityCosts.size(), randomInstance.getNumCustomers());
        for (double cost : facilityCosts) {
            EXPECT_GE(cost, 10.0);
            EXPECT_LE(cost, 50.0);
        }
    }
}

// Test del getter getBestSolution
TEST(InstanceTest, GetBestSolutionTest) {
    const Solution& solution = randomInstance.getBestSolution();
    EXPECT_EQ(solution.getCost(), 0.0);  // Asumiendo que el costo inicial es 0.0 en la solución por defecto
}

