# Añadir el ejecutable
add_executable(CapacitatedFacilityLocationProblem main.cpp
                                                CapacitatedFacilityLocationProblem/cflp_problem.cpp
                                                Reader/beasley_instance_reader.cpp
                                                PLQT/plqt_node.cpp
                                                PLQT/power_of_two.cpp
                                                PLQT/plqt.cpp
                                                ContinuousKnapsackProblem/continuous_item.cpp
                                                ContinuousKnapsackProblem/continuous_knapsack.cpp
                                                TransportProblem/transport_problem.cpp
                                                TransportProblem/cflp_transport_problem.cpp
                                                
)

add_library(CapacityFacilityLocationLib STATIC
                                            CapacitatedFacilityLocationProblem/cflp_problem.cpp
                                            Reader/beasley_instance_reader.cpp
                                            PLQT/plqt_node.cpp
                                            PLQT/power_of_two.cpp
                                            PLQT/plqt.cpp
                                            ContinuousKnapsackProblem/continuous_item.cpp
                                            ContinuousKnapsackProblem/continuous_knapsack.cpp
                                            TransportProblem/transport_problem.cpp
                                            TransportProblem/cflp_transport_problem.cpp
)

target_include_directories(CapacityFacilityLocationLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../include)