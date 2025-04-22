#include "Reader/beasley_instance_reader.h"

#include <iostream>

int main()
{
    BeasleyInstanceReader reader;
    std::string filename = "./instances/Beasley/cap41.txt"; // Reemplaza con el nombre de tu archivo Beasley


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
    

    return 0;
}
