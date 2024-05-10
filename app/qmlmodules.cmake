qt6_add_qml_module(
  ${executable_name}
  URI
  "Main"
  VERSION
  1.0
  IMPORTS
  contentplugin
  RESOURCE_PREFIX
  "/qt/qml"
  NO_PLUGIN
  QML_FILES
  main.qml)

add_subdirectory(content)

target_link_libraries(${executable_name} PRIVATE contentplugin)
