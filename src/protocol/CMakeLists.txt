cmake_minimum_required(VERSION 2.8)
set(TARGE_NAME ${LIBPROTOCOL})
project(${TARGE_NAME})

IF(WIN32)
SET(PROTOC ${CMAKE_CURRENT_SOURCE_DIR}/protoc.exe)
ELSE(WIN32)
SET(PROTOC protoc)
ENDIF(WIN32)

EXECUTE_PROCESS(COMMAND ${PROTOC} --proto_path=${CMAKE_CURRENT_SOURCE_DIR} --cpp_out=${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/msg_info.proto)
EXECUTE_PROCESS(COMMAND ${PROTOC} --proto_path=${CMAKE_CURRENT_SOURCE_DIR} --cpp_out=${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/msg_cmd.proto)
EXECUTE_PROCESS(COMMAND ${PROTOC} --proto_path=${CMAKE_CURRENT_SOURCE_DIR} --cpp_out=${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/gm_msg.proto)
EXECUTE_PROCESS(COMMAND ${PROTOC} --proto_path=${CMAKE_CURRENT_SOURCE_DIR} --cpp_out=${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/game_base.proto)

AUX_SOURCE_DIRECTORY(. TARGET_SRC)

if(WIN32)
    file(GLOB TARGET_HDR *.h)
    add_library(${TARGE_NAME} SHARED ${TARGET_SRC} ${TARGET_HDR})
else(WIN32)
	SET_CCACHE_COMPLER()
    add_library(${TARGE_NAME} SHARED ${TARGET_SRC})
endif(WIN32)
