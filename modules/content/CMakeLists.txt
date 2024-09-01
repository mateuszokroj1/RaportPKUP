qt_add_library(content STATIC)
set_source_files_properties(Theme.qml PROPERTIES QT_QML_SINGLETON_TYPE true)
set_source_files_properties(Fonts.qml PROPERTIES QT_QML_SINGLETON_TYPE true)

qt6_add_qml_module(
  content
  URI
  "content"
  VERSION
  1.0
  RESOURCE_PREFIX
  "/qt/qml"
  IMPORTS
  logic
  RESOURCES
  fonts/FiraSans-Regular.ttf
  QML_FILES
  Fonts.qml
  FormField.ui.qml
  FormItemBase.qml
  Theme.qml
  UIText.ui.qml
  UIButton.qml
  InputField.ui.qml
  StepSelectorButton.ui.qml
  StepSelector.qml
  App.qml
  MainViewItem.qml
  DataInputStepView.qml
  DataFilteringStepView.qml
  DatePicker.qml
  ReportingStepView.qml)

target_link_libraries(content PUBLIC RaportPKUP.Core)
target_link_libraries(content PRIVATE logic logicplugin Qt6::Core Qt6::Qml
                                      Qt6::Quick)
