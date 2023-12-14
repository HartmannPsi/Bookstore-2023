# specify the minimum version of CMake that is supported
cmake_minimum_required(VERSION 3.16)

# include a project name
project(code)

# set C++ Version & executable output path
set(CMAKE_CXX_STANDARD 17)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

# make your compiler aware of header directory
include_directories(${PROJECT_SOURCE_DIR}/include)

# find & include ncurses library
# find_package(Curses REQUIRED)
# include_directories(${CURSES_INCLUDE_DIR})

# create a variable which includes needed source files
set(MY_SOURCES
    ${PROJECT_SOURCE_DIR}/code/main.cpp
    ${PROJECT_SOURCE_DIR}/code/account.cpp
    ${PROJECT_SOURCE_DIR}/code/block_list.cpp
    ${PROJECT_SOURCE_DIR}/code/command.cpp
    ${PROJECT_SOURCE_DIR}/code/string_check.cpp
    ${PROJECT_SOURCE_DIR}/code/book.cpp
    ${PROJECT_SOURCE_DIR}/code/diary.cpp
)

# specify an executable, 
# build from the specified source files
add_executable(code ${MY_SOURCES})

# link ncurses library with your executable
# target_link_libraries(code ${CURSES_LIBRARY})