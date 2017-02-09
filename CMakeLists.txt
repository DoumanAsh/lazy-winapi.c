project(lazy_winapi C)

cmake_minimum_required(VERSION 3.0)

set(MODULE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/.cmake/Modules")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${MODULE_DIR})
include(LazyUtils)

set_gcc_flags_if("-std=c99 -Wall -Wextra")
# For MinGW we assume that minimum runtime is Vista
set_gcc_flags_if("-D_WIN32_WINNT=0x0600")

file(GLOB lazy_winapi_SRC "src/**/*.c")

option(BUILD_SHARED "Build shared library" OFF)

if(BUILD_SHARED)
    add_library(lazy_winapi SHARED ${lazy_winapi_SRC})
else()
    add_library(lazy_winapi STATIC ${lazy_winapi_SRC})
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
###########################
# Unit tests
##########################
option(UNIT_TESTING "Build unit tests" OFF)
if (UNIT_TESTING)
    set(3PP_PREFIX ${CMAKE_BINARY_DIR}/3pp)
    include(ExternalProject)
    ExternalProject_Add(Criterion
        GIT_REPOSITORY git@github.com:Snaipe/Criterion.git
        GIT_TAG master

        PREFIX ${3PP_PREFIX}
        INSTALL_COMMAND ""
    )

    ExternalProject_Add(Mimick
        URL https://github.com/diacritic/Mimick/archive/master.zip

        PREFIX ${3PP_PREFIX}
        INSTALL_COMMAND ""
    )

    set_gcc_flags_if("-g -O0 -fPIC -fprofile-arcs -ftest-coverage")
    file(GLOB test_SRC "test/*.c")

    add_executable(ut "${test_SRC}")

    include_directories(src)
    target_link_libraries(ut lazy_winapi)

    add_dependencies(ut Criterion)
    include_directories(SYSTEM ${3PP_PREFIX}/src/Criterion/include)
    ExternalProject_Get_Property(Criterion binary_dir)
    link_directories(${binary_dir})
    target_link_libraries(ut libcriterion)

    add_dependencies(ut Mimick)
    ExternalProject_Get_Property(Mimick binary_dir)
    link_directories(${binary_dir})
    include_directories(SYSTEM ${3PP_PREFIX}/src/Mimick/include)
    target_link_libraries(ut mimick)

    enable_testing()
    add_test(NAME    all
             COMMAND ut -j1)
endif()

###########################
# cppcheck
##########################
add_custom_target(
    cppcheck
    COMMAND cppcheck
    --std=c99
    --enable=all
    --suppress=unusedFunction:*:*
    ${lazy_winapi_SRC}
)
