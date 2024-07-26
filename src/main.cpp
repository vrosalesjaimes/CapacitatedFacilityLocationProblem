#include "Reader/beasley_instance_reader.h"
#include "CapacitatedFacilityLocationProblem/initializer.h"
#include "TransportProblem/transport_problem.h"
#include <iostream>

int main()
{
    BeasleyInstanceReader reader;
    std::string filename = "./instances/Beasley/cap41.txt"; // Reemplaza con el nombre de tu archivo Beasley

    try
    {
        Instance instance = reader.readInstance(filename);

        // Imprimir información de la instancia
        std::cout << "Número de facilidades: " << instance.getNumFacilities() << std::endl;

        // Calcular y mostrar la suma total de la demanda
        int totalDemand = 0;
        const auto &customerDemands = instance.getCustomerDemands();
        for (int demand : customerDemands)
        {
            totalDemand += demand;
        }
        std::cout << "Suma total de la demanda: " << totalDemand << std::endl;

        // Imprimir el número de clientes
        std::cout << "Número de clientes: " << instance.getNumCustomers() << std::endl;

        // Verificar si la solución inicial es factible
        Initializer initializer(instance);
        Solution initialSolution = initializer.generateInitialSolution();
        bool feasible = initialSolution.isFeasible(instance);

        std::vector<std::vector<int>> costMatrix = {
            {8,15,10,0},
            {10,12,14,0},
            {14,9,15,0}};

        std::vector<int> supply = {35,50,40};
        std::vector<int> demand = {45,20,30,30};
        std::vector<uint8_t> facilityStatus = {1, 1, 1, 1};
        TransportProblem transportProblem(costMatrix, demand, supply, facilityStatus);
        transportProblem.VogelApproximationMethod();
        transportProblem.printResult();
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error al leer la instancia: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
