#include "wxGraphNodeMessage.h"

wxGraphNodeMessage::wxGraphNodeMessage(wxGraphContainer* parent)
    : wxGraphNode(parent)
{
	mTextForeground = wxColour(240,240,40);
	mBackGround = wxColour(87,87,157);
	mCol1 = wxColour(117,117,177);
	mColSel1 = wxColour(255,255,0,0);
	mColSel2 = wxColour(0,0,0,0);
	mColForeground = wxColour(0,0,0);
	mbHasTumbnail = false;

	AddLeftPlug("Gen");
	AddRightPlug("To");
	SetSize(200,-1);
}


wxGraphNodeMessage::~wxGraphNodeMessage()
{
}


bool wxGraphNodeMessage::OnAddNewConnection(wxGraphNode *pOther)
{
	if (pOther->GetType() == GNT_STATE)
	{
		return pOther->OnAddNewConnection(this);
	}
	return false;
}


void wxGraphNodeMessage::OnPaint(wxPaintEvent& event)
{
	inherited::OnPaint(event);
}

TiXmlNode* wxGraphNodeMessage::CreateLegacyXmlNodeWithChildren()
{
	TiXmlElement* GraphNodeXmlElement = dynamic_cast<TiXmlElement*>(inherited::CreateLegacyXmlNodeWithChildren());
	GraphNodeXmlElement->SetAttribute("Type", "MESSAGE");

	return GraphNodeXmlElement;
}

TiXmlNode* wxGraphNodeMessage::CreateXmlNodeWithChildren()
{
	TiXmlElement* nodeXmlElement = dynamic_cast<TiXmlElement*>(inherited::CreateXmlNodeWithChildren());
	nodeXmlElement->SetAttribute("type", "MESSAGE");

	return nodeXmlElement;
}


void wxGraphNodeMessage::ParseXmlElement(TiXmlElement *aXmlElement)
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
		int height;

		viewXmlElement->Attribute("posX", &posX);
		viewXmlElement->Attribute("posY", &posY);
		viewXmlElement->Attribute("width", &width);
		viewXmlElement->Attribute("height", &height);

		this->SetSize(posX, posY, width, height);
	}

	this->Refresh();
}
