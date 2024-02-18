project(RaportGenCore)

set(PRIVATE_LIBS
    TBB::tbb
    unofficial::libgit2::libgit2
)

set(PUBLIC_LIBS

)

set(PUBLIC_HEADERS
    base.hpp
    Author.hpp
    Commit.hpp
    IRepository.hpp
    GitRepository.hpp
)

set(SOURCES
    libgit_wrapper.cpp
    git_repository.cpp
)

set(TESTS
    test.cpp
)

set(CMAKE_CXX_STANDARD 23)

add_library(${PROJECT_NAME} SHARED)

target_link_libraries(${PROJECT_NAME} PRIVATE ${PRIVATE_LIBS})
target_link_libraries(${PROJECT_NAME} PUBLIC ${PUBLIC_LIBS})

generate_export_header(${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/include)
target_include_directories(${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>)

if(${ENABLE_TESTS})
    set(executable_name ${PROJECT_NAME}Tests)

    add_executable(${executable_name})
    target_link_libraries(${executable_name} PRIVATE GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main)
    target_link_libraries(${executable_name} PRIVATE ${PRIVATE_LIBS} ${PUBLIC_LIBS})

    target_include_directories(${executable_name} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/include)
    target_include_directories(${executable_name} PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

    include(GoogleTest)
    gtest_discover_tests(${executable_name})
endif()

list(TRANSFORM PUBLIC_HEADERS PREPEND "include/")
target_sources(
    ${PROJECT_NAME}
        PUBLIC FILE_SET public_headers_fileset
        TYPE HEADERS
        FILES
            ${PUBLIC_HEADERS}
)

foreach(filename ${SOURCES})
    target_sources(${PROJECT_NAME} PRIVATE src/${filename})

    if(${ENABLE_TESTS})
        target_sources(${executable_name} PRIVATE src/${filename})
    endif()
endforeach()

set(ConfigPackageLocation ${CMAKE_INSTALL_LIBDIR}/cmake)

install(
    TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}Targets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    FILE_SET public_headers_fileset DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(
    EXPORT ${PROJECT_NAME}Targets
    FILE ${PROJECT_NAME}Targets.cmake
    DESTINATION ${ConfigPackageLocation}
)

configure_package_config_file(
    ${PROJECT_NAME}Config.cmake.in
    ${PROJECT_NAME}Config.cmake
    INSTALL_DESTINATION ${ConfigPackageLocation}
    PATH_VARS CMAKE_INSTALL_PREFIX
)

install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
    DESTINATION ${ConfigPackageLocation}
)