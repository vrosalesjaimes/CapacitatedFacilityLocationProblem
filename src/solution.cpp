#include "CapacityFacilityLocationProblem/solution.h"

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
