#include "ContinuousKnapsackProblem/continuous_knapsack.h"
#include <algorithm>

ContinuousKnapsack::ContinuousKnapsack(double capacity, const std::vector<ContinuousItem> &items)
    : capacity(capacity), items(items) {}

double ContinuousKnapsack::getCapacity() const
{
    return capacity;
}

const std::vector<ContinuousItem> &ContinuousKnapsack::getItems() const
{
    return items;
}

void ContinuousKnapsack::setCapacity(double capacity)
{
    this->capacity = capacity;
}

void ContinuousKnapsack::setItems(const std::vector<ContinuousItem> &items)
{
    this->items = items;
}

std::vector<std::pair<int, double>> ContinuousKnapsack::solve() const
{
    std::vector<std::pair<int, double>> solution;
    std::vector<std::pair<double, int>> indexWithValue; // (valueIndex, index)

    for (size_t i = 0; i < items.size(); ++i)
    {
        indexWithValue.emplace_back(items[i].getValueIndex(), i);
    }

    std::sort(indexWithValue.begin(), indexWithValue.end(),
              [](const std::pair<double, int> &a, const std::pair<double, int> &b)
              {
                  return a.first > b.first;
              });

    double totalWeight = 0.0;

    for (const auto &[valueIndex, idx] : indexWithValue)
    {
        if (totalWeight >= capacity)
            break;

        double availableWeight = items[idx].getWeight();
        double takeAmount = std::min(capacity - totalWeight, availableWeight);

        if (takeAmount > 0)
        {
            solution.emplace_back(idx, takeAmount);
            totalWeight += takeAmount;
        }
    }

    return solution;
}
