cmake_minimum_required(VERSION 3.16)

project(Classical)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)

# Add the path to MCL libraries
set(MCL_INCLUDE_DIRS /usr/local/include/mcl)
set(MCL_LIBRARIES /usr/local/lib/libmcl.a)

# Include MCL headers
include_directories(${MCL_INCLUDE_DIRS})

# Sources
set(SOURCES
    src/kzgcommit.cpp
    src/main.cpp
)

# Target executable
add_executable(kzgcommit ${SOURCES})

# Link MCL library
target_link_libraries(kzgcommit ${MCL_LIBRARIES})

# Optional: Use OpenMP for parallel computing (if needed)
# find_package(OpenMP)
# target_link_libraries(kzgcommit OpenMP::OpenMP_CXX)
