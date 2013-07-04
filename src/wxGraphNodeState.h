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

#ifndef WXGRAPHNODESTATE_H__
#define WXGRAPHNODESTATE_H__

#include "wxGraphNode.h"
void SetScriptToEdit(const char *szName, wxString* pCode);
class wxGraphNodeState : public wxGraphNode //,public wxZEdit
{
private:
	typedef wxGraphNode inherited;

public:
	wxGraphNodeState(wxGraphContainer* parent);
	virtual ~wxGraphNodeState();

	virtual GraphNodeType GetType()
	{
		return GNT_STATE;
	}


	virtual bool OnAddNewConnection(wxGraphNode *pOther)
	{
		if (pOther->GetType() == GNT_MESSAGE)
		{
			int i = mButCount;
			wxString onmachin =  wxString(_("On ")) + wxString(pOther->GetFunctionName());
			// check already present
			std::list<wxToggleButton*>::iterator iter = mButs.begin();
			for (; iter != mButs.end(); ++iter)
			{
				if ( (*iter)->GetLabel() == onmachin)
					return false;
			}

			wxToggleButton *mButton = new wxToggleButton(this, 2, onmachin, wxPoint(30, 28+i*18), wxSize(120,18));
			mButCount ++;
			mButs.push_back(mButton);
			mButton->Connect(wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, (wxObjectEventFunction)&wxGraphNodeState::OnBtClick);


			AddLeftPlug("In");
			AddRightPlug("To");

			SetSize(-1, -1, GetSize().x, GetSize().y+18);

			return true;
		}
		return false;
	}

	virtual void ChangeMessageName(const wxChar *szPrevious, const wxChar *szNew)
	{
		wxString prev = _("On ");
		prev += szPrevious;
		wxString neww = _("On ");
		neww += szNew;
		std::list<wxToggleButton*>::iterator iter = mButs.begin();
		for (; iter != mButs.end(); ++iter)
		{
			if ( prev == (*iter)->GetLabel() )
			{
				(*iter)->SetLabel(neww);
			}
		}
	}

	virtual void MessageHasBeenRemoved(const wxChar *szMessageName)
	{
		wxString prev;
		if ((szMessageName[0] == 'O') && (szMessageName[1] == 'n'))
			prev = wxT("");
		else
			prev = _("On ");

		int indx = 0;
		prev += szMessageName;
		std::list<wxToggleButton*>::iterator iter = mButs.begin();
		for (; iter != mButs.end(); ++iter)
		{
			if ( ( prev == (*iter)->GetLabel() ) && (indx>2))
			{
				wxToggleButton *abut = (*iter);
				delete abut;
				mButs.remove( abut );
				mRightPlugs.pop_back();
				mLeftPlugs.pop_back();
				mButCount--;
				return;
			}
			indx ++;
		}
	}

	int mButCount;

	void OnBtClick(wxEvent& event);
	virtual void LoseFocus()
	{
		std::list<wxToggleButton*>::iterator iter = mButs.begin();
		for (;iter != mButs.end(); ++iter)
		{
			(*iter)->SetValue(false);
		}
		mCurSelectedButton = wxT("");
	}
	virtual const wxString GetSubItem() { return mCurSelectedButton; }
	virtual wxString *GetCode(const wxString& szSubItem = wxT(""))
	{
		return &mEventsCode[szSubItem];
	}
	std::list<wxToggleButton*> mButs;
	wxString mCurSelectedButton;
	std::map<wxString, wxString> mEventsCode;

	virtual TiXmlNode* CreateLegacyXmlNodeWithChildren();
	virtual TiXmlNode* CreateXmlNodeWithChildren();
	virtual void ParseXmlElement(TiXmlElement *aXmlElement);

	virtual int GetAnchorIndexByName(const wxChar *szName);
	virtual void GetAllCodeConnections(std::map< wxString, std::vector<wxString> > & aCon);
	void GetStateAssignments(const wxChar*pSource, std::vector<wxString>& list);
};

#endif
