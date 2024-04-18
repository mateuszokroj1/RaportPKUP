find_program(windep NAMES windeployqt REQUIRED)

execute_process(COMMAND ${windep} ${CMAKE_INSTALL_PREFIX})
