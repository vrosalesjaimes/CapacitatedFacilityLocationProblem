#include "TabuSearch/tabu_search_solver.h"
#include "../PLQT/plqt_node.h"
#include "../ContinuousKnapsackProblem/continuous_item.h"
#include "../ContinuousKnapsackProblem/continuous_knapsack.h"
#include <algorithm>
#include <cmath>
#include <chrono>
#include <iostream>
#include <functional>

using namespace std;

namespace
{
    const unsigned int seed = 12345;

    std::mt19937 gen(seed);

    std::uniform_int_distribution<int> dist_l0(l0_l, l0_u);
    std::uniform_int_distribution<int> dist_l1(l1_l, l1_u);
}

TabuSearchSolver::TabuSearchSolver(CFLPProblem &problem)
    : problem(problem), m(problem.getCapacities().size())
{
    y.resize(m, 0);
    y_best.resize(m, 0);
    t.resize(m, 0);
    h.resize(m, 0);
    plqt_ = PLQT();
}

void TabuSearchSolver::solve()
{
    initialize();
    mainSearchProcess();
}

void TabuSearchSolver::computePriorities()
{
    const std::vector<double> &f = problem.getOpeningCosts();
    const std::vector<int> &a = problem.getCapacities();
    const std::vector<std::vector<int>> &c = problem.getCostMatrix();
    const std::vector<int> &b = problem.getDemands();

    int m = f.size();
    int n = b.size();

    int k_near = std::max(1, n / 3);

    priorityP2_.resize(m);
    priorityP3_.resize(m);

    for (int i = 0; i < m; ++i)
    {
        std::vector<int> transport_costs = c[i]; // copia
        std::nth_element(transport_costs.begin(), transport_costs.begin() + k_near, transport_costs.end());
        double sum_k_nearest = std::accumulate(transport_costs.begin(), transport_costs.begin() + k_near, 0.0);

        priorityP2_[i] = (sum_k_nearest / k_near) + f[i];
        priorityP3_[i] = (sum_k_nearest / k_near) + (f[i] / a[i]);
    }

    std::vector<std::pair<int, double>> P2(m);
    for (int i = 0; i < m; ++i)
    {
        P2[i] = {i, priorityP2_[i]};
    }
    std::sort(P2.begin(), P2.end(), [](auto &a, auto &b)
              { return a.second < b.second; });

    I2_.clear();
    for (auto &[index, _] : P2)
    {
        I2_.push_back(index);
    }

    std::vector<std::pair<int, double>> P3(m);
    for (int i = 0; i < m; ++i)
    {
        P3[i] = {i, priorityP3_[i]};
    }
    std::sort(P3.begin(), P3.end(), [](auto &a, auto &b)
              { return a.second < b.second; });

    I3_.clear();
    for (auto &[index, _] : P3)
    {
        I3_.push_back(index);
    }
}

void TabuSearchSolver::initialize()
{
    computePriorities();
    const std::vector<int> &a = problem.getCapacities();
    const std::vector<int> &b = problem.getDemands();

    // Abrir instalaciones hasta que la capacidad cubra la demanda total
    y.assign(m, 0);
    double total_demand = std::accumulate(b.begin(), b.end(), 0.0);
    double total_capacity = 0.0;

    for (int i : I3_)
    {
        y[i] = 1;
        total_capacity += a[i];
        if (total_capacity >= total_demand)
            break;
    }
    y_P3 = y;

    y_P2.assign(m, 0);
    double total_capacity_P2 = 0.0;
    for (int i : I2_)
    {
        y_P2[i] = 1;
        total_capacity_P2 += a[i];
        if (total_capacity_P2 >= total_demand)
            break;
    }

    problem.initializeSubproblem(y);

    currentSupply = problem.getCurrentTotalSupply();
    totalDemand_ = problem.getTotalDemand();

    z0 = problem.getCurrentCost();
    z00 = z0;

    l0 = dist_l0(gen);
    l1 = dist_l1(gen);

    for (int i = 0; i < m; ++i)
    {
        if (y[i] == 0)
        {
            t[i] = -l0;
            h[i] = 0;
        }
        else
        {
            t[i] = -l1;
            h[i] = t[i];
        }
    }

    y_best = y;
    m1 = std::count(y.begin(), y.end(), 1);
    plqt_ = PLQT(m, new PLQTNode(y));
}

bool TabuSearchSolver::isFeasibleToClose(int i)
{
    int temp = currentSupply;

    temp -= problem.getCapacities()[i];

    return temp >= totalDemand_;
}

void TabuSearchSolver::mainSearchProcess()
{
    evaluateNeighborhood();

    if (alpha1 * m < k0)
    {
        intensification();
    }
    else
    {
        handleTabuMove();
    }
}

void TabuSearchSolver::handleTabuMove()
{
    determineBestFacility();
    if (!isTabu(bestFacility))
    {
        executeMove(bestFacility);
        mainSearchProcess();
    }
    else
    {

        if (aspirationCriterion(computeDeltaZ(bestFacility)))
        {
            executeMove(bestFacility);
            mainSearchProcess();
        }
        else
        {
            deltaZ_values[bestFacility] = std::numeric_limits<int>::max();
            handleTabuMove();
        }
    }
}

void TabuSearchSolver::determineBestFacility()
{
    bestDelta = numeric_limits<int>::max();
    bestFacility = -1;
    for (int i : deltaZ_values)
    {
        if (deltaZ_values[i] < bestDelta)
        {
            bestDelta = deltaZ_values[i];
            bestFacility = i;
        }
    }
}

void TabuSearchSolver::executeMove(int i)
{
    if (y[i] == 1)
    {
        m1--;
    }
    else
    {
        m1++;
    }

    t[i] = k;
    k++;

    if (y[i] == 1)
    {
        h[i] = h[i] + k - t[i];
    }

    problem.toggleFacility(i);
    zk = problem.getCurrentCost();
    currentSupply = problem.getCurrentTotalSupply();
    plqt_.insert(y);

    if (zk < z0)
    {
        z0 = zk;
        k0 = k;
    }

    if (z0 < z00)
    {
        z00 = z0;
        y_best = y;
    }

    if (k - k0 < (alpha1 - alpha2) * m)
    {
        mainSearchProcess();
    }
}

bool TabuSearchSolver::isTabu(int i)
{
    return (k - t[i]) <= h[i];
}

bool TabuSearchSolver::aspirationCriterion(int deltaZ)
{
    return (zk + deltaZ) < z0;
}

void TabuSearchSolver::determineNeighborhood()
{
    bar_I.clear();
    int startIndex = ((k - 1) * bar_m) % m;
    for (int i = 0; i < bar_m; ++i)
    {
        int index = (startIndex + i) % m;
        bar_I.push_back(index);
    }
}

void TabuSearchSolver::evaluateNeighborhood()
{
    determineNeighborhood();

    deltaZ_values.assign(m, 0);
    for (int i : bar_I)
    {
        deltaZ_values[i] = computeDeltaZ(i); // no implementada
    }
}

void TabuSearchSolver::intensification()
{
    criterionAltering();
    solutionReconciling();
    pathRelinking(y);
}

void TabuSearchSolver::criterionAltering()
{
    // Step 5
    evaluateNeighborhoodAltering(); // no implementada

    // Step 6 y 7
    handleTabuMoveAltering();

    // Step 8
    if (k0 <= (alpha1 + alpha2) * m)
    {
        mainSearchProcess();
    }
}

void TabuSearchSolver::evaluateNeighborhoodAltering()
{
    determineNeighborhood();

    deltaZ_values_altering.assign(m, 0);
    for (int i : bar_I)
    {
        deltaZ_values_altering[i] = computeDeltaZ_altering(i); // no implementada
    }
}

void TabuSearchSolver::determineBestFacilityAltering()
{
    bestDelta_altering = numeric_limits<int>::max();
    bestFacility = -1;
    for (int i : deltaZ_values_altering)
    {
        if (deltaZ_values_altering[i] < bestDelta_altering)
        {
            bestDelta_altering = deltaZ_values_altering[i];
            bestFacility = i;
        }
    }
}

void TabuSearchSolver::handleTabuMoveAltering()
{
    determineBestFacilityAltering();
    if (!isTabu(bestFacility))
    {
        executeMove(bestFacility);
    }
    else
    {
        if (aspirationCriterion(computeDeltaZ_altering(bestFacility)))
        {
            executeMove(bestFacility);
        }
        else
        {
            deltaZ_values_altering[bestFacility] = std::numeric_limits<int>::max();
            handleTabuMoveAltering();
        }
    }
}

void TabuSearchSolver::solutionReconciling()
{
    // Step 9
    reconcilingY = c % 2 == 0 ? y_P2 : y_P3;

    initialIndex = 0;
    finalIndex = m - 1;

    backIndex();
    advanceIndex();
}

void TabuSearchSolver::backIndex()
{
    std::vector<int> tempY = y;

    while (tempY[finalIndex] == 0)
    {
        finalIndex--;
    }

    if (initialIndex < finalIndex)
    {
        if (isFeasibleToClose(finalIndex))
        {
            tempY[finalIndex] = 0;
            if (plqt_.search(tempY) != nullptr)
            {
                finalIndex--;
            }
            else
            {
                executeMove(finalIndex);
                backIndex();
            }
        }
    }
}

void TabuSearchSolver::advanceIndex()
{
    std::vector<int> tempY = y;

    while (tempY[initialIndex] == 1)
    {
        initialIndex++;
    }

    if (initialIndex < finalIndex)
    {
        tempY[initialIndex] = 1;
        if (plqt_.search(tempY) != nullptr)
        {
            initialIndex++;
            advanceIndex();
        }
        else
        {
            executeMove(initialIndex);
            backIndex();
        }
    }
}

void TabuSearchSolver::pathRelinking(const std::vector<int> &source)
{
    targetSolution = y_best;
    int targetCost = z00;

    std::vector<int> I0T;
    std::vector<int> I1T;

    for (size_t i = 0; i < source.size(); ++i)
    {
        if (source[i] == 0 && targetSolution[i] == 1)
        {
            I0T.push_back(i); // Está cerrada en source pero abierta en target
        }
        else if (source[i] == 1 && targetSolution[i] == 0)
        {
            I1T.push_back(i); // Está abierta en source pero cerrada en target
        }
    }

    // Paso 16
    while (!I1T.empty())
    {
        int i = I1T.back();
        if (isFeasibleToClose(i))
        {
            break;
        }

        std::vector<int> tempY = y;
        tempY[i] = 0; // Cerrar instalación

        if (plqt_.search(tempY) == nullptr)
        {
            I1T.pop_back();
            continue;
        }

        I1T.pop_back();
        executeMove(i);
    }

    // Paso 18
    while (!I1T.empty())
    {
        int i = I1T.back();

        vector<int> tempY = y;
        tempY[i] = 1; // Abrir instalación

        if (plqt_.search(tempY) == nullptr)
        {
            I1T.pop_back();
            continue;
        }

        I1T.pop_back();
        executeMove(i);
    }

    if (targetCost < z0)
    {
        return;
    }
    else if (targetCost < z00)
    {
        y = targetSolution;
        z0 = targetCost;
        pathRelinking(y);
    }
}
