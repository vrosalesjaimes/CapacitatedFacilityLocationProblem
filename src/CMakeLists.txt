# Añadir el ejecutable
add_executable(CapacitatedFacilityLocationProblem main.cpp
                                                CapacitatedFacilityLocationProblem/initializer.cpp
                                                CapacitatedFacilityLocationProblem/instance.cpp
                                                CapacitatedFacilityLocationProblem/solution.cpp
                                                Reader/beasley_instance_reader.cpp
                                                Reader/holmberg_instance_reader.cpp
                                                Reader/tbed_instance_reader.cpp
                                                Reader/yang_instance_reader.cpp
                                                TransportProblem/transport_problem.cpp
                                                TransportProblem/transport_solver.cpp)

add_library(CapacityFacilityLocationLib STATIC 
                                            CapacitatedFacilityLocationProblem/initializer.cpp
                                            CapacitatedFacilityLocationProblem/instance.cpp
                                            CapacitatedFacilityLocationProblem/solution.cpp
                                            Reader/beasley_instance_reader.cpp
                                            Reader/holmberg_instance_reader.cpp
                                            Reader/tbed_instance_reader.cpp
                                            Reader/yang_instance_reader.cpp
                                            TransportProblem/transport_problem.cpp
                                            TransportProblem/transport_solver.cpp
)

target_include_directories(CapacityFacilityLocationLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../include)