function(upload_git_submodule dir)
    find_package(Git REQUIRED)

    if(NOT EXISTS ${dir}/CMakeLists.txt AND NOT EXISTS ${dir}/Makefile)
        execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive -- ${dir}
                WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
    endif()

endfunction(upload_git_submodule)

function(add_git_submodule dir)
    # add a Git submodule directory to CMake, assuming the
    # Git submodule directory is a CMake project.
    #
    # Usage: in CMakeLists.txt
    #
    # include(AddGitSubmodule.cmake)
    # add_git_submodule(mysubmod_dir)

    upload_git_submodule(${dir})
    add_subdirectory(${dir})

endfunction(add_git_submodule)
