# Define external library include directories and library files 
list(APPEND EXTERNAL_INCLUDE_DIRS 
    ${CMAKE_SOURCE_DIR}/externals/yaml/include 
    # Add more include directories as needed 
) 

list(APPEND EXTERNAL_LIBS 
    ${CMAKE_SOURCE_DIR}/externals/yaml/lib/libyaml-cpp.a
    # Add more library files as needed
)