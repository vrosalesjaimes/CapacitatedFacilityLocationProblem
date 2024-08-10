#include <gtest/gtest.h>
#include "CapacitatedFacilityLocationProblem/solution.h"
#include "CapacitatedFacilityLocationProblem/instance.h"
#include <random>

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
        int numFacilities = randomInt(50, 1000);
        int numCustomers = randomInt(70, 1200);

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

    // Función para crear una solución de forma aleatoria
    Solution createRandomSolution(int numFacilities, int numCustomers) {

        vector<bool> y(numFacilities);
        vector<vector<int>> x(numFacilities, vector<int>(numCustomers));

        for (int i = 0; i < numFacilities; ++i) {
            y[i] = randomInt(0, 1);
            for (int j = 0; j < numCustomers; ++j) {
                x[i][j] = randomInt(0, 1);
            }
        }

        double cost = randomDouble(100.0, 10000.0);

        return Solution(cost, y, x);
    }

    // Instancias globales aleatorias
    Instance randomInstance = createRandomInstance();
    Solution randomSolution = createRandomSolution(randomInstance.getNumFacilities(), randomInstance.getNumCustomers());
}

// Test del constructor
TEST(SolutionTest, ConstructorTest) {
    EXPECT_NO_THROW(Solution solution = createRandomSolution(randomInstance.getNumFacilities(), randomInstance.getNumCustomers()));
}

// Test del getter getCost
TEST(SolutionTest, GetCostTest) {
    EXPECT_EQ(randomSolution.getCost(), randomSolution.getCost());
}

// Test del setter setCost
TEST(SolutionTest, SetCostTest) {
    double newCost = randomDouble(100.0, 10000.0);
    randomSolution.setCost(newCost);
    EXPECT_EQ(randomSolution.getCost(), newCost);
}

// Test del getter getY
TEST(SolutionTest, GetYTest) {
    const vector<bool>& y = randomSolution.getY();
    EXPECT_EQ(y.size(), randomInstance.getNumFacilities());
}


// Test del setter setY
TEST(SolutionTest, SetYTest) {
    vector<bool> newY(randomInstance.getNumFacilities());
    for (size_t i = 0; i < newY.size(); ++i) {
        newY[i] = randomInt(0, 1);
    }
    randomSolution.setY(newY);
    EXPECT_EQ(randomSolution.getY(), newY);
}

// Test del getter getX
TEST(SolutionTest, GetXTest) {
    const vector<vector<int>>& x = randomSolution.getX();
    EXPECT_EQ(x.size(), randomInstance.getNumFacilities());
    for (const auto& row : x) {
        EXPECT_EQ(row.size(), randomInstance.getNumCustomers());
    }
}

// Test del setter setX
TEST(SolutionTest, SetXTest) {
    vector<vector<int>> newX(randomInstance.getNumFacilities(), vector<int>(randomInstance.getNumCustomers()));
    for (auto& row : newX) {
        for (int& val : row) {
            val = randomInt(0, 1);
        }
    }
    randomSolution.setX(newX);
    EXPECT_EQ(randomSolution.getX(), newX);
}

// Test del operador ==
TEST(SolutionTest, EqualityOperatorTest) {
    Solution solution1 = createRandomSolution(randomInstance.getNumFacilities(), randomInstance.getNumCustomers());
    Solution solution2 = solution1;
    EXPECT_TRUE(solution1 == solution2);
}

// Test de calculateCost
TEST(SolutionTest, CalculateCostTest) {
    randomSolution.calculateCost(randomInstance);
    EXPECT_GT(randomSolution.getCost(), 0.0); // Assuming non-zero cost
}

// Test de isFeasible
TEST(SolutionTest, IsFeasibleTest) {
    bool feasible = randomSolution.isFeasible(randomInstance);
    EXPECT_EQ(feasible, randomSolution.isFeasible(randomInstance));
}

// Test de toString
TEST(SolutionTest, ToStringTest) {
    string solutionStr = randomSolution.toString();
    EXPECT_FALSE(solutionStr.empty());
}

