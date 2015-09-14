CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(cm-test C)

################################################################################
# Set external libraries                                                       #
################################################################################
INCLUDE(FindPkgConfig)
pkg_check_modules(pkgs REQUIRED
        glib-2.0
        gobject-2.0
)

FOREACH(flag ${pkgs_CFLAGS})
        SET(EXTRA_CFLAGS "${EXTRA_CFLAGS} ${flag}")
ENDFOREACH(flag)

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/client/include/)
################################################################################
# Custom configure                                                             #
################################################################################
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${EXTRA_CFLAGS}")
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIE")
SET(CMAKE_C_FLAGS_DEBUG "-O0 -g")
SET(CMAKE_LDFLAGS "-Wl,-zdefs" )
SET(CMAKE_EXE_LINKER_FLAGS "-Wl,--as-needed -pie")

################################################################################
# Set source files                                                             #
################################################################################
SET(SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/test.c
)

################################################################################
# Set build target                                                             #
################################################################################
ADD_EXECUTABLE(${PROJECT_NAME} ${SRCS})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${pkgs_LDFLAGS} call-manager)

################################################################################
# Install Target                                                               #
################################################################################
INSTALL(TARGETS ${PROJECT_NAME} DESTINATION bin)

