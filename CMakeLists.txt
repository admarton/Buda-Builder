cmake_minimum_required(VERSION 3.0.0)

macro(highlight msg)
    string(ASCII 27 Esc)
    message("${Esc}[1;34m${msg}${Esc}[m")
endmacro()

# ========== macro to find sdl2_image =============
macro(findsdl2lib)
    # SDL2 Library required
    find_package(SDL2 QUIET)
    if(NOT SDL2_FOUND)
        set(SDL2_IMAGE_SDL2_NOT_FOUND "Could NOT find SDL2 (SDL2 is required by SDL2_image).")
        if(SDL2_image_FIND_REQUIRED)
            message(FATAL_ERROR ${SDL2_IMAGE_SDL2_NOT_FOUND})
        else()
            if(NOT SDL2_image_FIND_QUIETLY)
                message(STATUS ${SDL2_IMAGE_SDL2_NOT_FOUND})
            endif()
            return()
        endif()
        unset(SDL2_IMAGE_SDL2_NOT_FOUND)
    endif()


    # Define options for searching SDL2_image Library in a custom path

    set(SDL2_IMAGE_PATH "" CACHE STRING "Custom SDL2_image Library path")

    set(_SDL2_IMAGE_NO_DEFAULT_PATH OFF)
    if(SDL2_IMAGE_PATH)
    set(_SDL2_IMAGE_NO_DEFAULT_PATH ON)
    endif()

    set(SDL2_IMAGE_NO_DEFAULT_PATH ${_SDL2_IMAGE_NO_DEFAULT_PATH}
        CACHE BOOL "Disable search SDL2_image Library in default path")
    unset(_SDL2_IMAGE_NO_DEFAULT_PATH)

    set(SDL2_IMAGE_NO_DEFAULT_PATH_CMD)
    if(SDL2_IMAGE_NO_DEFAULT_PATH)
    set(SDL2_IMAGE_NO_DEFAULT_PATH_CMD NO_DEFAULT_PATH)
    endif()

    # Search for the SDL2_image include directory
    find_path(SDL2_IMAGE_INCLUDE_DIR SDL_image.h
    HINTS
        ENV SDL2IMAGEDIR
        ENV SDL2DIR
        ${SDL2_IMAGE_NO_DEFAULT_PATH_CMD}
    PATH_SUFFIXES SDL2
                    # path suffixes to search inside ENV{SDL2DIR}
                    # and ENV{SDL2IMAGEDIR}
                    include/SDL2 include
    PATHS ${SDL2_IMAGE_PATH}
    DOC "Where the SDL2_image headers can be found"
    )

    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(VC_LIB_PATH_SUFFIX lib/x64)
    else()
    set(VC_LIB_PATH_SUFFIX lib/x86)
    endif()

    # Search for the SDL2_image library
    find_library(SDL2_IMAGE_LIBRARY
    NAMES SDL2_image
    HINTS
        ENV SDL2IMAGEDIR
        ENV SDL2DIR
        ${SDL2_IMAGE_NO_DEFAULT_PATH_CMD}
    PATH_SUFFIXES lib ${VC_LIB_PATH_SUFFIX}
    PATHS ${SDL2_IMAGE_PATH}
    DOC "Where the SDL2_image Library can be found"
    )

    # Read SDL2_image version
    if(SDL2_IMAGE_INCLUDE_DIR AND EXISTS "${SDL2_IMAGE_INCLUDE_DIR}/SDL_image.h")
    file(STRINGS "${SDL2_IMAGE_INCLUDE_DIR}/SDL_image.h" SDL2_IMAGE_VERSION_MAJOR_LINE REGEX "^#define[ \t]+SDL_IMAGE_MAJOR_VERSION[ \t]+[0-9]+$")
    file(STRINGS "${SDL2_IMAGE_INCLUDE_DIR}/SDL_image.h" SDL2_IMAGE_VERSION_MINOR_LINE REGEX "^#define[ \t]+SDL_IMAGE_MINOR_VERSION[ \t]+[0-9]+$")
    file(STRINGS "${SDL2_IMAGE_INCLUDE_DIR}/SDL_image.h" SDL2_IMAGE_VERSION_PATCH_LINE REGEX "^#define[ \t]+SDL_IMAGE_PATCHLEVEL[ \t]+[0-9]+$")
    string(REGEX REPLACE "^#define[ \t]+SDL_IMAGE_MAJOR_VERSION[ \t]+([0-9]+)$" "\\1" SDL2_IMAGE_VERSION_MAJOR "${SDL2_IMAGE_VERSION_MAJOR_LINE}")
    string(REGEX REPLACE "^#define[ \t]+SDL_IMAGE_MINOR_VERSION[ \t]+([0-9]+)$" "\\1" SDL2_IMAGE_VERSION_MINOR "${SDL2_IMAGE_VERSION_MINOR_LINE}")
    string(REGEX REPLACE "^#define[ \t]+SDL_IMAGE_PATCHLEVEL[ \t]+([0-9]+)$" "\\1" SDL2_IMAGE_VERSION_PATCH "${SDL2_IMAGE_VERSION_PATCH_LINE}")
    set(SDL2_IMAGE_VERSION_STRING ${SDL2_IMAGE_VERSION_MAJOR}.${SDL2_IMAGE_VERSION_MINOR}.${SDL2_IMAGE_VERSION_PATCH})
    unset(SDL2_IMAGE_VERSION_MAJOR_LINE)
    unset(SDL2_IMAGE_VERSION_MINOR_LINE)
    unset(SDL2_IMAGE_VERSION_PATCH_LINE)
    unset(SDL2_IMAGE_VERSION_MAJOR)
    unset(SDL2_IMAGE_VERSION_MINOR)
    unset(SDL2_IMAGE_VERSION_PATCH)
    endif()

    set(SDL2_IMAGE_LIBRARIES ${SDL2_IMAGE_LIBRARY})
    set(SDL2_IMAGE_INCLUDE_DIRS ${SDL2_IMAGE_INCLUDE_DIR})

    include(FindPackageHandleStandardArgs)

    FIND_PACKAGE_HANDLE_STANDARD_ARGS(SDL2_image
                                    REQUIRED_VARS SDL2_IMAGE_LIBRARIES SDL2_IMAGE_INCLUDE_DIRS
                                    VERSION_VAR SDL2_IMAGE_VERSION_STRING)


    mark_as_advanced(SDL2_IMAGE_PATH
                    SDL2_IMAGE_NO_DEFAULT_PATH
                    SDL2_IMAGE_LIBRARY
                    SDL2_IMAGE_INCLUDE_DIR)


    if(SDL2_IMAGE_FOUND)

    # SDL2::Image target
    if(SDL2_IMAGE_LIBRARY AND NOT TARGET SDL2::Image)
        add_library(SDL2::Image UNKNOWN IMPORTED)
        set_target_properties(SDL2::Image PROPERTIES
                            IMPORTED_LOCATION "${SDL2_IMAGE_LIBRARY}"
                            INTERFACE_INCLUDE_DIRECTORIES "${SDL2_IMAGE_INCLUDE_DIR}"
                            INTERFACE_LINK_LIBRARIES SDL2::Core)
    endif()
    endif()
endmacro()
# =================================================

# Name of the project identical to directory name
get_filename_component(ProjectName ${CMAKE_CURRENT_SOURCE_DIR} NAME)
set(CMAKE_CXX_STANDARD 17)

project(CG_PROJ CXX)

set(local_dependencies "")

# Find MyApp library
if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${ProjectName}/MyApp.cpp)
    add_library(myapp ${ProjectName}/MyApp.cpp)
    list(APPEND local_dependencies myapp)
endif()

# Find imgui library
file(GLOB IMGUI_SRC "$ENV{OGLPACK_PATH}/include/imgui/*.cpp")
add_library(imgui ${IMGUI_SRC})
list(APPEND local_dependencies imgui)

# ============ other project-dependent files ======
if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${ProjectName}/gCamera.cpp")
    add_library(camera ${ProjectName}/gCamera.cpp)
    list(APPEND local_dependencies camera)
endif()

if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${ProjectName}/GLDebugMessageCallback.cpp")
    add_library(callback ${ProjectName}/GLDebugMessageCallback.cpp)
    list(APPEND local_dependencies callback)
endif()

if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${ProjectName}/includes)
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/${ProjectName}/includes)
    FILE(GLOB HELPER_SRC "${CMAKE_CURRENT_SOURCE_DIR}/${ProjectName}/includes/*.cpp")
    add_library(helpers SHARED ${HELPER_SRC})
    list(APPEND local_dependencies helpers)
endif()

if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${ProjectName}/Mesh_OGL3.cpp")
    add_library(objparser ${ProjectName}/Mesh_OGL3.cpp ${ProjectName}/ObjParser_OGL3.cpp)
    list(APPEND local_dependencies objparser)
endif()

# other resources
FILE(GLOB INL_SRC "${CMAKE_CURRENT_SOURCE_DIR}/${ProjectName}/*.cpp")
if ( INL_SRC )
    add_library(inlobjects ${INL_SRC})
    list(APPEND local_dependencies inlobjects)
endif()
# =================================================


set(OpenGL_GL_PREFERENCE LEGACY)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/)

# required installed libraries
find_package(SDL2 REQUIRED)
findsdl2lib()
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS})
include_directories(${SDL2_IMAGE_INCLUDE_DIRS})
include_directories(${GLEW_INCLUDE_DIRS})
include_directories(${OPENGL_INCLUDE_DIRS})

# ===== OGLPACK_PATH environtal var must be set ===
if (NOT EXISTS "$ENV{OGLPACK_PATH}/include") 
    message(FATAL_ERROR "OGLPACK_PATH environmental variable must be set")
endif()
include_directories("$ENV{OGLPACK_PATH}/include")

add_executable(CG_PROJ ${ProjectName}/main.cpp)

add_dependencies(CG_PROJ ${local_dependencies})

message(${SDL2_IMAGE_INCLUDE_DIRS})

target_link_libraries(CG_PROJ ${local_dependencies} ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${GLEW_LIBRARIES} ${OPENGL_LIBRARIES})
target_compile_options(CG_PROJ PRIVATE -Wall -Wextra -Wpedantic)

file(GLOB frag_shaders "${ProjectName}/*.frag" "${ProjectName}/*.vert" "${ProjectName}/*.jpg" "${ProjectName}/*.png" "${ProjectName}/*.bmp" "${ProjectName}/*.obj" "${ProjectName}/assets")

if (frag_shaders)
    highlight("\nCreating symlink of resources")

    foreach(shader ${frag_shaders})
        get_filename_component(FNAME ${shader} NAME)
        message("${FNAME}")
        
        add_custom_command(
        TARGET CG_PROJ POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E create_symlink              
            ${shader} $<TARGET_FILE_DIR:CG_PROJ>/${FNAME}
            MAIN_DEPENDENCY ${ProjectName}/MyApp.cpp
    )
    endforeach()

    highlight("---------------------------\n")
endif()
