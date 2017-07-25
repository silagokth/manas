cmake_minimum_required(VERSION 2.8.9)
project(manus)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
# Show verbose command while compiling
#set(CMAKE_VERBOSE_MAKEFILE ON)

# The project version number.
set(VERSION_MAJOR 0 CACHE STRING "Project major version number.")
set(VERSION_MINOR 1 CACHE STRING "Project minor version number.")
set(VERSION_PATCH 0 CACHE STRING "Project patch version number.")
mark_as_advanced(VERSION_MAJOR VERSION_MINOR VERSION_PATCH)

find_package(BISON)
find_package(FLEX)
#find_package(TCL REQUIRED)
find_package(Boost COMPONENTS regex system filesystem program_options log unit_test_framework REQUIRED)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(include)

file(GLOB SOURCES_PARSER "src/parser/*.cpp" "src/parser/*/*.cpp")
file(GLOB SOURCES_IR "src/ir/*.cpp" "src/ir/*/*.cpp")
file(GLOB SOURCES_GENERATOR "src/generator/*.cpp" "src/generator/*/*.cpp")
file(GLOB SOURCES_VESYLA "src/vesyla/*.cpp")

file(GLOB SOURCES_FLEX_MANUS "src/parser/ParManus/*.l")
file(GLOB SOURCES_BISON_MANUS "src/parser/ParManus/*.y")

set(CMAKE_CXX_FLAGS "-std=c++11 -w -pedantic -lpthread -g -O2 -DBOOST_LOG_DYN_LINK")
set(PROJECT_LINK_LIBS)

BISON_TARGET(ParManusParser ${SOURCES_BISON_MANUS} ${CMAKE_CURRENT_BINARY_DIR}/ParManus_parser.cpp)
FLEX_TARGET(ParManusScanner ${SOURCES_FLEX_MANUS} ${CMAKE_CURRENT_BINARY_DIR}/ParManus_lexer.cpp)
ADD_FLEX_BISON_DEPENDENCY(ParManusScanner ParManusParser)

add_library(parser SHARED ${SOURCES_PARSER} ${BISON_ParManusParser_OUTPUTS} ${FLEX_ParManusScanner_OUTPUTS})
add_library(ir SHARED ${SOURCES_IR})
add_library(generator SHARED ${SOURCES_GENERATOR})
add_executable(manus ${SOURCES_VESYLA})
target_link_libraries(manus parser ir generator ${Boost_LIBRARIES} ${PROJECT_LINK_LIBS})
