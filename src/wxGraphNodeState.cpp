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
#include "RapidoIO.h"

wxGraphNodeState::wxGraphNodeState(wxGraphContainer* parent) : wxGraphNode(parent) //,wxZEdit(static_cast<wxPanel*>(this))
{
	mTextForeground = wxColour(240,240,40);
	mBackGround = wxColour(157,87,87);
	mCol1 = wxColour(177,117,117);
	mColSel1 = wxColour(255,255,0,0);
	mColSel2 = wxColour(0,0,0,0);
	mColForeground = wxColour(0,0,0);
	mbHasTumbnail = false;
	mCurSelectedButton = wxT("");


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

wxGraphNodeState::~wxGraphNodeState()
{
}


TiXmlNode* wxGraphNodeState::CreateLegacyXmlNodeWithChildren()
{
	TiXmlElement* GraphNodeXmlElement = dynamic_cast<TiXmlElement*>(inherited::CreateLegacyXmlNodeWithChildren());
	GraphNodeXmlElement->SetAttribute("Type", "STATE");

	// Create Event nodes
	for (std::map<wxString, wxString>::iterator it = mEventsCode.begin(), end = mEventsCode.end() ; it != end ; ++it)
	{
		TiXmlElement* eventXmlElement = new TiXmlElement("Event");
		eventXmlElement->SetAttribute("name", (*it).first.Mid(3).ToUTF8());
		eventXmlElement->SetAttribute("code", (*it).second.ToUTF8());

		GraphNodeXmlElement->LinkEndChild(eventXmlElement);
	}

	return GraphNodeXmlElement;
}


TiXmlNode* wxGraphNodeState::CreateXmlNodeWithChildren()
{
	TiXmlElement* nodeXmlElement = dynamic_cast<TiXmlElement*>(inherited::CreateXmlNodeWithChildren());

	nodeXmlElement->SetAttribute("type", "STATE");

	for (std::map<wxString, wxString>::iterator it = mEventsCode.begin(), end = mEventsCode.end() ; it != end ; ++it)
	{
		TiXmlElement* eventXmlElement = new TiXmlElement("event");
		eventXmlElement->SetAttribute("name", (*it).first.Mid(3).ToUTF8());

		nodeXmlElement->LinkEndChild(eventXmlElement);

		if (!(*it).second.IsEmpty())
		{
			TiXmlElement* codeXmlElement = new TiXmlElement("code");
			TiXmlText* codeXmlText = new TiXmlText((*it).second.ToUTF8());
			codeXmlText->SetCDATA(true);
			codeXmlElement->LinkEndChild(codeXmlText);

			eventXmlElement->LinkEndChild(codeXmlElement);
		}
	}

	return nodeXmlElement;
}


void wxGraphNodeState::ParseXmlElement(TiXmlElement* aXmlElement)
{
	if (aXmlElement == NULL)
		return;

	inherited::ParseXmlElement(aXmlElement);

	// Read view
	TiXmlElement* viewXmlElement = aXmlElement->FirstChildElement("view");
	if (viewXmlElement)
	{
		int posX;
		int posY;
		int width;

		viewXmlElement->Attribute("posX", &posX);
		viewXmlElement->Attribute("posY", &posY);
		viewXmlElement->Attribute("width", &width);

		this->SetSize(posX, posY, width, -1);
	}

	// Read event
	TiXmlElement* eventElement = aXmlElement->FirstChildElement("event");
	while (eventElement != NULL)
	{

		TiXmlHandle eventElementHandle(eventElement);

		wxString eventName =  wxString::FromUTF8(eventElement->Attribute("name"));
		if (eventName.IsEmpty())
		{
			TiXmlText* codeXmlText = eventElementHandle.FirstChildElement("code").FirstChild().ToText();
			if (codeXmlText != NULL)
				mEventsCode[wxT("")] = wxString::FromUTF8(codeXmlText->Value());
			else
				mEventsCode[wxT("")] = wxT("");
		}
		else
		{
			wxString prefixedEventName = wxT("On ") + eventName;

			TiXmlText* codeXmlText = eventElementHandle.FirstChildElement("code").FirstChild().ToText();
			if (codeXmlText != NULL)
				mEventsCode[prefixedEventName] = wxString::FromUTF8(codeXmlText->Value());
			else
				mEventsCode[prefixedEventName] = wxT("");
		}
		eventElement = eventElement->NextSiblingElement("event");
	}

	this->Refresh();
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
