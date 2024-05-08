qt_add_library(shared STATIC)
set_source_files_properties(Theme.qml PROPERTIES QT_QML_SINGLETON_TYPE true)
set_source_files_properties(Fonts.qml PROPERTIES QT_QML_SINGLETON_TYPE true)

qt6_add_qml_module(
  shared
  URI
  "shared"
  VERSION
  1.0
  RESOURCE_PREFIX
  "/qt/qml"
  RESOURCES
  fonts/FiraSans-Regular.ttf
  QML_FILES
  Fonts.qml
  Theme.qml
  Text.ui.qml
  Button.qml
  ToolBar.ui.qml
  InputField.ui.qml
  StepSelectorButton.ui.qml
  StepSelector.ui.qml
  SOURCES
  ../src/MainViewItem.hpp
  ../src/MainViewItem.cpp
  ../src/Command.hpp
  ../src/Command.cpp
  ../src/InputDataState.hpp)

target_link_libraries(shared PUBLIC RaportPKUP.Core)
target_include_directories(shared PRIVATE ${CMAKE_SOURCE_DIR}/app/src)
