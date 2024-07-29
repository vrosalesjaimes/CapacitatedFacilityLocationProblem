#ifndef TRANSPORT_PROBLEM_H
#define TRANSPORT_PROBLEM_H

#include "TransportProblem/transport_solver.h"

#include <vector>
#include <limits>
#include <cstdint>

/**
 * @class TransportProblem
 * @brief Represents a transportation problem.
 */
class TransportProblem
{
public:
    /**
     * @brief Constructor to initialize a transportation problem.
     * @param costs Cost matrix for the transportation problem.
     * @param customerDemands Vector of customer demands.
     * @param facilityCapacities Vector of facility capacities.
     * @param facilityStatus Vector indicating whether each facility is open.
     */
    TransportProblem(std::vector<std::vector<int>> &costMatrix, std::vector<int> &customerDemands,  std::vector<int> &facilityCapacities, std::vector<uint8_t> &facilityStatus);

    /**
     * @brief Generates an initial solution for the transportation problem.
     */
    void generateInitialSolution();

    /**
     * @brief Generates the next initial solution by changing the status of a facility.
     * @param index Index of the facility to modify.
     * @param open Facility status (open/closed).
     */
    void generateNextInitialSolution(int index, bool open, std::vector<int> dummyAssignamentIndex);

    void solveVAM();  // Método para el algoritmo VAM

    void printProblem();

private:
    std::vector<std::vector<int>> &costMatrix;                         ///< Cost matrix for the transportation problem.
    std::vector<int> &customerDemands;                            ///< Vector of customer demands.
    std::vector<int> &facilityCapacities;                         ///< Vector of facility capacities.
    std::vector<uint8_t> &facilityStatus;                         ///< Vector indicating whether each facility is open.
    std::vector<std::vector<int>> assignamentDemandsOfCustomers;        ///< Assignment of customer demands to facilities.
    std::vector<std::vector<int>> modifiedCostMatrix;                   ///< Modified cost matrix for the transportation problem.
    int totalDemand;                                                    ///< Total demand of the customers.
    int totalCapacity;                                                  ///< Total capacity of the open facilities.
    std::vector<std::vector<int>> assignments;                          ///< Assignment of customer demands.
    int totalCost;                                                      ///< Total cost of the solution.
    TransportSolver solver;                                             ///< Solver for the transportation problem.

    /**
     * @brief Initializes the modified cost matrix.
     */
    void initializeModifiedCostMatrix();

    /**
     * @brief Updates the modified cost matrix.
     * @param index Index of the facility to modify.
     * @param open Facility status (open/closed).
     */
    void updateCostMatrix(int index, bool open);

    /**
     * @brief Updates the total capacity.
     * @param index Index of the facility to modify.
     * @param open Facility status (open/closed).
     */
    void updateCapacity(int index, bool open);

    /**
     * @brief Finds indices of customers with unmet demand by a facility.
     * @param index Index of the facility to check.
     * @return Vector of indices of customers with unmet demand.
     */
    std::vector<int> findNonZeroIndicesFacility(int index);

    /**
     * @brief Reassigns customer demands when changing the status of a facility.
     * @param index Index of the facility to modify.
     * @param open Facility status (open/closed).
     */
    void reAssignDemands(int index, bool open, std::vector<int> dummyAssignamentIndex);

    void initializeAssignamentDemandsOfCustomers();

    int calculateRestCapacity(int index);
    
};

#endif //TRANSPORT_PROBLEM_H