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
    std::vector<int> y_P2;   // solución de referencia para P2
    std::vector<int> y_P3;   // solución de referencia para P3
    std::vector<int> y_best; // mejor solución global
    std::vector<int> t;      // tiempo del último cambio
    std::vector<int> h;      // contador de duración
    int k = 1, k0 = 1, c = 1, c0 = 0;
    int z0, zk, z00;
    int m1;
    int bestFacility = -1; ///< Mejor instalación encontrada
    int initialIndex;
    int finalIndex;
    std::vector<double> priorityP2_; ///< Prioridades de instalaciones para P2
    std::vector<double> priorityP3_; ///< Prioridades de instalaciones para P3
    std::vector<int> reconcilingY;   ///< Solución de reconciliación
    int currentSupply = 0;           ///< Suministro actual
    int totalDemand_ = 0;            ///< Total demand (sum of all clients).

    // Tabú List y PLQT
    PLQT plqt_;

    // Solución de referencia para path relinking
    std::vector<int> targetSolution;

    std::vector<int> I2_; ///< Order of facilities used in initialization.
    std::vector<int> I3_; ///< Order of facilities used in initialization.

    std::vector<int> bar_I;
    int bestDelta = std::numeric_limits<int>::max();
    double bestDelta_altering = std::numeric_limits<int>::max();
    std::vector<int> deltaZ_values;
    std::vector<double> deltaZ_values_altering;

    // Funciones internas
    void initialize();
    void mainSearchProcess();
    void intensification();
    void solutionReconciling();
    void pathRelinking(const std::vector<int> &source);
    void diversification();
    void executeMove(int i);
    bool isTabu(int i);
    bool aspirationCriterion(int deltaZ);
    int computeDeltaZ(int i);
    int computeDeltaZ_altering(int i);
    void computePriorities();
    bool isFeasibleToClose(int i);
    void evaluateNeighborhood();
    void evaluateNeighborhoodAltering();
    void criterionAltering();
    void determineBestFacility();
    void determineBestFacilityAltering();
    void handleTabuMove();
    void handleTabuMoveAltering();
    void determineNeighborhood();
    void advanceIndex();
    void backIndex();
    int selectMinFrequency(const std::vector<int>& indices);

};
