cmake_minimum_required(VERSION 3.10)  # CMake version check
project(shadow-builder)               # Create project "shadow-builder"
set(CMAKE_CXX_STANDARD 14)            # Enable c++14 standard

list(APPEND LIBSB_SRC 
	src/ClangParser.cpp
	src/CodeParser.cpp
	src/ShadowBuilder.cpp
)

add_definitions(
-D__STDC_LIMIT_MACROS
-D__STDC_CONSTANT_MACROS
)
set_source_files_properties(src/ClangParser.cpp PROPERTIES COMPILE_FLAGS -fno-rtti)

find_package(LLVM REQUIRED CONFIG)
find_package(Clang REQUIRED CONFIG)

include_directories(${LLVM_INCLUDE_DIRS})
add_library(shadow-builder SHARED ${LIBSB_SRC})

target_link_libraries(shadow-builder common tinyxml2)
target_link_libraries(shadow-builder LLVMOption LLVMSupport)

target_link_libraries(shadow-builder 
	tfa
	common
	clangTooling
	clangASTMatchers
	clangBasic
	clangDriver
	clangFrontend
	clangRewriteFrontend
	clangSerialization
	clangStaticAnalyzerFrontend
)

list(APPEND SB_TEST_SRC
	tests/CoderParser_01.cpp
	tests/ShadowBuilder_01.cpp
)

create_tests(shadow-builder "${LIBSB_SRC}" "${SB_TEST_SRC}")

list (APPEND LIB_SB_DEPENDENDIES
	tfa
	common
	LLVMOption
	LLVMSupport
	tinyxml2
	clangTooling
	clangASTMatchers
	clangBasic
	clangDriver
	clangFrontend
	clangRewriteFrontend
	clangSerialization
	clangStaticAnalyzerFrontend
)
create_link_tests(shadow-builder "${LIB_SB_DEPENDENDIES}")
