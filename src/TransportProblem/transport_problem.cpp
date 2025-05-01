#include "TransportProblem/transport_problem.h"
#include <stdexcept>
#include <numeric>
#include <algorithm>
#include <limits>
#include <climits>

/**
 * @brief Constructor for the TransportationProblem class.
 */
TransportationProblem::TransportationProblem(const std::vector<int> &supply,
                                             const std::vector<int> &demand,
                                             const std::vector<std::vector<int>> &costMatrix)
    : supply_(supply),
      demand_(demand),
      costMatrix_(costMatrix),
      totalCost_(0)
{
    if (costMatrix_.size() != supply_.size())
        throw std::invalid_argument("Cost matrix row count must match supply size.");
    for (const auto &row : costMatrix_)
    {
        if (row.size() != demand_.size())
            throw std::invalid_argument("Each cost matrix row must match demand size.");
    }

    initializeAssignment();
}

/**
 * @brief Initializes or resets the assignment matrix and total cost.
 */
void TransportationProblem::initializeAssignment()
{
    assignmentMatrix_ = std::vector<std::vector<int>>(supply_.size(), std::vector<int>(demand_.size(), 0));
    totalCost_ = 0;
}

/**
 * @brief Placeholder for Northwest Corner Method.
 */
void TransportationProblem::solveNorthwestCorner()
{
    initializeAssignment();
    size_t i = 0; // Index for supply (rows)
    size_t j = 0; // Index for demand (columns)

    std::vector<int> remainingSupply = supply_;
    std::vector<int> remainingDemand = demand_;

    while (i < remainingSupply.size() && j < remainingDemand.size())
    {
        int quantity = std::min(remainingSupply[i], remainingDemand[j]);

        assignmentMatrix_[i][j] = quantity;

        totalCost_ += quantity * costMatrix_[i][j];

        remainingSupply[i] -= quantity;
        remainingDemand[j] -= quantity;

        if (remainingSupply[i] == 0 && remainingDemand[j] == 0)
        {
            ++i;
            ++j;
        }
        else if (remainingSupply[i] == 0)
        {
            ++i;
        }
        else if (remainingDemand[j] == 0)
        {
            ++j;
        }
    }
}

/**
 * @brief Placeholder for Vogel's Approximation Method.
 */
void TransportationProblem::solveVogelsApproximation()
{
    initializeAssignment();

    std::vector<int> supply = supply_;
    std::vector<int> demand = demand_;
    size_t m = supply.size();
    size_t n = demand.size();
    std::vector<bool> rowDone(m, false);
    std::vector<bool> colDone(n, false);

    while (std::count(rowDone.begin(), rowDone.end(), false) > 0 &&
           std::count(colDone.begin(), colDone.end(), false) > 0)
    {
        // Calculate penalties for rows
        std::vector<int> rowPenalties(m, -1);
        for (size_t i = 0; i < m; ++i)
        {
            if (rowDone[i])
                continue;

            std::vector<int> costs;
            for (size_t j = 0; j < n; ++j)
                if (!colDone[j])
                    costs.push_back(costMatrix_[i][j]);

            if (costs.size() >= 2)
            {
                std::nth_element(costs.begin(), costs.begin() + 1, costs.end());
                rowPenalties[i] = costs[1] - costs[0];
            }
            else if (costs.size() == 1)
                rowPenalties[i] = costs[0];
        }

        // Calculate penalties for columns
        std::vector<int> colPenalties(n, -1);
        for (size_t j = 0; j < n; ++j)
        {
            if (colDone[j])
                continue;

            std::vector<int> costs;
            for (size_t i = 0; i < m; ++i)
                if (!rowDone[i])
                    costs.push_back(costMatrix_[i][j]);

            if (costs.size() >= 2)
            {
                std::nth_element(costs.begin(), costs.begin() + 1, costs.end());
                colPenalties[j] = costs[1] - costs[0];
            }
            else if (costs.size() == 1)
                colPenalties[j] = costs[0];
        }

        // Find max penalty
        int maxPenalty = -1;
        bool isRow = true;
        size_t index = 0;

        for (size_t i = 0; i < m; ++i)
        {
            if (!rowDone[i] && rowPenalties[i] > maxPenalty)
            {
                maxPenalty = rowPenalties[i];
                isRow = true;
                index = i;
            }
        }
        for (size_t j = 0; j < n; ++j)
        {
            if (!colDone[j] && colPenalties[j] > maxPenalty)
            {
                maxPenalty = colPenalties[j];
                isRow = false;
                index = j;
            }
        }

        // Choose cell with minimum cost in that row/column
        size_t chosenRow = 0, chosenCol = 0;
        int minCost = std::numeric_limits<int>::max();

        if (isRow)
        {
            chosenRow = index;
            for (size_t j = 0; j < n; ++j)
            {
                if (!colDone[j] && costMatrix_[chosenRow][j] < minCost)
                {
                    minCost = costMatrix_[chosenRow][j];
                    chosenCol = j;
                }
            }
        }
        else
        {
            chosenCol = index;
            for (size_t i = 0; i < m; ++i)
            {
                if (!rowDone[i] && costMatrix_[i][chosenCol] < minCost)
                {
                    minCost = costMatrix_[i][chosenCol];
                    chosenRow = i;
                }
            }
        }

        // Allocate as much as possible
        int quantity = std::min(supply[chosenRow], demand[chosenCol]);
        assignmentMatrix_[chosenRow][chosenCol] = quantity;
        totalCost_ += quantity * costMatrix_[chosenRow][chosenCol];
        supply[chosenRow] -= quantity;
        demand[chosenCol] -= quantity;

        // Update availability
        if (supply[chosenRow] == 0)
            rowDone[chosenRow] = true;
        if (demand[chosenCol] == 0)
            colDone[chosenCol] = true;
    }
}

/**
 * @brief Placeholder for Hungarian Method.
 */
void TransportationProblem::solveHungarianMethod()
{
    
}

/**
 * @brief Returns the total cost of the current assignment.
 */
int TransportationProblem::getTotalCost() const
{
    return totalCost_;
}

/**
 * @brief Returns the current assignment matrix.
 */
const std::vector<std::vector<int>> &TransportationProblem::getAssignmentMatrix() const
{
    return assignmentMatrix_;
}

/**
 * @brief Returns the current cost matrix.
 */
const std::vector<std::vector<int>> &TransportationProblem::getCostMatrix() const
{
    return costMatrix_;
}

/**
 * @brief Returns the current supply vector.
 */
const std::vector<int> &TransportationProblem::getSupply() const
{
    return supply_;
}

/**
 * @brief Returns the current demand vector.
 */
const std::vector<int> &TransportationProblem::getDemand() const
{
    return demand_;
}

/**
 * @brief Sets a new cost matrix and resets assignment.
 */
void TransportationProblem::setCostMatrix(const std::vector<std::vector<int>> &newCostMatrix)
{
    if (newCostMatrix.size() != supply_.size())
        throw std::invalid_argument("New cost matrix row count must match supply size.");
    for (const auto &row : newCostMatrix)
    {
        if (row.size() != demand_.size())
            throw std::invalid_argument("Each new cost matrix row must match demand size.");
    }
    costMatrix_ = newCostMatrix;
    initializeAssignment();
}

/**
 * @brief Sets a new supply vector and resets assignment.
 */
void TransportationProblem::setSupply(const std::vector<int> &newSupply)
{
    if (newSupply.size() != costMatrix_.size())
        throw std::invalid_argument("New supply size must match cost matrix rows.");
    supply_ = newSupply;
    initializeAssignment();
}

/**
 * @brief Sets a new demand vector and resets assignment.
 */
void TransportationProblem::setDemand(const std::vector<int> &newDemand)
{
    if (!costMatrix_.empty() && newDemand.size() != costMatrix_[0].size())
        throw std::invalid_argument("New demand size must match cost matrix columns.");
    demand_ = newDemand;
    initializeAssignment();
}

void TransportationProblem::balance()
{
    int totalSupply = std::accumulate(supply_.begin(), supply_.end(), 0);
    int totalDemand = std::accumulate(demand_.begin(), demand_.end(), 0);

    if (totalSupply == totalDemand)
    {
        return;
    }

    if (totalSupply < totalDemand)
    {
        throw std::logic_error("This class only supports total supply >= total demand.");
    }

    int dummyDemand = totalSupply - totalDemand;

    demand_.push_back(dummyDemand);

    for (auto &row : costMatrix_)
    {
        row.push_back(0);
    }

    for (auto &row : assignmentMatrix_)
    {
        row.push_back(0);
    }
}
