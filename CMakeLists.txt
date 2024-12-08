cmake_minimum_required(VERSION 3.24)

if(NOT EXISTS "${CMAKE_C_COMPILER}" OR NOT EXISTS "${CMAKE_CXX_COMPILER}")
  if(${WIN32})
    find_program(cl NAMES cl.exe REQUIRED)
    set(CMAKE_C_COMPILER ${cl})
    set(CMAKE_CXX_COMPILER ${cl})
  else()
    find_program(clang NAMES clang REQUIRED)
    set(CMAKE_C_COMPILER ${clang})
    set(CMAKE_CXX_COMPILER ${clang})
  endif()
endif()

find_program(git_app NAMES git REQUIRED)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

if(NOT DEFINED DISABLE_PRECOMMIT)
  include(PreCommitInstall)
  precommitinstall()
endif()

set(GIT_REPOSITORY_ROOT ${CMAKE_CURRENT_LIST_DIR})

set(ENABLE_TESTS
    True
    CACHE BOOL "Enable unit tests")

project(RaportPKUP)

find_package(unofficial-libgit2 CONFIG REQUIRED)

include(config/Version.cmake)
include(cmake/GenerateVersionInformation.cmake)
message("Version: ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")
generateversioninformation()

configure_file(config/Version.h.in ${CMAKE_CURRENT_LIST_DIR}/config/Version.h
               @ONLY)

include(GenerateExportHeader)
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if(${ENABLE_TESTS})
  enable_testing()
  find_package(GTest CONFIG REQUIRED)
  include(GoogleTest)
endif()

add_subdirectory(lib)

find_package(Qt6 6.8 REQUIRED COMPONENTS Core Quick Gui Widgets)
set(CMAKE_AUTOMOC true)
add_subdirectory(modules/logic)
add_subdirectory(modules/content)
add_subdirectory(app)

set(QML_IMPORT_PATH
    "${CMAKE_BINARY_DIR}/modules"
    CACHE PATH "Generated modules for QML editor")

include(CPack)
