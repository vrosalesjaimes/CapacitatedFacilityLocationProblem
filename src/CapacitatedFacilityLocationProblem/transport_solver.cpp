#include "CapacitatedFacilityLocationProblem/transport_solver.h"
#include <iostream>
#include <algorithm>
#include <numeric>
#include <limits>

const double INF = std::numeric_limits<double>::infinity();

/**
 * @brief Constructor for the TransportSolver class.
 * @param instance The instance of the capacitated facility location problem.
 * @param facilityOpen A vector indicating whether each facility is open (1) or closed (0).
 */
TransportSolver::TransportSolver(const Instance& instance, const std::vector<uint8_t>& facilityOpen)
    : instance(instance), facilityOpen(facilityOpen), capacity(0) {

    initializeCostMatrix();

    customerDemands = instance.getCustomerDemands();
    totalDemand = std::accumulate(customerDemands.begin(), customerDemands.end(), 0);
    
    const auto& facilityCapacities = instance.getFacilityCapacities();
    for (size_t i = 0; i < facilityCapacities.size(); ++i) {
        if (facilityOpen[i]) {
            capacity += facilityCapacities[i];
        }
    }

    customerDemands.push_back(capacity - totalDemand); // Add slack demand

    assignamentDemandsOfCustomers.resize(instance.getNumFacilities() + 1, std::vector<int>(instance.getNumCustomers() + 1, 0));
}

/**
 * @brief Initializes the cost matrix for the transportation problem.
 */
void TransportSolver::initializeCostMatrix() {
    int numFacilities = instance.getNumFacilities();
    int numCustomers = instance.getNumCustomers();
    const auto& transportationCosts = instance.getTransportationCosts();

    costMatrix.resize(numFacilities + 1, std::vector<double>(numCustomers + 1, std::numeric_limits<double>::max()));

    for (int i = 0; i < numFacilities; ++i) {
        if (facilityOpen[i]) {
            for (int j = 0; j < numCustomers; ++j) {
                costMatrix[i][j] = transportationCosts[i][j];
            }
            costMatrix[i][numCustomers] = 0; // Cost for the slack node
        }
    }
    for (int j = 0; j < numCustomers; ++j) {
        costMatrix[numFacilities][j] = std::numeric_limits<double>::max(); // Slack node has no costs towards customers
    }
    costMatrix[numFacilities][numCustomers] = 0; // Slack node to itself
}

/**
 * @brief Updates the cost matrix when a facility is opened or closed.
 * @param index The index of the facility to update.
 * @param open Boolean indicating whether the facility is being opened (true) or closed (false).
 */
void TransportSolver::updateCostMatrix(int index, bool open) {
    int numCustomers = instance.getNumCustomers();
    const auto& transportationCosts = instance.getTransportationCosts();

    for (int j = 0; j < numCustomers; ++j) {
        costMatrix[index][j] = open ? transportationCosts[index][j] : std::numeric_limits<double>::max();
    }
    costMatrix[index][numCustomers] = open ? 0 : std::numeric_limits<double>::max(); // Slack node
}

/**
 * @brief Updates the total capacity when a facility is opened or closed.
 * @param index The index of the facility to update.
 * @param open Boolean indicating whether the facility is being opened (true) or closed (false).
 */
void TransportSolver::updateCapacity(int index, bool open) {
    capacity += open ? instance.getFacilityCapacities()[index] : -instance.getFacilityCapacities()[index];
    customerDemands.back() = capacity - totalDemand;
}

/**
 * @brief Reassigns customer demands when a facility is opened or closed.
 * @param index The index of the facility to update.
 * @param open Boolean indicating whether the facility is being opened (true) or closed (false).
 */
void TransportSolver::reAssignamentDemands(int index, bool open) {
    if (open) {
        std::fill(assignamentDemandsOfCustomers[index].begin(), assignamentDemandsOfCustomers[index].end(), 0);
        assignamentDemandsOfCustomers[index][instance.getNumCustomers()] = instance.getFacilityCapacities()[index];
    } else {
        auto nonZeroIndices = findNonZeroIndicesHolgura();
        auto nonZeroIndicesFacilityClose = findNonZeroIndicesFacilities(index);

        for (auto i : nonZeroIndicesFacilityClose) {
            int demand = assignamentDemandsOfCustomers[index][i];
            for (auto j : nonZeroIndices) {
                if (demand <= assignamentDemandsOfCustomers[j].back()) {
                    assignamentDemandsOfCustomers[j][i] = demand;
                    assignamentDemandsOfCustomers[j].back() -= demand;
                    break;
                } else {
                    demand -= assignamentDemandsOfCustomers[j].back();
                    assignamentDemandsOfCustomers[j][i] = assignamentDemandsOfCustomers[j].back();
                    assignamentDemandsOfCustomers[j].back() = 0;
                }
            }
            assignamentDemandsOfCustomers[index][i] = 0;
        }
    }
}

/**
 * @brief Finds the indices of non-zero entries in the slack demand row.
 * @return A vector of indices with non-zero entries.
 */
std::vector<int> TransportSolver::findNonZeroIndicesHolgura() {
    std::vector<int> indices;
    for (size_t i = 0; i < costMatrix.size(); ++i) {
        if (costMatrix[i].back() != 0) {
            indices.push_back(i);
        }
    }
    return indices;
}

/**
 * @brief Finds the indices of non-zero entries for a given facility.
 * @param index The index of the facility.
 * @return A vector of indices with non-zero entries for the given facility.
 */
std::vector<int> TransportSolver::findNonZeroIndicesFacilities(int index) {
    std::vector<int> indices;
    for (size_t i = 0; i < instance.getCustomerDemands().size(); ++i) {
        if (assignamentDemandsOfCustomers[index][i] != 0) {
            indices.push_back(i);
        }
    }
    return indices;
}

/**
 * @brief Solves the transportation problem and returns the solution.
 * @return A Solution object representing the solution to the transportation problem.
 */
Solution TransportSolver::solve() {
    // Implementar el algoritmo de red para resolver el problema de transporte
    // utilizando la matriz de costos y las demandas
    // Esta parte es compleja y requerirá más detalles para la implementación completa

    // Para ahora, devolver una solución vacía
    return Solution(0, std::vector<uint8_t>(), std::vector<std::vector<int>>());
}

/**
 * @brief Generates the next initial solution by updating the cost matrix, capacity, and demand assignments.
 * @param index The index of the facility to update.
 * @param open Boolean indicating whether the facility is being opened (true) or closed (false).
 */
void TransportSolver::generateNextInitialSolution(int index, bool open) {
    updateCostMatrix(index, open);
    updateCapacity(index, open);
    reAssignamentDemands(index, open);
}

/**
 * @brief Finds the differences in rows and columns of the cost matrix.
 * @param grid The cost matrix.
 * @return A pair of vectors containing row and column differences.
 */
std::pair<std::vector<int>, std::vector<int>> TransportSolver::findDiff(const std::vector<std::vector<double>>& grid) {
    std::vector<int> rowDiff;
    std::vector<int> colDiff;

    for (const auto& row : grid) {
        std::vector<double> arr = row;
        std::sort(arr.begin(), arr.end());
        rowDiff.push_back(static_cast<int>(arr[1] - arr[0]));
    }

    for (size_t col = 0; col < grid[0].size(); ++col) {
        std::vector<double> arr;
        for (const auto& row : grid) {
            arr.push_back(row[col]);
        }
        std::sort(arr.begin(), arr.end());
        colDiff.push_back(static_cast<int>(arr[1] - arr[0]));
    }

    return {rowDiff, colDiff};
}

/**
 * @brief Solves the transportation problem using the Vogel Approximation Method (VAM) and returns the solution.
 * @return A Solution object representing the solution to the transportation problem.
 */
std::vector<std::vector<int>> TransportSolver::VogelApproximationMethod() {
    std::vector<int> supply = instance.getFacilityCapacities();
    std::vector<int> demand = customerDemands;
    int n = costMatrix.size();
    int m = costMatrix[0].size();
    double ans = 0;

    while (*std::max_element(supply.begin(), supply.end()) != 0 || *std::max_element(demand.begin(), demand.end()) != 0) {
        auto diffs = findDiff(costMatrix);
        std::vector<int> row = diffs.first;
        std::vector<int> col = diffs.second;

        int maxi1 = *std::max_element(row.begin(), row.end());
        int maxi2 = *std::max_element(col.begin(), col.end());

        if (maxi1 >= maxi2) {
            for (size_t ind = 0; ind < row.size(); ++ind) {
                if (row[ind] == maxi1) {
                    double mini1 = *std::min_element(costMatrix[ind].begin(), costMatrix[ind].end());
                    for (size_t ind2 = 0; ind2 < costMatrix[ind].size(); ++ind2) {
                        if (costMatrix[ind][ind2] == mini1) {
                            int mini2 = std::min(supply[ind], demand[ind2]);
                            ans += mini2 * mini1;
                            supply[ind] -= mini2;
                            demand[ind2] -= mini2;

                            if (demand[ind2] == 0) {
                                for (int r = 0; r < n; ++r) {
                                    costMatrix[r][ind2] = INF;
                                }
                            } else {
                                std::fill(costMatrix[ind].begin(), costMatrix[ind].end(), INF);
                            }
                            break;
                        }
                    }
                    break;
                }
            }
        } else {
            for (size_t ind = 0; ind < col.size(); ++ind) {
                if (col[ind] == maxi2) {
                    double mini1 = INF;
                    for (int j = 0; j < n; ++j) {
                        mini1 = std::min(mini1, costMatrix[j][ind]);
                    }

                    for (size_t ind2 = 0; ind2 < n; ++ind2) {
                        if (costMatrix[ind2][ind] == mini1) {
                            int mini2 = std::min(supply[ind2], demand[ind]);
                            ans += mini2 * mini1;
                            supply[ind2] -= mini2;
                            demand[ind] -= mini2;

                            if (demand[ind] == 0) {
                                for (int r = 0; r < n; ++r) {
                                    costMatrix[r][ind] = INF;
                                }
                            } else {
                                std::fill(costMatrix[ind2].begin(), costMatrix[ind2].end(), INF);
                            }
                            break;
                        }
                    }
                    break;
                }
            }
        }
    }

    return assignamentDemandsOfCustomers;
}