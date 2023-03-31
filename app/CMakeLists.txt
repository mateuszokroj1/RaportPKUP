project(app VERSION 0.0.1)

find_package(Qt6Core CONFIG REQUIRED)
find_package(Qt6Gui CONFIG REQUIRED)
find_package(Qt6Widgets CONFIG REQUIRED)
find_package(Qt6Quick CONFIG REQUIRED)
find_package(unofficial-libharu CONFIG REQUIRED)

qt_add_executable(RaportGen main.cpp)

	target_link_libraries(RaportGen PUBLIC Qt6::Gui Qt6::Quick)

	qt_add_qml_module(RaportGen
		URI app
		VERSION ${PROJECT_VERSION}
		QML_FILES
			MainWindow.qml
	)

	get_filename_component(qplugins "${Qt6Gui_DIR}/../../Qt6/plugins" ABSOLUTE)

	message(${qplugins})

	add_custom_command(
		TARGET RaportGen POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory ${qplugins} $<TARGET_FILE_DIR:RaportGen>
	)

	install(
		TARGETS RaportGen
		EXPORT RaportGenTargets
		RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
		ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
		LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	)