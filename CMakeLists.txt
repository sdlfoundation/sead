cmake_minimum_required(VERSION 3.29.6)

project(Sead VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_library(Sead STATIC ${SOURCES})

set_target_properties(Sead PROPERTIES
    VERSION ${PROJECT_VERSION}
    OUTPUT_NAME "Sead"
)

if (NOT TARGET sdl-utils)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/lib/sdl-utils)
endif()

target_include_directories(Sead PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/lib/sdl-utils/Include)
target_link_libraries(Sead sdl-utils)

target_include_directories(Sead PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/engine/library/include)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/engine/library/modules)

target_compile_definitions(Sead PRIVATE cafe NOT_HAVE_QUADMATH)
