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

#include "wxEdit.h"
#include "ZString.h"

#include "wxEditGraphNode.h"
#include "wxGraphNode.h"
#include "wxGraphContainer.h"
#include "wxGraphNodeState.h"
#include "wxScriptEditPanel.h"

//#include "res.h"
extern wxGraphContainer *mScrollV;

/*

*/


/////
void SetScriptToEdit(const wxChar *szName, wxString* pCode);
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
			SetScriptToEdit(info.c_str(), pNode->GetCode(pNode->GetSubItem()));
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
                mEditScriptPanel->SetScriptToEdit(_(""), NULL);
	}
	EndPanel();
}

void wxZEditNode::BuildInterfaceForCodeGeneration()
{
	mEditingNode = NULL;


	BeginPanel();


        AddFolder(_("Generate"));


        this->AddFileOpener(_("Export File"),_("*.h;*.cpp"), &mFileExport);
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
        wxString enumList = _("");
	for (unsigned int i=0;i<list.size();i++)
	{
		enumList += prefix;
		enumList += list[i];
		if (i != (list.size()-1))
                        enumList += _(",\n");
		else
                        enumList += _("\n");
	}
	return enumList;

}

wxString GenerateSwitchCase(std::vector<wxString> & list, const wxString& functionPrefix, 
							const wxString& variableName, const wxString& functionParameter)
{
        wxString res = _("	switch(")+variableName+_(")\n	{\n");

	for (unsigned int i=0;i<list.size();i++)
	{
                res += _("		case ")+list[i]+_(":\n");
                res += _("			")+functionPrefix+list[i]+_("(")+functionParameter+_(");\n");
                res += _("			break;\n");
	}

        res += _("	}");

	return res;
}

wxString BuildMessagesStruct(std::vector<wxString> &list, 
							 std::vector<wxString> &code,
							 std::vector<wxString> &comment)
{
	wxString res;
	for (unsigned int i = 0;i<list.size();i++)
	{
                res += _("\n\n\n/*\n")+comment[i]+_("\n*/\n");
                res += _("typedef struct ")+list[i]+_("\n{\n");
                res += code[i]+_("\n} ")+list[i]+_(";\n\n");
	}

	return res;
}

wxString BuildFunctions(std::vector<wxString> &list, std::vector<wxString> &code,
						const wxString& prefix, const wxString& parameter, const wxString& className)
{
	wxString res;
	for (unsigned int i = 0;i<list.size();i++)
	{
		//res += "\n/*"+""/*comment[i]*/+"*/\n";
                res += _("inline void ")+prefix+list[i]+_("(")+parameter+_(")\n{\n\n");
                res += code[i]+_("\n}\n");
	}

	return res;
}

wxString BuildMessagesFunctions(std::vector<wxString> &list, std::vector<wxString> &codeList, std::vector<wxString> &stateList)
{
	wxString res;
	for (unsigned int i = 0;i<list.size();i++)
	{
                res += _("void Handle(const ")+list[i]+_("& mes)\n{\n");
                res += GenerateSwitchCase(stateList, _("Handle"), _("mState"), _("mes"));
                res += _("\n}\n\n");
	}
	return res;
}

wxString BuildRaknetPacketRead(std::vector<wxString> &meslist,const wxString& enumPrefix)
{
	wxString res;
        res += _("bool HandleRaknetPacket(Packet& packet)\n{\n");
        res += _("switch (packet->data[0])\n");
        res += _("{\n");
	for (unsigned int i=0;i<meslist.size();i++)
	{
                res += _("case ")+enumPrefix+meslist[i]+_(":\n");
                res += _("{\n");
                res += meslist[i]+_(" mes;\n");//		RakNetTime time, dataLength;
                res += _("RakNet::BitStream rakStream( packet->data+1, sizeof(")+meslist[i]+_("), false);\n");
                res += _("rakStram.Read(mes);\n");
                res += _("Handle(mes);\n");
                res += _("return true;\n");
                res += _("}\n");
	}
        res += _("}\nreturn false;\n}\n");

	return res;
}

wxString MakeAllStatesHandles(std::vector<wxString> &statelist, std::vector<wxString> &meslist)
{
	wxString res;

	for (unsigned int i=0;i<statelist.size();i++)
	{
		for (unsigned int j=0;j<meslist.size();j++)
		{
                        res += _("void Handle")+statelist[i]+_("(const ")+meslist[j]+_("& mes)\n");
                        res += _("{\n");

                        wxGraphNode *pStateNode = mScrollV->GetNodeByName(statelist[i].c_str());
                        wxString onit = _("On ")+meslist[j];
			res += *pStateNode->GetCode(onit.c_str());
                        res += _("\n}\n\n");
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

        wxString enumMessages = GenerateEnum(mMesList, _("Type_"));
        wxString enumStates = GenerateEnum(mStateList, _(""));

	wxString strTemplate;
	FILE *fp = fopen("CPPTemplate.template","rb");
	if (fp)
	{
		int len = _filelength(fileno(fp));
                wxChar *tmps = new wxChar [len+1];
		
		fread(tmps, len, 1, fp);
		tmps[len] = 0;
		fclose(fp);

		strTemplate = tmps;
		delete [] tmps;
	}
	else 
                return _("CPPTemplate.template NOT FOUND!!!");

	std::map<wxString, wxString> mTokens;

	wxString className = mScrollV->mGraphName.c_str();// gClassName.c_str();
        mTokens[_("%%STATEMACHINENAME%%")] = className;

	bool bHasFloatTime = true;
	bool bRaknetPacket = false;
	if (bHasFloatTime)
                mTokens[_("%%HASFLOATTIME%%")] = _("float timeEllapsed");
	else
                mTokens[_("%%HASFLOATTIME%%")] = _("");

        mTokens[_("%%MESSAGESENUM%%")] = enumMessages;

        mTokens[_("%%TICKSTATECASE%%")] = GenerateSwitchCase(mStateList, _("Tick"),
                _("mState"), bHasFloatTime?_("timeEllapsed"):_(""));
	

        mTokens[_("%%STATESENUM%%")] = enumStates;

        mTokens[_("%%MESSAGEALLSTRUCTS%%")] = BuildMessagesStruct(mMesList, mMesCodeList, mMesCommentList);

        mTokens[_("%%ALLTICKFUNCTIONS%%")] = BuildFunctions(mStateList, mTickCodeList, _("Tick"),
                bHasFloatTime?_("float timeEllapsed"):_(""), className);

        mTokens[_("%%ALLENTERFUNCTIONS%%")] = BuildFunctions(mStateList, mEnterCodeList, _("Enter"),
                _(""), className);

        mTokens[_("%%ALLLEAVEFUNCTIONS%%")] = BuildFunctions(mStateList, mLeaveCodeList, _("Leave"),
                _(""), className);


        mTokens[_("%%SETSTATESWITCH%%")] = GenerateSwitchCase(mStateList, _("Leave"),
                _("mState"), _(""))+_("\n")+
                _("mState = newState;")+
                GenerateSwitchCase(mStateList, _("Enter"),
                _("newState"), _(""));


        mTokens[_("%%ALLHANDLEMESSAGES%%")] = BuildMessagesFunctions(mMesList, mMesList, mStateList);
        mTokens[_("%%INITIALSTATE%%")] = mStateList[mScrollV->mInitialState];

        mTokens[_("%%HANDLERAKNETPACKET%%")] = bRaknetPacket?BuildRaknetPacketRead(mMesList, _("Type_")):wxString::FromUTF8("");
        mTokens[_("%%ALLSTATEMESSAGESHANDLE%%")] = MakeAllStatesHandles(mStateList, mMesList);

        mTokens[_("%%INCLUDES%%")] = *mScrollV->GetIncludes();
        mTokens[_("%%MEMBERSINIT%%")] = *mScrollV->GetMembersInit();
        mTokens[_("%%MEMBERSDECLARE%%")] = *mScrollV->GetMemberVariables();

	//wxString *GetMemberVariables() { return mMemberVariables; }

	SYSTEMTIME systime;
	GetSystemTime(&systime);

	int size = ::GetDateFormat(LOCALE_USER_DEFAULT, DATE_SHORTDATE, &systime, NULL, NULL, 0);
        wxChar *tmpdate = new wxChar [size+1];
	tmpdate[size] = 0;
	::GetDateFormat(LOCALE_USER_DEFAULT, DATE_SHORTDATE, &systime, NULL, tmpdate, size);


	size = ::GetTimeFormat(LOCALE_USER_DEFAULT, TIME_FORCE24HOURFORMAT, &systime, NULL, NULL, 0);
        wxChar *tmptime = new wxChar [size+1];
	tmptime[size] = 0;
	::GetTimeFormat(LOCALE_USER_DEFAULT, TIME_FORCE24HOURFORMAT, &systime, NULL, tmptime, size);

        mTokens[_("%%DATE%%")] = tmpdate;
        mTokens[_("%%TIME%%")] = tmptime;

	delete [] tmpdate;
	delete [] tmptime;
	// replacing
	std::map<wxString, wxString>::iterator iter = mTokens.begin();

	for (; iter != mTokens.end(); ++iter)
		strTemplate.Replace((*iter).first, (*iter).second);

	// write down
        fp = fopen(mScrollV->mOutputFileName.mb_str(),"wb");
	if (fp)
	{
		fwrite(strTemplate.c_str(), strTemplate.Len(), 1, fp);
		fclose(fp);
	}

	// Count "\n"
	int aCount = 0;
        for (unsigned int cnt = 0;cnt<strTemplate.Len(); cnt++)
	{
		if (strTemplate[cnt] == '\n')
			aCount++;
	}
        wxString GenInfos=_("Generation of ");
	GenInfos += mScrollV->mOutputFileName.c_str();//"res.h ";
        GenInfos += _(" OK!\n");
        GenInfos.Append(aCount);
        GenInfos += _(" Lines generated.\n");
	return GenInfos;
	

}
