#include "TransportProblem/transport_problem.h"

#include <iostream>
#include <queue>
#include <vector>
#include <numeric>
#include <cstdint>
#include <algorithm>
#include <climits>

using namespace std;

TransportProblem::TransportProblem(std::vector<std::vector<int>> &costMatrix, std::vector<int> &customerDemands, std::vector<int> &facilityCapacities, std::vector<uint8_t> &facilityStatus)
    : costMatrix(costMatrix), customerDemands(customerDemands), facilityCapacities(facilityCapacities), facilityStatus(facilityStatus)
{
    totalCapacity = 0;

    for (int i = 0; i < facilityCapacities.size(); i++)
    {
        if (facilityStatus[i] == 1)
        {
            totalCapacity += facilityCapacities[i];
        }
    }

    totalDemand = accumulate(customerDemands.begin(), customerDemands.end(), 0);
    initializeModifiedCostMatrix();
    initializeAssignamentDemandsOfCustomers();
}

void TransportProblem::initializeModifiedCostMatrix()
{
    modifiedCostMatrix = std::vector<std::vector<int>>(facilityStatus.size(), std::vector<int>(customerDemands.size(), std::numeric_limits<int>::max()));

    for (int i = 0; i < costMatrix.size(); i++)
    {
        if (facilityStatus[i] == 1)
        {
            modifiedCostMatrix[i] = costMatrix[i];
        }
    }
}

void TransportProblem::updateCostMatrix(int index, bool open)
{
    modifiedCostMatrix[index] = open ? costMatrix[index] : std::vector<int>(customerDemands.size(), std::numeric_limits<int>::max());
}

void TransportProblem::updateCapacity(int index, bool open)
{
    totalCapacity = open ? totalCapacity + facilityCapacities[index] : totalCapacity - facilityCapacities[index];
}

std::vector<int> TransportProblem::findNonZeroIndicesFacility(int index)
{
    std::vector<int> nonZeroIndices;
    for (int i = 0; i < customerDemands.size(); i++)
    {
        if (facilityStatus[index] == 1 && costMatrix[index][i] != 0)
        {
            nonZeroIndices.push_back(i);
        }
    }
    return nonZeroIndices;
}

void TransportProblem::reAssignDemands(int index, bool open, vector<int> dummyAssignament)
{
    if (open)
    {
        for (int i : assignamentDemandsOfCustomers[index])
        {
            assignamentDemandsOfCustomers[index][i] = 0;
        }
    }
    else
    {
        auto nonZeroIndices = findNonZeroIndicesFacility(index);

        for (int i : nonZeroIndices)
        {
            int demand = assignamentDemandsOfCustomers[index][i];
            for (int j : dummyAssignament)
            {
                int restCapacity = calculateRestCapacity(j);
                if (demand <= restCapacity)
                {
                    assignamentDemandsOfCustomers[j][i] = demand;
                    break;
                }
                else
                {
                    assignamentDemandsOfCustomers[j][i] = restCapacity;
                    demand -= restCapacity;
                }
            }
        }
    }
}

int TransportProblem::calculateRestCapacity(int index)
{
    int restCapacity = 0;
    for (int i = 0; i < customerDemands.size(); i++)
    {
        restCapacity += assignamentDemandsOfCustomers[index][i];
    }

    return facilityCapacities[index] - restCapacity;
}

void TransportProblem::generateNextInitialSolution(int index, bool open, vector<int> dummyAssignamentIndex)
{
    updateCostMatrix(index, open);
    updateCapacity(index, open);
    reAssignDemands(index, open, dummyAssignamentIndex);
}

void TransportProblem::printProblem()
{
    std::cout << "Cost matrix: " << std::endl;
    for (const auto &row : modifiedCostMatrix)
    {
        for (int cost : row)
        {
            std::cout << cost << " ";
        }
        std::cout << std::endl;
    }

    std::cout << "Customer demands: ";
    for (int demand : customerDemands)
    {
        std::cout << demand << " ";
    }
    std::cout << std::endl;

    std::cout << "Facility capacities: ";
    for (int capacity : facilityCapacities)
    {
        std::cout << capacity << " ";
    }
    std::cout << std::endl;

    std::cout << "Facility status: ";
    for (uint8_t status : facilityStatus)
    {
        std::cout << static_cast<int>(status) << " ";
    }
    std::cout << std::endl;

    std::cout << "Assignaments: " << std::endl;
    for (int i = 0; i < assignamentDemandsOfCustomers.size(); i++)
    {
        for (int j = 0; j < assignamentDemandsOfCustomers[i].size(); j++)
        {
            std::cout << assignamentDemandsOfCustomers[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

void TransportProblem::initializeAssignamentDemandsOfCustomers()
{
    assignamentDemandsOfCustomers = std::vector<std::vector<int>>(modifiedCostMatrix.size(), std::vector<int>(modifiedCostMatrix[0].size(), 0));
}