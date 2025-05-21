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

    problem.initializeSubproblem(y);

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

void TabuSearchSolver::mainSearchProcess()
{
    evaluateNeighborhood();

    if (alpha1 * m < k0)
    {
        // Step5
    }
    else
    {
        if (!isTabu(bestFacility))
        {
            executeMove(bestFacility);
        }
        else
        {
            if (aspirationCriterion(computeDeltaZ(bestFacility)))
            {
                executeMove(bestFacility);
                mainSearchProcess();
            }else {
                // cambiar deltaz mejor
                // Step 3
            }
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

    if (zk < z0)
    {
        z0 = zk;
        k0 = k;
    }

    if (z0 < z00)
    {
        z00 = z0;
    }

    if (k - k0 < (alpha1 - alpha2) * m)
    {
        // Step 2
    }
    else
    {
        // Step origen
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

void TabuSearchSolver::evaluateNeighborhood()
{
    int startIndex = ((k - 1) * bar_m) % m;
    for (int i = 0; i < bar_m; ++i)
    {
        int index = (startIndex + i) % m;
        bar_I.push_back(index);
    }

    bestDelta = numeric_limits<int>::max();

    for (int i : bar_I)
    {
        int delta = computeDeltaZ(i);
        if (delta < bestDelta)
        {
            bestDelta = delta;
            bestFacility = i;
        }
    }
}
