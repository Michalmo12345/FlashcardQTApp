macro(fetch_git_lib NAME GIT_URL GIT_TAG)
    include(FetchContent)
    message(STATUS "Fetching git library ${NAME} with tag ${GIT_TAG}")
    FetchContent_Declare(${NAME}
        GIT_REPOSITORY ${GIT_URL}
        GIT_TAG ${GIT_TAG}
        USES_TERMINAL_DOWNLOAD ON
    )
    FetchContent_MakeAvailable(${NAME})
    message(STATUS "Completed fetching git library ${NAME}")
endmacro()