#ifndef INSTANCE_H
#define INSTANCE_H

#include <vector>
#include "CapacitedFacilityLocationProblem/solution.h"

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

    /**
     * @brief Gets the number of facilities.
     * 
     * @return int Number of facilities.
     */
    int getNumFacilities() const;

    /**
     * @brief Sets the number of facilities.
     * 
     * @param numFacilities The new number of facilities.
     */
    void setNumFacilities(int numFacilities);

    /**
     * @brief Gets the number of customers.
     * 
     * @return int Number of customers.
     */
    int getNumCustomers() const;

    /**
     * @brief Sets the number of customers.
     * 
     * @param numCustomers The new number of customers.
     */
    void setNumCustomers(int numCustomers);

    /**
     * @brief Gets the facility capacities.
     * 
     * @return const std::vector<int>& The facility capacities.
     */
    const std::vector<int>& getFacilityCapacities() const;

    /**
     * @brief Sets the facility capacities.
     * 
     * @param facilityCapacities The new facility capacities.
     */
    void setFacilityCapacities(const std::vector<int>& facilityCapacities);

    /**
     * @brief Gets the customer demands.
     * 
     * @return const std::vector<int>& The customer demands.
     */
    const std::vector<int>& getCustomerDemands() const;

    /**
     * @brief Sets the customer demands.
     * 
     * @param customerDemands The new customer demands.
     */
    void setCustomerDemands(const std::vector<int>& customerDemands);

    /**
     * @brief Gets the opening costs.
     * 
     * @return const std::vector<double>& The opening costs.
     */
    const std::vector<double>& getOpeningCosts() const;

    /**
     * @brief Sets the opening costs.
     * 
     * @param openingCosts The new opening costs.
     */
    void setOpeningCosts(const std::vector<double>& openingCosts);

    /**
     * @brief Gets the transportation costs.
     * 
     * @return const std::vector<std::vector<double>>& The transportation costs.
     */
    const std::vector<std::vector<double>>& getTransportationCosts() const;

    /**
     * @brief Sets the transportation costs.
     * 
     * @param transportationCosts The new transportation costs.
     */
    void setTransportationCosts(const std::vector<std::vector<double>>& transportationCosts);

    /**
     * @brief Gets the best solution for this instance.
     * 
     * @return const Solution& The best solution.
     */
    const Solution& getBestSolution() const;

    /**
     * @brief Sets the best solution for this instance.
     * 
     * @param bestSolution The new best solution.
     */
    void setBestSolution(const Solution& bestSolution);

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
