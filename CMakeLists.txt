cmake_minimum_required(VERSION 3.2)

project(Sead VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_library(Sead STATIC ${SOURCES})

set_target_properties(Sead PROPERTIES
    VERSION ${PROJECT_VERSION}
    OUTPUT_NAME "Sead"
)

target_include_directories(Sead PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/engine/library/include)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/engine/library/modules)

#target_compile_definitions(Sead PRIVATE cafe)
