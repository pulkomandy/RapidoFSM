
set(TINYXML_SOURCES
	tinyxml.cpp
	tinystr.cpp
	tinyxmlerror.cpp
	tinyxmlparser.cpp
	)

set(TINYXML_HEADERS
	tinystr.h
	tinyxml.h
	)

add_library(tinyXML STATIC ${TINYXML_SOURCES})
