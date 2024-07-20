#ifndef TRANSPORTSOLVER_H
#define TRANSPORTSOLVER_H

#include <vector>
#include <limits>
#include "CapacitatedFacilityLocationProblem/instance.h"
#include "CapacitatedFacilityLocationProblem/solution.h"

/**
 * @class TransportSolver
 * @brief Class to solve the transportation problem for the Capacitated Facility Location Problem.
 */
class TransportSolver {
public:
    /**
     * @brief Constructor for TransportSolver.
     * @param instance Reference to an Instance object.
     */
    TransportSolver(const Instance& instance);

    /**
     * @brief Solves the transportation problem given a set of open facilities.
     * @param y A vector indicating which facilities are open.
     * @return A Solution object representing the solution to the transportation problem.
     */
    Solution solve(const std::vector<uint8_t>& y);

private:
    const Instance& instance; ///< Reference to the instance of the problem.

    /**
     * @brief Uses the Allocation Table Method to find an initial feasible solution.
     * @param costMatrix The cost matrix for transportation.
     * @param allocation The allocation matrix to be filled.
     */
    void allocationTableMethod(std::vector<std::vector<double>>& costMatrix, std::vector<std::vector<int>>& allocation);

    /**
     * @brief Uses the Vogel Approximation Method to optimize the initial solution.
     * @param costMatrix The cost matrix for transportation.
     * @param allocation The allocation matrix to be optimized.
     */
    void vogelApproximationMethod(std::vector<std::vector<double>>& costMatrix, std::vector<std::vector<int>>& allocation);

    /**
     * @brief Calculates the total cost of a given allocation.
     * @param costMatrix The cost matrix for transportation.
     * @param allocation The allocation matrix.
     * @return The total transportation cost.
     */
    double calculateTotalCost(const std::vector<std::vector<double>>& costMatrix, const std::vector<std::vector<int>>& allocation);
};

#endif // TRANSPORTSOLVER_H
