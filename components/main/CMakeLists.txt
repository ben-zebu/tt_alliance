# main/CMakeLists.txt
project(tt_alliance)
create_executable(alliance.exe ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(alliance.exe amech adata amath abase)

# Define installation directories for release and debug configurations
install(TARGETS alliance.exe
    RUNTIME DESTINATION "$<IF:$<CONFIG:Release>,${RELEASE_INSTALL_DIR},${DEBUG_INSTALL_DIR}>"
)
