enable_testing()
find_package(GTest REQUIRED)
include(GoogleTest)

add_executable(tests  main.cpp
                      Reader/beasley_instance_reader_test.cpp  
                      Reader/holmberg_instance_reader_test.cpp
                      Reader/yang_instance_reader_test.cpp
                      TransportProblem/transport_solver.cpp
)

target_link_libraries(tests
  CapacityFacilityLocationLib
  GTest::gmock
  GTest::gtest
)
gtest_discover_tests(tests)