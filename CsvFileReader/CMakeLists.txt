cmake_minimum_required(VERSION 3.20.0)

project(tastycode LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)


### test
###
include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        v1.14.0
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

include_directories(
    ${CMAKE_CURRENT_LIST_DIR}/src
)

add_executable(
    main_test
    
    "test/CsvFileReader_test.cpp"
    "test/TextFileReader_test.cpp"
)

target_link_libraries(main_test gtest_main)
