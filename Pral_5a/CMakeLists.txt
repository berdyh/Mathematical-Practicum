# Copyright (c) 2021, Tim A. Werthmann.

cmake_minimum_required(VERSION 3.13.3 FATAL_ERROR)
cmake_policy(VERSION 3.13.3)

# ##############################################################################
# Build type (needs to be handled before project command below)
# ##############################################################################
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE
      "Release"
      CACHE
        STRING
        "Configuration type (one of Debug, Release, RelWithDebInfo, MinSizeRel)"
  )
endif()
message(STATUS "The build type is ${CMAKE_BUILD_TYPE}")

# ##############################################################################
# Project metadata
# ##############################################################################
project(
  mapra
  LANGUAGES CXX
  VERSION 1.0.0)

# ##############################################################################
# Find or fetch SFML
# ##############################################################################

# Dies laedt die SFML herunter
include(${mapra_SOURCE_DIR}/cmake/dependencies.cmake)

# ##############################################################################
# Copy data to target
# ##############################################################################

# Dies kopiert die Daten an den richtigen Ort
include(${mapra_SOURCE_DIR}/cmake/data_copy.cmake)

# ##############################################################################
# Add some helper functions to make this more pretty for you
# ##############################################################################
include(${mapra_SOURCE_DIR}/cmake/functions.cmake)

# ##############################################################################
# Handle Unit
# ##############################################################################

set(a_stern_unit)
if(APPLE)
  list(APPEND a_stern_unit lib/unit_mac.cpp.o)
elseif(UNIX AND NOT APPLE)
  list(APPEND a_stern_unit lib/unit.cpp.o)
endif()

# ##############################################################################
# Unit Testing
# ##############################################################################

enable_testing()
mapra_add_test(example_test)

# ##############################################################################
# Add Binaries
# ##############################################################################

# Setzt diese Variable auf ON, wenn ihr Teil b bearbeitet
option(teil_b "use SFML for part b" ON)

# Prepare source files for a_stern
set(a_stern_sources src/a_stern.cpp ${a_stern_unit})

# Add SFML visualizer sources if teil_b is enabled
if(teil_b)
  list(APPEND a_stern_sources src/sfml_visualizer.cpp)
  message(STATUS "SFML visualization enabled - adding sfml_visualizer.cpp")
else()
  message(STATUS "SFML visualization disabled - text-only mode")
endif()

# Create main executable
add_executable(a_stern ${a_stern_sources})

# ##############################################################################
# Compiler settings and definitions
# ##############################################################################

set_target_properties(a_stern PROPERTIES 
  CXX_STANDARD 17 
  CXX_STANDARD_REQUIRED ON 
  CXX_EXTENSIONS OFF
)

target_compile_options(
  a_stern
  PRIVATE
    $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:-Wall
    -Wextra
    -Wpedantic>
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
)

target_include_directories(a_stern PRIVATE "${mapra_SOURCE_DIR}/include/")

# Define SFML_ENABLED preprocessor macro when SFML is used
if(teil_b)
  target_compile_definitions(a_stern PRIVATE SFML_ENABLED)
  message(STATUS "SFML_ENABLED macro defined for conditional compilation")
endif()

# ##############################################################################
# SFML Linking
# ##############################################################################

if(${teil_b})
  # Try to find system SFML first
  find_package(PkgConfig QUIET)
  if(PKG_CONFIG_FOUND)
      pkg_check_modules(SFML_SYSTEM QUIET sfml-all>=2.5)
  endif()
  
  if(SFML_SYSTEM_FOUND)
      message(STATUS "Using system SFML")
      target_link_libraries(a_stern PRIVATE ${SFML_SYSTEM_LIBRARIES})
      target_include_directories(a_stern PRIVATE ${SFML_SYSTEM_INCLUDE_DIRS})
      set(SFML_FOUND TRUE)
  else()
      # Fall back to downloaded SFML
      if((UNIX AND NOT APPLE) AND CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
          message(STATUS "Using downloaded SFML for GNU/Linux")
          target_include_directories(a_stern PRIVATE "${sfml_SOURCE_DIR}/include/")
          target_link_libraries(
            a_stern
            PRIVATE "${sfml_SOURCE_DIR}/lib/libsfml-graphics.so"
                    "${sfml_SOURCE_DIR}/lib/libsfml-window.so"
                    "${sfml_SOURCE_DIR}/lib/libsfml-system.so")
      else()
          message(STATUS "Using downloaded SFML via CMake targets")
          target_link_libraries(a_stern PRIVATE sfml-graphics sfml-window sfml-system)
      endif()
  endif()

  # ############################################################################
  # SFML Test Executable
  # ############################################################################

  add_executable(sfml_test "src/sfml_test.cpp")

  set_target_properties(
    sfml_test PROPERTIES 
    CXX_STANDARD 17 
    CXX_STANDARD_REQUIRED ON 
    CXX_EXTENSIONS OFF
  )
  
  target_compile_options(
    sfml_test
    PRIVATE
      $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:GNU>>:-Wall
      -Wextra
      -Wpedantic>
      $<$<CXX_COMPILER_ID:MSVC>: /W4>
  )
  
  target_include_directories(sfml_test PRIVATE "${mapra_SOURCE_DIR}/include/")

  # Link sfml_test - use system SFML if available
  if(SFML_SYSTEM_FOUND)
      message(STATUS "Linking sfml_test to system SFML")
      target_link_libraries(sfml_test PRIVATE ${SFML_SYSTEM_LIBRARIES})
      target_include_directories(sfml_test PRIVATE ${SFML_SYSTEM_INCLUDE_DIRS})
  else()
      # Fall back to downloaded SFML
      if((UNIX AND NOT APPLE) AND CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
          target_include_directories(sfml_test PRIVATE "${sfml_SOURCE_DIR}/include/")
          target_link_libraries(
            sfml_test
            PRIVATE "${sfml_SOURCE_DIR}/lib/libsfml-graphics.so"
                    "${sfml_SOURCE_DIR}/lib/libsfml-window.so"
                    "${sfml_SOURCE_DIR}/lib/libsfml-system.so")
      else()
          target_link_libraries(sfml_test PRIVATE sfml-graphics sfml-window sfml-system)
      endif()
  endif()

  message(STATUS "SFML test executable created")
endif()

# ##############################################################################
# Summary
# ##############################################################################

message(STATUS "=== Build Configuration Summary ===")
message(STATUS "teil_b (SFML): ${teil_b}")
if(teil_b)
  message(STATUS "SFML visualization will be available")
  message(STATUS "Use option 2 in the program for graphical visualization")
else()
  message(STATUS "Text-only visualization will be used")
endif()
message(STATUS "=====================================")