project(
  RaportPKUPGenerator
  LANGUAGES CXX
  VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}.${VERSION_REVISION}
)

option(LINK_INSIGHT "Link Qt Insight Tracker library" OFF)

set(CPACK_PACKAGE_VERSION_MAJOR ${VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${VERSION_PATCH})
set(CPACK_PACKAGE_NAME "GeneratorRaportowPKUP")
set(CPACK_PACKAGE_VENDOR "Mateusz Okroj")
set(CPACK_SOURCE_IGNORE_FILES "/cmake/;/include/")

set(CMAKE_AUTOMOC ON)

qt_standard_project_setup()

set(os_mode "")

if(WIN32)
  set(os_mode "WIN32")
endif()

qt_add_executable(${PROJECT_NAME} ${os_mode} src/main.cpp)

qt6_add_qml_module(
  ${PROJECT_NAME}
  URI
  "Main"
  VERSION
  ${VERSION_MAJOR}.${VERSION_MINOR}
  IMPORTS
  logic
  content
  RESOURCE_PREFIX
  "/qt/qml"
  NO_PLUGIN
  QML_FILES
  main.qml)

target_link_libraries(${PROJECT_NAME} PRIVATE logicplugin logic contentplugin
                                              content)

target_include_directories(${PROJECT_NAME}
                           PRIVATE ${CMAKE_CURRENT_LIST_DIR}/src)

target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/config")

qt_add_resources(${PROJECT_NAME} "configuration" PREFIX "/" FILES
                 qtquickcontrols2.conf)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt6::Core Qt6::Gui Qt6::Qml
                                              Qt6::Quick)

target_link_libraries(${PROJECT_NAME} PUBLIC RaportPKUP.Core)
if(WIN32)
  configure_file(app.rc.in app.rc @ONLY)
  target_sources(${PROJECT_NAME} PUBLIC app.rc)
endif()

if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
  target_compile_definitions(${PROJECT_NAME} PRIVATE RAPORTPKUP_NOSIGNAL)
endif()

if(WIN32)
  add_custom_command(
    TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:${PROJECT_NAME}>
            $<TARGET_FILE_DIR:${PROJECT_NAME}>
    COMMAND_EXPAND_LISTS)
  find_program(windep NAMES windeployqt REQUIRED)

  add_custom_command(
    TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND ${windep} --qmldir ${CMAKE_SOURCE_DIR}
            $<TARGET_FILE:RaportPKUPGenerator>)
endif()

set(_miktex_package ${CMAKE_SOURCE_DIR}/3rd/miktex-portable.7z)
set(_miktex_extract_location ${CMAKE_CURRENT_BINARY_DIR}/miktex)

if(NOT EXISTS ${_miktex_extract_location})
  if(NOT EXISTS ${_miktex_package})
    message(FATAL_ERROR "Not found miktex-portable.7z.")
  endif()

  message(STATUS "Extracting miktex-package")
  file(ARCHIVE_EXTRACT INPUT ${_miktex_package} DESTINATION
       ${_miktex_extract_location} VERBOSE)
endif()

find_program(
  miktex_app
  PATHS
    "${_miktex_extract_location}/miktex-portable/texmfs/install/miktex/bin/x64"
  NAMES miktex REQUIRED)
message(STATUS "MIKTEX: Updating packages...")
execute_process(COMMAND ${miktex_app} packages update)

if(BUILD_QDS_COMPONENTS)
  include(qmlcomponents)
endif()

install(
  TARGETS ${PROJECT_NAME}
  BUNDLE DESTINATION ${CMAKE_INSTALL_PREFIX}
  RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX})

install(DIRECTORY ${_miktex_extract_location} USE_SOURCE_PERMISSIONS
        DESTINATION ${CMAKE_INSTALL_PREFIX})

if(WIN32)
  install(CODE "set(_qmldir \"${CMAKE_SOURCE_DIR}\")")

  install(
    CODE [[
    find_program(windep NAMES windeployqt REQUIRED)

    execute_process(
      WORKING_DIRECTORY ${CMAKE_INSTALL_PREFIX}
      COMMAND ${windep} --qmldir ${_qmldir} ${CMAKE_INSTALL_PREFIX}
    )
  ]])

  file(GLOB _vcpkg_dlls
       ${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/bin/*.dll)

  foreach(dll_path ${_vcpkg_dlls})
    install(FILES ${dll_path} DESTINATION ${CMAKE_INSTALL_PREFIX})
  endforeach()
endif()
