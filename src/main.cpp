#include "TransportProblem/transport_problem.h"

#include <iostream>

int main()
{
    TransportationProblem problem(
        {5,4,6},
        {2,3,3,5,2},
        {{5,3,4,5,6},
         {2,6,5,3,2},
         {6,4,3,4,4}});

    problem.solveHungarianMethod();
    

    return 0;
}
