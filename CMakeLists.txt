cmake_minimum_required(VERSION 2.8)
set(PROJECT_NAME_STR gro)
project(${PROJECT_NAME_STR} C CXX)


#linux flags

    # TB_C_FLAGS will be also used for C++
    set(TB_C_FLAGS "")
    set(TB_CXX_FLAGS "")
	#list(APPEND TB_C_FLAGS "")
    #set(CMAKE_EXE_LINKER_FLAGS    "")
    #set(CMAKE_MODULE_LINKER_FLAGS "")
    #set(CMAKE_SHARED_LINKER_FLAGS "")  # dynamic
    #set(CMAKE_STATIC_LINKER_FLAGS "")
    add_definitions(   # only use for -D flags applied to ALL files
        "-Wall -Wno-deprecated-declarations -pthread -rdynamic -g"  # TODO move options -W -mylib -g elsewhere
    )

    if("${CMAKE_CXX_COMPILER_VERSION}" VERSION_GREATER "4.7")
		list(APPEND TB_CXX_FLAGS "-std=c++11 ")
	else()
        if("${CMAKE_CXX_COMPILER_VERSION}" VERSION_GREATER "4.3")
		    list(APPEND TB_CXX_FLAGS "-std=c++0x ")
        endif()
    endif()

    # now, assign TB_<LANG>_FLAGS to CMAKE_<LANG>_FLAGS
    set(CMAKE_C_FLAGS "${TB_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${TB_C_FLAGS} ${TB_CXX_FLAGS}")


set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin CACHE PATH "Output directory for libraries")
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin CACHE PATH "Output directory for executables")

file(GLOB SRC_FILES_GRO ./src/*.cpp)

	set(CURSES_NEED_NCURSES TRUE)
	find_package(Curses)
	MESSAGE("Robot GUI will be compiled:")
	if(CURSES_FOUND)
		include_directories(${CURSES_INCLUDE_DIR})
		MESSAGE("ncurses DIR is: " ${CURSES_INCLUDE_DIR}) 
	else(CURSES_FOUND)
		MESSAGE(WARNING "Library ncurses not found. Robot GUI won't compile")
	endif(CURSES_FOUND)

add_library(gro
	${SRC_FILES_GRO}
)


file (GLOB INCLUDE_FILES_GRO ${PROJECT_SOURCE_DIR}/include/*.h )

set(COMMON_INCLUDES
${PROJECT_SOURCE_DIR}/include/
${CURSES_INCLUDE_DIR})

set(PROJECT_LINK_LIBS

)

MESSAGE("Included files are: " ${INCLUDE_FILES_GRO})
MESSAGE("Included directories are: " ${COMMON_INCLUDES})
include_directories(${COMMON_INCLUDES})
include_directories(${PROJECT_SOURCE_DIR}/include/)



LINK_DIRECTORIES(${PROJECT_LINK_DIRS})

set(PROJECT_LIB_NAME ${PROJECT_NAME_STR}_lib)
set(PROJECT_EXEC_NAME ${PROJECT_NAME_STR})

add_executable(${PROJECT_EXEC_NAME} ${INCLUDE_FILES_GRO} ${PROJECT_SOURCE_DIR}/src/zmain.cpp)

MESSAGE("Linked Libraries are: " ${PROJECT_LINK_LIBS})	
target_link_libraries(${PROJECT_EXEC_NAME}
gro
pthread
${CURSES_LIBRARY}
)
