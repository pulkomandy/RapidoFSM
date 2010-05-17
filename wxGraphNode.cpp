///////////////////////////////////////////////////////////////////////////////////////////////////
// Zenith Engine
// File Name : wxGraphNode.cpp
// Creation : 22/03/2007
// Author : Cedric Guillemet
// Description : 
///////////////////////////////////////////////////////////////////////////////////////////////////

#include "wxGraphNode.h"
#include "wxGraphContainer.h"

///////////////////////////////////////////////////////////////////////////////////////////////////

BEGIN_EVENT_TABLE(wxGraphNode, wxPanel)
EVT_PAINT(wxGraphNode::OnPaint)
EVT_SIZE(wxGraphNode::OnSize)
EVT_LEFT_DOWN(wxGraphNode::OnLButtonDown) 	//Process a wxEVT_LEFT_DOWN event. The handler of this event should normally call event.Skip() to allow the default processing to take place as otherwise the window under mouse wouldn't get the focus.
EVT_LEFT_UP(wxGraphNode::OnLButtonUp) 	        //Process a wxEVT_LEFT_UP event.
EVT_MOTION(wxGraphNode::OnMouseMotion) 	        //Process a wxEVT_MOTION event.
EVT_ERASE_BACKGROUND(wxGraphNode::OnEraseBack) 	        
END_EVENT_TABLE()

///////////////////////////////////////////////////////////////////////////////////////////////////

wxGraphNode::wxGraphNode(wxGraphContainer * parent)
    : wxPanel((wxGraphContainer*)(parent), -1, wxDefaultPosition, wxDefaultSize, wxNO_BORDER)
{
    mHeader = _("Header");
    mbHasTumbnail = false;
    mbMoving = false;
    mbSelected = false;
    mParent = (wxGraphContainer*)parent;
    mbCreatingNewConnection = false;

	mTextForeground = wxColour(240,240,40);
	mBackGround = wxColour(107,107,107);
	mCol1 = wxColour(127,127,127);
	mColSel1 = wxColour(255,255,0,0);
	mColSel2 = wxColour(0,0,0,0);
	mColForeground = wxColour(0,0,0);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

wxGraphNode::~wxGraphNode()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void wxGraphNode::OnPaint(wxPaintEvent& )
{
    wxPaintDC odc(this);

    wxRect rc;

    rc = GetRect();
    
    if ( (rc.GetWidth() !=  mMemBmp.GetWidth()) || (rc.GetHeight() !=  mMemBmp.GetHeight()) )
        mMemBmp.Create(rc.GetWidth(), rc.GetHeight());

    memDC.SelectObject(mMemBmp);


    wxFont font(wxNORMAL_FONT->GetPointSize(),
                        wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL,
                        wxFONTWEIGHT_NORMAL);

    wxSize txtSize; // for header
    txtSize = memDC.GetTextExtent(mHeader) + wxSize(0,8);
    memDC.SetBrush(wxBrush(mCol1));
    if (mbSelected)
        memDC.SetPen(wxPen(mColSel1));
    else
        memDC.SetPen(wxPen(mColSel2));

    memDC.DrawRectangle(0,0,rc.GetWidth(),txtSize.y);
    memDC.SetBrush(wxBrush(mBackGround));
    memDC.DrawRectangle(0,txtSize.y+1,rc.GetWidth(), rc.GetHeight()-txtSize.y -1 );


    memDC.SetFont(font);

    memDC.SetTextForeground(mColForeground);
    memDC.DrawText(mHeader,5,5);
    memDC.SetTextForeground(mTextForeground);
    memDC.DrawText(mHeader,4,4);



    int decalY = txtSize.y+5;
    int minimalRightWidth = 120;
    int minimalLeftWidth = 5;
    int mMaxHeight = 120;
    unsigned int i;

    // Right
    for (i=0;i<mRightPlugs.size();i++)
    {

        wxSize plugTxtSize = memDC.GetTextExtent(mRightPlugs[i].mLabel) + wxSize(10,0);

        memDC.SetTextForeground(wxColour(0,0,0));
        memDC.DrawText(mRightPlugs[i].mLabel, rc.GetWidth() - plugTxtSize.x , decalY);
        memDC.SetTextForeground(wxColour(240,240,240));
        memDC.DrawText(mRightPlugs[i].mLabel, rc.GetWidth() - plugTxtSize.x-1 , decalY-1);
        

        if ((plugTxtSize.x+5)>minimalRightWidth) minimalRightWidth = plugTxtSize.x+5;


        memDC.SetPen(wxPen(wxColour(0,0,0,0)));
        memDC.SetBrush(wxBrush(wxColour(0,0,0)));
        int dfl = (plugTxtSize.y-8)/2;
        memDC.DrawRectangle(rc.GetWidth() - 8, decalY + dfl , 8, 8);


        decalY+=20;//plugTxtSize.y+2;

    }
    if (decalY>mMaxHeight) mMaxHeight = decalY;

    decalY = txtSize.y+5;
    for (i=0;i<mLeftPlugs.size();i++)
    {
		wxSize plugTxtSize;


			
		if ((plugTxtSize.x+5)>minimalLeftWidth) minimalLeftWidth = plugTxtSize.x+5;

		if (!mLeftPlugs[i].mLabel.empty())
		{
			plugTxtSize = memDC.GetTextExtent(mLeftPlugs[i].mLabel) + wxSize(10,0);

			memDC.SetTextForeground(wxColour(0,0,0));
			memDC.DrawText(mLeftPlugs[i].mLabel, 11 , decalY);
			memDC.SetTextForeground(wxColour(240,240,240));
			memDC.DrawText(mLeftPlugs[i].mLabel, 10 , decalY-1);
	        

			


			memDC.SetPen(wxPen(wxColour(0,0,0,0)));
			memDC.SetBrush(wxBrush(wxColour(0,0,0)));
			int dfl = (plugTxtSize.y-8)/2;
			memDC.DrawRectangle(0, decalY + dfl , 8, 8);
		}
		else
		{
                        plugTxtSize = memDC.GetTextExtent(_("AA")) + wxSize(10,0);
		}

        decalY+=20;//plugTxtSize.y+2;

    }
    if (decalY>mMaxHeight) mMaxHeight = decalY;

    if (mbHasTumbnail)
    {
        memDC.SetBrush(wxBrush(wxColour(0,0,0)));
        memDC.DrawRectangle(minimalLeftWidth, txtSize.y+8 , mThumbSize.x, mThumbSize.y);
        if ( (mThumbSize.y+txtSize.y+12)>mMaxHeight) mMaxHeight = mThumbSize.y+txtSize.y+12;
    }

    int ThumbSizeX = mbHasTumbnail?mThumbSize.x+12:0;

    if ( ((minimalRightWidth+ThumbSizeX+minimalLeftWidth)>rc.GetWidth()) )
    {
        SetSize(wxSize(minimalRightWidth+ThumbSizeX+minimalLeftWidth, -1));
        GetParent()->Refresh();
        //Refresh();
        
    }

    if (mMaxHeight>rc.GetHeight())
    {
        SetSize(wxSize(-1, mMaxHeight));
        //Refresh();
        GetParent()->Refresh();
    }


/*
    memDC.Blit(4, 5, mSize.x-8, mSize.y-9, &memDC, 0, 0 );
    for (int i=0;i<mEntries.size();i++)
    {
        if (mpSelected == &mEntries[i])
            memDC.DrawBitmap(mPointerSelected, mEntries[i].mPosition*(mSize.x-9), 0, true);
        else
            memDC.DrawBitmap(mPointer, mEntries[i].mPosition*(mSize.x-9), 0, true);
    }
    */

    odc.Blit(0, 0, mMemBmp.GetWidth(), mMemBmp.GetHeight(), &memDC, 0, 0);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void wxGraphNode::OnLButtonDown(wxMouseEvent& event)
{
    event.Skip();
    // try to select a link point
    int aSide, aNumber, aMaxCon;
    if (GetAnchor(wxPoint(event.m_x, event.m_y), aSide, aNumber, aMaxCon))
    {
        if (aMaxCon ==1)
        {
            mParent->RemoveConnection(this, aSide, aNumber);
        }
        CaptureMouse();
        mParent->BeginNewConnection(this, aSide, aNumber);
        mbCreatingNewConnection = true;
        mParent->SetNewConnectionEndPoint(GetPosition() + wxPoint(event.m_x, event.m_y));
        return;
    }


    // move node
    mbMoving = true;
    mSvgPtr = wxPoint(event.m_x, event.m_y);
    CaptureMouse();
    if (!mbSelected)
    {
        mParent->SetSelectedNode(this);
    }

}

///////////////////////////////////////////////////////////////////////////////////////////////////

void wxGraphNode::OnLButtonUp(wxMouseEvent& event)
{
    if (mbCreatingNewConnection)
    {
        ReleaseMouse();
        mbCreatingNewConnection = false;

        // get node
        wxGraphNode *pNodeDest= mParent->GetNode(wxPoint(event.m_x, event.m_y)+GetPosition());
        if (pNodeDest)
        {
            int aSide, aNumber, aMaxCon;
            if (pNodeDest->GetAnchor(wxPoint(event.m_x, event.m_y)+GetPosition()-pNodeDest->GetPosition(), aSide, aNumber, aMaxCon))
            {
                if (aMaxCon ==1)
                {
                    mParent->RemoveConnection(pNodeDest, aSide, aNumber);
                }
				if (pNodeDest->OnAddNewConnection(mParent->mNewConnectionSourceNode))
					mParent->EndNewConnection(pNodeDest, aSide, aNumber);
				else
					mParent->CancelNewConnection();

				
                return;

            }
        }
        

        // get node anchor

    

        mParent->CancelNewConnection();
        
        return;
    }

    if (mbMoving)
    {
        ReleaseMouse();
        mbMoving = false;
		
    }

}

///////////////////////////////////////////////////////////////////////////////////////////////////

void wxGraphNode::OnMouseMotion(wxMouseEvent& event)
{
    if (mbCreatingNewConnection)
    {
        mParent->SetNewConnectionEndPoint(GetPosition() + wxPoint(event.m_x, event.m_y));
        return;
    }

    if (mbMoving)
    {
        SetPosition(GetPosition()-mSvgPtr+wxPoint(event.m_x, event.m_y));

        GetParent()->Refresh();
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void wxGraphNode::OnSize(wxSizeEvent& event)
{
    mMemBmp.Create(event.m_size.x, event.m_size.y);
    Refresh();
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void wxGraphNode::AddThumbnail(unsigned int sizeX, unsigned int sizeY)
{
    mThumbSize = wxSize(sizeX, sizeY);
    mbHasTumbnail = true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

wxPoint wxGraphNode::GetPlugPointByName(const wxString &name)
{
    wxSize txtSize; // for header
    wxRect rc;
    unsigned int i;
    wxClientDC dc(this);

    txtSize = dc.GetTextExtent(mHeader) + wxSize(0,8);
    rc = GetRect();
    int decalY = txtSize.y+5;
    for (i=0;i<mRightPlugs.size();i++)
    {
        wxSize plugTxtSize = dc.GetTextExtent(mRightPlugs[i].mLabel);
        if (mRightPlugs[i].mLabel == name)
        {
            
            return wxPoint(rc.GetRight() +1, rc.GetTop()+decalY +2/*+ plugTxtSize.y/2*/);
        }
        decalY+=plugTxtSize.y+2;
    }

    decalY = txtSize.y+5;
    for (i=0;i<mLeftPlugs.size();i++)
    {
        wxSize plugTxtSize = dc.GetTextExtent(mLeftPlugs[i].mLabel);
        if (mLeftPlugs[i].mLabel == name)
        {
            
            return wxPoint(rc.GetLeft(), rc.GetTop()+decalY +2/*+ plugTxtSize.y/2*/);
        }
        decalY+=plugTxtSize.y+2;
    }
    return wxPoint(-1, -1);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

wxPoint wxGraphNode::GetPlugPointIndex(unsigned int side, unsigned int index)
{
    wxSize txtSize; // for header
    wxRect rc;
    unsigned int i;
    wxClientDC dc(this);

    txtSize = dc.GetTextExtent(mHeader) + wxSize(0,8);
    rc = GetRect();

    int decalY;

    if (side ==1)
    {
        decalY = txtSize.y+5;
        for (i=0;i<mRightPlugs.size();i++)
        {
            wxSize plugTxtSize = dc.GetTextExtent(mRightPlugs[i].mLabel);
            if (i == index)
            {
                
                return wxPoint(rc.GetRight() +1, rc.GetTop()+decalY +2/*+ plugTxtSize.y/2*/);
            }
            decalY+=20;//plugTxtSize.y-1;//+2;
        }
    }
    else if (side == 0)
    {
        decalY = txtSize.y+5;
        for (i=0;i<mLeftPlugs.size();i++)
        {
            wxSize plugTxtSize = dc.GetTextExtent(mLeftPlugs[i].mLabel);
            if (i == index)
            {
                
                return wxPoint(rc.GetLeft(), rc.GetTop()+decalY +2/*+ plugTxtSize.y/2*/);
            }
            decalY+=20;//plugTxtSize.y-1;//+2;
        }
    }
    return wxPoint(-1, -1);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void wxGraphNode::OnEraseBack(wxEraseEvent& )
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////

bool wxGraphNode::GetAnchor(wxPoint pos, int & aSide, int & aNumber, int &MaxCon)
{
    wxSize txtSize; // for header
    txtSize = memDC.GetTextExtent(mHeader) + wxSize(0,8);
    wxRect rc = GetRect();

    int decalY = txtSize.y+5;
    int minimalLeftWidth = 0;
    unsigned int i;

    for (i=0;i<mRightPlugs.size();i++)
    {

		if (!mRightPlugs[i].mLabel.empty())
		{
			wxSize plugTxtSize = memDC.GetTextExtent(mRightPlugs[i].mLabel) + wxSize(10,0);

			int dfl = (plugTxtSize.y-8)/2;

			int ex = pos.x - (rc.GetWidth() - 8);
			int ey = pos.y - (decalY + dfl);
			if ( ( ex>=0 ) && ( ex<=8) && (ey>=0) && (ey <= 8))
			{
				aSide = 1;
				aNumber = i;
				MaxCon = mRightPlugs[i].mConnectionPossibility;
				return true;
			}
		}


        decalY+=20;//plugTxtSize.y+2;
    }


    decalY = txtSize.y+5;
    for (i=0;i<mLeftPlugs.size();i++)
    {
		if (!mLeftPlugs[i].mLabel.empty())
		{
			wxSize plugTxtSize = memDC.GetTextExtent(mLeftPlugs[i].mLabel) + wxSize(10,0);


			if ((plugTxtSize.x+5)>minimalLeftWidth) minimalLeftWidth = plugTxtSize.x+5;
			int dfl = (plugTxtSize.y-8)/2;


			int ex = pos.x;
			int ey = pos.y - (decalY + dfl);
			if ( ( ex>=0 ) && ( ex<=8) && (ey>=0) && (ey <= 8))
			{
				aSide = 0;
				aNumber = i;
				MaxCon = mLeftPlugs[i].mConnectionPossibility;
				return true;
			}
		}


        decalY+=20;//plugTxtSize.y+2;

    }

    return false;
}


///////////////////////////////////////////////////////////////////////////////////////////////////


void wxGraphNode::SetFinalNode()
{
    mParent->SetfinalNode(this);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
wxString CodeToXML(const wxString & str);
wxString XMLToCode(const wxString & str);


wxString wxGraphNode::BuildGraphString()
{
	wxString res;

        res = _("<Node name=\"");
	res += GetFunctionName();
        res += _("\" comment=\"");
	res += mComment;
        res += _("\" code=\"");
	res += CodeToXML(mCode);
        res += _("\" ");

        wxString tmps;
        tmps.Printf(_("posx=\"%d\" posy=\"%d\" width=\"%d\" height=\"%d\""),
		GetPosition().x, 
		GetPosition().y,
		GetSize().x,
		GetSize().y);

	res += tmps;

        res += _(" />\n");

	return res;
}
