#include "CapacitatedFacilityLocationProblem/cflp_problem.h"
#include "TransportProblem/cflp_tansport_problem.h"
#include <numeric>

CFLPProblem::CFLPProblem(const std::vector<std::vector<int>> &costMatrix,
                         const std::vector<int> &capacities,
                         const std::vector<int> &demands,
                         const std::vector<double> &openingCosts)
    : currentCost_(0), costMatrix_(costMatrix),
      capacities_(capacities), demands_(demands),
      openingCosts_(openingCosts),
      subproblem_()
{
    totalDemand_ = accumulate(demands.begin(), demands.end(), 0);
}

void CFLPProblem::initializeSubproblem(const std::vector<int> &solution)
{
    subproblem_ = CFLPTransportSubproblem(costMatrix_, capacities_, demands_, solution);
    subproblem_.solve();
    costOfTransportation_ = subproblem_.getTotalCost();
    costOfFacilities_ = 0;

    for (size_t i = 0; i < solution.size(); ++i)
    {
        if (solution[i] == 1)
        {
            costOfFacilities_ += openingCosts_[i];
        }
    }

    currentTotalSupply_ = 0;

    for (size_t i = 0; i < solution.size(); ++i)
    {
        if (solution[i] == 1)
        {
            currentTotalSupply_ += capacities_[i];
        }
    }

    currentCost_ = costOfFacilities_ + costOfTransportation_;
}

int CFLPProblem::getCurrentCost() const
{
    return currentCost_;
}

void CFLPProblem::setCurrentCost(int cost)
{
    currentCost_ = cost;
}

const std::vector<int> &CFLPProblem::getBestSolution() const
{
    return bestSolution_;
}

void CFLPProblem::setBestSolution(const std::vector<int> &solution)
{
    bestSolution_ = solution;
}

CFLPTransportSubproblem &CFLPProblem::getSubproblem()
{
    return subproblem_;
}

const std::vector<std::vector<int>> &CFLPProblem::getCostMatrix() const
{
    return costMatrix_;
}

std::vector<std::vector<int>> &CFLPProblem::getCostMatrix()
{
    return const_cast<std::vector<std::vector<int>> &>(costMatrix_);
}

const std::vector<int> &CFLPProblem::getCapacities() const
{
    return capacities_;
}

std::vector<int> &CFLPProblem::getCapacities()
{
    return const_cast<std::vector<int> &>(capacities_);
}

const std::vector<int> &CFLPProblem::getDemands() const
{
    return demands_;
}

std::vector<int> &CFLPProblem::getDemands()
{
    return const_cast<std::vector<int> &>(demands_);
}

const std::vector<double> &CFLPProblem::getOpeningCosts() const
{
    return openingCosts_;
}

std::vector<double> &CFLPProblem::getOpeningCosts()
{
    return const_cast<std::vector<double> &>(openingCosts_);
}

int CFLPProblem::getCostOfFacilities() const
{
    return costOfFacilities_;
}

int CFLPProblem::getCostOfTransportation() const
{
    return costOfTransportation_;
}

int CFLPProblem::getTotalDemand() const
{
    return totalDemand_;
}

int CFLPProblem::getCurrentTotalSupply() const
{
    return currentTotalSupply_;
}

void CFLPProblem::toggleFacility(int facilityIndex)
{
    // Actualización de oferta total.
    if(bestSolution_[facilityIndex] == 1){
        bestSolution_[facilityIndex] = 0;
        currentTotalSupply_ -= capacities_[facilityIndex];
        costOfFacilities_ -= openingCosts_[facilityIndex];
    }
    else{
        bestSolution_[facilityIndex] = 1;
        currentTotalSupply_ += capacities_[facilityIndex];
        costOfFacilities_ += openingCosts_[facilityIndex];
    }

    subproblem_.toggleFacility(facilityIndex);
    subproblem_.solve();

    costOfTransportation_ = subproblem_.getTotalCost();
    currentCost_ = costOfFacilities_ + costOfTransportation_;
    currentTotalSupply_ = subproblem_.getCurrentTotalSupply();
}