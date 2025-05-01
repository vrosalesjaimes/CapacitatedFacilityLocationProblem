#ifndef TRANSPORTATION_PROBLEM_H
#define TRANSPORTATION_PROBLEM_H

#include <vector>

/**
 * @class TransportationProblem
 * @brief Efficient class to solve the transportation problem using various methods.
 *
 * This class stores supply, demand, cost matrix, and the current assignment matrix.
 * It supports solving the problem using Northwest Corner, Vogel's Approximation,
 * and the Hungarian method.
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
     * @brief Solves the problem using the Northwest Corner Method.
     */
    void solveNorthwestCorner();

    /**
     * @brief Solves the problem using Vogel's Approximation Method.
     */
    void solveVogelsApproximation();

    /**
     * @brief Solves the problem using the Hungarian Method.
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
     * @brief Returns the supply vector.
     * @return Vector of supply values.
     */
    const std::vector<int> &getSupply() const;

    /**
     * @brief Returns the demand vector.
     * @return Vector of demand values.
     */
    const std::vector<int> &getDemand() const;

    /**
     * @brief Sets the cost matrix (replaces the current one).
     * @param newCostMatrix New cost matrix to use.
     */
    void setCostMatrix(const std::vector<std::vector<int>> &newCostMatrix);

    /**
     * @brief Sets the supply vector.
     * @param newSupply New supply vector to use.
     */
    void setSupply(const std::vector<int> &newSupply);

    /**
     * @brief Sets the demand vector.
     * @param newDemand New demand vector to use.
     */
    void setDemand(const std::vector<int> &newDemand);

    /**
     * @brief Balances the transportation problem by adding a dummy demand if total supply exceeds total demand.
     */
    void balance();

private:
    std::vector<int> supply_;                        ///< Vector of supply values.
    std::vector<int> demand_;                        ///< Vector of demand values.
    std::vector<std::vector<int>> costMatrix_;       ///< Cost matrix.
    std::vector<std::vector<int>> assignmentMatrix_; ///< Current assignment matrix.
    int totalCost_;                                  ///< Total cost of current assignment.

    void initializeAssignment(); ///< Helper to reset/initialize assignment.
};

#endif // TRANSPORTATION_PROBLEM_H
