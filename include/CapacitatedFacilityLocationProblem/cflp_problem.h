#ifndef CFLP_PROBLEM_H
#define CFLP_PROBLEM_H

#include "../TransportProblem/cflp_tansport_problem.h"
#include <vector>
#include <numeric>

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
    CFLPProblem(std::vector<std::vector<int>> costMatrix,
                std::vector<int> capacities,
                std::vector<int> demands,
                std::vector<double> openingCosts);

    /// Destructor.
    ~CFLPProblem() = default;

    int getCurrentCost() const;
    void setCurrentCost(int cost);

    const std::vector<int> &getBestSolution() const;
    void setBestSolution(const std::vector<int> &solution);

    CFLPTransportSubproblem &getSubproblem();

    const std::vector<std::vector<int>> &getCostMatrix() const;
    std::vector<std::vector<int>> &getCostMatrix();

    const std::vector<int> &getCapacities() const;
    std::vector<int> &getCapacities();

    const std::vector<int> &getDemands() const;
    std::vector<int> &getDemands();

    const std::vector<double> &getOpeningCosts() const;
    std::vector<double> &getOpeningCosts();

    void initializeSubproblem(const std::vector<int> &solution);

    int getCostOfFacilities() const;
    int getCostOfTransportation() const;
    int getTotalDemand() const;
    int getCurrentTotalSupply() const;
    void setCurrentTotalSupply(int supply);

    void toggleFacility(int facilityIndex);

private:
    int currentCost_;
    std::vector<int> bestSolution_;
    CFLPTransportSubproblem subproblem_;
    std::vector<std::vector<int>> costMatrix_;
    std::vector<int> capacities_;
    std::vector<int> demands_;
    std::vector<double> openingCosts_;
    int costOfFacilities_ = 0;
    int costOfTransportation_ = 0;
    int totalDemand_;
    int currentTotalSupply_ = 0;
};

#endif // CFLP_PROBLEM_H
