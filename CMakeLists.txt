project(not-minidsp)
cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

### BINARY
add_executable(fir-rc)
add_executable(bypass)

include_directories(include)

add_library(compile_options INTERFACE)
target_compile_options(compile_options INTERFACE -Wall -Wextra -Wshadow -Wnon-virtual-dtor -Wold-style-cast -Wcast-align -Wunused -Woverloaded-virtual -Wpedantic -Wconversion -Wsign-conversion -Wmisleading-indentation -Wduplicated-cond -Wduplicated-branches -Wlogical-op -Wnull-dereference -Wuseless-cast -Wdouble-promotion -Wformat=2)

add_subdirectory(src)

find_package(Threads REQUIRED)
find_package(Boost COMPONENTS program_options REQUIRED)

target_link_libraries(fir-rc PUBLIC Threads::Threads portaudio Boost::program_options compile_options)
target_link_libraries(bypass PUBLIC Threads::Threads portaudio Boost::program_options compile_options)

### RUN BINARY TARGET (make run)
add_custom_target(
  run
  COMMAND ${CMAKE_BINARY_DIR}/fir-rc
)
add_dependencies(run fir-rc)

### EXTRA OPTIONS
option(ENABLE_CLANG_TIDY "Sets whether clang tidy should fix code or not" OFF)


if(ENABLE_CLANG_TIDY MATCHES ON)
  set(CMAKE_CXX_CLANG_TIDY "clang-tidy;-header-filter=$(realpath ..)")
endif()

option(ENABLE_SANITIZERS "Enable address and undefined snitizers" OFF)
if(ENABLE_SANITIZERS MATCHES ON)
  target_link_libraries(fir-rc PUBLIC -fsanitize=address,undefined)
  target_compile_options(fir-rc PUBLIC -fsanitize=address,undefined)
endif()
