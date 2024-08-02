#ifndef INSTANCE_H
#define INSTANCE_H

#include <vector>
#include "solution.h"

using namespace std;

/**
 * @brief Class representing an instance of the Capacitated Facility Location Problem.
 */
class Instance {
public:
    /**
     * @brief Constructor for Instance.
     * 
     * @param numFacilities Number of facilities.
     * @param numCustomers Number of customers.
     * @param facilityCapacities Capacities of each facility.
     * @param customerDemands Demands of each customer.
     * @param openingCosts Opening costs of each facility.
     * @param transportationCosts Transportation costs from facilities to customers.
     * @param bestSolution Initial best solution.
     */
    Instance(int numFacilities, int numCustomers, 
             const vector<int>& facilityCapacities, 
             const vector<int>& customerDemands,
             const vector<double>& openingCosts,
             const vector<vector<double>>& transportationCosts,
             Solution& bestSolution);

    /**
     * @brief Gets the number of facilities.
     * 
     * @return int Number of facilities.
     */
    int getNumFacilities() const;

    /**
     * @brief Gets the number of customers.
     * 
     * @return int Number of customers.
     */
    int getNumCustomers() const;

    /**
     * @brief Gets the facility capacities.
     * 
     * @return const vector<int>& The facility capacities.
     */
    const vector<int>& getFacilityCapacities() const;

    /**
     * @brief Gets the customer demands.
     * 
     * @return const vector<int>& The customer demands.
     */
    const vector<int>& getCustomerDemands() const;

    /**
     * @brief Gets the opening costs.
     * 
     * @return const vector<double>& The opening costs.
     */
    const vector<double>& getOpeningCosts() const;

    /**
     * @brief Gets the transportation costs.
     * 
     * @return const vector<vector<double>>& The transportation costs.
     */
    const vector<vector<double>>& getTransportationCosts() const;

    /**
     * @brief Gets the best solution for this instance.
     * 
     * @return const Solution& The best solution.
     */
    const Solution& getBestSolution() const;

private:
    const int numFacilities; ///< Number of facilities.
    const int numCustomers; ///< Number of customers.
    const vector<int> facilityCapacities; ///< Capacities of each facility.
    const vector<int> customerDemands; ///< Demands of each customer.
    const vector<double> openingCosts; ///< Opening costs of each facility.
    const vector<vector<double>> transportationCosts; ///< Transportation costs from facilities to customers.
    Solution bestSolution; ///< The best solution found for this instance.
};

#endif // INSTANCE_H
