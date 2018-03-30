cmake_minimum_required(VERSION 2.8.2)
project(unittests)

if (CMAKE_VERSION VERSION_LESS 3.2)
    set(UPDATE_DISCONNECTED_IF_AVAILABLE "")
else()
    set(UPDATE_DISCONNECTED_IF_AVAILABLE "UPDATE_DISCONNECTED 1")
endif()

include(${lexer_SOURCE_DIR}/cmake/DownloadProject.cmake)

download_project(PROJ                googletest
                 GIT_REPOSITORY      https://github.com/google/googletest.git
                 GIT_TAG             master
                 ${UPDATE_DISCONNECTED_IF_AVAILABLE}
)

add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})

# Trivial example using gtest and gmock
add_executable(unittests Parser_tests.cpp Derivative_tests.cpp Regex_tests.cpp Scanner_tests.cpp)
target_include_directories(unittests PRIVATE SYSTEM ${gtest_INCLUDE_DIR} ${gtest_SOURCE_DIR} ${lexer_SOURCE_DIR})
target_link_libraries(unittests gtest gtest_main core)
