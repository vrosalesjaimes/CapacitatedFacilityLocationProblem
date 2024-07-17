#include "CapacitedFacilityLocationProblem/instance.h"
#include <stdexcept>

Instance::Instance(int numFacilities, int numCustomers, 
                   const std::vector<int>& facilityCapacities, 
                   const std::vector<int>& customerDemands,
                   const std::vector<double>& openingCosts,
                   const std::vector<std::vector<double>>& transportationCosts,
                   const Solution& bestSolution)
    : numFacilities(numFacilities), numCustomers(numCustomers), 
      facilityCapacities(facilityCapacities), customerDemands(customerDemands), 
      openingCosts(openingCosts), transportationCosts(transportationCosts), 
      bestSolution(bestSolution) 
{
    if (facilityCapacities.size() != numFacilities || 
        openingCosts.size() != numFacilities || 
        transportationCosts.size() != numFacilities) {
        throw std::invalid_argument("Mismatch in number of facilities.");
    }
    if (customerDemands.size() != numCustomers || 
        transportationCosts[0].size() != numCustomers) {
        throw std::invalid_argument("Mismatch in number of customers.");
    }
}

int Instance::getNumFacilities() const {
    return numFacilities;
}

void Instance::setNumFacilities(int numFacilities) {
    this->numFacilities = numFacilities;
}

int Instance::getNumCustomers() const {
    return numCustomers;
}

void Instance::setNumCustomers(int numCustomers) {
    this->numCustomers = numCustomers;
}

const std::vector<int>& Instance::getFacilityCapacities() const {
    return facilityCapacities;
}

void Instance::setFacilityCapacities(const std::vector<int>& facilityCapacities) {
    this->facilityCapacities = facilityCapacities;
}

const std::vector<int>& Instance::getCustomerDemands() const {
    return customerDemands;
}

void Instance::setCustomerDemands(const std::vector<int>& customerDemands) {
    this->customerDemands = customerDemands;
}

const std::vector<double>& Instance::getOpeningCosts() const {
    return openingCosts;
}

void Instance::setOpeningCosts(const std::vector<double>& openingCosts) {
    this->openingCosts = openingCosts;
}

const std::vector<std::vector<double>>& Instance::getTransportationCosts() const {
    return transportationCosts;
}

void Instance::setTransportationCosts(const std::vector<std::vector<double>>& transportationCosts) {
    this->transportationCosts = transportationCosts;
}

const Solution& Instance::getBestSolution() const {
    return bestSolution;
}

void Instance::setBestSolution(const Solution& bestSolution) {
    this->bestSolution = bestSolution;
}
