#include "RapidoIO.h"

#include <map>

#include <wx/ffile.h>
#include <wx/string.h>
#include <wx/msgdlg.h>

#include "tinyxml.h"


const std::string RapidoIO::ROOTNODE_VALUE = "RapidoFSM_storage";


std::vector<wxGraphContainer*> RapidoIO::LoadStateMachinesFromFile(wxWindow* aParent, const wxString& aFileName)
{
	TiXmlDocument xmlDocument(aFileName.c_str());
	std::vector<wxGraphContainer*> stateMachineList;

	if (!xmlDocument.LoadFile()) {
		wxMessageBox(wxString::Format(_("Can't load XML file \"%s\""),
		                              aFileName.c_str()), _("Error message"),
		                              wxOK | wxICON_ERROR, NULL);
		return stateMachineList;
	}


	TiXmlElement* xmlElement = xmlDocument.RootElement();
	if (xmlElement != NULL) {
		std::string rootElementValue(xmlElement->Value());
		if (rootElementValue.compare(ROOTNODE_VALUE) == 0)
			stateMachineList = CreateStateMachinesFromXmlDocument(aParent, xmlDocument);
		else
			stateMachineList = CreateStateMachinesFromLegacyXmlDocument(aParent, xmlDocument);
	}

	return stateMachineList;
}


std::vector<wxGraphContainer*> RapidoIO::CreateStateMachinesFromLegacyXmlDocument(wxWindow* aParent, TiXmlDocument& aXmlDocument)
{
	std::vector<wxGraphContainer*> stateMachineList;

	TiXmlElement* xmlElement = aXmlDocument.RootElement();
	while(xmlElement != NULL)
	{
		wxGraphContainer* graphContainer = new wxGraphContainer(aParent);
		graphContainer->ParseLegacyXmlElement(xmlElement);
		stateMachineList.push_back(graphContainer);

		xmlElement = xmlElement->NextSiblingElement("GraphContainer");
	}

	return stateMachineList;
}


std::vector<wxGraphContainer*> RapidoIO::CreateStateMachinesFromXmlDocument(wxWindow* aParent, TiXmlDocument& aXmlDocument)
{
	std::vector<wxGraphContainer*> stateMachineList;

	TiXmlElement* xmlRootElement = aXmlDocument.RootElement();
	TiXmlElement* xmlElement = xmlRootElement->FirstChildElement("finiteStateMachine");
	while(xmlElement)
	{
		wxGraphContainer* graphContainer = new wxGraphContainer(aParent);
		graphContainer->ParseXmlElement(xmlElement);
		stateMachineList.push_back(graphContainer);

		xmlElement = xmlElement->NextSiblingElement("finiteStateMachine");
	}

	return stateMachineList;
}


void RapidoIO::SaveStateMachinesToFile(std::vector<wxGraphContainer*> aList, const wxString& aFileName, bool use_legacyFileFormat)
{
	TiXmlDocument xmlDocument(aFileName.c_str());

	if (use_legacyFileFormat) {

		for (std::vector<wxGraphContainer*>::iterator it = aList.begin(), end = aList.end() ; it != end ; ++it)
			xmlDocument.LinkEndChild((*it)->CreateLegacyXmlNodeWithChildren());

	} else {

		TiXmlDocument xmlDocument(aFileName.c_str());

		xmlDocument.InsertEndChild(TiXmlDeclaration("1.0", "UTF-8", "yes"));

		TiXmlElement* rootElement = new TiXmlElement(ROOTNODE_VALUE.c_str());

		for (std::vector<wxGraphContainer*>::iterator it = aList.begin(), end = aList.end() ; it != end ; ++it)
			rootElement->LinkEndChild((*it)->CreateXmlNodeWithChildren());

		xmlDocument.LinkEndChild(rootElement);

	}

	xmlDocument.SaveFile();
}


//------------------------------------------------------------------------------
// Source file generator

wxString RapidoIO::GenerateCppFile(wxGraphContainer& aFsmGraph)
{
	std::vector<wxString> statesList;
	std::vector<wxString> messagesList;
	std::vector<wxString> messagesCodeList;
	std::vector<wxString> messagesCommentList;

	std::vector<wxString> tickCodeList;
	std::vector<wxString> enterCodeList;
	std::vector<wxString> leaveCodeList;

	aFsmGraph.FillNodeList(statesList, GNT_STATE);
	aFsmGraph.FillNodeList(messagesList, GNT_MESSAGE);
	aFsmGraph.FillCodesFromNodeList(messagesCodeList, GNT_MESSAGE);
	aFsmGraph.FillCommentsFromNodeList(messagesCommentList, GNT_MESSAGE);

	aFsmGraph.FillCodesFromNodeList(tickCodeList, GNT_STATE, _("On Tick"));
	aFsmGraph.FillCodesFromNodeList(enterCodeList, GNT_STATE, _("On Enter"));
	aFsmGraph.FillCodesFromNodeList(leaveCodeList, GNT_STATE, _("On Leave"));

	wxString strTemplate;
	wxString templateFileName(wxT("CPPTemplate.template"));

	wxFFile templateFile(templateFileName, wxT("rt"));
	if (templateFile.IsOpened())
	{
		templateFile.ReadAll(&strTemplate);
	}
	else
		return wxString::Format(_("%s NOT FOUND!!!"), templateFileName.c_str());


	std::map<wxString, wxString> templateMap;

	wxString className = aFsmGraph.GetGraphName();
	templateMap[wxT("%%STATEMACHINENAME%%")] = className;

	bool bHasFloatTime = true;
	bool bRaknetPacket = false;

	if (bHasFloatTime)
		templateMap[wxT("%%HASFLOATTIME%%")] = wxT("float timeEllapsed");
	else
		templateMap[wxT("%%HASFLOATTIME%%")] = wxT("");

	templateMap[wxT("%%MESSAGESENUM%%")] = BuildEnum(messagesList, wxT("Type_"));

	templateMap[wxT("%%TICKSTATECASE%%")] = BuildSwitchCase(statesList,
	                                                           wxT("Tick"),
	                                                           wxT("mState"),
	                                                           bHasFloatTime ? wxT("timeEllapsed") : wxT(""));

	templateMap[wxT("%%STATESENUM%%")] = BuildEnum(statesList, wxT(""));

	templateMap[wxT("%%MESSAGEALLSTRUCTS%%")] = BuildMessagesStruct(messagesList,
	                                                                messagesCodeList,
	                                                                messagesCommentList);

	templateMap[wxT("%%ALLTICKFUNCTIONS%%")] = BuildFunctions(statesList,
	                                                          tickCodeList,
	                                                          wxT("Tick"),
	                                                          bHasFloatTime ? wxT("float timeEllapsed") : wxT(""),
	                                                          className);

	templateMap[wxT("%%ALLENTERFUNCTIONS%%")] = BuildFunctions(statesList,
	                                                           enterCodeList,
	                                                           wxT("Enter"),
	                                                           wxT(""),
	                                                           className);

	templateMap[wxT("%%ALLLEAVEFUNCTIONS%%")] = BuildFunctions(statesList,
	                                                           leaveCodeList,
	                                                           wxT("Leave"),
	                                                           wxT(""),
	                                                           className);

	templateMap[wxT("%%SETSTATESWITCH%%")] = BuildSwitchCase(statesList, wxT("Leave"), wxT("mState"),  wxT(""))
	                                         + wxT("\n")
	                                         + wxT("mState = newState;")
	                                         + BuildSwitchCase(statesList, wxT("Enter"), wxT("newState"), wxT(""));

	templateMap[wxT("%%ALLHANDLEMESSAGES%%")] = BuildMessagesFunctions(messagesList,
	                                                                   statesList);

	templateMap[wxT("%%INITIALSTATE%%")] = statesList[aFsmGraph.mInitialState];

	templateMap[wxT("%%HANDLERAKNETPACKET%%")] = bRaknetPacket ? BuildRaknetPacketRead(messagesList, wxT("Type_"))
	                                                           : wxT("");

	templateMap[wxT("%%ALLSTATEMESSAGESHANDLE%%")] = BuildAllStatesHandles(aFsmGraph, statesList, messagesList);

	templateMap[wxT("%%INCLUDES%%")] = *aFsmGraph.GetIncludes();
	templateMap[wxT("%%MEMBERSINIT%%")] = *aFsmGraph.GetMembersInit();
	templateMap[wxT("%%MEMBERSDECLARE%%")] = *aFsmGraph.GetMemberVariables();

	templateMap[wxT("%%DATE%%")] = wxDateTime::Today().FormatDate();
	templateMap[wxT("%%TIME%%")] = wxDateTime::Today().FormatTime();

	// Insert code in the template
	for (std::map<wxString, wxString>::iterator it = templateMap.begin(), end = templateMap.end() ; it != end ; ++it)
		strTemplate.Replace(it->first, it->second);

	// Write down
	wxString genInfos;
	wxFFile outputFile(aFsmGraph.GetOutputFileName(), wxT("wt"));
	if (outputFile.IsOpened()) 
	{
		outputFile.Write(strTemplate);
		outputFile.Flush();
		outputFile.Close();

		unsigned int newLineCount = 0;
		for (unsigned int i = 0 ; i < strTemplate.Len() ; i++)
		{
			if (strTemplate[i] == '\n')
				newLineCount++;
		}
		genInfos.Printf(_("Generation of %s OK!\n%u lines generated.\n"),
		                aFsmGraph.GetOutputFileName().c_str(), newLineCount);
	}
	else
	{
		genInfos.Printf(_("Generation of %s FAILED!\n"),
		                aFsmGraph.GetOutputFileName().c_str());
	}

	return genInfos;

}


wxString RapidoIO::BuildEnum(std::vector<wxString>& list, const wxString& prefix)
{
	wxString res;

	for (std::vector<wxString>::iterator it = list.begin(), end = list.end() ; it != end ; ++it)
	{
		res += prefix;
		res += *it;
		if (it != end)
			res += wxT(",\n");
	}

	return res;
}


wxString RapidoIO::BuildSwitchCase(std::vector<wxString> & list,
                                   const wxString& functionPrefix,
                                   const wxString& variableName,
                                   const wxString& functionParameter)
{
	wxString res = wxT("	switch(") + variableName + wxT(")\n	{\n");

	for (std::vector<wxString>::iterator it = list.begin(), end = list.end() ; it != end ; ++it)
	{
		res += wxT("		case ") + *it + wxT(":\n");
		res += wxT("			") + functionPrefix + *it + wxT("(") + functionParameter + wxT(");\n");
		res += wxT("			break;\n");
	}

	res += wxT("	}");

	return res;
}


wxString RapidoIO::BuildMessagesStruct(std::vector<wxString> &list,
                                       std::vector<wxString> &code,
                                       std::vector<wxString> &comment)
{
	wxString res;

	for (unsigned int i = 0, size = list.size() ; i < size ; ++i)
	{
		res += wxT("\n\n\n/*\n") + comment[i] + wxT("\n*/\n");
		res += wxT("typedef struct ") + list[i] + wxT("\n{\n");
		res += code[i] + wxT("\n} ") + list[i] + wxT(";\n\n");
	}

	return res;
}


wxString RapidoIO::BuildFunctions(std::vector<wxString> &list,
                                  std::vector<wxString> &code,
                                  const wxString& prefix,
                                  const wxString& parameter,
                                  const wxString& className)
{
	wxString res;

	for (unsigned int i = 0 ; i<list.size() ; ++i)
	{
		res += wxT("inline void ") + prefix + list[i] + wxT("(") + parameter + wxT(")\n{\n\n");
		res += code[i] + wxT("\n}\n");
	}

	return res;
}


wxString RapidoIO::BuildMessagesFunctions(std::vector<wxString> &list,
                                          std::vector<wxString> &stateList)
{
	wxString res;

	for (std::vector<wxString>::iterator it = list.begin(), end = list.end() ; it != end ; ++it)
	{
		res += wxT("void Handle(const ") + *it + wxT("& mes)\n{\n");
		res += BuildSwitchCase(stateList, wxT("Handle"), wxT("mState"), wxT("mes"));
		res += wxT("\n}\n\n");
	}

	return res;
}


wxString RapidoIO::BuildRaknetPacketRead(std::vector<wxString>& messageList,
                                         const wxString& enumPrefix)
{
	wxString res;

	res += wxT("bool HandleRaknetPacket(Packet& packet)\n{\n");
	res += wxT("switch (packet->data[0])\n");
	res += wxT("{\n");

	for (std::vector<wxString>::iterator it = messageList.begin(), end = messageList.end() ; it != end ; ++it)
	{
		res += wxT("case ") + enumPrefix + *it + wxT(":\n");
		res += wxT("{\n");
		res += *it + wxT(" mes;\n");//		RakNetTime time, dataLength;
		res += wxT("RakNet::BitStream rakStream( packet->data+1, sizeof(") + *it + wxT("), false);\n");
		res += wxT("rakStram.Read(mes);\n");
		res += wxT("Handle(mes);\n");
		res += wxT("return true;\n");
		res += wxT("}\n");
	}
	res += wxT("}\nreturn false;\n}\n");

	return res;
}


wxString RapidoIO::BuildAllStatesHandles(wxGraphContainer& aGraphContainer,
                                         std::vector<wxString>& statelist,
                                         std::vector<wxString>& messageList)
{
	wxString res;

	for (unsigned int i = 0 ; i < statelist.size() ; ++i)
	{
		for (unsigned int j = 0 ; j < messageList.size() ; ++j)
		{
			res += wxT("void Handle") + statelist[i] + wxT("(const ") + messageList[j] + wxT("& mes)\n");
			res += wxT("{\n");

			wxGraphNode *pStateNode = aGraphContainer.GetNodeByName(statelist[i].c_str());
			wxString onit = wxT("On ") + messageList[j];
			res += *pStateNode->GetCode(onit.c_str());
			res += wxT("\n}\n\n");
		}
	}

	return res;
}


