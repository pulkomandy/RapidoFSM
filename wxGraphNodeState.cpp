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

#include <wx/tglbtn.h>
#include "wxGraphContainer.h"
#include "wxGraphNodeState.h"

wxString CodeToXML(const wxString & str);
wxString XMLToCode(const wxString & str);

wxString wxGraphNodeState::BuildGraphString()
{
	wxString res = wxGraphNode::BuildGraphString();
	std::map<wxString, wxString>::iterator iter = mEventsCode.begin();
	for (;iter != mEventsCode.end() ; ++iter)
	{
		res += wxT("	<Event name=\"");
		res += (*iter).first.Mid(3);
		res += wxT("\" code=\"");
		res += CodeToXML((*iter).second);
		res += wxT("\" />\n");
	}

	return res;
}


int wxGraphNodeState::GetAnchorIndexByName(const wxChar *szName)
{
	int idx = 0;

	std::list<wxToggleButton*>::iterator iter = mButs.begin();
	for (;iter != mButs.end(); ++iter)
	{
		wxString szAnchor = (*iter)->GetLabel().Mid(3);
		if (szAnchor == szName)
			return idx;
		idx ++;
	}
	return -1;
}

void wxGraphNodeState::GetStateAssignments(const wxChar*pSource, std::vector<wxString>& list)
{
	//const char *start = pSource;
        wxString finds = pSource;
        while(int offset = finds.Find(wxT("SetState(")))
        {
            finds.Remove(0,offset);
            int end = finds.Find(')');
            wxString res = finds.Left(end);
            list.push_back(res);
        }
}

void wxGraphNodeState::GetAllCodeConnections(std::map<wxString, std::vector<wxString> > & aCon)
{
	std::map<wxString, wxString>::iterator iter = mEventsCode.begin();
	for (;iter != mEventsCode.end() ; ++iter)
	{
		const wxString& code = (*iter).first.Mid(3);
		std::vector<wxString> mAssignsTo;
		GetStateAssignments((*iter).second.c_str(), mAssignsTo);
		for (unsigned int i=0;i<mAssignsTo.size();i++)
		{
			aCon[code].push_back(mAssignsTo[i]);
		}
	}
}

void wxGraphNodeState::OnBtClick(wxEvent& event)
{
	wxToggleButton* pBut = (wxToggleButton*)event.GetEventObject();
	wxGraphNodeState *pParent = (wxGraphNodeState *)pBut->GetParent();
	pParent->LoseFocus();
	pBut->SetValue(true);

	pParent->mCurSelectedButton = pBut->GetLabel();
	pParent->mParent->SetSelectedNode(pParent);
}
