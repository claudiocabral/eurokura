cmake_minimum_required(VERSION 3.15)

project(eurokura)

# add the executable
set(BINARY taktaktak)

add_library(kura INTERFACE)

add_executable(${BINARY}
  srcs/main.cpp
  )
target_link_libraries(${BINARY} PUBLIC kura)
target_include_directories(kura INTERFACE
  AudioFile
  gcem/include
  srcs
  )

set_target_properties(${BINARY}
    PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    )

target_compile_options(${BINARY} PUBLIC
  $<$<CXX_COMPILER_ID:GNU>: -ffast-math>
  $<$<CXX_COMPILER_ID:GNU>: -Wall -Wextra -Werror >
  #$<$<CXX_COMPILER_ID:MSVC>: /W4 >
    )
