#include "CapacitatedFacilityLocationProblem/instance.h"
#include <stdexcept>

using namespace std;

Instance::Instance(int numFacilities, int numCustomers, 
                   const vector<int>& facilityCapacities, 
                   const vector<int>& customerDemands,
                   const vector<double>& openingCosts,
                   const vector<vector<double>>& transportationCosts,
                   Solution& bestSolution)
    : numFacilities(numFacilities), numCustomers(numCustomers), 
      facilityCapacities(facilityCapacities), customerDemands(customerDemands), 
      openingCosts(openingCosts), transportationCosts(transportationCosts), 
      bestSolution(bestSolution) 
{
    if (facilityCapacities.size() != numFacilities || 
        openingCosts.size() != numFacilities || 
        transportationCosts.size() != numFacilities) {
        throw invalid_argument("Mismatch in number of facilities.");
    }
    if (customerDemands.size() != numCustomers || 
        transportationCosts[0].size() != numCustomers) {
        throw invalid_argument("Mismatch in number of customers.");
    }
}

int Instance::getNumFacilities() const {
    return numFacilities;
}

int Instance::getNumCustomers() const {
    return numCustomers;
}

const vector<int>& Instance::getFacilityCapacities() const {
    return facilityCapacities;
}

const vector<int>& Instance::getCustomerDemands() const {
    return customerDemands;
}

const vector<double>& Instance::getOpeningCosts() const {
    return openingCosts;
}

const vector<vector<double>>& Instance::getTransportationCosts() const {
    return transportationCosts;
}

const Solution& Instance::getBestSolution() const {
    return bestSolution;
}
