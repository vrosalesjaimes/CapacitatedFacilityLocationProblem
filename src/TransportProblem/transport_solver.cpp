#include "TransportProblem/transport_solver.h"

#include <algorithm>
#include <vector>
#include <iostream>
#include <climits>
#include <utility>

using namespace std;

TransportSolver::TransportSolver(vector<vector<int>> &costMatrix,
                                 vector<int> supply,
                                 vector<int> demand,
                                 vector<vector<int>> &assignamentMatrix,
                                 int &totalSupply,
                                 int &totalDemand)
    : costMatrix(costMatrix), supply(supply), demand(demand), assignamentMatrix(assignamentMatrix), totalCost(0), totalSupply(totalSupply), totalDemand(totalDemand)
{
}

const vector<vector<int>> &TransportSolver::getCostMatrix() const
{
    return costMatrix;
}

const vector<int> &TransportSolver::getSupply() const
{
    return supply;
}

void TransportSolver::setSupply(const vector<int> &newSupply)
{
    supply = newSupply;
}

const vector<int> &TransportSolver::getDemand() const
{
    return demand;
}

void TransportSolver::setDemand(const vector<int> &newDemand)
{
    demand = newDemand;
}

int TransportSolver::getTotalSupply() const
{
    return totalSupply;
}

void TransportSolver::setTotalSupply(int newTotalSupply)
{
    totalSupply = newTotalSupply;
}

const vector<vector<int>> &TransportSolver::getAssignamentMatrix() const
{
    return assignamentMatrix;
}

void TransportSolver::setAssignamentMatrix(const vector<vector<int>> &newAssignamentMatrix)
{
    assignamentMatrix = newAssignamentMatrix;
}

int TransportSolver::getTotalDemand() const
{
    return totalDemand;
}

void TransportSolver::setTotalDemand(int newTotalDemand)
{
    totalDemand = newTotalDemand;
}

int TransportSolver::getTotalCost() const
{
    return totalCost;
}

void TransportSolver::setTotalCost(int newTotalCost)
{
    totalCost = newTotalCost;
}

int TransportSolver::VogelsApproximationMethod()
{
    return 0;
}

int TransportSolver::MODI()
{
    return 0;
}

int TransportSolver::PSR()
{
    return 0;
}