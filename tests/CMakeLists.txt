enable_testing()
find_package(GTest REQUIRED)
include(GoogleTest)

add_executable(tests  main.cpp
                      CapacitatedFacilityLocationProblem/instance_test.cpp
                      CapacitatedFacilityLocationProblem/solution_test.cpp
                      Reader/beasley_instance_reader_test.cpp  
                      Reader/holmberg_instance_reader_test.cpp
                      Reader/tbed_instance_reader_test.cpp
                      Reader/yang_instance_reader_test.cpp
                      PLQT/plqt_node_test.cpp
                      PLQT/power_of_two_test.cpp
                      PLQT/plqt_test.cpp
)

target_link_libraries(tests
  CapacityFacilityLocationLib
  GTest::gmock
  GTest::gtest
)
gtest_discover_tests(tests)