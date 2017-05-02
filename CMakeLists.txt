cmake_minimum_required(VERSION 2.8)

project(serialization)

include(FindProtobuf)
include(FindFlatBuffers.cmake)

find_package(Protobuf REQUIRED VERSION 2.6)
add_compile_options(-std=c++14 -Wno-deprecated-declarations)
#find_package(FlatBuffers REQUIRED)

include_directories(${CMAKE_CURRENT_BINARY_DIR} ${PROTOBUF_INCLUDE_DIR})

protobuf_generate_cpp(proto_srcs proto_hdrs
    "protobuf.proto")

build_flatbuffers(
    "${CMAKE_CURRENT_SOURCE_DIR}/flatbuffers.fbs"
    ""
    flatbuffers
    ""
    "${CMAKE_CURRENT_BINARY_DIR}"
    ""
    "")

add_definitions(-DBENCHMARK_ENABLE_LTO=1)
add_executable(bench
    "common.h"
    "common.cpp"
    "serialization.h"
    "serialization.cpp"
    "overgeneralized.cpp"
    "protobuf.cpp"
    "protobuf.proto"
    "flatbuffers.fbs"
    "flatbuffers.cpp"
    "${CMAKE_CURRENT_BINARY_DIR}/flatbuffers_generated.h"
    ${proto_srcs}
    ${proto_hdrs}
    "main.cpp")
target_link_libraries(bench benchmark ${PROTOBUF_LIBRARY})
add_dependencies(bench flatbuffers)

FLATBUFFERS_GENERATE_C_HEADERS("flatbuffers.fbs")

