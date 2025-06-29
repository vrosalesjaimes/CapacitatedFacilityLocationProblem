#include "util/drop_lo_greedy_approximator.h"
#include <algorithm>
#include <limits>

DropLOGreedyApproximator::DropLOGreedyApproximator(const CFLPProblem& problem)
    : problem(problem)
{}

double DropLOGreedyApproximator::computeDelta(
    int iPrime,
    const std::vector<int>& y,
    const std::vector<std::vector<int>>& x,
    const std::vector<int>& slackSupply) const
{
    int facilityCount = problem.getCapacities().size();
    int clientCount = problem.getDemands().size();

    // 1. Identify I_S: open facilities ≠ i′ with positive slack
    std::vector<int> IS;
    for (int i = 0; i < facilityCount; ++i)
    {
        if (i != iPrime && y[i] == 1 && slackSupply[i] > 0.0)
        {
            IS.push_back(i);
        }
    }

    // 2. Identify J_D: clients currently served by i′
    std::vector<int> JD;
    std::unordered_map<int, double> demand;
    for (int j = 0; j < clientCount; ++j)
    {
        double flow = x[iPrime][j];
        if (flow > 0.0)
        {
            JD.push_back(j);
            demand[j] = flow;
        }
    }

    if (IS.empty() || JD.empty())
    {
        return 0.0; // Nothing to reassign, no cost increase
    }

    // 3. Build arcs with reduced costs
    struct Arc
    {
        int i;
        int j;
        double reducedCost;
    };

    std::vector<Arc> arcs;
    for (int i : IS)
    {
        for (int j : JD)
        {
            double cij = problem.getCostMatrix()[i][j];
            double ciPrimej = problem.getCostMatrix()[iPrime][j];
            double reducedCost = cij - ciPrimej;

            arcs.push_back({i, j, reducedCost});
        }
    }

    // 4. Sort arcs by reduced cost ascending
    std::sort(arcs.begin(), arcs.end(), [](const Arc& a, const Arc& b) {
        return a.reducedCost < b.reducedCost;
    });

    // 5. Track remaining slack and demand
    std::unordered_map<int, double> availableSlack;
    for (int i : IS)
    {
        availableSlack[i] = slackSupply[i];
    }

    std::unordered_map<int, double> remainingDemand = demand;

    // 6. Greedy assignment
    double totalCost = 0.0;
    for (const Arc& arc : arcs)
    {
        double supplyAvailable = availableSlack[arc.i];
        double demandAmount = remainingDemand[arc.j];

        if (supplyAvailable <= 0.0 || demandAmount <= 0.0)
            continue;

        double assigned = std::min(supplyAvailable, demandAmount);
        totalCost += arc.reducedCost * assigned;

        availableSlack[arc.i] -= assigned;
        remainingDemand[arc.j] -= assigned;
    }

    // 7. Check for infeasibility (unassigned demand)
    for (const auto& entry : remainingDemand)
    {
        if (entry.second > 1e-6)
        {
            return std::numeric_limits<double>::max(); // Infeasible
        }
    }

    return totalCost;
}
