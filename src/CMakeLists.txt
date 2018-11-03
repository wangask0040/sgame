cmake_minimum_required (VERSION 2.8)
project(server)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wno-unused -Wno-unused-but-set-variable -Wno-unused-result -g -pipe -fPIC -std=c++11")
include_directories(${PROJECT_SOURCE_DIR})

set(LIBCOMMON "common")
set(LIBPROTOCOL "protocol")
set(LIBCONFIG "config")

#自己源代码中的库
set(TOTALLIB ${LIBCOMMON} ${LIBPROTOCOL} ${LIBCONFIG})
#第三方的库
set(TOTALEXLIB pthread protobuf event_extra event_core glog zmq hiredis)

if(WIN32)
	include_directories("${PROJECT_SOURCE_DIR}/../win32")
else(WIN32)
	add_definitions(-Wextra -Wpointer-arith -Wcast-qual -Wunreachable-code -Wredundant-decls)

	#该选项的作用是在链接的库列表中，无论执行程序有没有用到库的内容，统统链接进来
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--no-as-needed")
	
	#定义使用ccache的宏
	EXECUTE_PROCESS(COMMAND which ccache RESULT_VARIABLE WHICHCCACHE)
	MACRO(SET_CCACHE_COMPLER)
		IF (${WHICHCCACHE} EQUAL 0)
		SET(CMAKE_CXX_COMPILER "ccache")
		SET(CMAKE_CXX_COMPILER_ARG1 "c++")
		ENDIF(${WHICHCCACHE} EQUAL 0)
	ENDMACRO(SET_CCACHE_COMPLER)
endif(WIN32)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(common)
add_subdirectory(config)
add_subdirectory(protocol)
add_subdirectory(logicsrv)
add_subdirectory(gamesrv)
add_subdirectory(gatesrv)
add_subdirectory(routersrv)
add_subdirectory(gmsrv)


