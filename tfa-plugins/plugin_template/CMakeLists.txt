cmake_minimum_required(VERSION 3.13)  # CMake version check
project(plugin_template VERSION 0.1 DESCRIPTION "MY Plugin")
set(CMAKE_CXX_STANDARD 14)            # Enable c++14 standard

set(PLUGIN_NAME "plugin_template")

# Needed to get the function to do tests 
include(../../CMakeMacros/CMakeTesting.txt)


# Plugins include folders
include_directories(inc/)

# Plugins source files
list(APPEND TEST_PLUGIN_SRC
	src/plugin_template.cpp
)

# Needed to create a shareable library
add_library(${PLUGIN_NAME} SHARED ${TEST_PLUGIN_SRC})

# Target library that we need to link against
target_link_libraries(tfa)

# Needed to create a version of the shared library
set_target_properties(${PLUGIN_NAME} PROPERTIES SOVERSION ${PROJECT_VERSION})
