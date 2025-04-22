#include "ContinuousKnapsackProblem/continuous_item.h"

ContinuousItem::ContinuousItem(double valueIndex, double weight)
    : valueIndex(valueIndex), weight(weight) {}

double ContinuousItem::getValueIndex() const
{
    return valueIndex;
}

double ContinuousItem::getWeight() const
{
    return weight;
}

void ContinuousItem::setValueIndex(double valueIndex)
{
    this->valueIndex = valueIndex;
}

void ContinuousItem::setWeight(double weight)
{
    this->weight = weight;
}
