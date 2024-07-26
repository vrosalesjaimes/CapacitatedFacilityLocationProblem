#ifndef TRANSPORT_SOLVER_H
#define TRANSPORT_SOLVER_H

#include "CapacitatedFacilityLocationProblem/instance.h"
#include "CapacitatedFacilityLocationProblem/solution.h"
#include <vector>
#include <limits>
#include <iostream>

/**
 * @class TransportSolver
 * @brief Solver for the transportation problem in the capacitated facility location problem.
 */
class TransportSolver
{
public:
    /**
     * @brief Constructor for the TransportSolver class.
     * @param instance The instance of the capacitated facility location problem.
     * @param facilityOpen A vector indicating whether each facility is open (1) or closed (0).
     */
    TransportSolver(const Instance &instance, const std::vector<uint8_t> &facilityOpen);

    /**
     * @brief Generates the next initial solution by updating the cost matrix, capacity, and demand assignments.
     * @param index The index of the facility to update.
     * @param open Boolean indicating whether the facility is being opened (true) or closed (false).
     */
    void generateNextInitialSolution(int index, bool open);

    /**
     * @brief Generates the initial solution for the transportation problem.
     */
    void generateInitialSolution();

    /**
     * @brief Solves the transportation problem and returns the solution.
     * @return A Solution object representing the solution to the transportation problem.
     */
    Solution solve();

    void test();

private:
    const Instance &instance;                                    ///< Reference to the problem instance.
    int capacity;                                                ///< Total capacity of the open facilities.
    int totalDemand;                                             ///< Total demand of the customers.
    std::vector<std::vector<double>> costMatrix;                 ///< Cost matrix for the transportation problem.
    std::vector<int> customerDemands;                            ///< Vector of customer demands.
    std::vector<uint8_t> facilityOpen;                           ///< Vector indicating whether each facility is open.
    std::vector<std::vector<int>> assignamentDemandsOfCustomers; ///< Assignment of customer demands to facilities.

    /**
     * @brief Finds the indices of non-zero entries in the slack demand row.
     * @return A vector of indices with non-zero entries.
     */
    std::vector<int> findNonZeroIndicesHolgura();

    /**
     * @brief Finds the indices of non-zero entries for a given facility.
     * @param index The index of the facility.
     * @return A vector of indices with non-zero entries for the given facility.
     */
    std::vector<int> findNonZeroIndicesFacilities(int index);

    /**
     * @brief Initializes the cost matrix for the transportation problem.
     */
    void initializeCostMatrix();

    /**
     * @brief Updates the cost matrix when a facility is opened or closed.
     * @param index The index of the facility to update.
     * @param open Boolean indicating whether the facility is being opened (true) or closed (false).
     */
    void updateCostMatrix(int index, bool open);

    /**
     * @brief Updates the total capacity when a facility is opened or closed.
     * @param index The index of the facility to update.
     * @param open Boolean indicating whether the facility is being opened (true) or closed (false).
     */
    void updateCapacity(int index, bool open);

    /**
     * @brief Reassigns customer demands when a facility is opened or closed.
     * @param index The index of the facility to update.
     * @param open Boolean indicating whether the facility is being opened (true) or closed (false).
     */
    void reAssignamentDemands(int index, bool open);

    /**
     * @brief Finds the differences in rows and columns of the cost matrix.
     * @param grid The cost matrix.
     * @return A pair of vectors containing row and column differences.
     */
    std::pair<std::vector<int>, std::vector<int>> findDiff(const std::vector<std::vector<double>> &grid);

    /**
     * @brief Solves the transportation problem using the Vogel Approximation Method (VAM) and returns the solution.
     * @return A Solution object representing the solution to the transportation problem.
     */
    std::vector<std::vector<int>> TransportSolver::VogelApproximationMethod();
};

#endif // TRANSPORT_SOLVER_H
