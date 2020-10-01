project(not-minidsp)
cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

### BINARY
add_executable(prog)

target_include_directories(prog PUBLIC include)
target_compile_options(
  prog PRIVATE
  -Wall -Wextra -Wshadow -Wnon-virtual-dtor -Wold-style-cast -Wcast-align -Wunused -Woverloaded-virtual -Wpedantic -Wconversion -Wsign-conversion -Wmisleading-indentation -Wduplicated-cond -Wduplicated-branches -Wlogical-op -Wnull-dereference -Wuseless-cast -Wdouble-promotion -Wformat=2
)

add_subdirectory(src)
# add_subdirectory(deps)

find_package(Threads REQUIRED)
find_package(fmt REQUIRED)

target_link_libraries(prog PUBLIC Threads::Threads portaudio fmt)

### RUN BINARY TARGET (make run)
add_custom_target(
  run
  COMMAND ${CMAKE_BINARY_DIR}/prog
)
add_dependencies(run prog)

### TIDY/FORMAT
get_target_property(PROGRAM_SOURCES prog SOURCES)
add_custom_target(
  format
  COMMAND clang-format -i ${PROGRAM_SOURCES}
)
add_custom_target(
  tidy
  COMMAND clang-tidy ${PROGRAM_SOURCES}
)

### EXTRA OPTIONS
option(ENABLE_CLANG_TIDY "Sets whether clang tidy should fix code or not" OFF)


if(ENABLE_CLANG_TIDY MATCHES ON)
  set(CMAKE_CXX_CLANG_TIDY "clang-tidy;-header-filter=$(realpath ..)")
endif()

if(CMAKE_BUILD_TYPE MATCHES Debug)
  target_link_libraries(prog PUBLIC -fsanitize=address,undefined)
  target_compile_options(prog PUBLIC -fsanitize=address,undefined)
endif()
