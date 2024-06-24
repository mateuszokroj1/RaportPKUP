cmake_minimum_required(VERSION 3.24)

option(LINK_INSIGHT "Link Qt Insight Tracker library" ON)

set(executable_name RaportPKUPGenerator)

project(
  ${executable_name}
  LANGUAGES CXX
  VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH})

set(CMAKE_AUTOMOC ON)

find_package(Qt6 6.7 REQUIRED COMPONENTS Core Gui Qml Quick)
qt_standard_project_setup()

set(os_mode "")

if(WIN32)
  set(os_mode "WIN32")
endif()

qt_add_executable(${executable_name} ${os_mode} src/main.cpp)

qt6_add_qml_module(
  ${executable_name}
  URI
  "Main"
  VERSION
  1.0
  IMPORTS
  logic
  content
  RESOURCE_PREFIX
  "/qt/qml"
  NO_PLUGIN
  QML_FILES
  main.qml)

target_link_libraries(
  ${executable_name}
  PRIVATE logic
  PRIVATE content)

target_include_directories(${executable_name}
                           PRIVATE ${CMAKE_CURRENT_LIST_DIR}/src)

target_include_directories(${executable_name}
                           PRIVATE "${CMAKE_SOURCE_DIR}/config/Version.h")

qt_add_resources(${executable_name} "configuration" PREFIX "/" FILES
                 qtquickcontrols2.conf)

target_link_libraries(${executable_name} PRIVATE Qt6::Core Qt6::Gui Qt6::Qml
                                                 Qt6::Quick)

target_link_libraries(${executable_name} PUBLIC RaportPKUP.Core)
if(WIN32)
  configure_file(app.rc.in app.rc @ONLY)
  target_sources(${executable_name} PUBLIC app.rc)
endif()

if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
  target_compile_definitions(${executable_name} PRIVATE RAPORTPKUP_NOSIGNAL)
endif()

if(WIN32)
  add_custom_command(
    TARGET ${executable_name}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:${executable_name}>
            $<TARGET_FILE_DIR:${executable_name}>
    COMMAND_EXPAND_LISTS)
  find_program(windep NAMES windeployqt REQUIRED)

  add_custom_command(
    TARGET ${executable_name}
    POST_BUILD
    COMMAND ${windep} --qmldir ${CMAKE_SOURCE_DIR}
            $<TARGET_FILE_DIR:${executable_name}>)
endif()

if(BUILD_QDS_COMPONENTS)
  include(qmlcomponents)
endif()

if(LINK_INSIGHT)
  include(insight)
endif()

install(
  TARGETS ${executable_name}
  BUNDLE DESTINATION ${CMAKE_INSTALL_PREFIX}
  LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}
  RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX})

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
endif()

# make IDEs aware of the QML import path set(QML_IMPORT_PATH
# ${PROJECT_BINARY_DIR}/qml CACHE PATH "Path to the custom QML components
# defined by the project")
