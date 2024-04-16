cmake_minimum_required(VERSION 3.24)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

if(NOT DEFINED DISABLE_PRECOMMIT)
  include(PreCommitInstall)
  precommitinstall()
endif()

find_program(vcpkg NAMES vcpkg REQUIRED)
get_filename_component(vcpkg_dir ${vcpkg} DIRECTORY)

set(toolchain_file "${vcpkg_dir}/scripts/buildsystems/vcpkg.cmake")

if(NOT EXISTS ${toolchain_file})
  message(FATAL_ERROR "Cannot find VCPKG CMake script.")
endif()

set(CMAKE_TOOLCHAIN_FILE ${toolchain_file})
message("Toolchain file: ${toolchain_file}")

set(ENABLE_TESTS
    True
    CACHE BOOL "Enable unit tests")

project(RaportPKUP)

find_package(TBB CONFIG REQUIRED)

include(config/Version.cmake)
configure_file(config/Version.h.in ${CMAKE_CURRENT_LIST_DIR}/config/Version.h
               @ONLY)

include(GenerateExportHeader)
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if(${ENABLE_TESTS})
  enable_testing()
  find_package(GTest CONFIG REQUIRED)
  include(GoogleTest)
endif()

add_subdirectory(lib)
add_subdirectory(app)
