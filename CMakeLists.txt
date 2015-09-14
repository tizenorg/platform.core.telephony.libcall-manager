CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(call-manager C)

SET(PREFIX ${CMAKE_INSTALL_PREFIX})
SET(EXEC_PREFIX "\${prefix}")
SET(LIBDIR ${LIB_INSTALL_DIR})
SET(INCLUDEDIR "\${prefix}/include")
SET(CLIENT "call-manager")

INCLUDE(FindPkgConfig)
pkg_check_modules(client_pkgs REQUIRED
	glib-2.0
	gio-2.0
	dlog
	vconf
	capi-system-device
	capi-base-common
	capi-system-info
)

FOREACH(flag ${client_pkgs_CFLAGS})
	SET(EXTRA_CFLAGS "${EXTRA_CFLAGS} ${flag}")
ENDFOREACH(flag)

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${EXTRA_CFLAGS}")
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Werror -Wextra -fvisibility=hidden -fPIC -fdata-sections -ffunction-sections -Wl,--gc-sections")
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-unused-parameter -Wno-missing-field-initializers -Wmissing-declarations -Wcast-align")

SET(CLIENT_SRCS
	src/call-manager.c
)
IF(_ENABLE_EXT_FEATURE)
SET(CLIENT_SRCS ${CLIENT_SRCS}
	src/call-manager-product.c
)
ENDIF(_ENABLE_EXT_FEATURE)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include/)

ADD_LIBRARY(${CLIENT} SHARED ${CLIENT_SRCS})
TARGET_LINK_LIBRARIES(${CLIENT} ${client_pkgs_LDFLAGS})
SET_TARGET_PROPERTIES(${CLIENT} PROPERTIES VERSION 0.0.0 SOVERSION 0 OUTPUT_NAME ${CLIENT})

# pkgconfig file
CONFIGURE_FILE(callmgr_client.pc.in callmgr_client.pc @ONLY)

# install
INSTALL(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/ DESTINATION include/${CLIENT})
INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/callmgr_client.pc DESTINATION ${LIBDIR}/pkgconfig)
INSTALL(TARGETS ${CLIENT} DESTINATION ${LIBDIR})
INSTALL(FILES ${CMAKE_SOURCE_DIR}/LICENSE DESTINATION /usr/share/license RENAME libcall-manager)
ADD_SUBDIRECTORY(test)
