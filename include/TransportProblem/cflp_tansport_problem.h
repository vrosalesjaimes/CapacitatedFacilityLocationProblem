#ifndef CFLP_TRANSPORT_SUBPROBLEM_H
#define CFLP_TRANSPORT_SUBPROBLEM_H

#include "TransportProblem/transport_problem.h"
#include <vector>
#include <unordered_map>

/**
 * @class CFLPTransportSubproblem
 * @brief Represents the transportation subproblem induced by a solution of the CFLP.
 *
 * Efficiently builds and maintains a transportation problem based on which facilities are open.
 * Allows dynamic mutation by toggling the status of a single facility and maintains
 * global supply/demand tracking with efficient updates.
 */
class CFLPTransportSubproblem
{
public:
    /**
     * @brief Constructor.
     * @param fullCostMatrix Complete CFLP cost matrix (facilities x clients).
     * @param capacities Vector of facility capacities.
     * @param demands Vector of client demands.
     * @param openFacilities Boolean vector indicating which facilities are initially open.
     */
    CFLPTransportSubproblem(const std::vector<std::vector<int>> &fullCostMatrix,
                            const std::vector<int> &capacities,
                            const std::vector<int> &demands,
                            const std::vector<bool> &openFacilities);

    /// @brief Destructor
    ~CFLPTransportSubproblem() = default;

    /**
     * @brief Mutates the subproblem by toggling the open/closed status of a facility.
     * @param facilityIndex Index of the facility to toggle.
     */
    void toggleFacility(int facilityIndex);

    /**
     * @brief Solves the current transportation subproblem using the Hungarian method.
     * @return A pair containing the total cost and the assignment matrix mapped to original indices.
     */
    void solve();

    /**
     * @brief Returns the current total supply (sum of open facilities' capacities).
     * @return Total supply value.
     */
    int getCurrentTotalSupply() const;

    /**
     * @brief Returns the current total demand (sum of all client demands).
     * @return Total demand value.
     */
    int getCurrentTotalDemand() const;

    /**
     * @brief Access the underlying transportation problem.
     * @return Const reference to the TransportationProblem instance.
     */
    const TransportationProblem &getTransportProblem() const;

    /**
     * @brief Returns the current open/closed status of facilities.
     * @return Boolean vector of facility statuses.
     */
    const std::vector<bool> &getOpenFacilities() const;

    /**
     * @brief Returns the mapping from subproblem facility indices to original indices.
     * @return Vector of original indices.
     */
    const std::vector<size_t> &getFacilityIndexMap() const;

private:
    std::vector<std::vector<int>> fullCostMatrix_; ///< Full CFLP cost matrix.
    std::vector<int> allCapacities_;               ///< Capacities of all facilities.
    std::vector<int> clientDemands_;               ///< Demands of all clients.
    std::vector<bool> openFacilities_;             ///< Current open/closed status of facilities.
    std::vector<size_t> facilityIndexMap_;         ///< Maps subproblem indices to original indices
    std::vector<std::vector<int>> assignmentMatrix_; ///< Current assignment matrix.
    int totalCost_;                                  ///< Total cost of current assignment.
    std::vector<int> selectedSupplies_;
    std::vector<std::vector<int>> selectedCosts_;

    int totalSupply_;                              ///< Current total supply (sum of open facilities)
    int totalDemand_;                              ///< Total demand (sum of all clients)

    TransportationProblem transportProblem_; ///< Current subproblem (only open facilities).
};

#endif // CFLP_TRANSPORT_SUBPROBLEM_H