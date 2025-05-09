#include "TransportProblem/cflp_tansport_problem.h"
#include <stdexcept>
#include <numeric>
#include <algorithm>

CFLPTransportSubproblem::CFLPTransportSubproblem(const std::vector<std::vector<int>> &fullCostMatrix,
                                                 const std::vector<int> &capacities,
                                                 const std::vector<int> &demands,
                                                 const std::vector<bool> &openFacilities)
    : fullCostMatrix_(fullCostMatrix),
      allCapacities_(capacities),
      clientDemands_(demands),
      openFacilities_(openFacilities),
      totalDemand_(std::accumulate(demands.begin(), demands.end(), 0)),
      transportProblem_({}, {}, {}) // Will be overwritten in buildTransportProblem
{
    if (fullCostMatrix_.size() != capacities.size())
        throw std::invalid_argument("Cost matrix row count must match capacity size.");
    if (openFacilities_.size() != capacities.size())
        throw std::invalid_argument("Open facilities vector size must match capacities.");
    if (!fullCostMatrix_.empty() && fullCostMatrix_[0].size() != demands.size())
        throw std::invalid_argument("Cost matrix column count must match demand size.");

    totalDemand_ = std::accumulate(demands.begin(), demands.end(), 0);
    transportProblem_.setTotalDemand(totalDemand_);
    totalSupply_ = 0;

    for (size_t i = 0; i < openFacilities_.size(); ++i)
    {
        if (openFacilities_[i])
        {
            totalSupply_ += allCapacities_[i];
        }
    }

    std::vector<int> selectedSupplies;
    std::vector<std::vector<int>> selectedCosts;
    facilityIndexMap_.clear();

    // Build the subproblem with only open facilities
    for (size_t i = 0; i < openFacilities_.size(); ++i)
    {
        if (openFacilities_[i])
        {
            selectedSupplies.push_back(allCapacities_[i]);
            selectedCosts.push_back(fullCostMatrix_[i]);
            facilityIndexMap_.push_back(i);
        }
    }

    // Reconstruct the transportation problem
    transportProblem_ = TransportationProblem(selectedSupplies, clientDemands_, selectedCosts);
    transportProblem_.setTotalSupply(totalSupply_);
}

void CFLPTransportSubproblem::toggleFacility(int facilityIndex)
{
    if (facilityIndex < 0 || facilityIndex >= static_cast<int>(openFacilities_.size()))
        throw std::out_of_range("Invalid facility index");

    // Determine the change in supply
    int supplyChange = openFacilities_[facilityIndex] ? -allCapacities_[facilityIndex] : allCapacities_[facilityIndex];
    totalSupply_ += supplyChange;

    // Update the total supply in the transport problem
    transportProblem_.setTotalSupply(totalSupply_);

    // Toggle the open facility state
    openFacilities_[facilityIndex] = !openFacilities_[facilityIndex];

    // Update selectedSupplies, selectedCosts, and facilityIndexMap_ based on the facility's new state
    if (openFacilities_[facilityIndex]) {
        selectedSupplies.push_back(allCapacities_[facilityIndex]);
        selectedCosts.push_back(fullCostMatrix_[facilityIndex]);
        facilityIndexMap_.push_back(facilityIndex);
    } else {
        auto it = std::find(facilityIndexMap_.begin(), facilityIndexMap_.end(), facilityIndex);
        if (it != facilityIndexMap_.end()) {
            size_t index = std::distance(facilityIndexMap_.begin(), it);
            selectedSupplies.erase(selectedSupplies.begin() + index);
            selectedCosts.erase(selectedCosts.begin() + index);
            facilityIndexMap_.erase(it);
        }
    }
}

void CFLPTransportSubproblem::solve()
{
    transportProblem_.balance();
    transportProblem_.solveHungarianMethod();
    totalCost_ = transportProblem_.getTotalCost();

    // Get the assignment matrix from the subproblem
    const auto &subAssign = transportProblem_.getAssignmentMatrix();

    // Map the subproblem assignments back to original indices
    for (size_t sub_i = 0; sub_i < subAssign.size(); ++sub_i)
    {
        size_t original_i = facilityIndexMap_[sub_i];
        for (size_t j = 0; j < subAssign[sub_i].size(); ++j)
        {
            assignmentMatrix_[original_i][j] = subAssign[sub_i][j];
        }
    }
}

int CFLPTransportSubproblem::getCurrentTotalSupply() const
{
    return totalSupply_;
}

int CFLPTransportSubproblem::getCurrentTotalDemand() const
{
    return totalDemand_;
}

const TransportationProblem &CFLPTransportSubproblem::getTransportProblem() const
{
    return transportProblem_;
}

const std::vector<bool> &CFLPTransportSubproblem::getOpenFacilities() const
{
    return openFacilities_;
}

const std::vector<size_t> &CFLPTransportSubproblem::getFacilityIndexMap() const
{
    return facilityIndexMap_;
}