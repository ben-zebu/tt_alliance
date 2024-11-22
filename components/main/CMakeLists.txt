# main/CMakeLists.txt
project(tt_alliance)
create_executable(alliance.exe ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(alliance.exe amath abase)

# Install the executable based on the build type 
# if (CMAKE_BUILD_TYPE STREQUAL "Release") 
#     install(TARGETS alliance DESTINATION ${RELEASE_INSTALL_DIR}) 
# elseif (CMAKE_BUILD_TYPE STREQUAL "Debug") 
#     install(TARGETS alliance DESTINATION ${DEBUG_INSTALL_DIR}) 
# else() 
#     message(FATAL_ERROR "Unsupported build type") 
# endif()

# Define installation directories for release and debug configurations
install(TARGETS alliance.exe
    RUNTIME DESTINATION "$<IF:$<CONFIG:Release>,${RELEASE_INSTALL_DIR},${RELEASE_INSTALL_DIR}>"
)
