#pragma once

#include "../CapacitatedFacilityLocationProblem/cflp_problem.h"
#include "../TransportProblem/transport_problem.h"
#include "../TransportProblem/transport_problem.h"
#include "TabuSearch/tabu_search_solver.h"
#include "PLQT/plqt.h"
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <limits>
#include <random>

class TabuSearchSolver
{
public:
    TabuSearchSolver(CFLPProblem &problem);

    void solve();
    std::vector<int> getBestSolution() const;
    double getBestCost() const;

private:
    // Referencia al problema
    CFLPProblem &problem;

    // Parámetros de búsqueda tabú
    int alpha1 = 1;
    int alpha2 = 0.5;
    int C = 10;
    int bar_m = 30;
    int l0_l = 10, l0_u = 15;
    int l1_l = 10, l1_u = 15;
    int l0 = 0, l1 = 0;

    // Estructuras de control
    int m;                   // número de instalaciones
    int n;                   // número de clientes
    std::vector<int> y;      // solución actual
    std::vector<int> y_best; // mejor solución global
    std::vector<int> t;      // tiempo del último cambio
    std::vector<int> h;      // contador de duración
    int k = 1, k0 = 1, c = 1, c0 = 0;
    int z0, zk, z00;
    int m1;
    int bestFacility = -1;           ///< Mejor instalación encontrada
    std::vector<double> priorityP2_; ///< Prioridades de instalaciones para P2
    std::vector<double> priorityP3_; ///< Prioridades de instalaciones para P3

    // Tabú List y PLQT
    PLQT plqt_;

    // Solución de referencia para path relinking
    std::vector<int> targetSolution;

    std::vector<int> I2_; ///< Order of facilities used in initialization.
    std::vector<int> I3_; ///< Order of facilities used in initialization.

    std::vector<int> bar_I;
    int bestDelta = std::numeric_limits<int>::max();

    // Funciones internas
    void initialize();
    void mainSearchProcess();
    void intensification();
    void solutionReconciling(bool useP2);
    void pathRelinking(const std::vector<int> &source, const std::vector<int> &target);
    void diversification();
    void executeMove(int i);
    bool isTabu(int i);
    bool aspirationCriterion(int deltaZ);
    int computeDeltaZ(int i);
    int computeDeltaZ(int i);
    int computeDeltaZ_altering(int i);
    bool isFeasibleToClose(int i);
    bool isInPLQT(const std::vector<int> &sol);
    void addToPLQT(const std::vector<int> &sol);
    int selectByDiver1();
    int selectByDiver2();
    void computePriorities();
    bool isFeasible(const std::vector<int> &solution);
    void evaluateNeighborhood() {}
};
