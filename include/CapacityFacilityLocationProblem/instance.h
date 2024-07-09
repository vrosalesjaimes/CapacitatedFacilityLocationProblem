#ifndef INSTANCE_H
#define INSTANCE_H

#include <vector>
#include "CapacityFacilityLocationProblem/solution.h"

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
             const std::vector<int>& facilityCapacities, 
             const std::vector<int>& customerDemands,
             const std::vector<double>& openingCosts,
             const std::vector<std::vector<double>>& transportationCosts,
             const Solution& bestSolution);

    // Getters for the attributes
    int getNumFacilities() const;
    void setNumFacilities(int numFacilities);

    int getNumCustomers() const;
    void setNumCustomers(int numCustomers);

    const std::vector<int>& getFacilityCapacities() const;
    void setFacilityCapacities(const std::vector<int>& facilityCapacities);

    const std::vector<int>& getCustomerDemands() const;
    void setCustomerDemands(const std::vector<int>& customerDemands);

    const std::vector<double>& getOpeningCosts() const;
    void setOpeningCosts(const std::vector<double>& openingCosts);

    const std::vector<std::vector<double>>& getTransportationCosts() const;
    void setTransportationCosts(const std::vector<std::vector<double>>& transportationCosts);

    void setBestSolution(const Solution& solution);
    const Solution& getBestSolution() const;

private:
    int numFacilities; ///< Number of facilities.
    int numCustomers; ///< Number of customers.
    std::vector<int> facilityCapacities; ///< Capacities of each facility.
    std::vector<int> customerDemands; ///< Demands of each customer.
    std::vector<double> openingCosts; ///< Opening costs of each facility.
    std::vector<std::vector<double>> transportationCosts; ///< Transportation costs from facilities to customers.
    Solution bestSolution; ///< The best solution found for this instance.
};

#endif // INSTANCE_H
