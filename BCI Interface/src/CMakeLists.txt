cmake_minimum_required(VERSION 2.6)

include_directories(../include)
file(GLOB HEADERS ../include/*.h*)
file(GLOB SRCS *.cpp)

#==================================================

FIND_PACKAGE(GTK2 COMPONENTS gtk glade )
FIND_PACKAGE(PkgConfig)
PKG_CHECK_MODULES (DATABOX gtkdatabox)
pkg_check_modules(GSTREAMERMM REQUIRED gstreamermm-0.10)

INCLUDE_DIRECTORIES(${GTK2_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${DATABOX_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(${GSTREAMERMM_INCLUDE_DIRS})

add_executable (

	BCI_Interface	
	${SRCS}
	${HEADERS}
	${GHEADERS}
)

target_link_libraries( BCI_Interface fftw3 bioamp  ${GTK2_LIBRARIES} ${DATABOX_LIBRARIES} ${GSTREAMERMM_LIBRARIES})
