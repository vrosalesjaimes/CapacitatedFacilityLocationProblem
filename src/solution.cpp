#include "CapacityFacilityLocationProblem/solution.h"

Solution::Solution(double cost) : cost(cost) {}

double Solution::getCost() const {
    return cost;
}

bool Solution::operator==(const Solution& other) const {
    return cost == other.cost;
}
