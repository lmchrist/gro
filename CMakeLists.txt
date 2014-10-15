cmake_minimum_required(VERSION 2.8)
set(PROJECT_NAME_STR gro)
project(${PROJECT_NAME_STR} C CXX)


# Add compiler flags --------------------------------
set(TB_C_FLAGS "")
set(TB_CXX_FLAGS "")

#add_definitions("-Wall -Wno-deprecated-declarations ")
   
if("${CMAKE_CXX_COMPILER_VERSION}" VERSION_GREATER "4.7")
	list(APPEND TB_CXX_FLAGS "-std=c++11 ")
else()
	if("${CMAKE_CXX_COMPILER_VERSION}" VERSION_GREATER "4.3")
		list(APPEND TB_CXX_FLAGS "-std=c++0x ")
        endif()
endif()

set(CMAKE_C_FLAGS "${TB_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${TB_C_FLAGS} ${TB_CXX_FLAGS}")


# Set up project -------------------------------------
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin CACHE PATH "Output directory for libraries")
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin CACHE PATH "Output directory for executables")

file(GLOB SRC_FILES_GRO ./src/*.cpp)
file (GLOB INCLUDE_FILES_GRO ${PROJECT_SOURCE_DIR}/include/*.h )

set(CURSES_NEED_NCURSES TRUE)
find_package(Curses)
if(CURSES_FOUND)
	include_directories(${CURSES_INCLUDE_DIR})
	MESSAGE("ncurses INCLUDE_DIR is: " ${CURSES_INCLUDE_DIR}) 
else(CURSES_FOUND)
	MESSAGE(FATAL_ERROR "ncurses not found - GRO won't compile")
endif(CURSES_FOUND)

add_library(gro
	${SRC_FILES_GRO}
)

set(COMMON_INCLUDES
	${PROJECT_SOURCE_DIR}/include/
	${CURSES_INCLUDE_DIR}
)

set(PROJECT_LINK_LIBS
	gro
	pthread
	${CURSES_LIBRARY}
)

include_directories(${COMMON_INCLUDES})
MESSAGE("Included directories are: " ${COMMON_INCLUDES})
MESSAGE("Linked Libraries are: " ${PROJECT_LINK_LIBS})	

set(PROJECT_LIB_NAME ${PROJECT_NAME_STR}_lib)
set(PROJECT_EXEC_NAME ${PROJECT_NAME_STR}_test)

add_executable(${PROJECT_EXEC_NAME} ${INCLUDE_FILES_GRO} ${PROJECT_SOURCE_DIR}/src/zmain.cpp)

target_link_libraries(${PROJECT_EXEC_NAME}
	${PROJECT_LINK_LIBS}
)

