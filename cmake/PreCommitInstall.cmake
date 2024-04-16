function(PreCommitInstall)
  message(STATUS "pre-commit hooks are installing...")

  find_program(python_app NAMES python)

  if(NOT EXISTS ${python_app})
    message("Python interpreter not found.")

    return()
  endif()

  find_program(pip_app NAMES pip)

  if(NOT EXISTS ${pip_app})
    message("Python package manager not found.")

    return()
  endif()

  execute_process(
    COMMAND ${pip_app} install pre-commit
    RESULT_VARIABLE _result
    OUTPUT_VARIABLE _out)

  if(NOT ${_result} EQUAL 0)
    message("Error while installing Python package 'pre-commit'.")

    return()
  endif()

  find_program(pre_commit NAMES "pre-commit")

  if(NOT EXISTS ${pre_commit})
    message("Python package manager not found.")

    return()
  endif()

  execute_process(
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMAND ${pre_commit} install
    RESULT_VARIABLE _result)

  if(NOT ${_result} EQUAL 0)
    message("Error while installing pre-commit hooks to Git repo.")

    return()
  endif()

  message("Installed successfully.")
endfunction()
