#ifndef INITIALIZER_H
#define INITIALIZER_H

#include <vector>
#include "instance.h"
#include "solution.h"

/**
 * @brief Class for initializing a solution to the Capacitated Facility Location Problem.
 */
class Initializer {
public:
    /**
     * @brief Constructor for Initializer.
     * 
     * @param instance The instance of the Capacitated Facility Location Problem.
     */
    Initializer(const Instance& instance);

    /**
     * @brief Finds the initial solution using the ADD method with the third priority rule.
     * 
     * @return Solution The initial feasible solution.
     */
    Solution findInitialSolution() const;

private:
    const Instance& instance; ///< The problem instance.

    /**
     * @brief Calculates Pi2 for each facility.
     * 
     * @param i Facility index.
     * @return double Pi2 value for facility i.
     */
    double calculatePi2(int i) const;

    /**
     * @brief Calculates Pi3 for each facility.
     * 
     * @param i Facility index.
     * @return double Pi3 value for facility i.
     */
    double calculatePi3(int i) const;
};

#endif // INITIALIZER_H
