#!/bin/bash

CURDIR=`/bin/pwd`
BASEDIR=$(dirname $0)
ABSPATH=$(readlink -f $0)
ABSDIR=$(dirname $ABSPATH)

DEFAULT_PREFIX_PATH="${ABSDIR}/ext/"
DEFAULT_DL_PATH="${ABSDIR}/ext/dl"
NUM_PROC=$(grep -c '^processor' /proc/cpuinfo)

LIST_DEPENDENCIES_SIMPLE="${ABSDIR}/ext/dl/cxxopts \
			 ${ABSDIR}/ext/dl/libgit2 \
			${ABSDIR}/ext/dl/tinyxml2 \
			${ABSDIR}/ext/dl/z3 \
			${ABSDIR}/ext/dl/ninja" 

LLVM_SOURCE="${DEFAULT_PREFIX_PATH}/dl/llvm/"
CLANG_SOURCE="${DEFAULT_PREFIX_PATH}/dl/clang/"

function function_return()
{
	if [ $1 -ne 0 ]; then
		echo "${2} failed"
		exit 255
	fi
}

function compile_install()
{
	mkdir -p build
	function_return $? "creating-directory"
	
	cd build
	function_return $? "changing-directory"

	cmake $@ -DCMAKE_INSTALL_PREFIX=$DEFAULT_PREFIX_PATH -DCMAKE_PREFIX_PATH=$DEFAULT_PREFIX_PATH .. 
	function_return $? "cmake"

	make -j$NUM_PROC
	function_return $? "make"

	make install
	function_return $? "make install"
}


function compile_install_parser()
{

	rm -rf  build

	mkdir -p build
	function_return $? "creating-directory"
	
	cd build
	function_return $? "changing-directory"

	cmake $@ -DCMAKE_INSTALL_PREFIX=$DEFAULT_PREFIX_PATH -DCMAKE_PREFIX_PATH=$DEFAULT_PREFIX_PATH .. 
	function_return $? "cmake"

	${ABSDIR}/ext/bin/ninja
	function_return $? "ninja"

	${ABSDIR}/ext/bin/ninja install
	function_return $? "ninja_install"
}

function get_and_compile_ext_souces()
{
	for EXT_SOURCE in $LIST_DEPENDENCIES_SIMPLE
	do
		cd $EXT_SOURCE
		function_return $? "changing-directory"
		compile_install ""
	done
}

function get_and_compile_parser()
{


	local ADDITIONAL_LLVM=" -G Ninja -DLLVM_TARGETS_TO_BUILD=X86 \
	       	-DLLVM_TARGET_ARCH=host  -DCMAKE_BUILD_TYPE=MinSizeRel \
		-DLLVM_BUILD_EXAMPLES=OFF -DLLVM_BUILD_TOOLS=NO \
		-DLLVM_BUILD_LLVM_DYLIB=YES -DLLVM_USE_SPLIT_DWARF=ON \
		-DCMAKE_MAKE_PROGRAM=${ABSDIR}/ext/bin/ninja \
		-DLLVM_PARALLEL_LINK_JOBS=2" 

	cd $LLVM_SOURCE
	compile_install_parser ${ADDITIONAL_LLVM}
	cd -

	local ADDITIONAL_CLANG=" -G Ninja -DCLANG_BUILD_EXAMPLES=OFF \
		-DCMAKE_MAKE_PROGRAM=${ABSDIR}/ext/bin/ninja \
		-DCLANG_TOOL_CLANG_CHECK_BUILD=OFF \
		-DCLANG_TOOL_CLANG_DIFF_BUILD=OFF \
		-DCLANG_ENABLE_PROTO_FUZZER=OFF \
		-DCLANG_TOOL_CLANG_FUZZER_BUILD=OFF \
		-DCLANG_TOOL_CLANG_OFFLOAD_BUNDLER_BUILD=OFF \
		-DCLANG_TOOL_CLANG_OFFLOAD_WRAPPER_BUILD=OFF \
		-DCLANG_TOOL_C_ARCMT_TEST_BUILD=OFF\
		-DCLANG_TOOL_C_INDEX_TEST_BUILD=OFF \
		-DCLANG_TOOL_DIAGTOOL_BUILD=OFF \		
		-DCLANG_TOOL_ARCMT_TEST_BUILD=OFF \
		-DCLANG_TOOL_CLANG_FORMAT_BUILD=OFF \
		-DLLVM_BUILD_EXAMPLES=OFF -DLLVM_BUILD_TOOLS=NO \
		-DLLVM_PARALLEL_LINK_JOBS=3 \
		"		
		
	cd $CLANG_SOURCE
	compile_install_parser  ${ADDITIONAL_CLANG}
	cd -

}

git submodule init
function_return $? "submodule init"

git submodule update
function_return $? "submodule update"

get_and_compile_ext_souces
get_and_compile_parser
