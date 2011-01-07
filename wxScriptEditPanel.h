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

////////////////////////////////////////////////////////////////////////////////
// Zenith Engine
// File Name : wxScriptEditPanel.h
// Creation : 08/11/2006
// Author : Cedric Guillemet
// Description :
//
////////////////////////////////////////////////////////////////////////////////

#ifndef WXSCRIPTEDITPANEL_H__
#define WXSCRIPTEDITPANEL_H__

#include <wx/laywin.h>
#include <wx/foldbar/foldpanelbar.h>
#include "wxStackedControls.h"

#define ZEEditClass(x) EditClass(x->mInfosSerializableFields, x->mInfosSerializableName, x)

///////////////////////////////////////////////////////////////////////////////////////////////////

class wxScriptEditPanel :  public wxSashLayoutWindow
{
public:

    wxScriptEditPanel(wxWindow* parent);

    virtual ~wxScriptEditPanel();

    void SetScriptToEdit(const wxChar *szName, wxString* pCode);
	void SetToEdit(wxString *pToEdit) { mToEdit = pToEdit; }
	//virtual void SetNewText(const wxString& text) { *mToEdit = text; }
	
	virtual void UpdateScriptText() 
	{ 
		//*mToEdit = mScriptEdit->GetText();//text; 
		*mToEdit = mScriptEdit->GetText();//text; 

	}
private:

    DECLARE_EVENT_TABLE();

	void onNewChar(wxEvent& event);
    void OnSize(wxSizeEvent& event)
    {
        wxSashLayoutWindow::OnSize(event);
		mPanel->SetSize(wxRect(0,0, event.GetSize().GetWidth(), event.GetSize().GetHeight()));
    }
	void onRemoveLink(wxEvent& event);
	
	wxStaticText *mLib;
	wxStyledTextCtrl* mScriptEdit;
	wxButton *mDeleteLink;
	wxPanel *mPanel;
    wxString *mpText;
	wxString *mToEdit;
};

///////////////////////////////////////////////////////////////////////////////////////////////////
#endif
