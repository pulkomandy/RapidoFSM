#include "RapidoIO.h"

#include <map>

#include <wx/ffile.h>
#include <wx/string.h>
#include <wx/msgdlg.h>

#include "tinyxml.h"


std::vector<wxGraphContainer*> RapidoIO::LoadStateMachinesFromFile(wxWindow* aParent, const wxString& aFileName)
{
	std::vector<wxGraphContainer*> stateMachineList;

	wxString xmlFileStr;
	wxFFile xmlFile(aFileName, wxT("rt"));

	if (xmlFile.IsOpened())
		xmlFile.ReadAll(&xmlFileStr);
	else {
		wxMessageBox(wxString::Format(_("Can't open file \"%s\""),
		                              aFileName.c_str()), _("Error message"),
		                              wxOK | wxICON_ERROR, aParent);
		return stateMachineList;
	}

	TiXmlDocument xmlDoc;
	if (!xmlDoc.Parse(xmlFileStr.mb_str()))
	{
		wxMessageDialog m(NULL, xmlFileStr);
		m.ShowModal();
		return stateMachineList;
	}

	TiXmlElement* xmlElement = xmlDoc.RootElement();
	if (xmlElement == NULL) {
		wxMessageBox(wxString::Format(_("File \"%s\" is not a valid XML file"),
		                              aFileName.c_str()), _("Error message"),
		                              wxOK | wxICON_ERROR, aParent);
	}

	while(xmlElement)
	{
		wxGraphContainer* graphContainer = new wxGraphContainer(aParent);
		graphContainer->ParseGraphString(xmlElement);
		stateMachineList.push_back(graphContainer);

		xmlElement = xmlElement->NextSiblingElement("GraphContainer");
	}

	return stateMachineList;
}


void RapidoIO::SaveStateMachinesToFile(std::vector<wxGraphContainer*> aList, const wxString& aFileName)
{
	wxString outputFileStr;

	for (std::vector<wxGraphContainer*>::iterator it = aList.begin(), end = aList.end() ; it != end ; ++it)
	{
		outputFileStr += (*it)->BuildGraphString();
		outputFileStr += wxT("\n");
	}

	wxFFile outputFile(aFileName, wxT("wt"));
	outputFile.Write(outputFileStr);
	outputFile.Flush();
	outputFile.Close();
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

	wxString className = aFsmGraph.mGraphName.c_str();
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
	wxFFile outputFile(aFsmGraph.mOutputFileName, wxT("wt"));
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
		                aFsmGraph.mOutputFileName.c_str(), newLineCount);
	}
	else
	{
		genInfos.Printf(_("Generation of %s FAILED!\n"),
		                aFsmGraph.mOutputFileName.c_str());
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


