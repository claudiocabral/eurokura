cmake_minimum_required(VERSION 3.15)

project(taktaktak)

# add the executable
set(NAME taktaktak)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(${NAME} 
  srcs/main.cpp
  )
target_include_directories(${NAME} PRIVATE
  AudioFile
  srcs
  )
