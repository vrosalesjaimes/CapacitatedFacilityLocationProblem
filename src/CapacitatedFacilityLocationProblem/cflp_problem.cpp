#include "CapacitatedFacilityLocationProblem/cflp_problem.h"
#include "TransportProblem/cflp_tansport_problem.h"

CFLPProblem::CFLPProblem(const std::vector<std::vector<int>> &costMatrix,
                         const std::vector<int> &capacities,
                         const std::vector<int> &demands,
                         const std::vector<double> &openingCosts)
    : currentCost_(0), costMatrix_(costMatrix),
      capacities_(capacities), demands_(demands),
      openingCosts_(openingCosts),
      subproblem_()
{
    subproblem_.solve();
    currentCost_ = subproblem_.getTransportProblem().getTotalCost();
}

int CFLPProblem::getCurrentCost() const
{
    return currentCost_;
}

void CFLPProblem::setCurrentCost(int cost)
{
    currentCost_ = cost;
}

const std::vector<bool> &CFLPProblem::getBestSolution() const
{
    return bestSolution_;
}

void CFLPProblem::setBestSolution(const std::vector<bool> &solution)
{
    bestSolution_ = solution;
}

CFLPTransportSubproblem &CFLPProblem::getSubproblem()
{
    return subproblem_;
}

void CFLPProblem::solveTabuSearch()
{
    // Tabu Search implementation to be added here
}
