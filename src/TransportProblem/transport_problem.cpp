#include "TransportProblem/transport_problem.h"

#include <iostream>
#include <queue>
#include <vector>
#include <numeric>
#include <cstdint>
#include <algorithm>
#include <climits>

using namespace std;

TransportProblem::TransportProblem(const std::vector<std::vector<int>> &costs, const std::vector<int> &customerDemands, const std::vector<int> &facilityCapacities, const std::vector<uint8_t> &facilityStatus)
    : costs(costs), customerDemands(customerDemands), facilityCapacities(facilityCapacities), facilityStatus(facilityStatus)
{
    for(auto& row: costs)
    {
        std::vector<int> temp = row;
        temp.push_back(0);
        costMatrix.push_back(temp);
    }
}



int TransportProblem::getTotalDemand() const {
    return totalDemand;
}

int TransportProblem::getTotalCapacity() const {
    return totalCapacity;
}

const std::vector<int>& TransportProblem::getCustomerDemandsWithDummyDemand() const {
    return customerDemandsWithDummyDemand;
}

const std::vector<std::vector<int>>& TransportProblem::getAssignments() const {
    return assignments;
}

int TransportProblem::getTotalCost() const {
    return totalCost;
}

const std::vector<std::vector<int>>& TransportProblem::getCostMatrix() const {
    return costMatrix;
}

// Setters
void TransportProblem::setAssignamentDemandsOfCustomers(const std::vector<std::vector<int>>& assignamentDemandsOfCustomers) {
    this->assignamentDemandsOfCustomers = assignamentDemandsOfCustomers;
}

void TransportProblem::setModifiedCostMatrix(const std::vector<std::vector<int>>& modifiedCostMatrix) {
    this->modifiedCostMatrix = modifiedCostMatrix;
}

void TransportProblem::setTotalDemand(int totalDemand) {
    this->totalDemand = totalDemand;
}

void TransportProblem::setTotalCapacity(int totalCapacity) {
    this->totalCapacity = totalCapacity;
}

void TransportProblem::addDummyDemand()
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
    int slackDemand = totalCapacity - totalDemand;

    customerDemandsWithDummyDemand = customerDemands;
    customerDemandsWithDummyDemand.push_back(slackDemand);
}

void TransportProblem::initializeModifiedCostMatrix()
{
    modifiedCostMatrix = std::vector<std::vector<int>>(facilityStatus.size(),std::vector<int>(customerDemandsWithDummyDemand.size(),std::numeric_limits<int>::max()));

    for (int i = 0; i < costMatrix.size(); i++)
    {   
        if (facilityStatus[i] == 1)
        {
            for (int j = 0; j < costMatrix[i].size(); j++)
            {
                
                    modifiedCostMatrix[i][j] = costMatrix[i][j];
            }
            modifiedCostMatrix[i][customerDemandsWithDummyDemand.size() - 1] = 0;
        }
    }
}

void TransportProblem::balanceProblem()
{
    addDummyDemand();
    initializeModifiedCostMatrix();
}

void TransportProblem::updateCostMatrix(int index, bool open)
{
    modifiedCostMatrix[index] = open ? costMatrix[index] : std::vector<int>(customerDemandsWithDummyDemand.size(), std::numeric_limits<int>::max());
}

void TransportProblem::updateCapacity(int index, bool open)
{
    totalCapacity = open ? totalCapacity + facilityCapacities[index] : totalCapacity - facilityCapacities[index];
    customerDemandsWithDummyDemand[customerDemandsWithDummyDemand.size() - 1] = totalCapacity - totalDemand;
}

std::vector<int> TransportProblem::findNonZeroIndicesHolgura(){
    std::vector<int> nonZeroIndices;
    for (int i = 0; i < facilityStatus.size(); i++)
    {
        if (facilityStatus[i] == 1 && costMatrix[i][customerDemandsWithDummyDemand.size() - 1] != 0)
        {
            nonZeroIndices.push_back(i);
        }
    }
    return nonZeroIndices;
}

std::vector<int> TransportProblem::findNonZeroIndicesFacility(int index){
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

void TransportProblem::reAssignDemands(int index, bool open){
    if (open)
    {
        for(int i : assignamentDemandsOfCustomers[index]){
            assignamentDemandsOfCustomers[index][i]=0;
        }
    } else {
        auto nonZeroIndices = findNonZeroIndicesFacility(index);
        auto nonZeroIndicesHolgura = findNonZeroIndicesHolgura();

        for(int i: nonZeroIndices){
            int demand = assignamentDemandsOfCustomers[index][i];
            for(int j: nonZeroIndicesHolgura){
                if(demand <= assignamentDemandsOfCustomers[j][customerDemandsWithDummyDemand.size() - 1])
                {
                    assignamentDemandsOfCustomers[j][i] = demand;
                    assignamentDemandsOfCustomers[j][customerDemandsWithDummyDemand.size() - 1] -= demand;
                    break;
                } else {
                    assignamentDemandsOfCustomers[j][i] = assignamentDemandsOfCustomers[j][customerDemandsWithDummyDemand.size() - 1];
                    assignamentDemandsOfCustomers[j][customerDemandsWithDummyDemand.size() - 1] = 0;
                    demand -= assignamentDemandsOfCustomers[j][i];
                }
            }
        }
    }
    
}

void TransportProblem::generateNextInitialSolution(int index, bool open)
{
    updateCostMatrix(index, open);
    updateCapacity(index, open);
    reAssignDemands(index, open);
}


