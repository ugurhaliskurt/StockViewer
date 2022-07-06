cmake_minimum_required(VERSION 3.12)

project(StockViewer)

find_library(CPPREST_LIB cpprest)
find_package(Boost REQUIRED COMPONENTS filesystem)
find_package(OpenSSL 1.0.0 REQUIRED)

set(LIB_NAME
  ${PROJECT_NAME}_lib
)

add_library( ${LIB_NAME} STATIC
  include/Interval.hpp
  include/yfapi.hpp
  src/Server/RestServer.cpp
)
set_target_properties(${LIB_NAME} PROPERTIES LINKER_LANGUAGE CXX)
set(CMAKE_BUILD_TYPE Debug)
include_directories(include)

add_executable(${PROJECT_NAME}
	src/main.cpp
)

target_link_libraries( ${PROJECT_NAME} PUBLIC
${CPPREST_LIB}
${LIB_NAME}
-lcurl
-lpthread
-lcrypto
OpenSSL::SSL
Boost::filesystem
)
