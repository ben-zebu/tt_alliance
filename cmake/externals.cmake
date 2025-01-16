# Define external library include directories and library files 
list(APPEND EXTERNAL_INCLUDE_DIRS 
    ${CMAKE_SOURCE_DIR}/externals/yaml/include
    ${CMAKE_SOURCE_DIR}/externals/lastus/include 
    # Add more include directories as needed 
) 

list(APPEND EXTERNAL_LIBS 
    ${CMAKE_SOURCE_DIR}/externals/yaml/lib/libyaml-cpp.a
    # ${CMAKE_SOURCE_DIR}/externals/lastus/lib/lastus.a
    # ${CMAKE_SOURCE_DIR}/externals/lastus/lib/libmedcoupling.so
    # ${CMAKE_SOURCE_DIR}/externals/lastus/lib/libmedloader.so
    # ${CMAKE_SOURCE_DIR}/externals/lastus/lib/libinterpkernel.so
    # ${CMAKE_SOURCE_DIR}/externals/lastus/lib/libmed.so.11
    # ${CMAKE_SOURCE_DIR}/externals/lastus/lib/libmedC.so.11
    # Add more library files as needed
)