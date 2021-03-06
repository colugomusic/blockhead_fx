project(fx_zap VERSION 1.0.0)

set(dir_submodules ${CMAKE_CURRENT_LIST_DIR}/../submodules)
set(dir_rack++ ${dir_submodules}/rackpp)
set(dir_rack ${dir_rack++}/submodules/rack)
set(dir_rack++module ${dir_rack++}/module)
set(dir_madronalib ${dir_rack++}/submodules/snd/submodules/madronalib)
set(dir_front ${CMAKE_CURRENT_LIST_DIR}/../../../front)

if (NOT TARGET rack++module)
	add_subdirectory(${dir_rack++/module} rack++module)
endif()

list(APPEND HEADERS
	./src/zap.h
)
list(APPEND SRC
	./src/zap.cpp
	./src/module.cpp
)

source_group(TREE ${CMAKE_CURRENT_LIST_DIR}/src PREFIX headers FILES ${HEADERS})
source_group(TREE ${CMAKE_CURRENT_LIST_DIR}/src PREFIX src FILES ${SRC})

add_library(${PROJECT_NAME} SHARED ${HEADERS} ${SRC})

target_include_directories(${PROJECT_NAME} PRIVATE
	${CMAKE_CURRENT_LIST_DIR}/..
	${dir_rack}/include
	${dir_rack++module}/include
	${dir_madronalib}/source
)

target_link_libraries(${PROJECT_NAME} PRIVATE
	rack++module
)

if (FX_OUTPUT_DIR)
	set_target_properties(${PROJECT_NAME} PROPERTIES
		RUNTIME_OUTPUT_DIRECTORY_DEBUG ${FX_OUTPUT_DIR}
		RUNTIME_OUTPUT_DIRECTORY_RELEASE ${FX_OUTPUT_DIR}
		RUNTIME_OUTPUT_DIRECTORY ${FX_OUTPUT_DIR}
        LIBRARY_OUTPUT_DIRECTORY_DEBUG ${FX_OUTPUT_DIR}
        LIBRARY_OUTPUT_DIRECTORY_RELEASE ${FX_OUTPUT_DIR}
        LIBRARY_OUTPUT_DIRECTORY ${FX_OUTPUT_DIR}
	)
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES
	OUTPUT_NAME blockhead_zap_${PLATFORM_DIR}
	SUFFIX ".fx"
    CXX_STANDARD 17
	C_STANDARD 99
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
	PROJECT_VERSION="${PROJECT_VERSION}"
)
