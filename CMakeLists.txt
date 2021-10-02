cmake_minimum_required(VERSION 3.15)

project(CrabNet)
set(CMAKE_CXX_STANDARD 11)

#idk
if (NOT APPLE)
    # check 64 bit
    if (CMAKE_SIZEOF_VOID_P MATCHES "4")
        set(HAVE_64_BIT 0)
    else (CMAKE_SIZEOF_VOID_P MATCHES "4")
        set(HAVE_64_BIT 1)
    endif (CMAKE_SIZEOF_VOID_P MATCHES "4")
endif (NOT APPLE)

#idk
IF (WIN32 AND NOT UNIX)
    set(PROGRAMFILESX86 $ENV{PROGRAMFILES})
    string(REPLACE "\\" "/" PROGRAMFILESX86 ${PROGRAMFILESX86})
ENDIF (WIN32 AND NOT UNIX)



# Add libraries.
if (WIN32 AND NOT UNIX)
    set(CRABNET_LIBRARY_LIBS ws2_32.lib)
else (WIN32 AND NOT UNIX)
    if (NOT ANDROID)
        set(CRABNET_LIBRARY_LIBS pthread)
    endif (NOT ANDROID)
endif (WIN32 AND NOT UNIX)

# Old add libraries.
#IF (WIN32 AND NOT UNIX)
#    set(CRABNET_LIBRARY_LIBS ws2_32.lib)
#    set(CRABNET_LIBRARY_SO_NAME RakNetDLL)
#ELSE (WIN32 AND NOT UNIX)
#    set(CRABNET_LIBRARY_LIBS pthread)
#    set(CRABNET_LIBRARY_SO_NAME raknet)
#ENDIF (WIN32 AND NOT UNIX)

# Debug postfix.
set(CMAKE_DEBUG_POSTFIX "_d")



# Options
option(CRABNET_ENABLE_SAMPLES "Generate RakNet sample projects." FALSE)
option(CRABNET_ENABLE_PVS "Enable PVS Studio Analyzer." FALSE)
option(CRABNET_ENABLE_LIBCAT_SECURITY "Enable secure connection support." FALSE)

# Dynamic Link Library
option(CRABNET_ENABLE_DLL "Generate the Dynamic Link Library project." TRUE)
set(CRABNET_DLL_NAME "RakNet" CACHE STRING "Dynamic Link Library project name.")

# Static Link Librart
option(CRABNET_ENABLE_STATIC "Generate the Static Link Library project." TRUE)
set( CRABNET_STATIC_NAME "RakNet_static" CACHE STRING "Static Link Library project name.")



# Include Modules
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/CmakeIncludes)

include(CmakeMacros)
include(PVS-Studio)

if (MSVC)
    FIXLINKOPTIONS()
    FIXCOMPILEOPTIONS()
endif (MSVC)


include_directories(SYSTEM include/raknet DependentExtensions Source)
add_subdirectory(lib)

set(RAKNET_COMMON_LIBS ${CRABNET_STATIC_NAME})

# Add samples
if (CRABNET_ENABLE_SAMPLES)
    add_subdirectory(Samples)
endif ()
