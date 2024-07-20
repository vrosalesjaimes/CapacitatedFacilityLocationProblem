#ifndef INITIALIZER_H
#define INITIALIZER_H

#include "CapacitatedFacilityLocationProblem/instance.h"
#include "CapacitatedFacilityLocationProblem/solution.h"
#include <vector>
#include <utility>

/**
 * @brief Class for initializing solutions for the Capacitated Facility Location Problem.
 */
class Initializer {
public:
    /**
     * @brief Constructor for Initializer.
     * 
     * @param instance Reference to the instance of the problem.
     */
    Initializer(const Instance& instance);

    /**
     * @brief Generates the initial solution using the ADD method and all three rules.
     * 
     * @return Solution The best initial solution.
     */
    Solution generateInitialSolution();

private:
    const Instance& instance;

    /**
     * @brief Computes the P_i values for Rule 1.
     * 
     * @return std::vector<std::pair<double, int>> Vector of pairs (P_i value, facility index).
     */
    std::vector<std::pair<double, int>> computeRule1();

    /**
     * @brief Computes the P_i values for Rule 2.
     * 
     * @return std::vector<std::pair<double, int>> Vector of pairs (P_i value, facility index).
     */
    std::vector<std::pair<double, int>> computeRule2();

    /**
     * @brief Computes the P_i values for Rule 3.
     * 
     * @return std::vector<std::pair<double, int>> Vector of pairs (P_i value, facility index).
     */
    std::vector<std::pair<double, int>> computeRule3();

    /**
     * @brief Sorts the facilities based on their P_i values.
     * 
     * @param piValues Vector of pairs (P_i value, facility index).
     * @return std::vector<int> Sorted indices of facilities.
     */
    std::vector<int> sortFacilities(const std::vector<std::pair<double, int>>& piValues);

    /**
     * @brief Implements the ADD method to generate a solution.
     * 
     * @param sortedFacilities Sorted indices of facilities.
     * @return Solution Generated solution.
     */
    Solution addMethod(const std::vector<int>& sortedFacilities);
};

#endif // INITIALIZER_H
