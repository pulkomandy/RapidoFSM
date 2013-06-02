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

#include "RapidoIO.h"


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
#if 0
		AddStatic("Comments");
		AddScriptEdit(pNode->GetComment(), 2);

		if (pNode->GetType() == GNT_STATE)
		{
			AddFolder("Properties");
		}
#endif
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


void wxZEditNode::HandleButtonClick(unsigned int aButtonID)
{
	RapidoIO::GenerateCppFile(*mScrollV);
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

