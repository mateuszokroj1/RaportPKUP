cmake_minimum_required(VERSION 3.24)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

find_program(vcpkg NAMES vcpkg REQUIRED)
get_filename_component(vcpkg_dir ${vcpkg} DIRECTORY)

set(toolchain_file "${vcpkg_dir}/scripts/buildsystems/vcpkg.cmake")

if(NOT EXISTS ${toolchain_file})
	message(FATAL_ERROR "Cannot find VCPKG CMake script.")
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
	set(VCPKG_BUILD_TYPE debug)
else()
	set(VCPKG_BUILD_TYPE release)
endif()

set(CMAKE_TOOLCHAIN_FILE ${toolchain_file})
message("Toolchain file: ${toolchain_file}")

option(ENABLE_TESTS "Enable unit tests" OFF)

project(RaportPKUP)

find_package(unofficial-libharu CONFIG REQUIRED)
find_package(rxcpp CONFIG REQUIRED)
find_package(RXQT REQUIRED)
find_package(unofficial-libgit2 CONFIG REQUIRED)

include(config/Version.cmake)
configure_file(config/Version.h.in ${CMAKE_CURRENT_LIST_DIR}/config/Version.h @ONLY)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)
