cmake_minimum_required(VERSION 2.6.2)
project(Rapido)
find_package(wxWidgets COMPONENTS core base stc foldbar aui adv REQUIRED)

include( ${wxWidgets_USE_FILE} )
#include_directories(${wxWidgets_INCLUDE_DIRS} $(PROJECT_SOURCE_DIR)/tinyxml)
include_directories($(PROJECT_SOURCE_DIR)/tinyxml)
#add_definitions(${wxWidgets_DEFINITIONS})

# For convenience we define the sources as a variable. You can add 
# header files and cpp/c files and CMake will sort them out
set(SRCS minimal.cpp wxEdit.cpp wxEditGraphNode.cpp wxGraphContainer.cpp wxGraphNode.cpp wxGraphNodeMessage.cpp wxGraphNodeState.cpp wxScriptEditPanel.cpp wxStackedControls.cpp tinyxml/tinyxml.cpp tinyxml/tinystr.cpp tinyxml/tinystr.h tinyxml/tinyxml.h tinyxml/tinyxmlerror.cpp tinyxml/tinyxmlparser.cpp)
 
add_executable(Rapido WIN32 ${SRCS})
target_link_libraries(Rapido ${wxWidgets_LIBRARIES})

