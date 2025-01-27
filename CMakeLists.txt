cmake_minimum_required(VERSION 3.15)
project(bademesh LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-Wall -Wextra -Werror -pedantic -Wstrict-aliasing
                    -Wno-unused-command-line-argument -Wno-reorder
                    -Wold-style-cast -Wno-variadic-macros -Wno-deprecated-copy)

execute_process(
    COMMAND getconf GNU_LIBC_VERSION
    OUTPUT_VARIABLE GLIBC_VERSION
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

set(LRT_REQUIRED_VERSION "2.17")
if (GLIBC_VERSION VERSION_GREATER_EQUAL LRT_REQUIRED_VERSION)
    list(APPEND EXTRA_LIBS "rt")
endif()

find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
endif()

find_program(MOLD_PATH mold)
if(MOLD_PATH)
    add_link_options(-fuse-ld=mold)
endif()

set(RELEASE_OPT_LEVEL 3 CACHE STRING "Optimization level for release builds")
set(PROFILE "debug" CACHE STRING "Build profile: debug, release, size, or check")

if ("${PROFILE}" STREQUAL "debug")
    add_compile_definitions(MESH_DEBUG)
    add_compile_options(-ggdb -O0)
elseif ("${PROFILE}" STREQUAL "release")
    add_compile_definitions(MESH_RELEASE)
    add_compile_options(-O${RELEASE_OPT_LEVEL} -s -flto -mtune=native -march=native
                        -fgraphite-identity -D_FORTIFY_SOURCE)
elseif ("${PROFILE}" STREQUAL "size")
    add_compile_options(-Oz -s)
elseif ("${PROFILE}" STREQUAL "check")
    add_compile_options(-fanalyzer -fdiagnostics-path-format=separate-events
                        -fno-diagnostics-show-caret -fno-lto -Wno-error)
else()
    message(FATAL_ERROR "Unknown profile '${PROFILE}'. Use debug, release, size, or check.")
endif()

find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    add_compile_options(${OpenMP_CXX_FLAGS})
    add_link_options(${OpenMP_CXX_FLAGS})
else()
    message(FATAL_ERROR "Openmp is required")
endif()

include_directories(src/ lib/cereal/include/)

file(GLOB_RECURSE MESH_SOURCE_FILES src/*.cpp)
add_library(mesh STATIC ${MESH_SOURCE_FILES})
add_executable(main ${MESH_SOURCE_FILES})

target_link_libraries(mesh PRIVATE ${EXTRA_LIBS})
target_link_libraries(main PRIVATE mesh)
