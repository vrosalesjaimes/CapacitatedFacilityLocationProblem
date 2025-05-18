#ifndef CFLP_PROBLEM_H
#define CFLP_PROBLEM_H

#include "../TransportProblem/cflp_tansport_problem.h"
#include <vector>

/**
 * @class CFLPProblem
 * @brief Represents a full instance of the Capacitated Facility Location Problem (CFLP).
 *
 * This class contains the main state and control logic for solving CFLP via metaheuristics
 * such as Tabu Search. It holds information about the current and best solutions,
 * and provides interfaces for managing problem state.
 */
class CFLPProblem
{
public:
    /**
     * @brief Constructor.
     * @param costMatrix Full cost matrix (facilities x clients).
     * @param capacities Vector of facility capacities.
     * @param demands Vector of client demands.
     * @param openingCosts Vector of facility opening costs.
     */
    CFLPProblem(const std::vector<std::vector<int>> &costMatrix,
                const std::vector<int> &capacities,
                const std::vector<int> &demands,
                const std::vector<double> &openingCosts);

    /// Destructor.
    ~CFLPProblem() = default;

    /**
     * @brief Returns the current total cost.
     * @return Total cost as an integer.
     */
    int getCurrentCost() const;

    /**
     * @brief Sets the current total cost.
     * @param cost New total cost.
     */
    void setCurrentCost(int cost);

    /**
     * @brief Returns the best solution found so far.
     * @return Constant reference to a boolean vector representing open facilities.
     */
    const std::vector<bool> &getBestSolution() const;

    /**
     * @brief Sets the best solution found so far.
     * @param solution Boolean vector representing open facilities.
     */
    void setBestSolution(const std::vector<bool> &solution);

    /**
     * @brief Returns a reference to the CFLP transport subproblem.
     * @return Reference to CFLPTransportSubproblem instance.
     */
    CFLPTransportSubproblem &getSubproblem();

    /**
     * @brief Returns a const reference to the cost matrix.
     * @return Cost matrix as const reference.
     */
    const std::vector<std::vector<int>> &getCostMatrix() const;

    /**
     * @brief Returns a modifiable reference to the cost matrix.
     * @return Cost matrix as reference.
     */
    std::vector<std::vector<int>> &getCostMatrix();

    /**
     * @brief Returns a const reference to the facility capacities.
     * @return Capacities vector as const reference.
     */
    const std::vector<int> &getCapacities() const;

    /**
     * @brief Returns a modifiable reference to the facility capacities.
     * @return Capacities vector as reference.
     */
    std::vector<int> &getCapacities();

    /**
     * @brief Returns a const reference to the client demands.
     * @return Demands vector as const reference.
     */
    const std::vector<int> &getDemands() const;

    /**
     * @brief Returns a modifiable reference to the client demands.
     * @return Demands vector as reference.
     */
    std::vector<int> &getDemands();

    /**
     * @brief Returns a const reference to the facility opening costs.
     * @return Opening costs vector as const reference.
     */
    const std::vector<double> &getOpeningCosts() const;

    /**
     * @brief Returns a modifiable reference to the facility opening costs.
     * @return Opening costs vector as reference.
     */
    std::vector<double> &getOpeningCosts();

private:
    int currentCost_;                                 ///< Current total cost.
    std::vector<bool> bestSolution_;                  ///< Best known facility opening solution.
    CFLPTransportSubproblem subproblem_;              ///< Current CFLP transport subproblem.
    const std::vector<std::vector<int>> &costMatrix_; ///< Full cost matrix.
    const std::vector<int> &capacities_;              ///< Facility capacities.
    const std::vector<int> &demands_;                 ///< Client demands.
    const std::vector<double> &openingCosts_;         ///< Facility opening costs.
};

#endif // CFLP_PROBLEM_H
