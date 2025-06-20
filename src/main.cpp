#include "TransportProblem/transport_problem.h"
#include "Reader/beasley_instance_reader.h"

#include <iostream>

int main()
{
    BeasleyInstanceReader reader;
    CFLPProblem problem = reader.readInstance("instances/Beasley/cap41.txt");
}
