cmake_minimum_required (VERSION 2.8.3)

project("Oscope")

#message(FATAL_ERROR "current source dir ${CMAKE_CURRENT_SOURCE_DIR}")

if(NOT IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/externals/nanogui")
  message(FATAL_ERROR "The Oscope dependency repositories (nanogui, etc.) are missing! "
    "You probably did not clone the project with --recursive. It is possible to recover "
    "by calling \"git submodule update --init --recursive\"")
endif()

option(OSCOPE_BUILD_SHARED  "Build NanoGUI as a shared library?" ON)


# Required libraries for linking against nanogui (all targets)
set(OSCOPE_EXTRA_LIBS "")

# Platform-dependent files for liboscope
set(LIBOSCOPE_EXTRA_SOURCE "")
set(OSCOPE_EXTRA_SOURCE "")

if(APPLE AND OSCOPE_BUILD_SHARED)
  set(CMAKE_MACOSX_RPATH ON)
endif()

include(CheckCXXCompilerFlag)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to 'Release' as none was specified.")
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release"
    "MinSizeRel" "RelWithDebInfo")
endif()
string(TOUPPER "${CMAKE_BUILD_TYPE}" U_CMAKE_BUILD_TYPE)

set(BUILD_SHARED_LIBS ${OSCOPE_BUILD_SHARED} CACHE BOOL " " FORCE)

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  # Quench annoying deprecation warnings when compiling GLFW on OSX
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-deprecated-declarations")
endif()

# Compile NanoGUI
SET(NANOGUI_BUILD_EXAMPLE OFF CACHE BOOL "Turning off nanogui examples")
SET(NANOGUI_BUILD_PYTHON OFF CACHE BOOL "Turning off nanogui examples")

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/externals/nanogui" "externals_build/nanogui")
list(APPEND OSCOPE_EXTRA_LIBS nanogui)

# Shared library mode: add OSCOPE_SHARED flag to all targets
if (OSCOPE_BUILD_SHARED)
  add_definitions ("-DOSCOPE_SHARED")
endif()

if (MSVC)
  # Disable annoying MSVC warnings (all targets)
  add_definitions(/D "_CRT_SECURE_NO_WARNINGS")

  # Parallel build on MSVC (all targets)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")

  if (NOT CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /arch:SSE2")

    # Disable Eigen vectorization for Windows 32 bit builds (issues with unaligned access segfaults)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /DEIGEN_DONT_ALIGN")
  endif()
endif()

# Compile with compiler warnings turned on
if(MSVC)
  if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
    string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
  endif()
elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "GNU")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
endif()

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "GNU")
  CHECK_CXX_COMPILER_FLAG("-std=c++14" HAS_CPP14_FLAG)
  CHECK_CXX_COMPILER_FLAG("-std=c++11" HAS_CPP11_FLAG)

  if (HAS_CPP14_FLAG)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
  elseif (HAS_CPP11_FLAG)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
  else()
    message(FATAL_ERROR "Unsupported compiler -- pybind11 requires C++11 support!")
  endif()
endif()

if (WIN32)
  # Build and include GLAD on Windows
  list(APPEND LIBOSCOPE_EXTRA_SOURCE
     "${CMAKE_CURRENT_SOURCE_DIR}/ext/glad/src/glad.c"
	 "${CMAKE_CURRENT_SOURCE_DIR}/ext/glad/include/glad/glad.h"
	 "${CMAKE_CURRENT_SOURCE_DIR}/ext/glad/include/KHR/khrplatform.h")
  set_source_files_properties("${CMAKE_CURRENT_SOURCE_DIR}/ext/glad/src/glad.c" PROPERTIES COMPILE_DEFINITIONS GLAD_GLAPI_EXPORT_BUILD)
  if (MSVC)
    set_source_files_properties("${CMAKE_CURRENT_SOURCE_DIR}/ext/glad/src/glad.c" PROPERTIES COMPILE_FLAGS "/wd4055 ")
  endif()
  include_directories(ext/glad/include)
endif()

# Required core libraries on various platforms
if (WIN32)
  list(APPEND OSCOPE_EXTRA_LIBS opengl32)
elseif (APPLE)
  find_library(cocoa_library Cocoa)
  find_library(opengl_library OpenGL)
  find_library(corevideo_library CoreVideo)
  find_library(iokit_library IOKit)
  list(APPEND OSCOPE_EXTRA_LIBS ${cocoa_library} ${opengl_library} ${corevideo_library} ${iokit_library})
  list(APPEND LIBOSCOPE_EXTRA_SOURCE src/app/ScreenDimensions.mm)
elseif(CMAKE_SYSTEM MATCHES "Linux")
  list(APPEND OSCOPE_EXTRA_LIBS GL Xxf86vm Xrandr Xinerama Xcursor Xi X11 pthread dl rt)
endif()



include_directories(externals/nanogui/include externals/nanogui/ext/eigen externals/nanogui/ext/glfw/include externals/nanogui/ext/nanovg/src include ${CMAKE_CURRENT_BINARY_DIR})

# Needed to generated files
include_directories(${CMAKE_CURRENT_BINARY_DIR})

# Set library type
if (OSCOPE_BUILD_SHARED)
  set(OSCOPE_LIBRARY_TYPE "SHARED")
else()
  set(OSCOPE_LIBRARY_TYPE "STATIC")
endif()

# Compile main Oscope library
add_library(oscope ${OSCOPE_LIBRARY_TYPE}
  # Merge NanoGUI into the Oscope library
  #$<TARGET_OBJECTS:nanogui>
  # Merge NanoVG into the NanoGUI library
  #ext/nanovg/src/nanovg.c
  # Merge GLAD into the NanoGUI library (only if needed)
  ${LIBOSCOPE_EXTRA_SOURCE}
  # Fonts etc.
  include/oscope/version.h src/lib/version.cpp
)

# Compile/link flags for Oscope
set_property(TARGET oscope APPEND PROPERTY COMPILE_DEFINITIONS "OSCOPE_BUILD ")

if (OSCOPE_BUILD_SHARED)
  target_link_libraries(oscope ${OSCOPE_EXTRA_LIBS})
endif()

if (NOT ${U_CMAKE_BUILD_TYPE} MATCHES DEBUG AND OSCOPE_BUILD_SHARED)
  # Link-time code generation (only for shared library)
  if (MSVC)
    set_property(TARGET oscope APPEND_STRING PROPERTY LINK_FLAGS "/LTCG ")
    set_property(TARGET oscope APPEND_STRING PROPERTY COMPILE_FLAGS "/GL ")
  elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    set_property(TARGET oscope APPEND_STRING PROPERTY COMPILE_FLAGS "-fvisibility=hidden ")

    # Check for Link Time Optimization support
    CHECK_CXX_COMPILER_FLAG("-flto" HAS_LTO_FLAG)
    if (HAS_LTO_FLAG)
      set_property(TARGET oscope APPEND_STRING PROPERTY COMPILE_FLAGS "-flto ")
    endif()
  endif()
endif()

if (OSCOPE_BUILD_SHARED AND NOT ${U_CMAKE_BUILD_TYPE} MATCHES DEBUG)
  if (APPLE)
    # Strip .dylib library on OSX
    add_custom_command(TARGET oscope POST_BUILD COMMAND strip -u -r ${CMAKE_CURRENT_BINARY_DIR}/liboscope.dylib)
  elseif(UNIX)
    # Strip .so library on Linux
    add_custom_command(TARGET oscope POST_BUILD COMMAND strip ${CMAKE_CURRENT_BINARY_DIR}/liboscope.so)
  endif()
endif()

get_directory_property(OSCOPE_HAS_PARENT PARENT_DIRECTORY)
if(OSCOPE_HAS_PARENT)
  # This project is included from somewhere else. Export OSCOPE_EXTRA_LIBS variable
  set(OSCOPE_EXTRA_LIBS ${OSCOPE_EXTRA_LIBS} PARENT_SCOPE)
else()
  # Create documentation for python plugin (optional target for developers)
  add_custom_target(mkdoc COMMAND
    python3 ${CMAKE_CURRENT_SOURCE_DIR}/ext/pybind11/tools/mkdoc.py
    -Iinclude -Iext/nanovg/src -Iext/eigen -Iext/glfw/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include/nanogui/*.h > ${CMAKE_CURRENT_SOURCE_DIR}/python/py_doc.h)
endif()

list(APPEND OSCOPE_EXTRA_SOURCE src/app/ScreenDimensions.mm)

# Compile the application Oscope
add_executable(oscope_app 
  ${OSCOPE_EXTRA_SOURCE}
  src/app/main.cpp
  src/app/Application.hpp src/app/Application.cpp
  src/app/NonMovableWindow.hpp src/app/NonMovableWindow.cpp
)
target_link_libraries(oscope_app oscope ${OSCOPE_EXTRA_LIBS})
# Copy icons for example application
file(COPY resources/icons DESTINATION ${CMAKE_CURRENT_BINARY_DIR})


# vim: set et ts=2 sw=2 ft=cmake nospell:
