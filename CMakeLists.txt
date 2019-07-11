cmake_minimum_required(VERSION 3.10)
project(pipeline_processing)

set(CMAKE_CXX_STANDARD 14)
set(PATH_TO_LLVM "${CMAKE_SOURCE_DIR}/ext/")

set(LLVM_DIR "${PATH_TO_LLVM}/lib/cmake/llvm")
set(Clang_DIR "${PATH_TO_LLVM}/lib/cmake/clang")


include_directories(
	${CMAKE_SOURCE_DIR}/ext/include
	${CMAKE_SOURCE_DIR}/common/inc/
	${CMAKE_SOURCE_DIR}/shadow-builder/inc/
	${CMAKE_SOURCE_DIR}/tfa_core/inc/
	)


include(CMakeMacros/CMakeTesting.txt)
# This enables the tets with the options -DENABLE_TEST


option(ENABLE_TESTS "Enable tests" OFF)
if (${ENABLE_TESTS})
	set(GCC_COVERAGE_COMPILE_FLAGS "-g -O0")
	SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
	enable_testing()
endif()

# List of the projects 
add_subdirectory(common)

#This enable the compilation of shadow-builder or/and
#the Trace Framework Abstraction.
#Build Shadow-Builder?
link_directories(${CMAKE_SOURCE_DIR}/ext/lib)

#Build TFA?
option(BUILD_TFA "Build Trace Framework Abstraction" ON)
if(BUILD_TFA) 
	add_subdirectory(tfa_core)
	add_subdirectory(tfa-plugins)
endif(BUILD_TFA)

option(BUILD_SB "Build Shadow-Builder" ON)
if(BUILD_SB) 
	add_subdirectory(shadow-builder)
endif(BUILD_SB)

# Main file
add_executable(shadow-program main.cpp)

target_link_libraries(shadow-program 
			tinyxml2
			common
			tfa
			shadow-builder
		)
