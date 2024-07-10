#include <iostream>
#include "CapacityFacilityLocationProblem/instance.h"
#include "CapacityFacilityLocationProblem/solution.h"

int main() {
    std::vector<int> facilityCapacities = {100, 150, 200};
    std::vector<int> customerDemands = {80, 90, 120};
    std::vector<double> openingCosts = {500, 700, 800};
    std::vector<std::vector<double>> transportationCosts = {
        {4, 6, 8},
        {5, 7, 9},
        {3, 5, 7}
    };

    // Initial solution with arbitrary assignments
    std::vector<uint8_t> y = {1, 0, 1};
    std::vector<std::vector<uint8_t>> x = {
        {1, 0, 1},
        {0, 1, 0},
        {1, 0, 0}
    };
    Solution initialSolution(0.0, y, x);

    Instance instance(3, 3, facilityCapacities, customerDemands, openingCosts, transportationCosts, initialSolution);

    initialSolution.calculateCost(instance);

    std::cout << "Number of facilities: " << instance.getNumFacilities() << std::endl;
    std::cout << "Number of customers: " << instance.getNumCustomers() << std::endl;
    std::cout << "Total cost of the solution: " << initialSolution.getCost() << std::endl;

    return 0;
}
