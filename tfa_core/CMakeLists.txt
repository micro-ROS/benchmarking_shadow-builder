cmake_minimum_required(VERSION 3.10)  # CMake version check
project(tfa VERSION 0.1 DESCRIPTION "tfa plugin manager and loader")
set(CMAKE_CXX_STANDARD 14) # Enable c++14 standard

list(APPEND LIBTFA_SRC 
	src/ITFACommentListener.cpp
	src/PluginLoader.cpp	
	src/TFABenchMarkingPlatform.cpp
	src/PluginMngr.cpp
	src/TFACore.cpp
)

add_library(tfa SHARED ${LIBTFA_SRC})
set_target_properties(tfa PROPERTIES VERSION ${PROJECT_VERSION})
target_link_libraries(tfa dl)

list(APPEND TFA_TEST_SRC
	tests/plugins_01.cpp
)

create_tests(tfa "${LIBTFA_SRC}" "${TFA_TEST_SRC}")

list (APPEND LIB_TFA_DEPENDENCIES 
	common
	dl
)

create_link_tests(tfa "${LIB_TFA_DEPENDENCIES}")
