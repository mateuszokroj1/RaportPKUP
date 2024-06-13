project(RaportPKUP.Core)

set(PRIVATE_LIBS unofficial::libgit2::libgit2)

set(PUBLIC_LIBS)

set(PUBLIC_HEADERS
    base.hpp
    IProcess.hpp
    Author.hpp
    Commit.hpp
    IRepository.hpp
    IRepositoryDetector.hpp
    IRepositoryAccessor.hpp
    GitRepository.hpp
    ICommand.hpp)

set(SOURCES process_runner.cpp git_repository.cpp libgit.cpp)

set(TESTS git_repository.cpp)

list(TRANSFORM SOURCES PREPEND "src/")

add_library(${PROJECT_NAME} SHARED ${SOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE ${PRIVATE_LIBS})
target_link_libraries(${PROJECT_NAME} PUBLIC ${PUBLIC_LIBS})

generate_export_header(${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
                           PRIVATE ${CMAKE_CURRENT_LIST_DIR}/include)
target_include_directories(
  ${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>)

if(${ENABLE_TESTS})
  set(executable_name ${PROJECT_NAME}Tests)

  list(TRANSFORM TESTS PREPEND "tests/")

  add_executable(${executable_name} ${CMAKE_SOURCE_DIR}/config/test_main.cpp
                                    ${TESTS} ${SOURCES})
  target_link_libraries(${executable_name} PRIVATE GTest::gtest GTest::gmock
                                                   GTest::gmock_main)
  target_link_libraries(${executable_name} PRIVATE ${PRIVATE_LIBS}
                                                   ${PUBLIC_LIBS})

  target_compile_definitions(${executable_name}
                             PRIVATE RAPORTPKUP_CORE_STATIC_DEFINE)

  target_include_directories(${executable_name}
                             PRIVATE ${CMAKE_CURRENT_LIST_DIR}/include)
  target_include_directories(${executable_name}
                             PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

  gtest_discover_tests(${executable_name})
endif()

list(TRANSFORM PUBLIC_HEADERS PREPEND "include/")
target_sources(${PROJECT_NAME} PUBLIC FILE_SET public_headers_fileset TYPE
                                      HEADERS FILES ${PUBLIC_HEADERS})

set(ConfigPackageLocation ${CMAKE_INSTALL_PREFIX}/cmake)

install(
  TARGETS ${PROJECT_NAME}
  EXPORT ${PROJECT_NAME}Targets
  ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}
  LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}
  RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}
  INCLUDES
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  FILE_SET public_headers_fileset
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(
  EXPORT ${PROJECT_NAME}Targets
  FILE ${PROJECT_NAME}Targets.cmake
  DESTINATION ${ConfigPackageLocation})

configure_package_config_file(
  ${PROJECT_NAME}Config.cmake.in ${PROJECT_NAME}Config.cmake
  INSTALL_DESTINATION ${ConfigPackageLocation}
  PATH_VARS CMAKE_INSTALL_PREFIX)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
        DESTINATION ${ConfigPackageLocation})
