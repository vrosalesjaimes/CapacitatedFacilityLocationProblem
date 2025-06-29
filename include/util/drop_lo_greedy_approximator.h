#ifndef DROPLO_GREEDY_APPROXIMATOR_H
#define DROPLO_GREEDY_APPROXIMATOR_H

#include "CapacitatedFacilityLocationProblem/cflp_problem.h"
#include <vector>
#include <unordered_map>

/**
 * @class DropLOGreedyApproximator
 * @brief Approximates δ_i′ for closing a facility using a greedy transportation heuristic.
 */
class DropLOGreedyApproximator
{
private:
    const CFLPProblem& problem;

public:
    /**
     * @brief Constructor.
     * @param problem Reference to the CFLP problem data.
     */
    DropLOGreedyApproximator(const CFLPProblem& problem);

    /**
     * @brief Computes the approximate δ_i′ value for closing facility iPrime.
     *
     * @param iPrime The facility to close.
     * @param y Vector indicating open/closed facilities.
     * @param x Current flow matrix.
     * @param slackSupply Current slack supplies of open facilities.
     * @return Approximate δ_i′ (upper bound on cost increase).
     */
    double computeDelta(int iPrime,
                        const std::vector<int>& y,
                        const std::vector<std::vector<int>>& x,
                        const std::vector<int>& slackSupply) const;
};

#endif
