#ifndef SOLUTION_H
#define SOLUTION_H

#include <vector>
#include <cstdint>
#include <string>

using namespace std;

class Instance; // Forward declaration

/**
 * @brief Class representing a solution for the Capacitated Facility Location Problem.
 */
class Solution {
public:
    /**
     * @brief Constructor for Solution.
     *
     * @param cost The cost of the solution.
     * @param y Vector indicating if there is a warehouse at site i (1 or 0).
     * @param x Matrix indicating if customer j is served by warehouse i (1 or 0).
     */
    Solution(double cost, vector<bool> y, vector<vector<int>> x);

    /**
     * @brief Gets the cost of the solution.
     *
     * @return double The cost of the solution.
     */
    double getCost() ;

    /**
     * @brief Sets the cost of the solution.
     *
     * @param cost The new cost of the solution.
     */
    void setCost(double cost);

    /**
     * @brief Gets the warehouse assignment vector.
     *
     * @return vector<uint8_t>& The warehouse assignment vector.
     */
    vector<bool>& getY() ;

    /**
     * @brief Sets the warehouse assignment vector.
     *
     * @param y The new warehouse assignment vector.
     */
    void setY(vector<bool>& y);

    /**
     * @brief Gets the customer assignment matrix.
     *
     * @return vector<vector<uint8_t>>& The customer assignment matrix.
     */
    vector<vector<int>>& getX() ;

    /**
     * @brief Sets the customer assignment matrix.
     *
     * @param x The new customer assignment matrix.
     */
    void setX(vector<vector<int>>& x);

    /**
     * @brief Compares two solutions.
     *
     * @param other Another instance of Solution to compare.
     * @return true If the solutions are equal.
     * @return false If the solutions are different.
     */
    bool operator==(Solution& other) ;

    /**
     * @brief Calculate the cost of the solution given an instance.
     *
     * @param instance The instance for which to calculate the solution cost.
     */
    void calculateCost(Instance& instance);

    /**
     * @brief Converts the solution to a string representation.
     *
     * @return string The string representation of the solution.
     */
    string toString() ;

    /**
     * @brief Checks if the solution is feasible.
     *
     * @param instance The instance to check the solution against.
     * @return true If the solution is feasible.
     * @return false If the solution is not feasible.
     */
    bool isFeasible(Instance& instance);

private:
    /**
     * @brief Calculates the total demand of customers.
     *
     * @param instance The instance to calculate the demand from.
     * @return int The total demand.
     */
    int calculateTotalDemand(Instance& instance);

    double cost;                         ///< The cost of the solution.
    vector<bool> y;              ///< Warehouse assignment vector.
    vector<vector<int>> x; ///< Customer assignment matrix.
    int totalDemand;                     ///< Total demand of customers.
    bool demandCalculated;               ///< Flag to check if the total demand has been calculated.
};

#endif // SOLUTION_H
