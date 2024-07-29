#ifndef TRANSPORT_SOLVER_H
#define TRANSPORT_SOLVER_H

#include <vector>
using namespace std;

class TransportSolver
{
public:
    TransportSolver( vector<vector<int>> &costMatrix,
                     vector<int> supply,
                     vector<int> demand,
                    vector<vector<int>> &assignamentMatrix,
                     int &totalSupply,
                     int &totalDemand);
    TransportSolver();
    
    void solveVAM();  // Método para el algoritmo VAM
    void solveMODI(); // Método para el algoritmo MODI
    void printProblem();

private:
     vector<vector<int>> &costMatrix;
     vector<int> supply;
     vector<int> demand;
     int &totalSupply;
     int &totalDemand;
    vector<vector<int>> &assignamentMatrix;
    int totalCost;

    pair<vector<int>, vector<int>> findDiff(vector<vector<int>>& grid);

    void calculateRowPenality(vector<vector<int>> &c,vector<bool> &fr,vector<bool> &fc,int factories,vector<int> &rp);
    void calculateColPenality(vector<vector<int>> &c,vector<bool> &fr,vector<bool> &fc,int factories,vector<int> &cp);
    int calculateMax(vector<int> v, int n);
    void calculateLocation(vector<vector<int>> &cost, vector<int> &supply, vector<int>&demand, vector<int> &row_penalty,vector<int> &column_penalty,
                        int factories,int warehouses,int &maximum,int &i_iter,int &j_iter,int maxi,vector<bool> &fr, vector<bool> &fc);
    pair<int,int> calculateAllocation(vector<vector<int>> &c, vector<int> &s,vector<int> &d,vector<bool> &fr,vector<bool> &fc,int p,int q);
};
#endif