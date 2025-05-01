#ifndef CFLP_TRANSPORT_SUBPROBLEM_H
#define CFLP_TRANSPORT_SUBPROBLEM_H

#include "TransportProblem/transport_problem.h"
#include <vector>

/**
 * @class CFLPTransportSubproblem
 * @brief Represents the transportation subproblem induced by a solution of the CFLP.
 *
 * Efficiently builds and maintains a transportation problem based on which facilities are open.
 * Allows dynamic mutation by toggling the status of a single facility.
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
     * @brief Updates the internal transportation problem to reflect current open facilities.
     */
    void updateSubproblem();

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

private:
    std::vector<std::vector<int>> fullCostMatrix_; ///< Full CFLP cost matrix.
    std::vector<int> allCapacities_;               ///< Capacities of all facilities.
    std::vector<int> clientDemands_;               ///< Demands of all clients.
    std::vector<bool> openFacilities_;             ///< Current open/closed status of facilities.

    TransportationProblem transportProblem_; ///< Current subproblem (only open facilities).

    /**
     * @brief Builds the TransportationProblem from current open facilities.
     */
    void buildTransportProblem();
};

#endif // CFLP_TRANSPORT_SUBPROBLEM_H
