///////////////////////////////////////////////////////////////////////////////////////////////////
// Zenith Engine
// File Name : wxNavigator.cpp
// Creation : 08/11/2006
// Author : Cedric Guillemet
// Description :
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; version 2 of the License.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
///////////////////////////////////////////////////////////////////////////////////////////////////

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
#include <wx/stc/stc.h>
#include <wx/aui/aui.h>
#include "wxScriptEditPanel.h"
#include "wxStackedControls.h"
#include "wxEditGraphNode.h"
#include "wxGraphNode.h"
#include "wxGraphNodeState.h"
#include "wxGraphContainer.h"

///////////////////////////////////////////////////////////////////////////////////////////////////

BEGIN_EVENT_TABLE(wxScriptEditPanel, wxSashLayoutWindow)
    EVT_SIZE(wxScriptEditPanel::OnSize)
END_EVENT_TABLE()

///////////////////////////////////////////////////////////////////////////////////////////////////

wxScriptEditPanel::wxScriptEditPanel(wxWindow* parent)
                     : wxSashLayoutWindow(parent, -1, wxDefaultPosition, wxDefaultSize, wxNO_BORDER)
{

		mPanel = new wxPanel(this);
		wxSizer *mSizer = new wxBoxSizer(wxVERTICAL);
		wxSizer *mSizer2 = new wxBoxSizer(wxHORIZONTAL);


                mDeleteLink = new wxButton(mPanel, wxID_ANY, _("X"), wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT);
		mDeleteLink->Connect(wxEVT_COMMAND_BUTTON_CLICKED, (wxObjectEventFunction)&wxScriptEditPanel::onRemoveLink);
		mScriptEdit = wxZenScriptEdit::BuildScriptPanel(mPanel, (wxObjectEventFunction)&wxScriptEditPanel::onNewChar);


                mLib = new wxStaticText(mPanel, wxID_ANY, _(""), wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT);

		mSizer2->Add(mDeleteLink, 0, wxEXPAND + wxALL, 0);
		mSizer2->Add(mLib, 1, wxEXPAND + wxALL, 0);

		mSizer->Add(mSizer2, 0, wxEXPAND + wxALL, 0);
		mSizer->Add(mScriptEdit, 1, wxEXPAND + wxALL, 0);

		mPanel->SetSizer(mSizer);
		mpText = NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

wxScriptEditPanel::~wxScriptEditPanel()
{
}

void wxScriptEditPanel::SetScriptToEdit(const wxChar *szName, wxString* pCode)
{
	if (!pCode)
	{
		mScriptEdit->SetReadOnly(true);
		return;
	}
	mScriptEdit->SetReadOnly(false);
	mpText = NULL;
	mScriptEdit->SetText(*pCode);
	mpText = pCode;
	mLib->SetLabel(szName);

	//mScriptEdit->SetToEdit(mpText);
}
extern wxGraphContainer *mScrollV;
extern wxZEditNode * mEditNode;
void RefreshSelectedNode(wxGraphNode *pNode);
extern wxGraphContainer *mScrollV;

void wxScriptEditPanel::onNewChar(wxEvent& event)
{
	wxScriptEditPanel* pthis = (wxScriptEditPanel*)this->GetParent()->GetParent();
	if (!pthis->mpText)
		return;

	*pthis->mpText = pthis->mScriptEdit->GetText();//text; 
	mScrollV->RebuildConnectionsFor(mEditNode->mEditingNode);
}


void wxScriptEditPanel::onRemoveLink(wxEvent& event)
{
	if (mEditNode->mEditingNode)
	{
		if (mEditNode->mEditingNode->GetType() == GNT_STATE)
		{
			wxGraphNodeState *pst = ((wxGraphNodeState*)mEditNode->mEditingNode);
			mScrollV->RemoveConnection(pst, 0, pst->GetAnchorIndexByName(pst->mCurSelectedButton.Mid(3)));
			mEditNode->mEditingNode->MessageHasBeenRemoved(pst->mCurSelectedButton);
			mScrollV->Refresh();
		}
	}
}
