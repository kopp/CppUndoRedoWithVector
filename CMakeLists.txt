cmake_minimum_required (VERSION 3.0.0)
project (undo_redo_test)




# See https://cmake.org/cmake/help/v3.0/module/FindGTest.html for how to use google test in cmake.
enable_testing()
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(undo_redo_stack_ut undo_redo_stack_ut.cpp undo_redo_stack_vector.cpp)
target_link_libraries(undo_redo_stack_ut ${GTEST_BOTH_LIBRARIES})

add_test(AllTestsInThisProject undo_redo_stack_ut)


