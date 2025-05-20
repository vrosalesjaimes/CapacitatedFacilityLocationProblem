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

    for (int i : I3_) {
        y[i] = 1;
        total_capacity += a[i];
        if (total_capacity >= total_demand)
            break;
    }

    problem.initializeSubproblem(y);

    z0 = problem.getCurrentCost();
    z00 = z0;

    y_best = y;
    m1 = std::count(y.begin(), y.end(), 1);
    resetTabuLengths();
    plqt_ = PLQT(m, new PLQTNode(y));
}

void TabuSearchSolver::mainSearchProcess()
{
    while (true)
    {
        // 1. Generar todos los movimientos candidatos
        vector<int> candidates;
        for (int i = 0; i < m; ++i)
        {
            if (!isTabu(i) || aspirationCriterion(computeDeltaZ(i), z0))
            {
                candidates.push_back(i);
            }
        }

        if (candidates.empty())
            break;

        // 2. Elegir el mejor movimiento
        int bestMove = -1;
        int bestDelta = numeric_limits<int>::max();

        for (int i : candidates)
        {
            int delta = computeDeltaZ(i);
            if (delta < bestDelta)
            {
                bestDelta = delta;
                bestMove = i;
            }
        }

        // 3. Ejecutar el movimiento
        if (bestMove != -1)
        {
            executeMove(bestMove);
            c++;
            if (/* criterio de parada */ false)
                break;

            if (c >= C)
            {
                intensification();
                c = 0;
            }
        }
    }
}

void TabuSearchSolver::executeMove(int i)
{
    // Alternar apertura/cierre
    y[i] = 1 - y[i];

    // TODO: Recalcular asignación con el nuevo y usando CFLPTransportSubproblem o TransportationProblem
    // y actualizar z0 con el nuevo costo total

    if (z0 < z00)
    {
        z00 = z0;
        y_best = y;
        k0 = k;
    }

    t[i] = k;
    k++;
    addToPLQT(y);
}

bool TabuSearchSolver::isTabu(int i)
{
    return (k - t[i]) <= h[i];
}

bool TabuSearchSolver::aspirationCriterion(int deltaZ, int currentZ)
{
    return (currentZ + deltaZ) < z00;
}

int TabuSearchSolver::computeDeltaZ(int i)
{
    // 1. Si la solución ya ha sido visitada, retorna infinito

    std::vector<int> currentPartition = y;
    currentPartition[i] = 1 - currentPartition[i]; // Alternar apertura/cierre

    if (plqt_.search(currentPartition) != nullptr)
    {
        return std::numeric_limits<int>::max(); // cota infinita
    }

    // 2. Caso 1: Apertura de instalación (ADD-LO)
    if (!y[i])
    {
        // (a) Crear lista de ContinuousItem con (c_ij - c_i'j, x_ij) para cada cliente j donde c_i'j < c_ij
        std::vector<ContinuousItem> items;
        int remainingCapacity = problem.getCapacities()[i];
        for (size_t j = 0; j < problem.getDemands().size(); ++j)
        {
            int bestFacility = -1;
            int currentCost = std::numeric_limits<int>::max();
            int currentAssignment = 0;

            std::vector<std::vector<int>> currentAssignment_ = problem.getSubproblem().getAssignmentMatrix();

            // Encuentra instalación abierta actualmente asignada a j
            for (size_t k = 0; k < y.size(); ++k)
            {
                if (y[k] && currentAssignment_[k][j] > 0)
                {
                    bestFacility = k;
                    currentCost = problem.getCostMatrix()[k][j];
                    currentAssignment = currentAssignment_[k][j];
                    break;
                }
            }

            if (bestFacility != -1 && problem.getCostMatrix()[i][j] < currentCost)
            {
                double valueIndex = problem.getCostMatrix()[i][j] - currentCost;
                double weight = currentAssignment;
                items.emplace_back(valueIndex, weight);
            }
        }

        // (b) Resolver Continuous Knapsack
        ContinuousKnapsack knapsack(remainingCapacity, items);
        double totalDelta = 0.0;
        for (const auto &solution : knapsack.solve())
        {
            int itemIndex = solution.first;
            double quantityTaken = solution.second;
            totalDelta += quantityTaken * items[itemIndex].getValueIndex();
        }

        return static_cast<int>(std::round(totalDelta + problem.getOpeningCosts()[i]));
    }

    // 3. Caso 2: Cierre de instalación (DROP-LO)
    else
    {
        std::vector<std::tuple<int, int, int>> candidateMoves; // (i, j, deltaCost)
        std::unordered_map<int, int> availableSupply;
        std::vector<int> affectedClients;

        std::vector<std::vector<int>> currentAssignment_ = problem.getSubproblem().getAssignmentMatrix();

        // (a) Identifica clientes atendidos por i
        for (size_t j = 0; j < problem.getDemands().size(); ++j)
        {
            if (currentAssignment_[i][j] > 0)
            {
                affectedClients.push_back(j);
            }
        }

        // (b) Identifica otras instalaciones abiertas con capacidad libre
        for (size_t k = 0; k < y.size(); ++k)
        {
            if (k != i && y[k])
            {
                int usedCapacity = std::accumulate(currentAssignment_[k].begin(), currentAssignment_[k].end(), 0);
                int freeCapacity = problem.getCapacities()[k] - usedCapacity;
                if (freeCapacity > 0)
                {
                    availableSupply[k] = freeCapacity;
                }
            }
        }

        // (c) Construye lista de posibles asignaciones (greedy)
        for (int j : affectedClients)
        {
            int demandToAssign = currentAssignment_[i][j];
            for (const auto &entry : availableSupply)
            {
                int k = entry.first;
                int freeCap = entry.second;

                if (freeCap == 0)
                    continue;

                int assignQty = std::min(freeCap, demandToAssign);
                int deltaCost = problem.getCostMatrix()[k][j] - problem.getCostMatrix()[i][j];
                candidateMoves.emplace_back(k, j, deltaCost);
                availableSupply[k] -= assignQty;
                demandToAssign -= assignQty;

                if (demandToAssign == 0)
                    break;
            }
        }

        // (d) Ordena y suma costos (greedy)
        std::sort(candidateMoves.begin(), candidateMoves.end(),
                  [](const auto &a, const auto &b)
                  { return std::get<2>(a) < std::get<2>(b); });

        int totalDelta = 0;
        for (const auto &[k, j, delta] : candidateMoves)
        {
            int amount = currentAssignment_[i][j]; // cuánta demanda hay que mover
            totalDelta += delta * amount;
        }

        return totalDelta - problem.getOpeningCosts()[i];
    }
}

