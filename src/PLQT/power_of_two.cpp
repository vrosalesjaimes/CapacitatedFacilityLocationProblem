#include "PLQT/power_of_two.h"

/**
 * @brief Constructor that computes the first 'numPowers' powers of 2.
 * 
 * @param num The number of powers of 2 to compute.
 * @throws std::out_of_range If 'num' is greater than the maximum number of powers.
 */
PowerOfTwo::PowerOfTwo(size_t num) : numPowers(num) {
    if (num > 64) {
        throw std::out_of_range("Number of powers must be 64 or less.");
    }

    for (size_t i = 0; i < num; ++i) {
        powersOfTwo[i] = 1ULL << i; 
    }
}

/**
 * @brief Get the precomputed power of 2 at index 'i'.
 * 
 * @param i The index of the power of 2 to retrieve.
 * @return uint64_t The power of 2 at index 'i'.
 * @throws std::out_of_range If 'i' is outside the valid range.
 */
uint64_t PowerOfTwo::getPower(size_t i) const {
    if (i >= numPowers) {
        throw std::out_of_range("Index out of range.");
    }
    return powersOfTwo[i];
}

/**
 * @brief Get the total number of powers of 2 computed.
 * 
 * @return size_t The total number of precomputed powers of 2.
 */
size_t PowerOfTwo::getNumPowers() const {
    return numPowers;
}
