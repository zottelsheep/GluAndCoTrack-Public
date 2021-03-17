cmake_minimum_required(VERSION 3.17)
project(GluAndCoTrack VERSION 1.2)
set(CMAKE_CXX_STANDARD 17)

## Main Executable
add_executable(${PROJECT_NAME} "")

## Set Optimisation Flags for Relase Build
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

## Set Compile Options for FLTK
target_compile_definitions(${PROJECT_NAME}
        PRIVATE
        _LARGEFILE64_SOURCE
        _LARGEFILE_SOURCE
        _FILE_OFFSET_BITS=64
        _REENTRANT
        _THREAD_SAFE
        )

## Include Files
target_include_directories(${PROJECT_NAME} PRIVATE include/ icons/)

## Build Main Source-Files
FILE(GLOB SRCS "src/*.cpp")
target_sources(${PROJECT_NAME} PRIVATE ${SRCS})

## Build Classes
FILE(GLOB SRCS_ClASS "src/Classes/*.cpp")
target_sources(${PROJECT_NAME} PRIVATE ${SRCS_ClASS})

## Link SQLite and if not found build it
find_package(SQLite3)

if(${SQLite3_FOUND})
    message("Using System Lib of SQLite3")
    target_include_directories(${PROJECT_NAME} PRIVATE ${SQLite3_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} ${SQLite3_LIBRARIES})

else()
    message("Using Built-in Lib of SQLite3")
    add_subdirectory(dependencies/sqlite3)
    target_link_libraries(${PROJECT_NAME} sqlite3)
    target_include_directories(${PROJECT_NAME} PRIVATE dependencies/sqlite3)
endif()


## Link FLTK
set(FLTK_SKIP_FORMS true)
set(FLTK_SKIP_FLUID true)

set(FLTK_DIR "C:/Program Files/FLTK/" CACHE STRING "Location of FLTK Installation")
message("Using FLTK_DIR: " ${FLTK_DIR})

find_package(FLTK REQUIRED NO_MODULE PATHS ${FLTK_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ${FLTK_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} fltk_images fltk_png z fltk pthread fontconfig)

## WINDOWS SPECIFIC
if(WIN32)
    if(CMAKE_BUILD_TYPE MATCHES "Release")
        # Removes terminal from popping up in Windows when using Release-Version
        target_link_options(${PROJECT_NAME} PRIVATE "-mwindows")
    endif()
    # Adds icon, version and other information to executable
    configure_file(icons/icon.rc.in icon.rc)
    target_sources(${PROJECT_NAME} PRIVATE icon.rc)

    ## Installer
    option(USE_NSIS "Use NSIS generator to produce installer" OFF)

    # Custom target for packaging.
    if(USE_NSIS)
        set(CMAKE_CPACK_GENERATOR "NSIS")
    else(USE_NSIS)
        set(CMAKE_CPACK_GENERATOR "ZIP")
    endif(USE_NSIS)

    # Install config
    INSTALL(TARGETS ${PROJECT_NAME} )

    #CPACK Config
    include(InstallRequiredSystemLibraries)
    set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/License.md")
    set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
    set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
    include(CPack)

endif (WIN32)

