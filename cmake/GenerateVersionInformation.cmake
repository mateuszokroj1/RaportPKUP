function(GenerateVersionInformation)
  set(out "")

  if(NOT DEFINED GIT_HASH)
    execute_process(
      COMMAND ${git_app} rev-parse HEAD
      WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
      OUTPUT_VARIABLE out
      OUTPUT_STRIP_TRAILING_WHITESPACE)

    set(GIT_HASH
        "${out}"
        PARENT_SCOPE)

    message("Git HASH: ${out}")
  else()
    message("Git HASH: ${GIT_HASH}")
  endif()

  if(NOT DEFINED GIT_BRANCH)
    execute_process(
      COMMAND ${git_app} rev-parse --abbrev-ref HEAD
      WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
      OUTPUT_VARIABLE out
      OUTPUT_STRIP_TRAILING_WHITESPACE)

    set(GIT_BRANCH
        "${out}"
        PARENT_SCOPE)

    message("Git branch: ${out}")
  else()
    message("Git branch: ${GIT_BRANCH}")

    set(out "${GIT_BRANCH}")
  endif()

  if(NOT DEFINED VERSION_REVISION)
    execute_process(
      COMMAND ${git_app} rev-list --count --first-parent ${out} --
      WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
      OUTPUT_VARIABLE out
      OUTPUT_STRIP_TRAILING_WHITESPACE)

    set(VERSION_REVISION
        "${out}"
        PARENT_SCOPE)

    message("Version revision: ${out}")
  else()
    message("Version revision: ${VERSION_REVISION}")
  endif()
endfunction()
