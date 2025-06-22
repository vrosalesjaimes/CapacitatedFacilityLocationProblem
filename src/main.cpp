#include "TransportProblem/transport_problem.h"
#include "Reader/beasley_instance_reader.h"
#include "TabuSearch/tabu_search_solver.h"

#include <iostream>

int main()
{
    BeasleyInstanceReader reader;
    CFLPProblem problem = reader.readInstance("instances/Beasley/cap41.txt");

    TabuSearchSolver solver(problem, 42);
    solver.solve();

    return 0;
}
