#include "TransportProblem/transport_solver.h"

#include <algorithm>
#include <vector>
#include <iostream>
#include <climits>
#include <utility>

using namespace std;

TransportSolver::TransportSolver( vector<vector<int>> &costMatrix,
                                  vector<int> supply,
                                  vector<int> demand,
                                 vector<vector<int>> &assignamentMatrix,
                                  int &totalSupply,
                                  int &totalDemand)
    : costMatrix(costMatrix), supply(supply), demand(demand), assignamentMatrix(assignamentMatrix), totalCost(0), totalSupply(totalSupply), totalDemand(totalDemand)
{
}

void TransportSolver::solveVAM()
{
    if (totalDemand < totalSupply)
    {
        demand.push_back(totalSupply - totalDemand);
        for (int i = 0; i < costMatrix.size(); i++)
        {
            costMatrix[i].push_back(0);
        }
        totalDemand += (totalSupply - totalDemand);
    }
    
    vector<vector<int>> output(supply.size(), vector<int>(demand.size(), 0));
    vector<int> rowPenality(supply.size(), 0);
    vector<int> colPenality(demand.size(), 0);
    vector<bool> flagRow(supply.size(), false);
    vector<bool> flagCol(demand.size(), false);

    std::cout << "Vogel's Approximation Method\n";
    
    vector<int> supplyCopy = supply;
    vector<int> demandCopy = demand;

    int finish = 1;
    int till = supplyCopy.size() + demandCopy.size() - 1;
    vector<vector<int>> cost = costMatrix;

    while (finish++ <= till)
    {
        calculateRowPenality(cost, flagRow, flagCol, supplyCopy.size(), rowPenality);
        calculateColPenality(cost, flagRow, flagCol, demandCopy.size(), colPenality);
        
        int maxRow = *max_element(rowPenality.begin(), rowPenality.end());
        int maxCol = *max_element(colPenality.begin(), colPenality.end());
        int maxi = max(maxRow, maxCol);

        int i_iter = -1, j_iter = -1, maximum=0;

        calculateLocation(cost, supplyCopy, demandCopy, rowPenality, colPenality, supplyCopy.size(), demandCopy.size(), maximum, i_iter, j_iter, maxi, flagRow, flagCol);
        
        /* MODIFYING CERTAIN QUANTITIES FOR FURTHER EVALUATION */ 
        if(maximum == demand[j_iter])
        {
            int temp = supplyCopy[i_iter];
            supplyCopy[i_iter] -= maximum;
            demandCopy[j_iter] = 0;
            flagCol[j_iter] = true;
            colPenality[j_iter] = -1;
            if(maximum == temp)
            {
                supplyCopy[i_iter] = 0;
                demandCopy[j_iter] -= maximum;
                flagRow[i_iter] = true;
                rowPenality[i_iter] = -1;
            }
        }
        else
        {
            supplyCopy[i_iter] = 0;
            demandCopy[j_iter] -= maximum;
            flagRow[i_iter] = true;
            rowPenality[i_iter] = -1;
        }
        
            output[i_iter][j_iter] = maximum;
        
    }

    std::cout << "OUTPUT: \n";
    cout<<"OUTPUT: \n";
    cout<<"INITIAL BASIC FEASIBLE SOLUTION: \n";
    int total_cost = 0,count=0,mini=INT_MAX,p=0,q=0;
    for(int i=0;i<supply.size();i++)
    {
        for(int j=0;j<demand.size();j++)
        {
            if(output[i][j]>=0)
                cout<<"X"<<i<<j<<"= "<<output[i][j]<<"\n";
            if(output[i][j]>=0)
            {
                count++;
                total_cost += output[i][j]*cost[i][j]; // CALCUTING TOTAL COST
            }
            else
            {
                if(mini>cost[i][j] && cost[i][j] > 0) // CALCULATING MINIMUM AMONG ALL UNOCCUPIED CELLS FOR MODI METHOD
                {
                    mini = cost[i][j];
                    p=i;
                    q=j;
                }
            }
        }
    }
    cout<<"BY VAM METHOD OPTIMUM COST  = "<<total_cost<<endl;
}

void TransportSolver::calculateRowPenality(vector<vector<int>> &c, vector<bool> &fr, vector<bool> &fc, int factories, vector<int> &rp)
{
    int n = factories, m = fc.size();

    for (int i = 0; i < n; i++)
    {
        if (fr[i] == true)
        {
            rp[i] = -1;
            continue;
        }
        vector<int> temp;
        for (int j = 0; j < m; j++)
        {
            if (fc[j] == false)
            {
                temp.push_back(c[i][j]);
            }
        }
        sort(temp.begin(), temp.end());
        if (temp.size() >= 2)
        {
            rp[i] = temp[1] - temp[0];
        }
        else if (temp.size() == 1)
        {
            rp[i] = temp[0];
        }
        else
            rp[i] = -1;
    }
}

void TransportSolver::calculateColPenality(vector<vector<int>> &c,vector<bool> &fr,vector<bool> &fc,int warehouses,vector<int> &cp)
{
    int n=warehouses,m=fr.size();
    
    for(int i=0;i<n;i++)
    {
        if(fc[i]==true) 
        {
            cp[i]=-1;
            continue;
        }
        vector<int> temp;
        for(int j=0;j<m;j++)
        {
            if(fr[j]==false)
            {
                temp.push_back(c[j][i]);
            }
        }
        sort(temp.begin(),temp.end());
        if(temp.size() >= 2)
        {
            cp[i]=temp[1]-temp[0];
        }
        else if(temp.size() == 1)
        {
            cp[i]=temp[0];
        }
        else cp[i]=-1;
    }
}

void TransportSolver::calculateLocation(vector<vector<int>> &cost, vector<int> &supply, vector<int>&demand, vector<int> &row_penalty,vector<int> &column_penalty,
                        int factories,int warehouses,int &maximum,int &i_iter,int &j_iter,int maxi,vector<bool> &fr, vector<bool> &fc)
{
    /*FINDING LOCATION OF POINT ALONG WITH HOW MUCH TO ALLOCATE AT THAT POINT */
    for(int i=0;i<factories;i++)
    {
        if(row_penalty[i]==maxi)
        {
            pair<int,int> p = calculateAllocation(cost,supply,demand,fr,fc,i,-1);
            int allo = p.first,index=p.second;
            if(allo>maximum)
            {
                maximum=allo;
                i_iter=i;
                j_iter=index;
            }
            
        }
    }
    
    for(int i=0;i<warehouses;i++)
    {
        if(column_penalty[i]==maxi)
        {
            pair<int,int> p = calculateAllocation(cost,supply,demand,fr,fc,-1,i);
            int allo = p.first,index=p.second;
            if(allo>maximum)
            {
                maximum=allo;
                i_iter=index;
                j_iter=i;
            }
        }
    }
}

pair<int,int> TransportSolver::calculateAllocation(vector<vector<int>> &c, vector<int> &s,vector<int> &d,vector<bool> &fr,vector<bool> &fc,int p,int q)
{
    int n=s.size(),m=d.size(),mini=__INT_MAX__,count=0,maxi=0,ind=-1;
    
    /* FOR EACH ROW AND COLUMN WHERE row_penalty[i] = maximum OR column_penalty[i] = maximum, FINDING 
        ROW OR COLUMN WHERE MAXIMUM ALLOCATION IS POSSIBLE*/
    if(q==-1)
    {
        for(int i=0;i<m;i++)
        {
            if(fc[i]==false)
                mini=min(mini,c[p][i]);
        }
        for(int i=0;i<m;i++)
        {
            if(mini==c[p][i])
            {
                int temp=min(s[p],d[i]);
                if(maxi<temp)
                {
                    maxi=temp;
                    ind=i;
                }
            }
        }
    }
    else
    {
        for(int i=0;i<n;i++)
        {
            if(fr[i]==false)
                mini=min(mini,c[i][q]);
        }
        for(int i=0;i<n;i++)
        {
            if(mini==c[i][q])
            {
                int temp=min(s[i],d[q]);
                if(maxi<temp)
                {
                    maxi=temp;
                    ind=i;
                }
            }
        }
    }
    return {maxi,ind};
}


void TransportSolver::printProblem()
{
    std::cout << "\nCost matrix solver\n";
    for (int i = 0; i < costMatrix.size(); i++)
    {
        for (int j = 0; j < costMatrix[i].size(); j++)
        {
            std::cout << costMatrix[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "\nSupply\n";
    for (int i = 0; i < supply.size(); i++)
    {
        std::cout << supply[i] << " ";
    }
    std::cout<< "\ndemand\n";
    for (int i = 0; i < demand.size(); i++)
    {
        std::cout << demand[i] << " ";
    }
}