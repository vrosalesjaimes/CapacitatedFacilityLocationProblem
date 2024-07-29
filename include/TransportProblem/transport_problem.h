#ifndef TRANSPORT_PROBLEM_H
#define TRANSPORT_PROBLEM_H

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
    TransportProblem(const std::vector<std::vector<int>> &costs, const std::vector<int> &customerDemands, const std::vector<int> &facilityCapacities, const std::vector<uint8_t> &facilityStatus);

    /**
     * @brief Generates an initial solution for the transportation problem.
     */
    void generateInitialSolution();

    /**
     * @brief Generates the next initial solution by changing the status of a facility.
     * @param index Index of the facility to modify.
     * @param open Facility status (open/closed).
     */
    void generateNextInitialSolution(int index, bool open);

    // Getters and Setters

    /**
     * @brief Gets the cost matrix.
     * @return The cost matrix.
     */
    const std::vector<std::vector<int>>& getCosts() const;

    /**
     * @brief Gets the customer demands.
     * @return The customer demands.
     */
    const std::vector<int>& getCustomerDemands() const;

    /**
     * @brief Gets the facility capacities.
     * @return The facility capacities.
     */
    const std::vector<int>& getFacilityCapacities() const;

    /**
     * @brief Gets the facility status.
     * @return The facility status.
     */
    const std::vector<uint8_t>& getFacilityStatus() const;

    /**
     * @brief Gets the assignment of customer demands to facilities.
     * @return The assignment of customer demands to facilities.
     */
    const std::vector<std::vector<int>>& getAssignamentDemandsOfCustomers() const;

    /**
     * @brief Gets the modified cost matrix.
     * @return The modified cost matrix.
     */
    const std::vector<std::vector<int>>& getModifiedCostMatrix() const;

    /**
     * @brief Gets the total demand.
     * @return The total demand.
     */
    int getTotalDemand() const;

    /**
     * @brief Gets the total capacity.
     * @return The total capacity.
     */
    int getTotalCapacity() const;

    /**
     * @brief Gets the customer demands with dummy demand.
     * @return The customer demands with dummy demand.
     */
    const std::vector<int>& getCustomerDemandsWithDummyDemand() const;

    /**
     * @brief Gets the assignments of customer demands.
     * @return The assignments of customer demands.
     */
    const std::vector<std::vector<int>>& getAssignments() const;

    /**
     * @brief Gets the total cost.
     * @return The total cost.
     */
    int getTotalCost() const;

    /**
     * @brief Gets the cost matrix with slack demand.
     * @return The cost matrix with slack demand.
     */
    const std::vector<std::vector<int>>& getCostMatrix() const;

    /**
     * @brief Sets the assignment of customer demands to facilities.
     * @param assignamentDemandsOfCustomers The new assignment of customer demands to facilities.
     */
    void setAssignamentDemandsOfCustomers(const std::vector<std::vector<int>>& assignamentDemandsOfCustomers);

    /**
     * @brief Sets the modified cost matrix.
     * @param modifiedCostMatrix The new modified cost matrix.
     */
    void setModifiedCostMatrix(const std::vector<std::vector<int>>& modifiedCostMatrix);

    /**
     * @brief Sets the total demand.
     * @param totalDemand The new total demand.
     */
    void setTotalDemand(int totalDemand);

    /**
     * @brief Sets the total capacity.
     * @param totalCapacity The new total capacity.
     */
    void setTotalCapacity(int totalCapacity);

    /**
     * @brief Sets the customer demands with dummy demand.
     * @param customerDemandsWithDummyDemand The new customer demands with dummy demand.
     */
    void setCustomerDemandsWithDummyDemand(const std::vector<int>& customerDemandsWithDummyDemand);

    /**
     * @brief Sets the assignments of customer demands.
     * @param assignments The new assignments of customer demands.
     */
    void setAssignments(const std::vector<std::vector<int>>& assignments);

    /**
     * @brief Sets the total cost.
     * @param totalCost The new total cost.
     */
    void setTotalCost(int totalCost);

    /**
     * @brief Sets the cost matrix with slack demand.
     * @param costMatrix The new cost matrix with slack demand.
     */
    void setCostMatrix(const std::vector<std::vector<int>>& costMatrix);

private:
    const std::vector<std::vector<int>> &costs;                         ///< Cost matrix for the transportation problem.
    const std::vector<int> &customerDemands;                            ///< Vector of customer demands.
    const std::vector<int> &facilityCapacities;                         ///< Vector of facility capacities.
    const std::vector<uint8_t> &facilityStatus;                         ///< Vector indicating whether each facility is open.
    std::vector<std::vector<int>> assignamentDemandsOfCustomers;        ///< Assignment of customer demands to facilities.
    std::vector<std::vector<int>> modifiedCostMatrix;                   ///< Modified cost matrix for the transportation problem.
    int totalDemand;                                                    ///< Total demand of the customers.
    int totalCapacity;                                                  ///< Total capacity of the open facilities.
    std::vector<int> customerDemandsWithDummyDemand;                    ///< Vector of customer demands with slack demand.
    std::vector<std::vector<int>> assignments;                          ///< Assignment of customer demands.
    int totalCost;                                                      ///< Total cost of the solution.
    std::vector<std::vector<int>> costMatrix;                           ///< Cost matrix with slack demand.

    /**
     * @brief Initializes the modified cost matrix.
     */
    void initializeModifiedCostMatrix();

    /**
     * @brief Adds a dummy demand to balance the problem.
     */
    void addDummyDemand();

    /**
     * @brief Balances the transportation problem by adding dummy demand.
     */
    void balanceProblem();

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
     * @brief Finds indices of facilities with available capacity.
     * @return Vector of indices of facilities with available capacity.
     */
    std::vector<int> findNonZeroIndicesHolgura();

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
    void reAssignDemands(int index, bool open);
};

#endif //TRANSPORT_PROBLEM_H