/*
 *                            COPYRIGHT
 *
 * This is a part of Rapido!
 *
 *  Copyright (C) 2006 Cedric Guillemet
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <wx/wx.h>
#include <wx/window.h>
#include <wx/grid.h>
#include <wx/treectrl.h>
#include <wx/spinctrl.h>
#include <wx/artprov.h>
#include <wx/clipbrd.h>
#include <wx/image.h>
#include <wx/colordlg.h>
#include <wx/wxhtml.h>
#include <vector>
#include <wx/aui/aui.h>
#include <wx/aui/dockart.h>
#include <wx/ffile.h>

#include "wxEditGraphNode.h"
#include "wxGraphNode.h"
#include "wxGraphContainer.h"
#include "wxGraphNodeState.h"
#include "wxScriptEditPanel.h"


extern wxGraphContainer *mScrollV;


wxZEditNode::wxZEditNode(wxWindow* parent, wxAuiManager* mgr)
	: wxZEdit(parent, this)
	, mEditingNode(NULL)
{
	this->Connect(-1, 14000, (wxObjectEventFunction)&wxZEditNode::OnRefresh);

	mFileExport = _("testExport");
}


/////
void SetScriptToEdit(const wxString& szName, wxString* pCode);
void wxZEditNode::BuildInterface(wxGraphNode *pNode)
{
    mEditingNode = pNode;


    BeginPanel();
    if (mEditingNode)
    {
		mNodeName = mEditingNode->GetFunctionName();
                AddFolder(_("Node"));
                AddTextEdit(_("Name"), &mNodeName, 200);
		/*
		AddStatic("Comments");
		AddScriptEdit(pNode->GetComment(), 2);

		if (pNode->GetType() == GNT_STATE)
		{
			AddFolder("Properties");
		}
		*/
		if (pNode->GetType() == GNT_MESSAGE)
		{
                        AddFolder(_("Source code"));
			AddScriptEdit(pNode->GetCode(pNode->GetSubItem()), 2);
		}

		if (mEditingNode->GetType() == GNT_STATE)
		{
			wxString info;
                        info = mNodeName+_(" > ");
			info += ((wxGraphNodeState*)mEditingNode)->mCurSelectedButton;
			SetScriptToEdit(info, pNode->GetCode(pNode->GetSubItem()));
		}
	}
	else
	{
		// editing tab

		extern wxNotebook * myNotebook;

		mNodeName = myNotebook->GetPageText(myNotebook->GetSelection());
                AddFolder(_("FSM"));
                AddTextEdit(_("Name"), &mNodeName, 199);
                AddCheckBox(_("Add Raknet packet support"), &mScrollV->mbRaknetMessage);
                AddCheckBox(_("Tick has time parameter"), &mScrollV->mbTickHasTime);
                AddTextEdit(_("State base"), &mScrollV->mStateEnumBase);
                AddTextEdit(_("Message base"), &mScrollV->mMessageEnumBase);
                AddTextEdit(_("Output"), &mScrollV->mOutputFileName);


		std::vector<wxString> mStateList;
		mScrollV->FillNodeList(mStateList, GNT_STATE);
		if (!mStateList.empty())
		{
			wxString mInitialStatesList;
			for (unsigned int i=0;i<mStateList.size()-1;i++)
			{
                                mInitialStatesList += mStateList[i]+_("|");
			}
			mInitialStatesList += mStateList[mStateList.size()-1];

			if (mScrollV->mInitialState > (mStateList.size()-1) )
				mScrollV->mInitialState = 0;

                        AddCombo(_("Initial State"), mInitialStatesList.c_str(), &mScrollV->mInitialState);
		}
                AddFolder(_("Code"));
		AddStatic(_("Includes"));
		AddScriptEdit(mScrollV->GetIncludes(), 14);
		AddStatic(_("Member Variables"));
		AddScriptEdit(mScrollV->GetMemberVariables(), 14);
		AddStatic(_("Members Init"));
		AddScriptEdit(mScrollV->GetMembersInit(), 14);

		extern wxScriptEditPanel *mEditScriptPanel;
                mEditScriptPanel->SetScriptToEdit(wxT(""), NULL);
	}
	EndPanel();
}

void wxZEditNode::BuildInterfaceForCodeGeneration()
{
	mEditingNode = NULL;


	BeginPanel();


        AddFolder(_("Generate"));


        this->AddFileOpener(_("Export File"), wxT("*.h;*.cpp"), &mFileExport);
        AddButton(_("Generate"), 101);

	EndPanel();
}

wxString GenerateCPP();

void wxZEditNode::HandleButtonClick(unsigned int aButtonID)
{
	GenerateCPP();
}

void wxZEditNode::HandleLostFocus(unsigned int aID)
{
	if (!mEditingNode)
		return;

	if (mEditingNode->GetType() == GNT_STATE)
	{

	}
}

void wxZEditNode::HandleValueChanged(unsigned int aID)
{
	if (aID == 199)
	{
		extern wxNotebook * myNotebook;
		myNotebook->SetPageText(myNotebook->GetSelection(), mNodeName.c_str());
		mScrollV->mGraphName = mNodeName.c_str();
	}
	else

	if (aID == 200)
	{
		if (mEditingNode->GetType() == GNT_MESSAGE)
		{
                        mScrollV->ChangeMessageName(mEditingNode->GetFunctionName(), mNodeName);
		}
                mEditingNode->SetFunctionName(mNodeName);
		return;
	}
	else

	if (mEditingNode)
	{
		mEditingNode->SetFunctionName(mNodeName);



	}
}


wxString GenerateEnum(std::vector<wxString> & list, const wxString& prefix)
{
	wxString enumList = wxT("");

	for (unsigned int i=0;i<list.size();i++)
	{
		enumList += prefix;
		enumList += list[i];
		if (i != (list.size()-1))
			enumList += wxT(",\n");
		else
			enumList += wxT("\n");
	}

	return enumList;
}

wxString GenerateSwitchCase(std::vector<wxString> & list, const wxString& functionPrefix,
							const wxString& variableName, const wxString& functionParameter)
{
	wxString res = wxT("	switch(")+variableName+wxT(")\n	{\n");

	for (unsigned int i=0;i<list.size();i++)
	{
		res += wxT("		case ") + list[i] + wxT(":\n");
		res += wxT("			") + functionPrefix+list[i] + wxT("(") + functionParameter + wxT(");\n");
		res += wxT("			break;\n");
	}

	res += wxT("	}");

	return res;
}

wxString BuildMessagesStruct(std::vector<wxString> &list,
							 std::vector<wxString> &code,
							 std::vector<wxString> &comment)
{
	wxString res;
	for (unsigned int i = 0 ; i<list.size() ; i++)
	{
		res += wxT("\n\n\n/*\n") + comment[i] + wxT("\n*/\n");
		res += wxT("typedef struct ") + list[i] + wxT("\n{\n");
		res += code[i] + wxT("\n} ") + list[i] + wxT(";\n\n");
	}

	return res;
}

wxString BuildFunctions(std::vector<wxString> &list, std::vector<wxString> &code,
						const wxString& prefix, const wxString& parameter, const wxString& className)
{
	wxString res;
	for (unsigned int i = 0 ; i<list.size() ; i++)
	{
		//res += "\n/*"+""/*comment[i]*/+"*/\n";
		res += wxT("inline void ") + prefix + list[i] + wxT("(") + parameter + wxT(")\n{\n\n");
		res += code[i] + wxT("\n}\n");
	}

	return res;
}

wxString BuildMessagesFunctions(std::vector<wxString> &list, std::vector<wxString> &codeList, std::vector<wxString> &stateList)
{
	wxString res;
	for (unsigned int i = 0;i<list.size();i++)
	{
		res += wxT("void Handle(const ") + list[i] + wxT("& mes)\n{\n");
		res += GenerateSwitchCase(stateList, wxT("Handle"), wxT("mState"), wxT("mes"));
		res += wxT("\n}\n\n");
	}
	return res;
}

wxString BuildRaknetPacketRead(std::vector<wxString> &meslist,const wxString& enumPrefix)
{
	wxString res;
	res += wxT("bool HandleRaknetPacket(Packet& packet)\n{\n");
	res += wxT("switch (packet->data[0])\n");
	res += wxT("{\n");

	for (unsigned int i=0 ; i<meslist.size() ; i++)
	{
		res += wxT("case ") + enumPrefix + meslist[i] + wxT(":\n");
		res += wxT("{\n");
		res += meslist[i] + wxT(" mes;\n");//		RakNetTime time, dataLength;
		res += wxT("RakNet::BitStream rakStream( packet->data+1, sizeof(") + meslist[i] + wxT("), false);\n");
		res += wxT("rakStram.Read(mes);\n");
		res += wxT("Handle(mes);\n");
		res += wxT("return true;\n");
		res += wxT("}\n");
	}
	res += wxT("}\nreturn false;\n}\n");

	return res;
}

wxString MakeAllStatesHandles(std::vector<wxString> &statelist, std::vector<wxString> &meslist)
{
	wxString res;

	for (unsigned int i=0;i<statelist.size();i++)
	{
		for (unsigned int j=0;j<meslist.size();j++)
		{
			res += wxT("void Handle") + statelist[i] + wxT("(const ") + meslist[j] + wxT("& mes)\n");
			res += wxT("{\n");

			wxGraphNode *pStateNode = mScrollV->GetNodeByName(statelist[i].c_str());
			wxString onit = wxT("On ") + meslist[j];
			res += *pStateNode->GetCode(onit.c_str());
			res += wxT("\n}\n\n");
		}
	}

	return res;
}


wxString GenerateCPP()
{
	std::vector<wxString> mMesList, mStateList, mMesCommentList, mMesCodeList;
	std::vector<wxString> mTickCodeList;
	std::vector<wxString> mEnterCodeList, mLeaveCodeList;
	mScrollV->FillNodeList(mMesList, GNT_MESSAGE);
	mScrollV->FillNodeList(mStateList, GNT_STATE);
	mScrollV->FillCodesFromNodeList(mMesCodeList, GNT_MESSAGE);
	mScrollV->FillCommentsFromNodeList(mMesCommentList, GNT_MESSAGE);


	mScrollV->FillCodesFromNodeList(mTickCodeList, GNT_STATE, _("On Tick"));
	mScrollV->FillCodesFromNodeList(mEnterCodeList, GNT_STATE, _("On Enter"));
	mScrollV->FillCodesFromNodeList(mLeaveCodeList, GNT_STATE, _("On Leave"));


	// build messages enum

	wxString enumMessages = GenerateEnum(mMesList, wxT("Type_"));
	wxString enumStates = GenerateEnum(mStateList, wxT(""));

	wxString strTemplate;


	wxFFile templateFile(wxT("CPPTemplate.template"), wxT("rt"));
	if (templateFile.IsOpened())
	{
		templateFile.ReadAll(&strTemplate);
	}
	else
		return _("CPPTemplate.template NOT FOUND!!!");


	std::map<wxString, wxString> mTokens;

	wxString className = mScrollV->mGraphName.c_str();// gClassName.c_str();
	mTokens[wxT("%%STATEMACHINENAME%%")] = className;

	bool bHasFloatTime = true;
	bool bRaknetPacket = false;
	if (bHasFloatTime)
		mTokens[wxT("%%HASFLOATTIME%%")] = wxT("float timeEllapsed");
	else
		mTokens[wxT("%%HASFLOATTIME%%")] = wxT("");

	mTokens[wxT("%%MESSAGESENUM%%")] = enumMessages;

	mTokens[wxT("%%TICKSTATECASE%%")] = GenerateSwitchCase(mStateList,
	                                                       wxT("Tick"),
	                                                       wxT("mState"),
	                                                       bHasFloatTime ? wxT("timeEllapsed") : wxT(""));


	mTokens[wxT("%%STATESENUM%%")] = enumStates;

	mTokens[wxT("%%MESSAGEALLSTRUCTS%%")] = BuildMessagesStruct(mMesList,
	                                                            mMesCodeList,
	                                                            mMesCommentList);

	mTokens[wxT("%%ALLTICKFUNCTIONS%%")] = BuildFunctions(mStateList,
	                                                      mTickCodeList,
	                                                      wxT("Tick"),
	                                                      bHasFloatTime ? wxT("float timeEllapsed") : wxT(""),
	                                                      className);

	mTokens[wxT("%%ALLENTERFUNCTIONS%%")] = BuildFunctions(mStateList,
	                                                       mEnterCodeList,
	                                                       wxT("Enter"),
	                                                       wxT(""),
	                                                       className);

	mTokens[wxT("%%ALLLEAVEFUNCTIONS%%")] = BuildFunctions(mStateList,
	                                                       mLeaveCodeList,
	                                                       wxT("Leave"),
	                                                       wxT(""),
	                                                       className);


	mTokens[wxT("%%SETSTATESWITCH%%")] = GenerateSwitchCase(mStateList, wxT("Leave"), wxT("mState"),  wxT(""))
	                                     + wxT("\n")
	                                     + wxT("mState = newState;")
	                                     + GenerateSwitchCase(mStateList, wxT("Enter"), wxT("newState"), wxT(""));


	mTokens[wxT("%%ALLHANDLEMESSAGES%%")] = BuildMessagesFunctions(mMesList,
	                                                               mMesList,
	                                                               mStateList);

	mTokens[wxT("%%INITIALSTATE%%")] = mStateList[mScrollV->mInitialState];

	mTokens[wxT("%%HANDLERAKNETPACKET%%")] = bRaknetPacket ? BuildRaknetPacketRead(mMesList, wxT("Type_"))
	                                                       : wxString::FromUTF8("");

	mTokens[wxT("%%ALLSTATEMESSAGESHANDLE%%")] = MakeAllStatesHandles(mStateList, mMesList);

	mTokens[wxT("%%INCLUDES%%")] = *mScrollV->GetIncludes();
	mTokens[wxT("%%MEMBERSINIT%%")] = *mScrollV->GetMembersInit();
	mTokens[wxT("%%MEMBERSDECLARE%%")] = *mScrollV->GetMemberVariables();

	mTokens[wxT("%%DATE%%")] = wxDateTime::Today().FormatDate();
	mTokens[wxT("%%TIME%%")] = wxDateTime::Today().FormatTime();

	// replacing
	std::map<wxString, wxString>::iterator iter = mTokens.begin();

	for (; iter != mTokens.end(); ++iter)
		strTemplate.Replace((*iter).first, (*iter).second);

	// write down
	wxString genInfos;
	wxFFile outputFile(mScrollV->mOutputFileName, wxT("wt"));
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
		                mScrollV->mOutputFileName.c_str(), newLineCount);
	}
	else
	{
		genInfos.Printf(_("Generation of %s FAILED!\n"),
		                mScrollV->mOutputFileName.c_str());
	}

	return genInfos;
}
