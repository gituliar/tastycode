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

### kwinto
add_executable(
  readline

  readline.cpp
)

target_link_libraries(
    readline

	PRIVATE
        Tracy::TracyClient
)
