cmake_minimum_required(VERSION 3.20.0)

if(DEFINED ENV{VCPKG_ROOT})
    if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
        set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")
    endif()
endif()

if(DEFINED WIN32)
    set(VCPKG_TARGET_TRIPLET x64-windows-static)

    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()


project(tastycode LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)

### TextFilreReader_test
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
    ${CMAKE_CURRENT_LIST_DIR}
)

add_executable(
    TextFileReader_test
    
    TextFileReader_test.cpp
)

target_link_libraries(TextFileReader_test gtest_main)


### TextFileReader_profile
###

# Link Tracy
#
#   Sadly, TRACY_ENABLE OFF doesn't disable Tracy from the build.  Hence,
#   need to undef TRACY_ENABLE from *.h files.
#
#set(TRACY_ENABLE OFF)
find_package(
    Tracy CONFIG REQUIRED)
find_package(
    Threads REQUIRED)


add_executable(
    TextFileReader_profile

    TextFileReader_profile.cpp
)

target_link_libraries(
    TextFileReader_profile

	PRIVATE
    Tracy::TracyClient
)
