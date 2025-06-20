#include "Reader/beasley_instance_reader.h"
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

/**
 * @brief Reads a Beasley instance from a file.
 * 
 * @param filename The name of the file containing the instance data.
 * @return Instance The read instance.
 */
CFLPProblem BeasleyInstanceReader::readInstance(const string& filename) const {
    ifstream file(filename);
    if (!file.is_open()) {
        throw runtime_error("Unable to open file");
    }

    int numFacilities, numCustomers;
    file >> numFacilities >> numCustomers;

    vector<int> facilityCapacities(numFacilities);
    vector<double> openingCosts(numFacilities);
    for (int i = 0; i < numFacilities; ++i) {
        file >> facilityCapacities[i] >> openingCosts[i];
    }

    vector<int> customerDemands(numCustomers);
    vector<vector<int>> transportationCosts(numFacilities, vector<int>(numCustomers));
    for (int j = 0; j < numCustomers; ++j) {
        file >> customerDemands[j];
        for (int i = 0; i < numFacilities; ++i) {
            file >> transportationCosts[i][j];
        }
    }

    return CFLPProblem(
        std::move(transportationCosts),
        std::move(facilityCapacities),
        std::move(customerDemands),
        std::move(openingCosts)
    );
}
