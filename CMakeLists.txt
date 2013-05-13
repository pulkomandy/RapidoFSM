cmake_minimum_required(VERSION 2.6.2)

project(RapidoFSM)

find_package(wxWidgets COMPONENTS core base stc foldbar aui adv REQUIRED)
include( ${wxWidgets_USE_FILE} )

include_directories($(PROJECT_SOURCE_DIR)/tinyxml)

set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${RapidoFSM_BINARY_DIR}/bin )

add_subdirectory(tinyxml)


set(RAPIDO_SOURCES
	minimal.cpp
	wxEdit.cpp
	wxEditGraphNode.cpp
	wxGraphContainer.cpp
	wxGraphNode.cpp
	wxGraphNodeMessage.cpp
	wxGraphNodeState.cpp
	wxScriptEditPanel.cpp
	wxStackedControls.cpp
	)

add_executable(rapido ${RAPIDO_SOURCES})

target_link_libraries(rapido
	${wxWidgets_LIBRARIES}
	tinyXML
	)

