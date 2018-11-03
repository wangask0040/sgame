cmake_minimum_required (VERSION 2.8)
set(TARGE_NAME ${LIBCONFIG})
project(${TARGE_NAME})

aux_source_directory(. SRC)

if(WIN32)
    file(GLOB HEADER *.h*)
    add_library(${TARGE_NAME} SHARED ${SRC} ${HEADER})
else(WIN32)
	SET_CCACHE_COMPLER()
    add_library(${TARGE_NAME} SHARED ${SRC})
endif(WIN32)