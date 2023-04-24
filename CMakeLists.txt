cmake_minimum_required(VERSION 3.18 FATAL_ERROR)

project(convex_hull)

# Compillation instructions
# cd build
# cmake -DCMAKE_BUILD_TYPE=Release ..
# make

# Dynamic library
add_library(${PROJECT_NAME}
    INTERFACE
    convex_hull_3d.hpp
)
set_target_properties(${PROJECT_NAME}
    PROPERTIES
    LINKER_LANGUAGE CXX
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)
target_include_directories(${PROJECT_NAME}
    INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
)
add_library(convex_hull::convex_hull
    ALIAS
    ${PROJECT_NAME}
)

# Add test
set(TEST_NAME test_${PROJECT_NAME})
add_executable(${TEST_NAME} test_convex_hull_3d.cpp)
target_link_libraries(${TEST_NAME}
    PRIVATE
    convex_hull::convex_hull
)