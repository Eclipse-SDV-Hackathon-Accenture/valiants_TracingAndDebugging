cmake_minimum_required(VERSION 3.0)

set(CMAKE_FIND_PACKAGE_PREFER_CONFIG ON)


project(tstVehPub)


set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_PREFIX_PATH /home/parallels/.local/)


find_package(eCAL REQUIRED)

find_package(Protobuf REQUIRED)

find_package(opentelemetry-cpp CONFIG REQUIRED)

find_package(gRPC CONFIG REQUIRED)




set(source_files src/val_pub.cpp)


set(protobuf_files

    ${CMAKE_CURRENT_SOURCE_DIR}/../proto_messages/test_message.proto

)



add_executable(${PROJECT_NAME} ${source_files})

target_include_directories(${PROJECT_NAME} PRIVATE ${OPENTELEMETRY_CPP_INCLUDE_DIRS})


PROTOBUF_TARGET_CPP(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/../proto_messages/ ${protobuf_files})


target_link_libraries(${PROJECT_NAME} PRIVATE

  eCAL::core

  protobuf::libprotobuf

  ${OPENTELEMETRY_CPP_LIBRARIES}
  )


install(TARGETS tstVehPub DESTINATION bin)
