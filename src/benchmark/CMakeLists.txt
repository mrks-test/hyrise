include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_executable(
    hyriseMicroBenchmarks

    micro_benchmark_basic_fixture.cpp
    micro_benchmark_basic_fixture.hpp
    micro_benchmark_main.cpp
    micro_benchmark_utils.cpp
    micro_benchmark_utils.hpp
    operators/aggregate_benchmark.cpp
    operators/difference_benchmark.cpp
    operators/join_benchmark.cpp
    operators/join_aggregate_benchmark.cpp
    operators/projection_benchmark.cpp
    operators/union_positions_benchmark.cpp
    operators/sort_benchmark.cpp
    operators/sql_benchmark.cpp
    operators/table_scan_benchmark.cpp
    operators/table_scan_sorted_benchmark.cpp
    operators/union_all_benchmark.cpp
    tpch_data_micro_benchmark.cpp
    tpch_table_generator_benchmark.cpp
)

target_link_libraries(
    hyriseMicroBenchmarks
 
    hyrise
    hyriseBenchmarkLib
    benchmark
)

add_executable(
    hyriseBenchmarkPlayground

    benchmark_playground.cpp
    micro_benchmark_basic_fixture.cpp
    micro_benchmark_basic_fixture.hpp
    micro_benchmark_main.cpp
    micro_benchmark_utils.cpp
    micro_benchmark_utils.hpp
)
target_link_libraries(
    hyriseBenchmarkPlayground

    hyrise
    hyriseBenchmarkLib
    benchmark
)

# General purpose benchmark runner
add_executable(
    hyriseBenchmarkFileBased

    file_based_benchmark.cpp
)

target_link_libraries(
    hyriseBenchmarkFileBased

    hyrise
    hyriseBenchmarkLib
)

# Configure hyriseBenchmarkTPCH
add_executable(hyriseBenchmarkTPCH tpch_benchmark.cpp)

target_link_libraries(
    hyriseBenchmarkTPCH

    hyrise
    hyriseBenchmarkLib
)

# Configure hyriseBenchmarkTPCC
add_executable(hyriseBenchmarkTPCC tpcc_benchmark.cpp)
target_link_libraries(
    hyriseBenchmarkTPCC

    hyrise
    hyriseBenchmarkLib
)

# Configure hyriseBenchmarkTPCDS
add_executable(hyriseBenchmarkTPCDS tpcds_benchmark.cpp)

target_link_libraries(
    hyriseBenchmarkTPCDS

    hyrise
    hyriseBenchmarkLib
)

# Configure hyriseBenchmarkJoinOrder
add_executable(
    hyriseBenchmarkJoinOrder

    join_order_benchmark.cpp
)

target_link_libraries(
    hyriseBenchmarkJoinOrder

    hyrise
    hyriseBenchmarkLib
)
