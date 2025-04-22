#ifndef CONTINUOUS_KNAPSACK_H
#define CONTINUOUS_KNAPSACK_H

#include "continuous_item.h"
#include <vector>
#include <utility>

/**
 * @class ContinuousKnapsack
 * @brief Solves the continuous knapsack problem using a greedy algorithm.
 */
class ContinuousKnapsack
{
private:
    double capacity; ///< Maximum weight capacity of the knapsack.
    std::vector<ContinuousItem> items; ///< List of available items.

public:
    /**
     * @brief Constructs the knapsack problem.
     * @param capacity The total capacity of the knapsack.
     * @param items The list of items to consider.
     */
    ContinuousKnapsack(double capacity, const std::vector<ContinuousItem> &items);

    /** @brief Gets the capacity of the knapsack. */
    double getCapacity() const;

    /** @brief Gets the items of the knapsack problem. */
    const std::vector<ContinuousItem> &getItems() const;

    /** @brief Sets the capacity of the knapsack. */
    void setCapacity(double capacity);

    /** @brief Sets the list of items. */
    void setItems(const std::vector<ContinuousItem> &items);

    /**
     * @brief Solves the problem using the greedy algorithm.
     * @return A list of pairs where each pair represents (item index, amount taken).
     */
    std::vector<std::pair<int, double>> solve() const;
};

#endif
