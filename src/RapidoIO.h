#ifndef RAPIDOIO_H
#define RAPIDOIO_H

#include <vector>

#include "wxGraphContainer.h"

class RapidoIO 
{
public :

	/*! Load and create wxGraphContainer(s) from a Rapido XML file.
	* 
	* \param[in] parent Pointer to a parent wxWindow for the wxGraphContainer(s) created.
	*                   Must be non-NULL
	* \param[in] aFileName Path to the file.
	* \return A pointer list of the wxGraphContainer(s) created
	*/
	static std::vector<wxGraphContainer*> LoadStateMachinesFromFile(wxWindow* aParent, const wxString& aFileName);

	/*! Save a list of finite-state machine to a Rapido XML file.
	*
	* \param[in] aList List of finite-state machine to store.
	* \param[in] aFileName Path to the file.
	* \param[in] use_legacyFileFormat
	*/
	static void SaveStateMachinesToFile(std::vector<wxGraphContainer*> aList,
	                                    const wxString& aFileName,
	                                    bool use_legacyFileFormat = true);

	/*! Generate a C++ source file of the finite-state machine.
	*
	* \param[in] The finite-sate machine to generate.
	*/
	static wxString GenerateCppFile(wxGraphContainer& aFsmGraph);

private :
	static const std::string ROOTNODE_VALUE;

	/*! Create wxGraphContainer(s) from a legacy Rapido XML document.
	*
	* \param[in] parent Pointer to a parent wxWindow for the wxGraphContainer(s) created.
	*                   Must be non-NULL
	* \param[in] aXmlDocument Path to the file.
	* \return A pointer list of the wxGraphContainer(s) created
	*/
	static std::vector<wxGraphContainer*> CreateStateMachinesFromLegacyXmlDocument(wxWindow* aParent, TiXmlDocument& aXmlDocument);

	/*! Create wxGraphContainer(s) from a Rapido XML document.
	*
	* \param[in] parent Pointer to a parent wxWindow for the wxGraphContainer(s) created.
	*                   Must be non-NULL
	* \param[in] aXmlDocument Path to the file.
	* \return A pointer list of the wxGraphContainer(s) created
	*/
	static std::vector<wxGraphContainer*> CreateStateMachinesFromXmlDocument(wxWindow* aParent, TiXmlDocument& aXmlDocument);


	static wxString BuildEnum(std::vector<wxString>& list,
	                          const wxString& prefix);

	static wxString BuildSwitchCase(std::vector<wxString> & list,
	                                const wxString& functionPrefix,
	                                const wxString& variableName,
	                                const wxString& functionParameter);

	static wxString BuildMessagesStruct(std::vector<wxString> &list,
	                                    std::vector<wxString> &code,
	                                    std::vector<wxString> &comment);

	static wxString BuildFunctions(std::vector<wxString> &list,
	                               std::vector<wxString> &code,
	                               const wxString& prefix,
	                               const wxString& parameter,
	                               const wxString& className);

	static wxString BuildMessagesFunctions(std::vector<wxString> &list,
	                                       std::vector<wxString> &stateList);

	static wxString BuildRaknetPacketRead(std::vector<wxString>& messageList,
	                                      const wxString& enumPrefix);
	                                      
	static wxString BuildAllStatesHandles(wxGraphContainer& aGraphContainer,
	                                      std::vector<wxString>& statelist,
	                                      std::vector<wxString>& messageList);

};

#endif
