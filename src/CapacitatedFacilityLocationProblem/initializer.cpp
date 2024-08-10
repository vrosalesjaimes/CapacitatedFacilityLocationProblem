#include "CapacitatedFacilityLocationProblem/initializer.h"
#include <algorithm>
#include <numeric>
#include <limits> 
#include <iostream>
#include <sstream>

using namespace std;


Initializer::Initializer(Instance& instance)
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

vector<pair<double, int>> Initializer::computeRule1() {
    vector<pair<double, int>> piValues;
    auto& capacities = instance.getFacilityCapacities();
    auto& openingCosts = instance.getOpeningCosts();

    for (int i = 0; i < capacities.size(); ++i) {
        double pi = static_cast<double>(capacities[i]) / openingCosts[i];
        piValues.emplace_back(pi, i);
    }

    return piValues;
}

vector<pair<double, int>> Initializer::computeRule2() {
    vector<pair<double, int>> piValues;
    auto& demands = instance.getCustomerDemands();
    auto& capacities = instance.getFacilityCapacities();
    auto& openingCosts = instance.getOpeningCosts();
    auto& transportationCosts = instance.getTransportationCosts();

    double totalDemand = accumulate(demands.begin(), demands.end(), 0.0);

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

vector<pair<double, int>> Initializer::computeRule3() {
    vector<pair<double, int>> piValues;
    auto& capacities = instance.getFacilityCapacities();
    auto& openingCosts = instance.getOpeningCosts();
    auto& transportationCosts = instance.getTransportationCosts();
    int n = instance.getNumCustomers();
    int thirdN = n / 3;

    for (int i = 0; i < capacities.size(); ++i) {
        vector<double> costs = transportationCosts[i];
        nth_element(costs.begin(), costs.begin() + thirdN, costs.end());
        double sumLeastCosts = accumulate(costs.begin(), costs.begin() + thirdN, 0.0);
        double pi = (sumLeastCosts / thirdN) + (openingCosts[i] / capacities[i]);
        piValues.emplace_back(pi, i);
    }

    return piValues;
}

vector<int> Initializer::sortFacilities(vector<pair<double, int>>& piValues) {
    auto sortedPiValues = piValues;
    sort(sortedPiValues.begin(), sortedPiValues.end());
    vector<int> sortedIndices;
    for (auto& [pi, index] : sortedPiValues) {
        sortedIndices.push_back(index);
    }
    return sortedIndices;
}

Solution Initializer::addMethod(vector<int>& sortedFacilities) {
    int numFacilities = instance.getNumFacilities();
    int numCustomers = instance.getNumCustomers();
    auto& customerDemands = instance.getCustomerDemands();
    auto& facilityCapacities = instance.getFacilityCapacities();
    auto& openingCosts = instance.getOpeningCosts();
    auto& transportationCosts = instance.getTransportationCosts();

    vector<bool> y(numFacilities, 0);
    vector<vector<int>> x(numFacilities, vector<int>(numCustomers, 0));

    int totalDemand = accumulate(customerDemands.begin(), customerDemands.end(), 0);

    int totalCapacity = 0;
    double totalCost = numeric_limits<double>::max();
    
    for (int i : sortedFacilities) {
        if (totalCapacity < totalDemand) {
            totalCapacity += facilityCapacities[i];
            y[i] = 1;
        }else {
            break;
        }

    }

    Solution solution(totalCost, y, x);
    return solution;
}