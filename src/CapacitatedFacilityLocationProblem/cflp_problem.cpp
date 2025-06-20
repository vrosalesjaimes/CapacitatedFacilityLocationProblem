#include "CapacitatedFacilityLocationProblem/cflp_problem.h"
#include "TransportProblem/cflp_tansport_problem.h"
#include <numeric>
#include <iostream>

CFLPProblem::CFLPProblem(std::vector<std::vector<int>> costMatrix,
                         std::vector<int> capacities,
                         std::vector<int> demands,
                         std::vector<double> openingCosts)
    : currentCost_(0),
      costMatrix_(std::move(costMatrix)),       // Movemos en lugar de copiar
      capacities_(std::move(capacities)),       // Movemos en lugar de copiar
      demands_(std::move(demands)),             // Movemos en lugar de copiar
      openingCosts_(std::move(openingCosts)),   // Movemos en lugar de copiar
      subproblem_()
{
    totalDemand_ = std::accumulate(demands_.begin(), demands_.end(), 0);
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
    return costMatrix_;
}

const std::vector<int> &CFLPProblem::getCapacities() const
{
    return capacities_;
}

std::vector<int> &CFLPProblem::getCapacities()
{
    return capacities_;
}

const std::vector<int> &CFLPProblem::getDemands() const
{
    return demands_;
}

std::vector<int> &CFLPProblem::getDemands()
{
    return demands_;
}

const std::vector<double> &CFLPProblem::getOpeningCosts() const
{
    return openingCosts_;
}

std::vector<double> &CFLPProblem::getOpeningCosts()
{
    return openingCosts_;
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
    if (bestSolution_[facilityIndex] == 1) {
        bestSolution_[facilityIndex] = 0;
        currentTotalSupply_ -= capacities_[facilityIndex];
        costOfFacilities_ -= openingCosts_[facilityIndex];
    } else { 
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
