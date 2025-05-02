#include "TransportProblem/transport_problem.h"
#include <stdexcept>
#include <numeric>
#include <algorithm>
#include <limits>
#include <climits>
#include <functional>
#include <iostream>

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
    initializeAssignment(); // Reset assignment and total cost

    size_t m = supply_.size();
    size_t n = demand_.size();

    if (totalSupply_ != totalDemand_)
    {
        throw std::logic_error("Supply and demand must be balanced for the Hungarian method.");
    }
    
    struct MatrixEntry
    {
        int init_cost;
        int val;
        int quota;
        bool starred;
        bool primed;
    };

    struct AgentJobEntry
    {
        bool marked;
        int discr; // discrepancia (oferta/demanda)
    };

    // Inicialización de estructuras
    std::vector<AgentJobEntry> agents(m);
    std::vector<AgentJobEntry> jobs(n);
    std::vector<std::vector<MatrixEntry>> matrix(m, std::vector<MatrixEntry>(n));

    // Llenar los datos iniciales
    for (size_t i = 0; i < m; ++i)
    {
        agents[i].marked = false;
        agents[i].discr = supply_[i];
    }

    for (size_t j = 0; j < n; ++j)
    {
        jobs[j].marked = false;
        jobs[j].discr = demand_[j];
    }

    for (size_t i = 0; i < m; ++i)
    {
        for (size_t j = 0; j < n; ++j)
        {
            matrix[i][j] = {
                costMatrix_[i][j], // init_cost
                costMatrix_[i][j], // val (se modificará)
                0,                 // quota
                false,             // starred
                false              // primed
            };
        }
    }

    // Declarar step2 antes de step1 para que step1 pueda usarla
    std::function<void(size_t, size_t)> step2;
    std::function<void()> step3;
    std::function<void()> step1;
    std::function<bool()> done;
    std::function<void()> prelim;

    // Implementación de step2
    step2 = [&](size_t i0, size_t j0)
    {
        size_t current_j = j0;
        int min_quota = agents[i0].discr;

        size_t max_dim = std::max(n, m);
        std::vector<MatrixEntry *> primed_seen(max_dim, nullptr);
        std::vector<MatrixEntry *> starred_seen(max_dim, nullptr);
        size_t idx = 0;

        bool seen_star = true;
        while (seen_star)
        {
            seen_star = false;
            for (size_t i = 0; i < m; ++i)
            {
                if (matrix[i][current_j].starred)
                {
                    seen_star = true;
                    min_quota = std::min(min_quota, matrix[i][current_j].quota);
                    starred_seen[idx] = &matrix[i][current_j];

                    for (size_t j = 0; j < n; ++j)
                    {
                        if (matrix[i][j].primed)
                        {
                            current_j = j;
                            primed_seen[idx] = &matrix[i][j];
                            idx++;
                            break;
                        }
                    }
                    break;
                }
            }
        }
        min_quota = std::min(min_quota, jobs[current_j].discr);

        matrix[i0][j0].quota += min_quota;
        agents[i0].discr -= min_quota;
        jobs[current_j].discr -= min_quota;

        while (idx > 0)
        {
            idx--;
            primed_seen[idx]->quota += min_quota;
            starred_seen[idx]->quota -= min_quota;
        }

        // Resetear marcas
        for (size_t i = 0; i < m; ++i)
        {
            for (size_t j = 0; j < n; ++j)
            {
                matrix[i][j].starred = false;
                matrix[i][j].primed = false;
            }
            agents[i].marked = false;
        }

        for (size_t j = 0; j < n; ++j)
        {
            if (jobs[j].discr == 0)
            {
                jobs[j].marked = true;
            }
        }
    };

    // Implementación de step3
    step3 = [&]()
    {
        int h = std::numeric_limits<int>::max();
        for (size_t i = 0; i < m; ++i)
        {
            for (size_t j = 0; j < n; ++j)
            {
                if (!agents[i].marked && !jobs[j].marked)
                {
                    h = std::min(h, matrix[i][j].val);
                }
            }
        }

        if (h <= 0)
        {
            throw std::runtime_error("Error en el paso 3: h no es positivo");
        }

        for (size_t i = 0; i < m; ++i)
        {
            for (size_t j = 0; j < n; ++j)
            {
                if (agents[i].marked)
                {
                    matrix[i][j].val += h;
                }
                if (!jobs[j].marked)
                {
                    matrix[i][j].val -= h;
                }
            }
        }
    };

    // Implementación de done
    done = [&]()
    {
        for (size_t i = 0; i < m; ++i)
        {
            if (agents[i].discr > 0)
                return false;
        }
        for (size_t j = 0; j < n; ++j)
        {
            if (jobs[j].discr > 0)
                return false;
        }
        return true;
    };

    // Implementación de prelim
    prelim = [&]()
    {
        // Reducción de filas
        for (size_t i = 0; i < m; ++i)
        {
            int min_val = std::numeric_limits<int>::max();
            for (size_t j = 0; j < n; ++j)
            {
                min_val = std::min(min_val, matrix[i][j].val);
            }
            for (size_t j = 0; j < n; ++j)
            {
                matrix[i][j].val -= min_val;
            }
        }

        // Reducción de columnas
        for (size_t j = 0; j < n; ++j)
        {
            int min_val = std::numeric_limits<int>::max();
            for (size_t i = 0; i < m; ++i)
            {
                min_val = std::min(min_val, matrix[i][j].val);
            }
            for (size_t i = 0; i < m; ++i)
            {
                matrix[i][j].val -= min_val;
            }
        }

        // Asignación inicial
        for (size_t i = 0; i < m; ++i)
        {
            for (size_t j = 0; j < n; ++j)
            {
                if (matrix[i][j].val == 0 && agents[i].discr > 0 && jobs[j].discr > 0)
                {
                    int min_quota = std::min(agents[i].discr, jobs[j].discr);
                    matrix[i][j].quota += min_quota;
                    agents[i].discr -= min_quota;
                    jobs[j].discr -= min_quota;
                }
            }
        }
    };

    // Implementación de step1 (ahora puede usar step2 que ya está definida)
    step1 = [&]()
    {
        bool something_changed = true;
        while (something_changed)
        {
            something_changed = false;
            for (size_t i = 0; i < m; ++i)
            {
                for (size_t j = 0; j < n; ++j)
                {
                    if (matrix[i][j].val == 0 && !agents[i].marked && !jobs[j].marked)
                    {
                        something_changed = true;
                        matrix[i][j].primed = true;

                        if (agents[i].discr > 0)
                        {
                            step2(i, j);
                            return;
                        }
                        else
                        {
                            agents[i].marked = true;
                            for (size_t k = 0; k < n; ++k)
                            {
                                if (agents[i].marked && jobs[k].marked && matrix[i][k].quota > 0)
                                {
                                    matrix[i][k].starred = true;
                                    jobs[k].marked = false;
                                }
                            }
                        }
                    }
                }
            }
        }
        step3();
    };

    // Ejecución del algoritmo
    prelim();

    while (!done())
    {
        step1();
    }

    // Llenar los resultados en assignmentMatrix_ y totalCost_
    totalCost_ = 0;
    for (size_t i = 0; i < m; ++i)
    {
        for (size_t j = 0; j < n; ++j)
        {
            assignmentMatrix_[i][j] = matrix[i][j].quota;
            totalCost_ += matrix[i][j].quota * matrix[i][j].init_cost;
        }
    }
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

void TransportationProblem::setTotalSupply(int newTotalSupply)
{
    totalSupply_ = newTotalSupply;
}

int TransportationProblem::getTotalSupply() const
{
    return totalSupply_;
}

void TransportationProblem::setTotalDemand(int newTotalDemand)
{
    totalDemand_ = newTotalDemand;
}

int TransportationProblem::getTotalDemand() const
{
    return totalDemand_;
}

void TransportationProblem::balance()
{

    if (totalSupply_ == totalDemand_)
    {
        return;
    }

    if (totalSupply_ < totalDemand_)
    {
        throw std::logic_error("This class only supports total supply >= total demand.");
    }

    int dummyDemand = totalSupply_ - totalDemand_;

    demand_.push_back(dummyDemand);

    for (auto &row : costMatrix_)
    {
        row.push_back(0);
    }

    for (auto &row : assignmentMatrix_)
    {
        row.push_back(0);
    }

    totalDemand_ = totalSupply_;
}

void TransportationProblem::calculateTotalSupplyAndDemand()
{
    totalSupply_ = std::accumulate(supply_.begin(), supply_.end(), 0);
    totalDemand_ = std::accumulate(demand_.begin(), demand_.end(), 0);
}