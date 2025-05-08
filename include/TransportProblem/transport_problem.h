#ifndef TRANSPORTATION_PROBLEM_H
#define TRANSPORTATION_PROBLEM_H

#include <vector>

/**
 * @class TransportationProblem
 * @brief Class to solve the transportation problem using the Hungarian method.
 *
 * This class stores supply, demand, cost matrix, and the current assignment matrix.
 * It supports solving the problem using the Hungarian method and includes methods
 * for getting and setting data members.
 */
class TransportationProblem
{
public:
    /**
     * @brief Constructor.
     * @param supply Vector of supply values.
     * @param demand Vector of demand values.
     * @param costMatrix 2D vector representing the cost matrix.
     */
    TransportationProblem(const std::vector<int> &supply,
                          const std::vector<int> &demand,
                          const std::vector<std::vector<int>> &costMatrix);

    /// @brief Destructor.
    ~TransportationProblem() = default;

    /**
     * @brief Solves the transportation problem using the Hungarian Method.
     */
    void solveHungarianMethod();

    /**
     * @brief Returns the total cost of the current assignment.
     * @return Total cost as an integer.
     */
    int getTotalCost() const;

    /**
     * @brief Returns the current assignment matrix.
     * @return 2D vector of assigned quantities.
     */
    const std::vector<std::vector<int>> &getAssignmentMatrix() const;

    /**
     * @brief Returns the cost matrix.
     * @return 2D vector of transportation costs.
     */
    const std::vector<std::vector<int>> &getCostMatrix() const;

    /**
     * @brief Sets the cost matrix.
     * @param newCostMatrix New cost matrix to be assigned.
     */
    void setCostMatrix(const std::vector<std::vector<int>> &newCostMatrix);

    /**
     * @brief Returns the supply vector.
     * @return Vector of supply values.
     */
    const std::vector<int> &getSupply() const;

    /**
     * @brief Sets the supply vector.
     * @param newSupply New supply vector to be assigned.
     */
    void setSupply(const std::vector<int> &newSupply);

    /**
     * @brief Returns the demand vector.
     * @return Vector of demand values.
     */
    const std::vector<int> &getDemand() const;

    /**
     * @brief Sets the demand vector.
     * @param newDemand New demand vector to be assigned.
     */
    void setDemand(const std::vector<int> &newDemand);

    /**
     * @brief Returns the total supply.
     * @return Total supply as an integer.
     */
    int getTotalSupply() const;

    /**
     * @brief Sets the total supply.
     * @param newSupply New total supply value.
     */
    void setTotalSupply(int newSupply);

    /**
     * @brief Returns the total demand.
     * @return Total demand as an integer.
     */
    int getTotalDemand() const;

    /**
     * @brief Sets the total demand.
     * @param newDemand New total demand value.
     */
    void setTotalDemand(int newDemand);

    /**
     * @brief Balances the transportation problem by adding dummy demand if necessary.
     */
    void balance();

    /**
     * @brief Calculates total supply and demand from the current vectors.
     */
    void calculateTotalSupplyAndDemand();

private:
    std::vector<int> supply_;                        ///< Vector of supply values.
    std::vector<int> demand_;                        ///< Vector of demand values.
    std::vector<std::vector<int>> costMatrix_;       ///< Cost matrix.
    std::vector<std::vector<int>> assignmentMatrix_; ///< Current assignment matrix.
    int totalCost_;                                  ///< Total cost of current assignment.
    int totalSupply_;                                ///< Total supply.
    int totalDemand_;                                ///< Total demand.

    /**
     * @brief Initializes or resets the assignment matrix.
     */
    void initializeAssignment();

    
};

#endif // TRANSPORTATION_PROBLEM_H
