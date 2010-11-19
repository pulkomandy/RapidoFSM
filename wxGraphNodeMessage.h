#ifndef WXGRAPHNODEMESSAGE_H__
#define WXGRAPHNODEMESSAGE_H__

#include "wxEdit.h"

class wxGraphNodeMessage : public wxGraphNode //,public wxZEdit
{
public:
	wxGraphNodeMessage(wxGraphContainer* parent) : wxGraphNode(parent) //,wxZEdit(static_cast<wxPanel*>(this))
	{
		mTextForeground = wxColour(240,240,40);
		mBackGround = wxColour(87,87,157);
		mCol1 = wxColour(117,117,177);
		mColSel1 = wxColour(255,255,0,0);
		mColSel2 = wxColour(0,0,0,0);
		mColForeground = wxColour(0,0,0);
		mbHasTumbnail = false;
                mLib = new wxStaticText(this, wxID_ANY, _(""));
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
