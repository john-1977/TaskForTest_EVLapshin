cmake_minimum_required(VERSION 3.20)
project (Flashlight)
message ( STATUS "CMAKE_VERSION:                    - [${CMAKE_VERSION}]" )
set (CMAKE_CONFIGURATION_TYPES "Release;Debug;RelWithDebInfo" CACHE STRING "Доступные конфигурации" )
set (PLATFORM_ROOT_DIRECTORY ${CMAKE_HOME_DIRECTORY})
message (STATUS "Project name:                   - [${CMAKE_PROJECT_NAME}]")
message (STATUS "Project directory:              - [${PLATFORM_ROOT_DIRECTORY}]")
set (PLATFORM_SRC_DIR ${PLATFORM_ROOT_DIRECTORY}/src)

add_subdirectory (src/common)
add_subdirectory(src/client)
add_subdirectory (src/server)
