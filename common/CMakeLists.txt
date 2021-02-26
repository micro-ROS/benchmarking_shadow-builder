cmake_minimum_required(VERSION 3.10)  # CMake version check
project(common VERSION 0.1 DESCRIPTION "Common tools needed")
set(CMAKE_CXX_STANDARD 14) # Enable c++14 standard

set(BASE_CFG_SB_XML_CONF "${CMAKE_SOURCE_DIR}/res/sb-res/bcf.xml")
set(BASE_CFG_TFA_XML_CONF "${CMAKE_SOURCE_DIR}/res/tfa-res/tfa.xml")
set(BASE_CFG_TFA_PLUGIN_DEFAULT_FOLDER "${CMAKE_BINARY_DIR}/tfa-plugins/")

set(TEST_EX_SIMPLE_STUPID_EXAMPLE "${CMAKE_SOURCE_DIR}/examples/example_stupid_main/")


configure_file(inc/common/TestsConfig.h.in ${CMAKE_CURRENT_SOURCE_DIR}/inc/common/TestsConfig.h)
configure_file(inc/common/BaseConfig.h.in ${CMAKE_CURRENT_SOURCE_DIR}/inc/common/BaseConfig.h)

configure_file(${BASE_CFG_TFA_XML_CONF}.in ${BASE_CFG_TFA_XML_CONF})
configure_file(${BASE_CFG_SB_XML_CONF}.in ${BASE_CFG_SB_XML_CONF})

list(APPEND LIBCOMMON_SRC 
	src/Status.cpp
	src/FileSystem.cpp
	src/CommIPC.cpp
	src/StringMan.cpp
)

add_library(common SHARED ${LIBCOMMON_SRC})
set_target_properties(common PROPERTIES VERSION ${PROJECT_VERSION})
target_link_libraries(common zmq)

list(APPEND COMMON_TEST_SRC
	tests/client_server_01.cpp
	tests/status_01.cpp
	tests/files_01.cpp
	tests/string_01.cpp
)

create_tests(common "${LIBCOMMON_SRC}" "${COMMON_TEST_SRC}")

list (APPEND LIB_COMMON_DEPENDENCIES 
	common
	pthread
	zmq
	)

create_link_tests(common "${LIB_COMMON_DEPENDENCIES}")
