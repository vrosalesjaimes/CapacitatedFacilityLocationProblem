#include "TransportProblem/cflp_tansport_problem.h"
#include <stdexcept>

/**
 * @brief Constructor.
 */
CFLPTransportSubproblem::CFLPTransportSubproblem(const std::vector<std::vector<int>> &fullCostMatrix,
                                                 const std::vector<int> &capacities,
                                                 const std::vector<int> &demands,
                                                 const std::vector<bool> &openFacilities)
    : fullCostMatrix_(fullCostMatrix),
      allCapacities_(capacities),
      clientDemands_(demands),
      openFacilities_(openFacilities),
      transportProblem_({}, {}, {}) // Will be overwritten in buildTransportProblem
{
    if (fullCostMatrix_.size() != capacities.size())
        throw std::invalid_argument("Cost matrix row count must match capacity size.");
    if (openFacilities_.size() != capacities.size())
        throw std::invalid_argument("Open facilities vector size must match capacities.");
    if (!fullCostMatrix_.empty() && fullCostMatrix_[0].size() != demands.size())
        throw std::invalid_argument("Cost matrix column count must match demand size.");

    buildTransportProblem();
    transportProblem_.calculateTotalSupplyAndDemand();
}

/**
 * @brief Toggles the status of a facility (open <-> closed).
 */
void CFLPTransportSubproblem::toggleFacility(int facilityIndex)
{
    if (facilityIndex < 0 || facilityIndex >= static_cast<int>(openFacilities_.size()))
        throw std::out_of_range("Invalid facility index");

    openFacilities_[facilityIndex] = !openFacilities_[facilityIndex];
    
    updateSubproblem();
    
    if (openFacilities_[facilityIndex])
    {
        transportProblem_.setTotalSupply(transportProblem_.getTotalSupply() + allCapacities_[facilityIndex]);
    }
    else
    {
        transportProblem_.setTotalSupply(transportProblem_.getTotalSupply() - allCapacities_[facilityIndex]);
    }
}

/**
 * @brief Rebuilds the internal TransportationProblem based on open facilities.
 */
void CFLPTransportSubproblem::updateSubproblem()
{
    buildTransportProblem();
}

/**
 * @brief Builds the TransportationProblem using only open facilities.
 */
void CFLPTransportSubproblem::buildTransportProblem()
{
    std::vector<int> selectedSupplies;
    std::vector<std::vector<int>> selectedCosts;

    for (size_t i = 0; i < openFacilities_.size(); ++i)
    {
        if (openFacilities_[i])
        {
            selectedSupplies.push_back(allCapacities_[i]);
            selectedCosts.push_back(fullCostMatrix_[i]);
        }
    }

    // Reconstruct the transportation problem
    transportProblem_ = TransportationProblem(selectedSupplies, clientDemands_, selectedCosts);
}

/**
 * @brief Returns a reference to the internal transportation problem.
 */
const TransportationProblem &CFLPTransportSubproblem::getTransportProblem() const
{
    return transportProblem_;
}

/**
 * @brief Returns current facility open/closed status.
 */
const std::vector<bool> &CFLPTransportSubproblem::getOpenFacilities() const
{
    return openFacilities_;
}
