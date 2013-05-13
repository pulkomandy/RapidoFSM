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
// File Name : wxGraphNode.h
// Creation : 22/03/2007
// Author : Cedric Guillemet
// Description :
////////////////////////////////////////////////////////////////////////////////

#ifndef WXGRAPHNODE_H__
#define WXGRAPHNODE_H__

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
#include <list>
#include <map>


///////////////////////////////////////////////////////////////////////////////////////////////////

class wxGraphContainer;
enum GraphNodeType
{
	GNT_DEFAULT,
	GNT_MESSAGE,
	GNT_STATE
};
///////////////////////////////////////////////////////////////////////////////////////////////////

class wxGraphNode : public wxPanel //, public wxGraphNode
{
public:

    wxGraphNode(wxGraphContainer* parent);

    ~wxGraphNode();

    virtual void SetFunctionName(const wxChar * sFunction) { mHeader = sFunction; Refresh(); }
    virtual const wxChar *GetFunctionName() { return mHeader.c_str(); }

        virtual void SetCode(const char * sFunction) { mCode.FromUTF8(sFunction); Refresh(); }
    virtual wxString *GetCode(const wxChar*szSubItem=_("")) { return &mCode; }

        virtual void SetComment(const char * sFunction) { mComment.FromUTF8(sFunction); Refresh(); }
    virtual wxString *GetComment() { return &mComment; }

        virtual const wxChar* GetSubItem() { return _(""); }

    virtual void AddRightPlug(const char * sPlugName, int aCon = 0)
    {
        mRightPlugs.push_back(NodePlug(wxString::FromUTF8(sPlugName),aCon));
    }

    virtual void AddLeftPlug(const char * sPlugName, int aCon = 0)
    {
        mLeftPlugs.push_back(NodePlug(wxString::FromUTF8(sPlugName), aCon));
    }

    virtual unsigned int GetNbRightPlugs() { return mRightPlugs.size(); }
    virtual unsigned int GetNbLeftPlugs() { return mLeftPlugs.size(); }
    virtual const wxChar * GetRightPlugName(unsigned int aPlugIndex) { return mRightPlugs[aPlugIndex].mLabel.c_str(); }
    virtual const wxChar * GetLeftPlugName(unsigned int aPlugIndex) { return mLeftPlugs[aPlugIndex].mLabel.c_str(); }

	virtual void LoseFocus() {}
	virtual bool OnAddNewConnection(wxGraphNode *pOther)
	{
		return false;
	}
	virtual GraphNodeType GetType()
	{
		return GNT_DEFAULT;
	}

	virtual wxString BuildGraphString();
    virtual void AddThumbnail(unsigned int sizeX, unsigned int sizeY);

    wxPoint GetPlugPointByName(const wxString &name);
    wxPoint GetPlugPointIndex(unsigned int side, unsigned int index);
    void SetSelected(bool bSel) { mbSelected = bSel; }
    bool GetAnchor(wxPoint pos, int & aSide, int & aNumber, int &MaxCon);

    void SetScript(const char *script)
    {
        mScript = wxString::FromUTF8(script);
    }
    void SetFinalNode();

	virtual int GetAnchorIndexByName(const wxChar *szName) { return 0; }
	virtual void GetAllCodeConnections(std::map<wxString, std::vector<wxString> > & aCon) {}
	virtual void ChangeMessageName(const wxChar *szPrevious, const wxChar *szNew) {}
	virtual void MessageHasBeenRemoved(const wxChar *szMessageName) {}


protected:
    DECLARE_EVENT_TABLE();
    virtual void OnPaint(wxPaintEvent& event);
    void OnLButtonDown(wxMouseEvent& event);
    void OnLButtonUp(wxMouseEvent& event);
    void OnMouseMotion(wxMouseEvent& event);
    void OnSize(wxSizeEvent& event);
    void OnEraseBack(wxEraseEvent& event);

    wxString mHeader;

    typedef struct NodePlug
    {
        NodePlug(const wxString &aLabel, int aConnectionPossibility)
        {
            mLabel = aLabel;
            mConnectionPossibility = aConnectionPossibility;
        }

        wxString mLabel;
        int mConnectionPossibility; //  0=infinite 1 = only one
    } NodePlug;

    std::vector<NodePlug> mRightPlugs, mLeftPlugs;
    wxSize mThumbSize;
    bool mbHasTumbnail;

    bool mbMoving;
    wxPoint mSvgPtr;
    wxBitmap mMemBmp;
    wxMemoryDC memDC;
    bool mbSelected;

    wxGraphContainer *mParent;
    bool mbCreatingNewConnection;

    wxString mScript;

	wxString mCode, mComment;

	// colors
	wxColour mTextForeground ,mBackGround ,mCol1 ,mColSel1 ,mColSel2 ,mColForeground;

};

#endif
