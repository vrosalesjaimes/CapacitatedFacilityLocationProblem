# Añadir el ejecutable
add_executable(CapacityFacilityLocationProblem  main.cpp
                                                CapacitedFacilityLocationProblem/instance.cpp
                                                CapacitedFacilityLocationProblem/solution.cpp
                                                Reader/beasley_instance_reader.cpp
                                                Reader/holmberg_instance_reader.cpp
                                                Reader/tbed_instance_reader.cpp
                                                Reader/yang_instance_reader.cpp)
