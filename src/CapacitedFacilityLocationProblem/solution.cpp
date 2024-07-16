#include "CapacitedFacilityLocationProblem/solution.h"
#include "CapacitedFacilityLocationProblem/instance.h"
#include <sstream>

Solution::Solution(double cost, const std::vector<uint8_t>& y, const std::vector<std::vector<uint8_t>>& x)
    : cost(cost), y(y), x(x) {}

double Solution::getCost() const {
    return cost;
}

void Solution::setCost(double cost) {
    this->cost = cost;
}

const std::vector<uint8_t>& Solution::getY() const {
    return y;
}

void Solution::setY(const std::vector<uint8_t>& y) {
    this->y = y;
}

const std::vector<std::vector<uint8_t>>& Solution::getX() const {
    return x;
}

void Solution::setX(const std::vector<std::vector<uint8_t>>& x) {
    this->x = x;
}

bool Solution::operator==(const Solution& other) const {
    return cost == other.cost && y == other.y && x == other.x;
}

void Solution::calculateCost(const Instance& instance) {
    double totalCost = 0.0;
    
    // Calculate opening costs
    const auto& openingCosts = instance.getOpeningCosts();
    for (size_t i = 0; i < y.size(); ++i) {
        if (y[i] == 1) {
            totalCost += openingCosts[i];
        }
    }
    
    // Calculate transportation costs
    const auto& transportationCosts = instance.getTransportationCosts();
    for (size_t i = 0; i < x.size(); ++i) {
        for (size_t j = 0; j < x[i].size(); ++j) {
            if (x[i][j] == 1) {
                totalCost += transportationCosts[i][j];
            }
        }
    }
    
    setCost(totalCost);
}

std::string Solution::toString() const {
    std::ostringstream oss;
    oss << "Total Cost: " << cost << "\n";
    oss << "Warehouses: \n";
    for (size_t i = 0; i < y.size(); ++i) {
        if (y[i] == 1) {
            oss << "Warehouse " << i + 1 << " is open\n";
        }
    }
    oss << "Customer Assignments: \n";
    for (size_t j = 0; j < x[0].size(); ++j) {
        oss << "Customer " << j + 1 << " is served by: ";
        for (size_t i = 0; i < x.size(); ++i) {
            if (x[i][j] == 1) {
                oss << "Warehouse " << i + 1 << " ";
            }
        }
        oss << "\n";
    }
    return oss.str();
}