#include "CapacitatedFacilityLocationProblem/transport_solver.h"
#include "CapacitatedFacilityLocationProblem/instance.h"
#include <algorithm>
#include <numeric>
#include <iostream>
#include <limits>

TransportSolver::TransportSolver(const Instance& instance)
    : instance(instance) {}

Solution TransportSolver::solve(const std::vector<uint8_t>& y) {
    int numFacilities = instance.getNumFacilities();
    int numCustomers = instance.getNumCustomers();
    const auto& transportationCosts = instance.getTransportationCosts();
    const auto& customerDemands = instance.getCustomerDemands();
    const auto& facilityCapacities = instance.getFacilityCapacities();

    std::vector<std::vector<double>> costMatrix(numFacilities, std::vector<double>(numCustomers, std::numeric_limits<double>::max()));
    std::vector<std::vector<int>> allocation(numFacilities, std::vector<int>(numCustomers, 0));

    // Build cost matrix considering only open facilities
    for (int i = 0; i < numFacilities; ++i) {
        if (y[i] == 1) {
            costMatrix[i] = transportationCosts[i];
        }
    }

    // Step 1: Allocation Table Method
    allocationTableMethod(costMatrix, allocation);

    // Step 2: Vogel Approximation Method
    vogelApproximationMethod(costMatrix, allocation);

    // Calculate total cost
    double totalCost = calculateTotalCost(costMatrix, allocation);

    return Solution(totalCost, y, allocation);
}

void TransportSolver::allocationTableMethod(std::vector<std::vector<double>>& costMatrix, std::vector<std::vector<int>>& allocation) {
    // Implement the Allocation Table Method to find the initial feasible solution
    int numFacilities = instance.getNumFacilities();
    int numCustomers = instance.getNumCustomers();
    const auto& customerDemands = instance.getCustomerDemands();
    const auto& facilityCapacities = instance.getFacilityCapacities();

    std::vector<int> remainingDemands = customerDemands;
    std::vector<int> remainingCapacities = facilityCapacities;

    for (int i = 0; i < numFacilities; ++i) {
        for (int j = 0; j < numCustomers; ++j) {
            if (remainingCapacities[i] > 0 && remainingDemands[j] > 0) {
                int allocationAmount = std::min(remainingCapacities[i], remainingDemands[j]);
                allocation[i][j] = allocationAmount;
                remainingCapacities[i] -= allocationAmount;
                remainingDemands[j] -= allocationAmount;
            }
        }
    }
}

void TransportSolver::vogelApproximationMethod(std::vector<std::vector<double>>& costMatrix, std::vector<std::vector<int>>& allocation) {
    int numFacilities = instance.getNumFacilities();
    int numCustomers = instance.getNumCustomers();
    const auto& customerDemands = instance.getCustomerDemands();
    const auto& facilityCapacities = instance.getFacilityCapacities();

    std::vector<int> remainingDemands = customerDemands;
    std::vector<int> remainingCapacities = facilityCapacities;

    while (std::accumulate(remainingDemands.begin(), remainingDemands.end(), 0) > 0) {
        std::vector<double> rowPenalties(numFacilities, 0);
        std::vector<double> columnPenalties(numCustomers, 0);

        for (int i = 0; i < numFacilities; ++i) {
            if (remainingCapacities[i] > 0) {
                std::vector<double> costs;
                for (int j = 0; j < numCustomers; ++j) {
                    if (remainingDemands[j] > 0) {
                        costs.push_back(costMatrix[i][j]);
                    }
                }
                std::sort(costs.begin(), costs.end());
                if (costs.size() >= 2) {
                    rowPenalties[i] = costs[1] - costs[0];
                } else if (costs.size() == 1) {
                    rowPenalties[i] = costs[0];
                }
            }
        }

        for (int j = 0; j < numCustomers; ++j) {
            if (remainingDemands[j] > 0) {
                std::vector<double> costs;
                for (int i = 0; i < numFacilities; ++i) {
                    if (remainingCapacities[i] > 0) {
                        costs.push_back(costMatrix[i][j]);
                    }
                }
                std::sort(costs.begin(), costs.end());
                if (costs.size() >= 2) {
                    columnPenalties[j] = costs[1] - costs[0];
                } else if (costs.size() == 1) {
                    columnPenalties[j] = costs[0];
                }
            }
        }

        auto maxRowPenaltyIt = std::max_element(rowPenalties.begin(), rowPenalties.end());
        auto maxColumnPenaltyIt = std::max_element(columnPenalties.begin(), columnPenalties.end());

        int selectedRow, selectedColumn;

        if (*maxRowPenaltyIt > *maxColumnPenaltyIt) {
            selectedRow = std::distance(rowPenalties.begin(), maxRowPenaltyIt);
            selectedColumn = std::min_element(costMatrix[selectedRow].begin(), costMatrix[selectedRow].end()) - costMatrix[selectedRow].begin();
        } else {
            selectedColumn = std::distance(columnPenalties.begin(), maxColumnPenaltyIt);
            selectedRow = std::min_element(costMatrix.begin(), costMatrix.end(), [selectedColumn](const std::vector<double>& a, const std::vector<double>& b) {
                return a[selectedColumn] < b[selectedColumn];
            }) - costMatrix.begin();
        }

        int allocationAmount = std::min(remainingCapacities[selectedRow], remainingDemands[selectedColumn]);
        allocation[selectedRow][selectedColumn] += allocationAmount;
        remainingCapacities[selectedRow] -= allocationAmount;
        remainingDemands[selectedColumn] -= allocationAmount;
    }
}

double TransportSolver::calculateTotalCost(const std::vector<std::vector<double>>& costMatrix, const std::vector<std::vector<int>>& allocation) {
    double totalCost = 0.0;
    for (int i = 0; i < costMatrix.size(); ++i) {
        for (int j = 0; j < costMatrix[i].size(); ++j) {
            totalCost += costMatrix[i][j] * allocation[i][j];
        }
    }
    return totalCost;
}
