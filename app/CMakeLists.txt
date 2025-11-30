project(
  RaportPKUPGenerator
  VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}.${VERSION_REVISION}
)

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

set(ENV_FILE
    ${GIT_REPOSITORY_ROOT}/config/generated.env
    CACHE PATH "Path to the generated environment file")
file(WRITE ${ENV_FILE} PATH=$ENV{PATH};${PROJECT_BINARY_DIR})

if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
  file(
    APPEND ${ENV_FILE}
    QT_QPA_PLATFORM_PLUGIN_PATH=${QT6_INSTALL_PREFIX}/Debug/${QT6_INSTALL_PLUGINS}/platforms
  )
else()
  file(
    APPEND ${ENV_FILE}
    QT_QPA_PLATFORM_PLUGIN_PATH=${QT6_INSTALL_PREFIX}/${QT6_INSTALL_PLUGINS}/platforms
  )
endif()

qt_add_executable(${PROJECT_NAME} ${os_mode} src/main.cpp)

qt6_add_qml_module(
  ${PROJECT_NAME}
  URI
  "Main"
  VERSION
  ${VERSION_MAJOR}.${VERSION_MINOR}
  NO_PLUGIN
  IMPORTS
  logic
  content
  RESOURCE_PREFIX
  "/qt/qml"
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
  BUNDLE DESTINATION ${CMAKE_INSTALL_BINDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

qt_generate_deploy_qml_app_script(TARGET ${PROJECT_NAME} OUTPUT_SCRIPT
                                  qt_deploy_script)
install(SCRIPT ${qt_deploy_script})

install(DIRECTORY ${_miktex_extract_location} USE_SOURCE_PERMISSIONS
        DESTINATION ${CMAKE_INSTALL_PREFIX})

# if(WIN32) install(CODE "set(_qmldir \"${CMAKE_SOURCE_DIR}\")")

install(CODE [[ find_program(windep NAMES windeployqt REQUIRED)
execute_process( WORKING_DIRECTORY ${CMAKE_INSTALL_PREFIX} COMMAND ${windep}
--qmldir ${_qmldir} ${CMAKE_INSTALL_PREFIX} ) ]])

# file(GLOB _vcpkg_dlls
# ${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/bin/*.dll)

# foreach(dll_path ${_vcpkg_dlls}) install(FILES ${dll_path} DESTINATION
# ${CMAKE_INSTALL_BINDIR}) endforeach() endif()
