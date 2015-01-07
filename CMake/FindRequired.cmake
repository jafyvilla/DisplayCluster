# generated by Buildyard, do not edit. Sets FOUND_REQUIRED if all
# required dependencies are found. Used by Buildyard.cmake
set(FIND_REQUIRED_FAILED)
# Copyright (c) 2014 Stefan.Eilemann@epfl.ch

# Provides common_package(Name args) which improves find_package.
# First invokes find_package with all the given arguments, and then
# falls back to using pkg_config if available. The pkg_config path
# does only implement the version, REQUIRED and QUIET find_package
# arguments (e.g. no COMPONENTS)

find_package(PkgConfig)
set(ENV{PKG_CONFIG_PATH}
  "${CMAKE_INSTALL_PREFIX}/lib/pkgconfig:$ENV{PKG_CONFIG_PATH}")

macro(COMMON_PACKAGE Name)
  string(TOUPPER ${Name} COMMON_PACKAGE_NAME)
  set(COMMON_PACKAGE_ARGS ${ARGN}) # ARGN is not a list. make one.
  set(COMMON_PACKAGE_VERSION)

  if(COMMON_PACKAGE_ARGS)
    list(GET COMMON_PACKAGE_ARGS 0 COMMON_PACKAGE_VERSION)
    if(COMMON_PACKAGE_VERSION MATCHES "^[0-9.]+$") # is a version
      set(COMMON_PACKAGE_VERSION ">=${COMMON_PACKAGE_VERSION}")
    else()
      set(COMMON_PACKAGE_VERSION)
    endif()
  endif()

  list(FIND COMMON_PACKAGE_ARGS "QUIET" COMMON_PACKAGE_QUIET_POS)
  if(COMMON_PACKAGE_QUIET_POS EQUAL -1)
    set(COMMON_PACKAGE_QUIET)
  else()
    set(COMMON_PACKAGE_QUIET "QUIET")
  endif()

  list(FIND COMMON_PACKAGE_ARGS "REQUIRED" COMMON_PACKAGE_REQUIRED_POS)
  if(COMMON_PACKAGE_REQUIRED_POS EQUAL -1) # Optional find
    find_package(${Name} ${COMMON_PACKAGE_ARGS}) # try standard cmake way
    if((NOT ${Name}_FOUND) AND (NOT ${COMMON_PACKAGE_NAME}_FOUND) AND PKG_CONFIG_EXECUTABLE)
      pkg_check_modules(${Name} ${Name}${COMMON_PACKAGE_VERSION}
        ${COMMON_PACKAGE_QUIET}) # try pkg_config way
    endif()
  else() # required find
    list(REMOVE_AT COMMON_PACKAGE_ARGS ${COMMON_PACKAGE_REQUIRED_POS})
    find_package(${Name} ${COMMON_PACKAGE_ARGS}) # try standard cmake way
    if((NOT ${Name}_FOUND) AND (NOT ${COMMON_PACKAGE_NAME}_FOUND) AND PKG_CONFIG_EXECUTABLE)
      pkg_check_modules(${Name} REQUIRED ${Name}${COMMON_PACKAGE_VERSION}
        ${COMMON_PACKAGE_QUIET}) # try pkg_config way (and fail if needed)
    endif()
  endif()
endmacro()

common_package(Boost 1.41.0 COMPONENTS program_options date_time serialization unit_test_framework regex system thread QUIET)
if(NOT Boost_FOUND AND NOT BOOST_FOUND)
  set(FIND_REQUIRED_FAILED "${FIND_REQUIRED_FAILED} Boost")
endif()
common_package(Deflect 0.4  QUIET)
if(NOT Deflect_FOUND AND NOT DEFLECT_FOUND)
  set(FIND_REQUIRED_FAILED "${FIND_REQUIRED_FAILED} Deflect")
endif()
common_package(FCGI   QUIET)
if(NOT FCGI_FOUND AND NOT FCGI_FOUND)
  set(FIND_REQUIRED_FAILED "${FIND_REQUIRED_FAILED} FCGI")
endif()
common_package(FFMPEG   QUIET)
if(NOT FFMPEG_FOUND AND NOT FFMPEG_FOUND)
  set(FIND_REQUIRED_FAILED "${FIND_REQUIRED_FAILED} FFMPEG")
endif()
common_package(MPI   QUIET)
if(NOT MPI_FOUND AND NOT MPI_FOUND)
  set(FIND_REQUIRED_FAILED "${FIND_REQUIRED_FAILED} MPI")
endif()
common_package(OpenGL   QUIET)
if(NOT OpenGL_FOUND AND NOT OPENGL_FOUND)
  set(FIND_REQUIRED_FAILED "${FIND_REQUIRED_FAILED} OpenGL")
endif()
common_package(Qt4 4.6 COMPONENTS QtCore QtGui QtNetwork QtOpenGL QtXml QtXmlPatterns QtSvg QtWebKit QUIET)
if(NOT Qt4_FOUND AND NOT QT4_FOUND)
  set(FIND_REQUIRED_FAILED "${FIND_REQUIRED_FAILED} Qt4")
endif()

if(FIND_REQUIRED_FAILED)
  set(FOUND_REQUIRED FALSE)
else()
  set(FOUND_REQUIRED TRUE)
endif()