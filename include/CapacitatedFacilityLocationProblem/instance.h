#ifndef INSTANCE_H
#define INSTANCE_H

#include <vector>

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
             vector<int>& facilityCapacities, 
             vector<int>& customerDemands,
             vector<double>& openingCosts,
             vector<vector<double>>& transportationCosts);

    /**
     * @brief Gets the number of facilities.
     * 
     * @return int Number of facilities.
     */
    int getNumFacilities() ;

    /**
     * @brief Gets the number of customers.
     * 
     * @return int Number of customers.
     */
    int getNumCustomers() ;

    /**
     * @brief Gets the facility capacities.
     * 
     * @return vector<int>& The facility capacities.
     */
    vector<int>& getFacilityCapacities() ;

    /**
     * @brief Gets the customer demands.
     * 
     * @return vector<int>& The customer demands.
     */
    vector<int>& getCustomerDemands() ;

    /**
     * @brief Gets the opening costs.
     * 
     * @return vector<double>& The opening costs.
     */
    vector<double>& getOpeningCosts() ;

    /**
     * @brief Gets the transportation costs.
     * 
     * @return vector<vector<double>>& The transportation costs.
     */
    vector<vector<double>>& getTransportationCosts() ;

private:
    int numFacilities; ///< Number of facilities.
    int numCustomers; ///< Number of customers.
    vector<int> facilityCapacities; ///< Capacities of each facility.
    vector<int> customerDemands; ///< Demands of each customer.
    vector<double> openingCosts; ///< Opening costs of each facility.
    vector<vector<double>> transportationCosts; ///< Transportation costs from facilities to customers.
};

#endif // INSTANCE_H
