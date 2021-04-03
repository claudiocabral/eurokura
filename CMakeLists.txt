cmake_minimum_required(VERSION 3.15)

project(eurokura)

# add the executable
set(BINARY taktaktak)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(${BINARY} 
  srcs/main.cpp
  )
target_include_directories(${BINARY} PRIVATE
  AudioFile
  srcs
  )

set_target_properties(${BINARY}
    PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    )

target_compile_options(${BINARY} PRIVATE
    $<$<CXX_COMPILER_ID:GNU>: -Wall -Wextra -Werror > 
    $<$<CXX_COMPILER_ID:MSVC>: /W4 >
    )
