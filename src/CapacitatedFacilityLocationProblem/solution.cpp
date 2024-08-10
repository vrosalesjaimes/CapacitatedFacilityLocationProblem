#include "CapacitatedFacilityLocationProblem/solution.h"
#include "CapacitatedFacilityLocationProblem/instance.h"
#include <sstream>
#include <iomanip>
#include <numeric>
#include <iostream>


using namespace std;

Solution::Solution(double cost, vector<bool> y, vector<vector<int>> x)
    : cost(cost), y(move(y)), x(move(x)), totalDemand(0), demandCalculated(false) {}

double Solution::getCost() {
    return cost;
}

void Solution::setCost(double cost) {
    this->cost = cost;
}

vector<bool>& Solution::getY() {
    return this->y;
}

void Solution::setY(vector<bool>& y) {
    this->y = y;
}

vector<vector<int>>& Solution::getX() {
    return x;
}

void Solution::setX(vector<vector<int>>& x) {
    this->x = x;
}

bool Solution::operator==(Solution& other) {
    return cost == other.cost && y == other.y && x == other.x;
}

void Solution::calculateCost(Instance& instance) {
    double totalCost = 0.0;
    auto& openingCosts = instance.getOpeningCosts();
    auto& transportationCosts = instance.getTransportationCosts();
    
    for (size_t i = 0; i < y.size(); ++i) {
        if (y[i] == 1) {
            totalCost += openingCosts[i];
            for (size_t j = 0; j < x[i].size(); ++j) {
                if (x[i][j] == 1) {
                    totalCost += transportationCosts[i][j];
                }
            }
        }
    }
    
    setCost(totalCost);
}

string Solution::toString() {
    stringstream ss;
    ss << fixed << setprecision(2);
    ss << "Cost: " << cost << "\nY: ";
    for (int i = 0; i < y.size(); ++i) {
        ss << static_cast<int>(y[i]) << " ";
    }
    ss << "\nX:\n";
    for (auto& row : x) {
        for (auto& val : row) {
            ss << static_cast<int>(val) << " ";
        }
        ss << "\n";
    }
    return ss.str();
}

int Solution::calculateTotalDemand(Instance& instance) {
    if (!demandCalculated) {
        auto& customerDemands = instance.getCustomerDemands();
        totalDemand = accumulate(customerDemands.begin(), customerDemands.end(), 0);
        demandCalculated = true;
    }
    return totalDemand;
}

bool Solution::isFeasible(Instance& instance) {
    int totalCapacity = 0;
    for (size_t i = 0; i < y.size(); ++i) {
        if (y[i] == 1) {
            totalCapacity += instance.getFacilityCapacities()[i];
        }
    }
    return totalCapacity >= calculateTotalDemand(instance);
}
