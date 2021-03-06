project(pokerth_protocol)

find_package(Qt5Core REQUIRED)
find_package(Protobuf REQUIRED)

add_definitions(-DENABLE_IPV6 -DTIXML_USE_STL)
include_directories(
    ${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

if(WIN32)
    add_definitions(-DCURL_STATICLIB -D_WIN32_WINNT=0x0501)
endif(WIN32)

if(PROTOBUF_PROTOC_EXECUTABLE)
    include_directories(${PROTOBUF_INCLUDE_DIRS})
    include_directories(${CMAKE_CURRENT_BINARY_DIR})
    PROTOBUF_GENERATE_CPP(PROTO_CHATCLEANER_SRCS PROTO_CHATCLEANER_HDRS ${CMAKE_SOURCE_DIR}/chatcleaner.proto)
    PROTOBUF_GENERATE_CPP(PROTO_POKERTH_SRCS PROTO_POKERTH_HDRS ${CMAKE_SOURCE_DIR}/pokerth.proto)
endif(PROTOBUF_PROTOC_EXECUTABLE)

if(PROTOBUF_PROTOC_EXECUTABLE)
    set(pokerth_protocol_HDRS
        ${PROTO_POKERTH_HDRS} ${PROTO_CHATCLEANER_HDRS})
    set(pokerth_protocol_SRCS
        ${PROTO_POKERTH_SRCS} ${PROTO_CHATCLEANER_SRCS})
else(PROTOBUF_PROTOC_EXECUTABLE)
    set(pokerth_protocol_HDRS
        ${CMAKE_SOURCE_DIR}/src/third_party/protobuf/pokerth.pb.h
        ${CMAKE_SOURCE_DIR}/src/third_party/protobuf/chatcleaner.pb.h)
    set(pokerth_protocol_SRCS
        ${CMAKE_SOURCE_DIR}/src/third_party/protobuf/pokerth.pb.cc
        ${CMAKE_SOURCE_DIR}/src/third_party/protobuf/chatcleaner.pb.cc)
endif(PROTOBUF_PROTOC_EXECUTABLE)

add_library(pokerth_protocol STATIC
    ${pokerth_protocol_MOC_SRCS}
    ${pokerth_protocol_SRCS}
    ${pokerth_protocol_HDRS}
    )
