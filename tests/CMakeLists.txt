enable_testing()
find_package(GTest REQUIRED)
include(GoogleTest)

add_executable(tests  main.cpp
                      TransportProblem/transport_solver.cpp
)

target_link_libraries(tests
  GTest::gmock
  GTest::gtest
)
gtest_discover_tests(tests)