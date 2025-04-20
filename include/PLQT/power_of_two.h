#ifndef POWER_OF_TWO_H
#define POWER_OF_TWO_H

#include <array>
#include <stdexcept>
#include <cstdint>

/**
 * @file power_of_two.h
 * @brief Class that defines precomputed powers of 2 using left shift.
 * 
 * This class allows the calculation of a specified number of powers of 2,
 * which are precomputed and stored in an internal array. The powers of 2
 * are calculated at runtime using the left shift operator.
 */
class PowerOfTwo {
private:
    std::array<uint64_t, 64> powersOfTwo;  // Array to store the powers of 2 (up to 2^63)
    size_t numPowers;                     // Number of powers of 2 to compute

public:
    /**
     * @brief Constructor that computes the first 'numPowers' powers of 2.
     * 
     * @param num The number of powers of 2 to compute.
     * @throws std::out_of_range If 'num' is greater than the maximum number of powers.
     */
    explicit PowerOfTwo(size_t num);

    /**
     * @brief Get the precomputed power of 2 at index 'i'.
     * 
     * @param i The index of the power of 2 to retrieve.
     * @return uint64_t The power of 2 at index 'i'.
     * @throws std::out_of_range If 'i' is outside the valid range.
     */
    uint64_t getPower(size_t i) const;

    /**
     * @brief Get the total number of powers of 2 computed.
     * 
     * @return size_t The total number of precomputed powers of 2.
     */
    size_t getNumPowers() const;
};

#endif // POWER_OF_TWO_H
