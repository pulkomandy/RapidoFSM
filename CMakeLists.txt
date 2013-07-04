cmake_minimum_required(VERSION 2.6.2)

project(RapidoFSM)

find_package(wxWidgets COMPONENTS core base stc foldbar aui adv REQUIRED)
include( ${wxWidgets_USE_FILE} )

include_directories($(PROJECT_SOURCE_DIR)/tinyxml)

add_definitions(-DTINYXML_USE_STL)

set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${RapidoFSM_BINARY_DIR}/bin )

add_subdirectory(tinyxml)


set(RAPIDO_SOURCES
	src/minimal.cpp
	src/wxEdit.cpp
	src/wxEditGraphNode.cpp
	src/wxGraphContainer.cpp
	src/wxGraphNode.cpp
	src/wxGraphNodeMessage.cpp
	src/wxGraphNodeState.cpp
	src/wxScriptEditPanel.cpp
	src/wxStackedControls.cpp
	src/RapidoIO.cpp
	)

add_executable(rapido ${RAPIDO_SOURCES})

target_link_libraries(rapido
	${wxWidgets_LIBRARIES}
	tinyXML
	)

