#include "Reader/beasley_instance_reader.h"
#include <fstream>
#include <sstream>
#include <iostream>

/**
 * @brief Reads a Beasley instance from a file.
 * 
 * @param filename The name of the file containing the instance data.
 * @return Instance The read instance.
 */
Instance BeasleyInstanceReader::readInstance(const std::string& filename) const {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Unable to open file");
    }

    int numFacilities, numCustomers;
    file >> numFacilities >> numCustomers;

    std::vector<int> facilityCapacities(numFacilities);
    std::vector<double> openingCosts(numFacilities);
    for (int i = 0; i < numFacilities; ++i) {
        file >> facilityCapacities[i] >> openingCosts[i];
    }

    std::vector<int> customerDemands(numCustomers);
    std::vector<std::vector<double>> transportationCosts(numFacilities, std::vector<double>(numCustomers));
    for (int j = 0; j < numCustomers; ++j) {
        file >> customerDemands[j];
        for (int i = 0; i < numFacilities; ++i) {
            file >> transportationCosts[i][j];
        }
    }

    Solution bestSolution(0.0, std::vector<uint8_t>(numFacilities, 0), std::vector<std::vector<int>>(numFacilities, std::vector<int>(numCustomers, 0)));

    return Instance(numFacilities, numCustomers, facilityCapacities, customerDemands, openingCosts, transportationCosts, bestSolution);
}
