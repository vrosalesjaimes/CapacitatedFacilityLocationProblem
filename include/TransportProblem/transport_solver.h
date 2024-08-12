#ifndef TRANSPORT_SOLVER_H
#define TRANSPORT_SOLVER_H

#include <vector>
using namespace std;

/**
 * @class TransportSolver
 * @brief A class to solve transportation problems using various algorithms like VAM and MODI.
 */
class TransportSolver
{
public:
    /**
     * @brief Constructor to initialize the transport solver with the cost matrix, supply, and demand.
     * @param costMatrix A reference to a 2D vector containing the cost matrix.
     * @param supply A vector containing the supply values for each source.
     * @param demand A vector containing the demand values for each destination.
     * @param assignamentMatrix A reference to a 2D vector to store the assignment of supply to demand.
     * @param totalSupply A reference to an integer representing the total supply.
     * @param totalDemand A reference to an integer representing the total demand.
     */
    TransportSolver(vector<vector<int>> &costMatrix,
                    vector<int> supply,
                    vector<int> demand,
                    vector<vector<int>> &assignamentMatrix,
                    int &totalSupply,
                    int &totalDemand);

    /**
     * @brief Default constructor.
     */
    TransportSolver();

    /**
     * @brief Solves the transportation problem using the Vogel's Approximation Method (VAM).
     */
    void solveVAM();  

    /**
     * @brief Solves the transportation problem using the Modified Distribution Method (MODI).
     */
    void solveMODI(); 

    /**
     * @brief Gets the current cost matrix.
     * @return A reference to the 2D vector representing the cost matrix.
     */
    const vector<vector<int>>& getCostMatrix() const;

    /**
     * @brief Gets the current supply vector.
     * @return A reference to the vector representing the supply.
     */
    const vector<int>& getSupply() const;

    /**
     * @brief Sets the supply vector.
     * @param newSupply A vector representing the new supply values.
     */
    void setSupply(const vector<int>& newSupply);

    /**
     * @brief Gets the current demand vector.
     * @return A reference to the vector representing the demand.
     */
    const vector<int>& getDemand() const;

    /**
     * @brief Sets the demand vector.
     * @param newDemand A vector representing the new demand values.
     */
    void setDemand(const vector<int>& newDemand);

    /**
     * @brief Gets the current total supply value.
     * @return An integer representing the total supply.
     */
    int getTotalSupply() const;

    /**
     * @brief Sets the total supply value.
     * @param newTotalSupply An integer representing the new total supply.
     */
    void setTotalSupply(int newTotalSupply);

    /**
     * @brief Gets the current total demand value.
     * @return An integer representing the total demand.
     */
    int getTotalDemand() const;

    /**
     * @brief Sets the total demand value.
     * @param newTotalDemand An integer representing the new total demand.
     */
    void setTotalDemand(int newTotalDemand);

    /**
     * @brief Gets the current assignment matrix.
     * @return A reference to the 2D vector representing the assignment matrix.
     */
    const vector<vector<int>>& getAssignamentMatrix() const;

    /**
     * @brief Sets the assignment matrix.
     * @param newAssignamentMatrix A reference to the 2D vector representing the new assignment matrix.
     */
    void setAssignamentMatrix(const vector<vector<int>>& newAssignamentMatrix);

    /**
     * @brief Gets the current total cost.
     * @return An integer representing the total cost.
     */
    int getTotalCost() const;

    /**
     * @brief Sets the total cost.
     * @param newTotalCost An integer representing the new total cost.
     */
    void setTotalCost(int newTotalCost);

    int VogelsApproximationMethod();

    int MODI();

    int PSR();

private:
    vector<vector<int>> &costMatrix;           ///< Reference to the cost matrix
    vector<int> supply;                        ///< Vector containing the supply values
    vector<int> demand;                        ///< Vector containing the demand values
    int &totalSupply;                          ///< Reference to the total supply
    int &totalDemand;                          ///< Reference to the total demand
    vector<vector<int>> &assignamentMatrix;    ///< Reference to the assignment matrix
    int totalCost;                             ///< Total cost of the transportation
};

#endif // TRANSPORT_SOLVER_H
