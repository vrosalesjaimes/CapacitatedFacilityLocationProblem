#ifndef CONTINUOUS_ITEM_H
#define CONTINUOUS_ITEM_H

#include <string>

/**
 * @class ContinuousItem
 * @brief Represents an item in the continuous knapsack problem.
 *
 * Each item is defined by its value index (value per unit weight)
 * and its weight.
 */
class ContinuousItem
{
private:
    double valueIndex; ///< Value per unit weight.
    double weight;     ///< Total weight of the item.

public:
    /**
     * @brief Constructs a ContinuousItem.
     * @param valueIndex The value per unit weight.
     * @param weight The total weight of the item.
     */
    ContinuousItem(double valueIndex, double weight);

    /** @brief Gets the value index of the item. */
    double getValueIndex() const;

    /** @brief Gets the weight of the item. */
    double getWeight() const;

    /** @brief Sets the value index of the item. */
    void setValueIndex(double valueIndex);

    /** @brief Sets the weight of the item. */
    void setWeight(double weight);
};

#endif
