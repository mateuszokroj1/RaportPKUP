qt_add_library(
  logic
  STATIC
  src/Application.hpp
  src/application.cpp
  src/ApplicationDefinition.hpp
  src/Command.hpp
  src/command.cpp
  src/CommitItem.hpp
  src/commit_item.cpp
  src/GitAccessImpl.hpp
  src/InputDataState.hpp
  src/Process.hpp
  src/process.cpp
  src/RepositoryListItem.hpp
  src/repository_list_item.cpp
  src/Preset.hpp
  src/preset.cpp
  src/WindowController.hpp
  src/window_controller_fields.cpp
  src/window_controller_calculation.cpp
  src/window_controller_reporting.cpp)

qt6_add_qml_module(
  logic
  URI
  "logic"
  VERSION
  1.0
  RESOURCE_PREFIX
  "/qt/qml"
  SOURCES
  src/Command.hpp
  src/command.cpp
  src/CommitItem.hpp
  src/commit_item.cpp
  src/InputDataState.hpp
  src/Process.hpp
  src/process.cpp
  src/RepositoryListItem.hpp
  src/repository_list_item.cpp
  src/Preset.hpp
  src/preset.cpp
  src/WindowController.hpp
  src/window_controller_fields.cpp
  src/window_controller_calculation.cpp
  src/window_controller_reporting.cpp)

target_link_libraries(
  logic
  PUBLIC RaportPKUP.Core
  PRIVATE Qt6::Core Qt6::Gui)

target_include_directories(logic PUBLIC ${CMAKE_CURRENT_LIST_DIR}/src)
