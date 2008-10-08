cmake_minimum_required(VERSION 2.4)
project(linqxx)

enable_testing()

add_executable(linqxx_test linqxx_test.cc)
target_link_libraries(linqxx_test gtest_main)
add_test(LinqxxTest linqxx_test)
