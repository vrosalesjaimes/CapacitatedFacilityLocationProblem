#ifndef SOLUTION_H
#define SOLUTION_H

#include <iostream>

/**
 * @file solution.h
 * 
 * @brief Class representing a solution.
 */
class Solution {
public:
    /**
     * @brief Constructor for Solution.
     * 
     * @param cost The cost of the solution.
     */
    Solution(double cost);

    /**
     * @brief Gets the cost of the solution.
     * 
     * @return double The cost of the solution.
     */
    double getCost() const;

    /**
     * @brief Compares two solutions.
     * 
     * @param other Another instance of Solution to compare.
     * @return true If the solutions are equal.
     * @return false If the solutions are different.
     */
    bool operator==(const Solution& other) const;

private:
    double cost; ///< The cost of the solution.
};

#endif // SOLUTION_H
