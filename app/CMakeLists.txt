cmake_minimum_required(VERSION 3.24)

option(LINK_INSIGHT "Link Qt Insight Tracker library" ON)
option(BUILD_QDS_COMPONENTS "Build design studio components" OFF)

set(executable_name RaportPKUPGenerator)

project(${executable_name} LANGUAGES CXX VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH})

set(CMAKE_AUTOMOC ON)

find_package(Qt6 6.2 REQUIRED COMPONENTS Core Gui Qml Quick)

if (Qt6_VERSION VERSION_GREATER_EQUAL 6.3)
    qt_standard_project_setup()
endif()

qt_add_executable(${executable_name} src/main.cpp src/window_controller.cpp src/WindowController.h)
if(WIN32)
    configure_file(app.rc.in app.rc @ONLY)
    target_sources(${executable_name} PUBLIC app.rc)
endif()

set_property(TARGET ${executable_name} PROPERTY CXX_STANDARD 23)

qt_add_resources(${executable_name} "configuration"
    PREFIX "/"
    FILES
        qtquickcontrols2.conf
)

target_link_libraries(${executable_name} PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Qml
    Qt6::Quick
    TBB::tbb
)
target_link_libraries(${executable_name} PUBLIC RaportGenCore)
if(WIN32)
    add_custom_command(TARGET ${executable_name}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
            $<TARGET_RUNTIME_DLLS:${executable_name}>
            $<TARGET_FILE_DIR:${executable_name}>
        COMMAND_EXPAND_LISTS
    )
endif()

target_include_directories(${executable_name} PRIVATE "${CMAKE_SOURCE_DIR}/config/Version.h")

if (BUILD_QDS_COMPONENTS)
    include(${CMAKE_CURRENT_SOURCE_DIR}/qmlcomponents)
endif()

include(${CMAKE_CURRENT_SOURCE_DIR}/qmlmodules)

if (LINK_INSIGHT)
    include(${CMAKE_CURRENT_SOURCE_DIR}/insight)
endif ()

include(GNUInstallDirs)
install(TARGETS ${executable_name}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# make IDEs aware of the QML import path
set(QML_IMPORT_PATH ${PROJECT_BINARY_DIR}/qml CACHE PATH
    "Path to the custom QML components defined by the project")

    qt_generate_deploy_app_script(
    TARGET ${executable_name}
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})