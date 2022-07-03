cmake_minimum_required(VERSION 3.12)

project(StockViewer)

set(LIB_NAME
  ${PROJECT_NAME}_lib
)

add_library( ${LIB_NAME} STATIC
  include/Interval.hpp
  include/yfapi.hpp 
)
target_compile_features(${LIB_NAME} PRIVATE cxx_std_17)
set_target_properties(${LIB_NAME} PROPERTIES LINKER_LANGUAGE CXX)
set(CMAKE_BUILD_TYPE Debug)
include_directories(include)

add_executable(${PROJECT_NAME}
	src/main.cpp
)

target_link_libraries( ${PROJECT_NAME} PUBLIC
${LIB_NAME}
-lcurl
)
