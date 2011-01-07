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
public:
	wxGraphNodeState(wxGraphContainer* parent) : wxGraphNode(parent) //,wxZEdit(static_cast<wxPanel*>(this))
	{
		mTextForeground = wxColour(240,240,40);
		mBackGround = wxColour(157,87,87);
		mCol1 = wxColour(177,117,117);
		mColSel1 = wxColour(255,255,0,0);
		mColSel2 = wxColour(0,0,0,0);
		mColForeground = wxColour(0,0,0);
		mbHasTumbnail = false;
                mCurSelectedButton = _("");


		AddRightPlug("To");
		AddRightPlug("To");
		AddRightPlug("To");

		//AddRightPlug("To");

		AddLeftPlug("In");
		AddLeftPlug("");
		AddLeftPlug("");
		//for (int i=0;i<2;i++)
		{
			AddLeftPlug("In");
			AddRightPlug("To");
		}

		mButCount = 3;


		for (int i=0;i<3;i++)
		{
                        static const wxChar *szLibOnEvent[]={_("On Enter"), _("On Tick"), _("On Leave")};
			wxToggleButton *mButton = new wxToggleButton(this, 2, szLibOnEvent[i], wxPoint(30, 28+i*18), wxSize(120,18));
			mButton->Connect(wxEVT_COMMAND_TOGGLEBUTTON_CLICKED, (wxObjectEventFunction)&wxGraphNodeState::OnBtClick);
			mButs.push_back(mButton);
		}

		SetSize(-1, -1, 180, 100);
	}
	virtual ~wxGraphNodeState()
	{
	}
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
                        prev = _("");
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
                mCurSelectedButton = _("");
	}
        virtual const wxChar* GetSubItem() { return mCurSelectedButton.c_str(); }
        virtual wxString *GetCode(const wxChar* szSubItem=_(""))
	{
		wxString keyItem = szSubItem;
		return &mEventsCode[keyItem];
	}
	std::list<wxToggleButton*> mButs;
	wxString mCurSelectedButton;
	std::map<wxString, wxString> mEventsCode;

	virtual wxString BuildGraphString();

        virtual int GetAnchorIndexByName(const wxChar *szName);
        virtual void GetAllCodeConnections(std::map< wxString, std::vector<wxString> > & aCon);
        void GetStateAssignments(const wxChar*pSource, std::vector<wxString>& list);
};

#endif
