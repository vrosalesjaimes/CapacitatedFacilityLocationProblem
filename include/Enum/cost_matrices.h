#ifndef COST_MATRICES_H
#define COST_MATRICES_H

#include <vector>

using namespace std;

class CostMatrices {
    public:
        const vector<vector<int>> PROBLEM_1_BALANCED_VAM = {{10,2,20,11},{12,7,9,20},{4,14,16,18}};

        const vector<vector<int>> PROBLEM_2_BALANCED_VAM = {{5,2,7,3},{3,6,6,1},{6,1,2,4}, {4,3,6,6}};

        const vector<vector<int>> PROBLEM_3_UNBALANCED_VAM = {{8,15,10},{10,12,14},{14,9,15}};

        const vector<vector<int>> PROBLEM_4_MODI = {{17,20,13,12},{15,21,26,25},{15,14,15,17}};

        const vector<vector<int>> PROBLEM_5_MODI = {{13,18,30,8},{55,20,25,40},{30,6,50,10}};

        const vector<vector<int>> PROBLEM_6_MODI = {{11,9,6},{12,14,11},{10,8,10}};

        const vector<vector<int>> PROBLEM_7_PMSJRA = {{13,18,30,8},{55,20,25,40},{30,6,50,10}};

        const vector<vector<int>> PROBLEM_8_PMSJRA = {{11,9,6},{12,14,11},{10,8,10}};
};

#endif // COST_MATRICES_H