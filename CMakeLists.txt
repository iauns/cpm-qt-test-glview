#-----------------------------------------------------------------------
# Compiler settings (C++11)
#-----------------------------------------------------------------------
if (UNIX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
  if (APPLE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -stdlib=libc++")
  endif ()
endif ()

#------------------------------------------------------------------------------
# Required CPM Setup - See: http://github.com/iauns/cpm
#------------------------------------------------------------------------------
set(CPM_MODULE_NAME "qt_glview")
set(CPM_LIB_TARGET_NAME ${CPM_MODULE_NAME})

if ((DEFINED CPM_DIR) AND (DEFINED CPM_UNIQUE_ID) AND (DEFINED CPM_TARGET_NAME))
  set(CPM_LIB_TARGET_NAME ${CPM_TARGET_NAME})
  set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CPM_DIR})
  include(CPM)
else()
  set (CPM_DIR "${CMAKE_CURRENT_BINARY_DIR}/cpm-packages" CACHE TYPE STRING)
  find_package(Git)
  if(NOT GIT_FOUND)
    message(FATAL_ERROR "CPM requires Git.")
  endif()
  if (NOT EXISTS ${CPM_DIR}/CPM.cmake)
    message(STATUS "Cloning repo (https://github.com/iauns/cpm)")
    execute_process(
      COMMAND "${GIT_EXECUTABLE}" clone https://github.com/iauns/cpm ${CPM_DIR}
      RESULT_VARIABLE error_code
      OUTPUT_QUIET ERROR_QUIET)
    if(error_code)
      message(FATAL_ERROR "CPM failed to get the hash for HEAD")
    endif()
  endif()
  include(${CPM_DIR}/CPM.cmake)
endif()

#-----------------------------------------------------------------------
# CPM Modules
#-----------------------------------------------------------------------

# ++ MODULE: Arc ball look-at camera
CPM_AddModule("look_at"
  GIT_REPOSITORY "https://github.com/iauns/cpm-arc-look-at"
  GIT_TAG "origin/master"
  EXPORT_MODULE TRUE)

# ++ MODULE: Spire
CPM_AddModule("spire"
  GIT_REPOSITORY "https://github.com/SCIInstitute/spire"
  GIT_TAG "origin/master"
  EXPORT_MODULE TRUE)

# ++ MODULE: GLM
CPM_AddModule("GLM"
  GIT_REPOSITORY "https://github.com/iauns/cpm-glm"
  GIT_TAG "origin/master"
  USE_EXISTING_VER TRUE
  EXPORT_MODULE TRUE    # Use EXPORT_MODULE sparingly. We expose GLM's interface
  )                     # through our own interface hence why we export it.

# ++ MODULE: Axis aligned bounding box
CPM_AddModule("glm_aabb"
  GIT_REPOSITORY "https://github.com/iauns/cpm-glm-aabb"
  GIT_TAG "1.0.1"
  USE_EXISTING_VER TRUE
  EXPORT_MODULE TRUE)

CPM_InitModule(${CPM_MODULE_NAME})

#-----------------------------------------------------------------------
# Configure OpenGL
#-----------------------------------------------------------------------
find_package(OpenGL REQUIRED)
set(QT_USE_QTOPENGL TRUE)

#-----------------------------------------------------------------------
# Compiler settings
#-----------------------------------------------------------------------

# Since moc and UIC happen outside of the source tree, we need to
# add the binary directory to the include path.
include_directories("${CMAKE_CURRENT_BINARY_DIR}")

#-----------------------------------------------------------------------
# Find and configure Qt
#-----------------------------------------------------------------------

set(QT_MIN_VERSION "4.7.0")
find_package(Qt4 REQUIRED)

if (QT_FOUND)
  include_directories(SYSTEM "${QT_INCLUDE_DIR}")
else ()
  message(FATAL_ERROR "QT 4.7 or greater is required to build Spire Viewer")
endif ()

include(${QT_USE_FILE})
add_definitions(${QT_DEFINITIONS})

#-----------------------------------------------------------------------
# Setup source
#-----------------------------------------------------------------------

file (GLOB Sources
  "test-glview/*.cpp"
  "test-glview/*.hpp"
  "test-glview/src/*.cpp"
  "test-glview/src/*.hpp"
  )

set(MocHeaders test-glview/src/MainWindow.h test-glview/src/GLWidget.h)
set(Forms test-glview/src/MainWindow.ui)

QT4_WRAP_CPP(MocSource ${MocHeaders})
QT4_WRAP_UI(UISource ${Forms})

#-----------------------------------------------------------------------
# Setup executable
#-----------------------------------------------------------------------

add_library(${CPM_LIB_TARGET_NAME} ${Sources} ${MocSource} ${UISource})
target_link_libraries(${CPM_LIB_TARGET_NAME}
  ${CPM_LIBRARIES}
  ${QT_OPENGL_LIBRARY}
  ${OPENGL_LIBRARIES}
  ${QT_LIBRARIES})

