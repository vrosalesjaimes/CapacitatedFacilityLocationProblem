#include "Reader/beasley_instance_reader.h"
#include "CapacitedFacilityLocationProblem/initializer.h"
#include <iostream>

int main() {
    BeasleyInstanceReader reader;
    std::string filename = "./instances/Beasley/cap41.txt";  // Reemplaza con el nombre de tu archivo Beasley

    try {
        Instance instance = reader.readInstance(filename);

        // Imprimir información de la instancia
        std::cout << "Número de facilidades: " << instance.getNumFacilities() << std::endl;

        // Calcular y mostrar la suma total de la demanda
        int totalDemand = 0;
        const auto& customerDemands = instance.getCustomerDemands();
        for (int demand : customerDemands) {
            totalDemand += demand;
        }
        std::cout << "Suma total de la demanda: " << totalDemand << std::endl;

        // Imprimir el número de clientes
        std::cout << "Número de clientes: " << instance.getNumCustomers() << std::endl;

        // Verificar si la solución inicial es factible
        Initializer initializer(instance);
        Solution initialSolution = initializer.generateInitialSolution();
        bool feasible = initialSolution.isFeasible(instance);
        std::cout << "¿Solución inicial es factible?: " << (feasible ? "Sí" : "No") << std::endl;

        // Imprimir la demanda satisfecha por la solución inicial
        int satisfiedDemand = 0;
        const auto& x = initialSolution.getX();
        for (size_t i = 0; i < initialSolution.getY().size(); ++i) {
            if (initialSolution.getY()[i] == 1) {
                for (size_t j = 0; j < x[i].size(); ++j) {
                    if (x[i][j] == 1) {
                        satisfiedDemand += instance.getCustomerDemands()[j];
                    }
                }
            }
        }
        std::cout << "Demanda satisfecha por la solución inicial: " << satisfiedDemand << std::endl;

        // Imprimir el costo de la solución inicial
        std::cout << "Costo de la solución inicial: " << initialSolution.getCost() << std::endl;

        // Imprimir las facilidades abiertas en la solución inicial
        std::cout << "Facilidades abiertas en la solución inicial:" << std::endl;
        const auto& y = initialSolution.getY();
        int totalCapacity = 0;
        for (size_t i = 0; i < y.size(); ++i) {
            if (y[i] == 1) {
                std::cout << "Facilidad " << i << std::endl;
                totalCapacity += instance.getFacilityCapacities()[i];
            }
        }
        std::cout << "Capacidad total de las facilidades abiertas: " << totalCapacity << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Error al leer la instancia: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
