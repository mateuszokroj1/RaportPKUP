cmake_minimum_required(VERSION 3.20)

if(WIN32)
	find_program(MSVC NAMES cl REQUIRED)
	set(CMAKE_C_COMPILER ${MSVC})
	set(CMAKE_CXX_COMPILER ${MSVC})
else()
	find_program(gcc NAMES gcc REQUIRED)
	set(CMAKE_C_COMPILER ${gcc})
	find_program(gpp NAMES g++ REQUIRED)
	set(CMAKE_CXX_COMPILER ${gpp})
endif()

project(Root)

find_program(vcpkg_app NAMES vcpkg REQUIRED)

get_filename_component(vcpkg_dir ${vcpkg_app} DIRECTORY)

message(${vcpkg_dir})

set(toolchain_path "${vcpkg_dir}/scripts/buildsystems/vcpkg.cmake")

if(NOT EXISTS ${toolchain_path})
	message(FATAL_ERROR "Cannot find VCPKG cmake script.")
endif()

set(CMAKE_TOOLCHAIN_FILE ${toolchain_path})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include(GNUInstallDirs)

add_subdirectory(app)