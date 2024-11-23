cmake_minimum_required(VERSION 3.10)
project(tt_alliance)

# Define the global include directory 
set(GLOBAL_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)

# Create the global include directory if it doesn't exist 
file(MAKE_DIRECTORY ${GLOBAL_INCLUDE_DIR})

# Define installation directories for the executable 
set(RELEASE_INSTALL_DIR ${CMAKE_SOURCE_DIR}/release) 
set(DEBUG_INSTALL_DIR ${CMAKE_SOURCE_DIR}/debug) 

# Create the installation directories if they don't exist 
file(MAKE_DIRECTORY ${RELEASE_INSTALL_DIR}) 
file(MAKE_DIRECTORY ${DEBUG_INSTALL_DIR}) 

# Add static externals libraries
include(${CMAKE_SOURCE_DIR}/cmake/externals.cmake)

# Include common CMake functions
include(${CMAKE_SOURCE_DIR}/cmake/common.cmake)

# Add subdirectories
add_subdirectory(components/abase)
add_subdirectory(components/amath)
add_subdirectory(components/main)

