project(RaportPKUP.Logic)

set(HEADERS
    Application.hpp
    ApplicationDefinition.hpp
    Command.hpp
    CommitItem.hpp
    GitAccessImpl.hpp
    InputDataState.hpp
    Process.hpp
    RepositoryListItem.hpp
    Preset.hpp
    PresetsManager.hpp
    WindowController.hpp)

set(SOURCES
    application.cpp
    command.cpp
    commit_item.cpp
    process.cpp
    repository_list_item.cpp
    presets_manager.cpp
    window_controller_fields.cpp
    window_controller_presets.cpp
    window_controller_calculation.cpp
    window_controller_reporting.cpp)

list(TRANSFORM HEADERS PREPEND "include/")
list(TRANSFORM SOURCES PREPEND "src/")

set(CMAKE_AUTOMOC ON)

qt_standard_project_setup()

qt_add_library(logic STATIC ${SOURCES})

target_sources(logic PUBLIC FILE_SET HEADERS FILES ${HEADERS})

qt6_add_qml_module(
  logic
  URI
  "logic"
  VERSION
  1.0
  RESOURCE_PREFIX
  "/qt/qml"
  SOURCES
  ${HEADERS}
  ${SOURCES})

target_link_libraries(
  logic
  PUBLIC RaportPKUP.Core
  PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets)

target_include_directories(logic PRIVATE include)

qt_generate_deploy_qml_app_script(TARGET logic OUTPUT_SCRIPT qt_deploy_script)
install(SCRIPT ${qt_deploy_script})
