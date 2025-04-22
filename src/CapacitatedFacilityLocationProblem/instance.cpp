#include "CapacitatedFacilityLocationProblem/instance.h"
#include <stdexcept>

using namespace std;

Instance::Instance(int numFacilities, int numCustomers, 
                   vector<int>& facilityCapacities, 
                   vector<int>& customerDemands,
                   vector<double>& openingCosts,
                   vector<vector<double>>& transportationCosts)
    : numFacilities(numFacilities), numCustomers(numCustomers), 
      facilityCapacities(facilityCapacities), customerDemands(customerDemands), 
      openingCosts(openingCosts), transportationCosts(transportationCosts) 
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

int Instance::getNumFacilities() {
    return numFacilities;
}

int Instance::getNumCustomers() {
    return numCustomers;
}

vector<int>& Instance::getFacilityCapacities() {
    return facilityCapacities;
}

vector<int>& Instance::getCustomerDemands() {
    return customerDemands;
}

vector<double>& Instance::getOpeningCosts() {
    return openingCosts;
}

vector<vector<double>>& Instance::getTransportationCosts() {
    return transportationCosts;
}