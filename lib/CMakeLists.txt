set(ProjectName RaportGenCore)

project(${ProjectName})

include(GenerateExportHeader)
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

set(PRIVATE_LIBS
    TBB::tbb
)

set(PUBLIC_LIBS

)

set(SOURCES
    git_repository.cpp
)

set(tests_dir ${CMAKE_CURRENT_LIST_DIR}/tests)
set(TESTS
    ${tests_dir}/test.cpp
)

add_library(${ProjectName} SHARED ${SOURCES})

target_link_libraries(${ProjectName} PRIVATE ${PRIVATE_LIBS})
target_link_libraries(${ProjectName} PUBLIC ${PUBLIC_LIBS})

target_include_directories(${ProjectName}
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
           $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_compile_definitions(${ProjectName} PRIVATE ${ProjectName}_EXPORTS)

if(${ENABLE_TESTS})
    add_executable(${ProjectName}Tests ${SOURCES} ${TESTS})
    target_link_libraries(${ProjectName}Tests PRIVATE GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main)
    target_link_libraries(${ProjectName}Tests PRIVATE ${PRIVATE_LIBS})
    target_compile_definitions(${ProjectName} PRIVATE ${ProjectName}_TESTS)

    target_include_directories(${ProjectName} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/include)

    include(GoogleTest)
    gtest_discover_tests(${ProjectName}Tests)
endif()

set(ConfigPackageLocation "${CMAKE_INSTALL_LIBDIR}/cmake")

install(
    TARGETS ${ProjectName}
    EXPORT ${ProjectName}Targets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(
    EXPORT ${ProjectName}Targets
    FILE ${ProjectName}Targets.cmake
    DESTINATION ${ConfigPackageLocation}
)

configure_package_config_file(
    ${ProjectName}Config.cmake.in
    ${ProjectName}Config.cmake
    INSTALL_DESTINATION ${ConfigPackageLocation}
    PATH_VARS CMAKE_INSTALL_PREFIX
)

install(
    FILES "${CMAKE_CURRENT_BINARY_DIR}/${ProjectName}Config.cmake"
    DESTINATION ${ConfigPackageLocation}
)