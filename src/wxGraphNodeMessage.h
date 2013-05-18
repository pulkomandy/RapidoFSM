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

#ifndef WXGRAPHNODEMESSAGE_H__
#define WXGRAPHNODEMESSAGE_H__

#include "wxGraphNode.h"

class wxGraphNodeMessage : public wxGraphNode
{
public:
	wxGraphNodeMessage(wxGraphContainer* parent) : wxGraphNode(parent)
	{
		mTextForeground = wxColour(240,240,40);
		mBackGround = wxColour(87,87,157);
		mCol1 = wxColour(117,117,177);
		mColSel1 = wxColour(255,255,0,0);
		mColSel2 = wxColour(0,0,0,0);
		mColForeground = wxColour(0,0,0);
		mbHasTumbnail = false;
		mLib = new wxStaticText(this, wxID_ANY, wxT(""));
		mLib->SetSize(160,120);
		mLib->SetPosition(wxPoint(30, 28));
		mLib->SetLabel(*GetCode());
		//mLib->SetBackgroundStyle(wxBG_STYLE_CUSTOM);
		//mLib->SetBackgroundColour(wxTRANSPARENT);

		AddLeftPlug("Gen");
		AddRightPlug("To");
		SetSize(200,-1);
	}

	virtual ~wxGraphNodeMessage()
	{
	}

	wxStaticText *mLib;

	virtual GraphNodeType GetType()
	{
		return GNT_MESSAGE;
	}

	virtual bool OnAddNewConnection(wxGraphNode *pOther)
	{
		if (pOther->GetType() == GNT_STATE)
		{
			return pOther->OnAddNewConnection(this);
		}
		return false;
	}

	virtual void OnPaint(wxPaintEvent& event)
	{
		mLib->SetLabel(*GetCode());
		wxGraphNode::OnPaint(event);
	}

	virtual wxString BuildGraphString()
	{
		return wxGraphNode::BuildGraphString();
	}

};

#endif
