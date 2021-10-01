FILE(GLOB ALL_CPP_SRCS
        ${CrabNet_SOURCE_DIR}/Source/*.cpp
        ${CrabNet_SOURCE_DIR}/Source/Utils/*.cpp
        ${CrabNet_SOURCE_DIR}/Source/Plugins/*.cpp
        ${CrabNet_SOURCE_DIR}/include/raknet/*.h
        )

# idk
set(CMAKE_CXX_FLAGS_DEBUG -D_DEBUG)

if (MSVC)
    add_definitions("/D WIN32 /D _CRT_NONSTDC_NO_DEPRECATE /D _CRT_SECURE_NO_DEPRECATE /GS- /GR-")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd")
endif (MSVC)


# Create Static Link Library project.
if (CRABNET_ENABLE_STATIC)
    add_library(${CRABNET_STATIC_NAME} STATIC ${ALL_CPP_SRCS})
    target_link_libraries(${CRABNET_STATIC_NAME} ${CRABNET_LIBRARY_LIBS})

    if (CRABNET_ENABLE_LIBCAT_SECURITY)
        add_definitions("-DLIBCAT_SECURITY")
    endif ()

    if (MSVC)
        #set_target_properties(${CRABNET_STATIC_NAME} PROPERTIES STATIC_LIBRARY_FLAGS "/NODEFAULTLIB:\"LIBCD.lib LIBCMTD.lib MSVCRT.lib\"" )
    endif (MSVC)

    #install(TARGETS ${CRABNET_STATIC_NAME} DESTINATION lib)
endif ()

#install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/
#        DESTINATION include
#        FILES_MATCHING PATTERN "*.h*"
#        )

# Create Dynamic Link Library project
if (CRABNET_ENABLE_DLL)
    add_library(${CRABNET_DLL_NAME} SHARED ${ALL_CPP_SRCS})
    target_link_libraries(${CRABNET_DLL_NAME} ${CRABNET_LIBRARY_LIBS})
    target_compile_definitions(${CRABNET_DLL_NAME} PUBLIC "_CRABNET_DLL")

    if (CRABNET_ENABLE_LIBCAT_SECURITY)
        add_definitions("-DLIBCAT_SECURITY")
    endif ()

    #SET_TARGET_PROPERTIES(${CRABNET_LIBRARY_SO_NAME} PROPERTIES SOVERSION 4.081)
    #install(TARGETS ${CRABNET_DLL_NAME} DESTINATION lib)
endif ()

# Enable PVS Studio Analyzer
if (CRABNET_ENABLE_PVS)
    pvs_studio_add_target(TARGET crabnet.analyze ALL
            OUTPUT FORMAT errorfile
            ANALYZE ${CRABNET_STATIC_NAME}
            LOG target.err)
endif ()