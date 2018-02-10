cmake_minimum_required(VERSION 2.8)

project(errut)
set(VERSION 1.0.1)

set (_DEFAULT_LIBRARY_INSTALL_DIR lib)
if (EXISTS "${CMAKE_INSTALL_PREFIX}/lib32/" AND CMAKE_SIZEOF_VOID_P EQUAL 4)
	set (_DEFAULT_LIBRARY_INSTALL_DIR lib32)
elseif (EXISTS "${CMAKE_INSTALL_PREFIX}/lib64/" AND CMAKE_SIZEOF_VOID_P EQUAL 8)
	set (_DEFAULT_LIBRARY_INSTALL_DIR lib64)
endif ()

set(LIBRARY_INSTALL_DIR "${_DEFAULT_LIBRARY_INSTALL_DIR}" CACHE PATH "Library installation directory")
if(NOT IS_ABSOLUTE "${LIBRARY_INSTALL_DIR}")
	set(LIBRARY_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/${LIBRARY_INSTALL_DIR}")
endif()

if (NOT UNIX)
	set(ERRUT_COMPILE_DLL OFF CACHE BOOL "Flag indicating if a DLL should be built")
endif (NOT UNIX)

if (ERRUT_COMPILE_DLL)
	set(ERRUT_DLLSTUFF "#ifdef ERRUT_COMPILING\n#define ERRUT_IMPORTEXPORT __declspec(dllexport)\n#else\n#define ERRUT_IMPORTEXPORT __declspec(dllimport)\n#endif")
	add_definitions(-DERRUT_COMPILING)
	set(CMAKE_DEBUG_POSTFIX _d)

	add_library(errut-dll SHARED src/errutcode.cpp ${HEADERS})
	include_directories("${PROJECT_BINARY_DIR}/src")
	set_target_properties(errut-dll PROPERTIES OUTPUT_NAME errut)
	get_target_property(ERRUT_LOCAL_LIBRARY_NAME errut-dll RELEASE_LOCATION)
	get_filename_component(ERRUT_LIBNAME_RELEASE "${ERRUT_LOCAL_LIBRARY_NAME}" NAME_WE)
	get_target_property(ERRUT_LOCAL_LIBRARY_NAME errut-dll DEBUG_LOCATION)
	get_filename_component(ERRUT_LIBNAME_DEBUG "${ERRUT_LOCAL_LIBRARY_NAME}" NAME_WE)
	set(ERRUT_LIBS "optimized \"${LIBRARY_INSTALL_DIR}/${ERRUT_LIBNAME_RELEASE}.lib\" debug \"${LIBRARY_INSTALL_DIR}/${ERRUT_LIBNAME_DEBUG}.lib\"")
	install(TARGETS errut-dll DESTINATION ${LIBRARY_INSTALL_DIR})
else (ERRUT_COMPILE_DLL)
	set(ERRUT_DLLSTUFF "#define ERRUT_IMPORTEXPORT")
	set(ERRUT_LIBS "\"\"")
endif (ERRUT_COMPILE_DLL)

configure_file("${PROJECT_SOURCE_DIR}/src/errorbase.h.in" "${PROJECT_BINARY_DIR}/src/errorbase.h")

set(HEADERS
	"${PROJECT_BINARY_DIR}/src/errorbase.h"
	)

install(FILES ${HEADERS} DESTINATION include/errut)
configure_file("${PROJECT_SOURCE_DIR}/cmake/ErrUtConfig.cmake.in" "${PROJECT_BINARY_DIR}/cmake/ErrUtConfig.cmake")
install(FILES "${PROJECT_BINARY_DIR}/cmake/ErrUtConfig.cmake" 
	DESTINATION ${LIBRARY_INSTALL_DIR}/cmake/ErrUt)

