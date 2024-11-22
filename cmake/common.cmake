# Function to create a library
function(create_library TARGET_NAME TARGET_SRC_DIR)
    file(GLOB TARGET_SOURCES ${TARGET_SRC_DIR}/*.cpp)
    file(GLOB TARGET_HEADERS ${TARGET_SRC_DIR}/*.h)
    add_library(${TARGET_NAME} STATIC ${TARGET_SOURCES})

    # Include directories
    target_include_directories(${TARGET_NAME} PUBLIC 
        ${TARGET_SRC_DIR}
        ${GLOBAL_INCLUDE_DIR}
    )
    foreach(INCLUDE_DIR ${EXTERNAL_INCLUDE_DIRS})
        target_include_directories(${TARGET_NAME} PUBLIC ${INCLUDE_DIR})
    endforeach()

    # Compile options
    target_compile_options(${TARGET_NAME} PRIVATE
        $<$<CONFIG:DEBUG>:-g -O0>
        $<$<CONFIG:RELEASE>:-O3>
    )

    # Link external libraries 
    foreach(LIB ${EXTERNAL_LIBS}) 
        target_link_libraries(${TARGET_NAME} ${LIB}) 
    endforeach()

    # Install headers
    install(FILES ${TARGET_HEADERS} DESTINATION ${GLOBAL_INCLUDE_DIR})

    # Custom command to copy headers 
    add_custom_command(
        TARGET ${TARGET_NAME} POST_BUILD 
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${TARGET_HEADERS} ${GLOBAL_INCLUDE_DIR}/ 
    )

endfunction()

# Function to create an executable
function(create_executable TARGET_NAME TARGET_SRC_DIR)
    file(GLOB TARGET_SOURCES ${TARGET_SRC_DIR}/*.cpp)
    file(GLOB TARGET_HEADERS ${TARGET_SRC_DIR}/*.h)
    add_executable(${TARGET_NAME} ${TARGET_SOURCES})

    # Include directories
    target_include_directories(${TARGET_NAME} PUBLIC 
        ${TARGET_SRC_DIR}
        ${GLOBAL_INCLUDE_DIR}
    )
    foreach(INCLUDE_DIR ${EXTERNAL_INCLUDE_DIRS})
        target_include_directories(${TARGET_NAME} PUBLIC ${INCLUDE_DIR})
    endforeach()

    # Compile options
    target_compile_options(${TARGET_NAME} PRIVATE
        $<$<CONFIG:DEBUG>:-g -O0>
        $<$<CONFIG:RELEASE>:-O3>
    )

    # Link external libraries 
    foreach(LIB ${EXTERNAL_LIBS}) 
        target_link_libraries(${TARGET_NAME} ${LIB}) 
    endforeach()

    # Install headers
    install(FILES ${TARGET_HEADERS} DESTINATION ${GLOBAL_INCLUDE_DIR})

endfunction()
