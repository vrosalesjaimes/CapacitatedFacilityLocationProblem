#include "CapacityFacilityLocationProblem/holmberg_instance_reader.h"
#include <fstream>
#include <sstream>

/**
 * @brief Reads a Holmberg instance from a file.
 * 
 * @param filename The name of the file containing the instance data.
 * @return Instance The read instance.
 */
Instance HolmbergInstanceReader::readInstance(const std::string& filename) const {
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
    for (int j = 0; j < numCustomers; ++j) {
        file >> customerDemands[j];
    }

    std::vector<std::vector<double>> transportationCosts(numFacilities, std::vector<double>(numCustomers));
    for (int i = 0; i < numFacilities; ++i) {
        for (int j = 0; j < numCustomers; ++j) {
            file >> transportationCosts[i][j];
        }
    }

    Solution bestSolution(0.0, std::vector<uint8_t>(numFacilities, 0), std::vector<std::vector<uint8_t>>(numFacilities, std::vector<uint8_t>(numCustomers, 0)));

    return Instance(numFacilities, numCustomers, facilityCapacities, customerDemands, openingCosts, transportationCosts, bestSolution);
}