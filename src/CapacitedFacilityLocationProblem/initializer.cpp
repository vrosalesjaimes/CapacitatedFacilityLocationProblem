#include "CapacitedFacilityLocationProblem/initializer.h"
#include <algorithm>
#include <numeric>
#include <functional>
#include <limits>
#include <cmath>

Initializer::Initializer(const Instance& instance) : instance(instance) {}

double Initializer::calculatePi2(int i) const {
    const auto& facilityCapacities = instance.getFacilityCapacities();
    const auto& customerDemands = instance.getCustomerDemands();
    const auto& transportationCosts = instance.getTransportationCosts();

    double numerator = 0.0;
    double denominator = 0.0;

    for (size_t j = 0; j < customerDemands.size(); ++j) {
        numerator += customerDemands[j];
        denominator += transportationCosts[i][j] * customerDemands[j];
    }

    double a_i = facilityCapacities[i];
    double f_i = instance.getOpeningCosts()[i];
    
    return f_i - (numerator / denominator) * a_i;
}

double Initializer::calculatePi3(int i) const {
    const auto& facilityCapacities = instance.getFacilityCapacities();
    const auto& transportationCosts = instance.getTransportationCosts();

    std::vector<double> sortedCosts(transportationCosts[i].begin(), transportationCosts[i].end());
    std::sort(sortedCosts.begin(), sortedCosts.end());

    size_t n = sortedCosts.size();
    double averageCost = std::accumulate(sortedCosts.begin(), sortedCosts.begin() + std::min(n, static_cast<size_t>(std::floor(n / 3.0))), 0.0) /
                         std::min(n, static_cast<size_t>(std::floor(n / 3.0)));
    double f_i = instance.getOpeningCosts()[i];
    double a_i = facilityCapacities[i];

    return averageCost + f_i / a_i;
}

Solution Initializer::findInitialSolution() const {
    size_t numFacilities = instance.getNumFacilities();
    std::vector<int> I1; // Facilities to be opened
    std::vector<int> I2(numFacilities); // Facilities not yet opened
    std::iota(I2.begin(), I2.end(), 0); // Initialize I2 with indices 0 to numFacilities-1

    double Z = std::numeric_limits<double>::infinity(); // Initial large value for Z
    std::vector<int> I1_final, I2_final; // To store final results

    // Calculate Pi2 and Pi3 values
    std::vector<std::pair<int, double>> Pi2_values(numFacilities);
    std::vector<std::pair<int, double>> Pi3_values(numFacilities);
    
    for (size_t i = 0; i < numFacilities; ++i) {
        Pi2_values[i] = {i, calculatePi2(i)};
        Pi3_values[i] = {i, calculatePi3(i)};
    }

    // Sort facilities based on Pi2 and Pi3
    std::sort(Pi2_values.begin(), Pi2_values.end(), [](const auto& lhs, const auto& rhs) {
        return lhs.second < rhs.second;
    });

    std::sort(Pi3_values.begin(), Pi3_values.end(), [](const auto& lhs, const auto& rhs) {
        return lhs.second < rhs.second;
    });

    // Extract sorted indices
    std::vector<int> I2_sorted(numFacilities);
    for (size_t i = 0; i < numFacilities; ++i) {
        I2_sorted[i] = Pi2_values[i].first;
    }

    std::vector<int> I3_sorted(numFacilities);
    for (size_t i = 0; i < numFacilities; ++i) {
        I3_sorted[i] = Pi3_values[i].first;
    }

    // Perform the ADD method
    while (!I2_sorted.empty()) {
        // Find the facility that provides the largest reduction in Z
        auto h = I2_sorted.front();
        double reduction = 0.0; // Calculate actual reduction based on opening h

        if (reduction > 0) {
            I1.push_back(h);
            I2_sorted.erase(std::remove(I2_sorted.begin(), I2_sorted.end(), h), I2_sorted.end());
        } else {
            break;
        }
    }

    // Construct initial solution
    std::vector<uint8_t> y(numFacilities, 0);
    std::vector<std::vector<uint8_t>> x(numFacilities, std::vector<uint8_t>(instance.getNumCustomers(), 0));
    double cost = 0.0;

    for (int facility : I1) {
        y[facility] = 1;
        cost += instance.getOpeningCosts()[facility];
    }

    Solution initialSolution(cost, y, x);
    initialSolution.calculateCost(instance);

    return initialSolution;
}
