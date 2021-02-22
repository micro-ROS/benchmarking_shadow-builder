cmake_minimum_required(VERSION 3.10)  # CMake version check
project(plugin_test VERSION 0.1 DESCRIPTION "Empty plugin doing nothing")
set(CMAKE_CXX_STANDARD 14)            # Enable c++14 standard

set(PLUGIN_NAME "plugin_test")

# Needed to get the function to do tests 
include(../../CMakeMacros/CMakeTesting.txt)


# Plugins include folders
include_directories(inc/)

# Plugins source files
list(APPEND TEST_PLUGIN_SRC
	src/plugin_test.cpp
)

# Needed to create a shareable library
add_library(${PLUGIN_NAME} SHARED ${TEST_PLUGIN_SRC})

# Target library that we need to link against
if (${ENABLE_TESTS}) 
	target_link_libraries(${PLUGIN_NAME} common)
endif()

# Needed to create a version of the shared library
set_target_properties(${PLUGIN_NAME} PROPERTIES SOVERSION ${PROJECT_VERSION})
