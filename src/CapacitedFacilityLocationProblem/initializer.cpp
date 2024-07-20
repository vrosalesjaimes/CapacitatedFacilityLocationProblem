#include "CapacitedFacilityLocationProblem/initializer.h"
#include <algorithm>
#include <numeric>
#include <limits> 
#include <iostream>
#include <sstream>


Initializer::Initializer(const Instance& instance)
    : instance(instance) {}

Solution Initializer::generateInitialSolution() {
    // Compute P_i values for each rule
    auto rule1Pi = computeRule1();
    auto rule2Pi = computeRule2();
    auto rule3Pi = computeRule3();

    // Sort facilities based on P_i values for each rule
    auto sortedFacilities1 = sortFacilities(rule1Pi);
    auto sortedFacilities2 = sortFacilities(rule2Pi);
    auto sortedFacilities3 = sortFacilities(rule3Pi);

    // Generate solutions using ADD method
    auto solution1 = addMethod(sortedFacilities1);
    auto solution2 = addMethod(sortedFacilities2);
    auto solution3 = addMethod(sortedFacilities3);

    // Compare solutions and return the best one
    if (solution1.getCost() <= solution2.getCost() && solution1.getCost() <= solution3.getCost()) {
        return solution1;
    } else if (solution2.getCost() <= solution1.getCost() && solution2.getCost() <= solution3.getCost()) {
        return solution2;
    } else {
        return solution3;
    }
}

std::vector<std::pair<double, int>> Initializer::computeRule1() {
    std::vector<std::pair<double, int>> piValues;
    const auto& capacities = instance.getFacilityCapacities();
    const auto& openingCosts = instance.getOpeningCosts();

    for (int i = 0; i < capacities.size(); ++i) {
        double pi = static_cast<double>(capacities[i]) / openingCosts[i];
        piValues.emplace_back(pi, i);
    }

    return piValues;
}

std::vector<std::pair<double, int>> Initializer::computeRule2() {
    std::vector<std::pair<double, int>> piValues;
    const auto& demands = instance.getCustomerDemands();
    const auto& capacities = instance.getFacilityCapacities();
    const auto& openingCosts = instance.getOpeningCosts();
    const auto& transportationCosts = instance.getTransportationCosts();

    double totalDemand = std::accumulate(demands.begin(), demands.end(), 0.0);

    for (int i = 0; i < capacities.size(); ++i) {
        double transportationCostSum = 0.0;
        for (int j = 0; j < demands.size(); ++j) {
            transportationCostSum += transportationCosts[i][j] * demands[j];
        }
        double pi = (totalDemand / transportationCostSum) * capacities[i] - openingCosts[i];
        piValues.emplace_back(pi, i);
    }

    return piValues;
}

std::vector<std::pair<double, int>> Initializer::computeRule3() {
    std::vector<std::pair<double, int>> piValues;
    const auto& capacities = instance.getFacilityCapacities();
    const auto& openingCosts = instance.getOpeningCosts();
    const auto& transportationCosts = instance.getTransportationCosts();
    int n = instance.getNumCustomers();
    int thirdN = n / 3;

    for (int i = 0; i < capacities.size(); ++i) {
        std::vector<double> costs = transportationCosts[i];
        std::nth_element(costs.begin(), costs.begin() + thirdN, costs.end());
        double sumLeastCosts = std::accumulate(costs.begin(), costs.begin() + thirdN, 0.0);
        double pi = (sumLeastCosts / thirdN) + (openingCosts[i] / capacities[i]);
        piValues.emplace_back(pi, i);
    }

    return piValues;
}

std::vector<int> Initializer::sortFacilities(const std::vector<std::pair<double, int>>& piValues) {
    auto sortedPiValues = piValues;
    std::sort(sortedPiValues.begin(), sortedPiValues.end());
    std::vector<int> sortedIndices;
    for (const auto& [pi, index] : sortedPiValues) {
        sortedIndices.push_back(index);
    }
    return sortedIndices;
}

Solution Initializer::addMethod(const std::vector<int>& sortedFacilities) {
    int numFacilities = instance.getNumFacilities();
    int numCustomers = instance.getNumCustomers();
    const auto& customerDemands = instance.getCustomerDemands();
    const auto& facilityCapacities = instance.getFacilityCapacities();
    const auto& openingCosts = instance.getOpeningCosts();
    const auto& transportationCosts = instance.getTransportationCosts();

    std::vector<uint8_t> y(numFacilities, 0);
    std::vector<std::vector<uint8_t>> x(numFacilities, std::vector<uint8_t>(numCustomers, 0));

    int totalDemand = std::accumulate(customerDemands.begin(), customerDemands.end(), 0);

    int totalCapacity = 0;
    double totalCost = std::numeric_limits<double>::max();
    
    for (int i : sortedFacilities) {
        totalCapacity += facilityCapacities[i];

        if (totalCapacity < totalDemand) {
            y[i] = 1;
        }else {
            break;
        }

    }

    Solution solution(totalCost, y, x);
    return solution;
}