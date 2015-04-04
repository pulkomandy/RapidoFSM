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
// File Name : wxGraphContainer.cpp
// Creation : 22/06/2007
// Author : Cedric Guillemet
// Description :
////////////////////////////////////////////////////////////////////////////////

#include <sstream>

#include <wx/ffile.h>
#include <wx/tglbtn.h>

#include "tinyxml.h"

#include "wxGraphContainer.h"
#include "wxGraphNodeMessage.h"
#include "wxGraphNodeState.h"
#include "RapidoIO.h"

///////////////////////////////////////////////////////////////////////////////////////////////////

BEGIN_EVENT_TABLE(wxGraphContainer, wxNotebookPage) //wxPanel)
	EVT_PAINT(wxGraphContainer::OnPaint)
	EVT_ERASE_BACKGROUND(wxGraphContainer::OnEraseBack)
	EVT_SCROLLWIN(wxGraphContainer::OnScroll)
	EVT_SIZE(wxGraphContainer::OnSize)
	EVT_LEFT_DOWN(wxGraphContainer::OnLButtonDown)      //Process a wxEVT_LEFT_DOWN event. The handler of this event should normally call event.Skip() to allow the default processing to take place as otherwise the window under mouse wouldn't get the focus.
	EVT_LEFT_UP(wxGraphContainer::OnLButtonUp)          //Process a wxEVT_LEFT_UP event.
	EVT_MOTION(wxGraphContainer::OnMouseMotion)         //Process a wxEVT_MOTION event.
END_EVENT_TABLE()

///////////////////////////////////////////////////////////////////////////////////////////////////

wxGraphContainer::wxGraphContainer(wxWindow* parent) :
	wxNotebookPage(parent, -1)
	, mGraphName(_("Unnamed"))
	, mbMoving(false)
	, mOutputFileName(wxT("res.h"))
	, mbCreatingNewConnection(false)
	, mbRaknetMessage(true)
	, mbTickHasTime(true)
	, mSelectedNode(NULL)
	, mFinalNode(NULL)
	, mInitialState(0)
{
    // fake connection
#if 0
	wxGraphNode * nGraphNode = AddNode();
    nGraphNode->SetFunctionName("DiffuseMaterial");
    // add plug: a ajouter en param: le type (composante, 3 composante, 4 composantes, plusieurs sorties possibles)
    // pour les liens, on stocke: le N° de node source/dest (index dans la liste), le coté (2 bits), le n° d'entree
    // self linking impossible
    nGraphNode->AddRightPlug("Diffuse",1);
    nGraphNode->AddRightPlug("SelfIllum",1);
    /*nGraphNode->AddRightPlug("Normal",1);
    nGraphNode->AddRightPlug("Emissive",1);
    nGraphNode->AddRightPlug("This is a uber uber long text",1);
*/

    nGraphNode->AddThumbnail(100,100);
    ((wxGraphNode*)nGraphNode)->SetPosition(wxPoint(40,100));

    nGraphNode->SetFinalNode();
    nGraphNode->SetScript("float4 DiffuseMaterial(float4 Diffuse, float4 SelfIllum) { return Diffuse+SelfIllum; }");


    wxGraphNode * nGraphNode4 = AddNode();

    nGraphNode4->SetFunctionName("Mul");
    nGraphNode4->AddLeftPlug("rgba");
    nGraphNode4->AddRightPlug("Op1",1);
    nGraphNode4->AddRightPlug("Op2",1);

    nGraphNode4->AddThumbnail(100,100);
    ((wxGraphNode*)nGraphNode4)->SetPosition(wxPoint(340,40));
    nGraphNode4->SetScript("float4 Add(float4 Op1, float4 Op2) { return Op1*Op2; }");


#endif


/*
    wxGraphNode * nGraphNode2 = AddNode();

    nGraphNode2->SetFunctionName("TextureUnit0");
    nGraphNode2->AddLeftPlug("rgba");
    nGraphNode2->AddLeftPlug("r");
    nGraphNode2->AddLeftPlug("g");
    nGraphNode2->AddLeftPlug("b");
    nGraphNode2->AddLeftPlug("a");
    nGraphNode2->AddRightPlug("UVs");

    nGraphNode2->AddThumbnail(100,100);
    ((wxGraphNode*)nGraphNode2)->SetPosition(wxPoint(640,100));
    nGraphNode2->SetScript("float4 TextureUnit0() { return tex2D(IN.texUnit0,float2(0,0)); }");
*/

#if 0
    wxGraphNode * nGraphNode3 = AddMessageNode();
    nGraphNode3->SetFunctionName("GameSet_t");
    nGraphNode3->SetPosition(wxPoint(40,40));
	nGraphNode3->SetCode("tstring mMapName;\nint mNbLaps;\nint mGameMode;\n");
	nGraphNode3->SetComment("Structure contenant les infos du jeu à lancer: nom du circuit, mode de jeu et nombre de tours a faire.");


    wxGraphNode * nGraphNode7 = AddMessageNode();
    nGraphNode7->SetFunctionName("ShipSelected_t");
    nGraphNode7->SetPosition(wxPoint(40,240));
	nGraphNode7->SetCode("tstring mShipName;\n");
	nGraphNode7->SetComment("Un joueur a choisi un vaisseau. Ce paquet réseau est envoyé au serveur quand il a validé son choix");

    wxGraphNode * nGraphNode8 = AddMessageNode();
    nGraphNode8->SetFunctionName("PlayerSpectator_t");
    nGraphNode8->SetPosition(wxPoint(40,440));
	nGraphNode8->SetCode("bool mbPlayerOrSpectator;\n");
	nGraphNode8->SetComment("Un joueur connecté demande d'etre spectateur ou joueur. S'il demande a joueur, on instancie avec réplication réseau un vaisseau. S'il est spectateur, il n'incrémente pas le compteur pour le nombre de joueur minimal pour lancer la partie.");


    wxGraphNode * nGraphNode4b = AddStateNode();
    nGraphNode4b->SetFunctionName("Idle");
    nGraphNode4b->SetPosition(wxPoint(240,40));
	nGraphNode4b->SetComment("Etat initial du serveur. On n'y retourne plus jamais ensuite puisque le serveur est toujours avec une map chargée. Cet etat est très éphémère puisque le serveur, même dédié, doit charger un circuit juste après son lancement.");
	*nGraphNode4b->GetCode("On Tick") = "SetState(Map_Loading);";



    wxGraphNode * nGraphNode5 = AddStateNode();
    nGraphNode5->SetFunctionName("Map_Loading");
    nGraphNode5->SetPosition(wxPoint(640,40));
	nGraphNode5->SetComment("Le serveur charge une map.");

    wxGraphNode * nGraphNode6 = AddStateNode();
    nGraphNode6->SetFunctionName("Waiting_Players");
    nGraphNode6->SetPosition(wxPoint(640,40));
	nGraphNode6->SetComment("En attente de validation de vaisseau des players connectés");

	*GetIncludes() = "#include <stdio.h>\n#include <math.h>";
	*this->GetMemberVariables() = "int toto;\nchar zob;\n";
	*this->GetMembersInit() = "toto = 4;";
  /*
    AddConnection(1,2,0,0,1,0);
    AddConnection(3,1,0,1,0,1);
    AddConnection(1,0,0,0,0,1);
    AddConnection(3,0,0,1,0,1);
*/

	AddConnection(this->GetNodeIndexByName("Idle"),
		this->GetNodeIndexByName("Map_Loading"),
		this->GetNodeByName("Idle")->GetAnchorIndexByName("Tick"), 0, // 0 = always on enter
		1,0); // right to left always
#endif
/*
    wxGraphNode * nGraphNode3 = AddMessageNode();
    nGraphNode3->SetFunctionName("Trigger_t");
    nGraphNode3->SetPosition(wxPoint(40,40));
	nGraphNode3->SetCode("");

	{
    wxGraphNode * nGraphNode5 = AddStateNode();
    nGraphNode5->SetFunctionName("DoorClosed");
    nGraphNode5->SetPosition(wxPoint(640,40));
	}

	{
    wxGraphNode * nGraphNode5 = AddStateNode();
    nGraphNode5->SetFunctionName("DoorClosing");
    nGraphNode5->SetPosition(wxPoint(320,40));
	}
	{
    wxGraphNode * nGraphNode5 = AddStateNode();
    nGraphNode5->SetFunctionName("DoorOpened");
    nGraphNode5->SetPosition(wxPoint(640,240));
	}

	{
    wxGraphNode * nGraphNode5 = AddStateNode();
    nGraphNode5->SetFunctionName("DoorOpening");
    nGraphNode5->SetPosition(wxPoint(320,240));
	}
*/

}


wxGraphContainer::~wxGraphContainer()
{
	for (unsigned int i=0 ; i<mNodes.size() ; i++)
		delete mNodes[i];
}

///////////////////////////////////////////////////////////////////////////////////////////////////

bool wxGraphContainer::GetConnected(wxGraphNode *pNodeDst, wxGraphNode **pNodeSrc, unsigned int aPlugDst, unsigned int & aPlugSrc)
{
    unsigned int i;

    for (i=0;i<mConnections.size(); i++)
    {
        if ((mNodes[mConnections[i].NodeDst] == pNodeDst)&&(mConnections[i].NodeDstAnchor == aPlugDst ))
        {
            (*pNodeSrc) = mNodes[mConnections[i].NodeSrc];
            aPlugSrc = mConnections[i].NodeSrcAnchor;
            return true;
        }
    }

    return false;
}

wxString wxGraphContainer::BuildCall(wxGraphNode *pNode)
{
    wxString sCall;

    for (unsigned int i=0;i<pNode->GetNbRightPlugs();i++)
    {
        unsigned int aPlug;
        wxGraphNode * aNode;
        if (GetConnected(pNode, &aNode, i, aPlug))
        {
            if (i>0)
                sCall += wxT(", ");
            sCall += aNode->GetFunctionName();
            sCall += wxT("(");
            sCall +=BuildCall(aNode);
            sCall += wxT(").");
            sCall += aNode->GetLeftPlugName(aPlug);
        }
    }
    return sCall;
}

wxString wxGraphContainer::BuildScript()
{
    /*unsigned int i;

    std::map<wxGraphNode*, unsigned int> nodeCounting;

    // count node use
    for (i = 0;i<mNodes.size();i++)
    {
        nodeCounting[mNodes[i]] = 0;
    }

    for (i = 0 ; i < mConnections.size(); i++)
    {
        nodeCounting[mNodes[mConnections[i].NodeSrc]]++;
    }
    // create temporary variables
    for (i = 0;i<mNodes.size();i++)
    {
        if (nodeCounting[mNodes[i]]>1)
        {
            script+=wxString::Format("%sVar =
        }
    }*/
    // make big function call
    wxString script ;
    if (mFinalNode)
    {
        script += wxT("return ") + mFinalNode->GetFunctionName() + wxT("(");

        script+= BuildCall(mFinalNode);
        script+= wxT(");\n");
    }


    return script;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void wxGraphContainer::OnLButtonDown(wxMouseEvent& event)
{
    event.Skip();

    mbMoving = true;
    mSvgPtr = wxPoint(event.m_x, event.m_y);
    CaptureMouse();

    SetSelectedNode(NULL);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void wxGraphContainer::OnLButtonUp(wxMouseEvent& )
{
    if (mbCreatingNewConnection)
    {
        CancelNewConnection();
        return;
    }

    if (mbMoving)
    {
        ReleaseMouse();
        mbMoving = false;
		Refresh();
    }

}

///////////////////////////////////////////////////////////////////////////////////////////////////

void wxGraphContainer::OnMouseMotion(wxMouseEvent& event)
{
    if (mbMoving)
    {
        wxPoint newPos(event.m_x, event.m_y);
        wxPoint dif = newPos - mSvgPtr;
        for (unsigned int i=0;i<mNodes.size() ; i++)
        {
            mNodes[i]->SetPosition(mNodes[i]->GetPosition() + dif);
        }

        mSvgPtr = newPos;

        Refresh();
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void wxGraphContainer::OnSize(wxSizeEvent &event)
{
    event.Skip();
    Refresh();
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void DrawmSpline(wxDC &dc, wxPoint &pt1, wxPoint &pt2, int aSide1, int aSide2, const wxColour& col )
{
    wxPoint middle = (pt1+pt2);
    middle.x *= 0.5f;
    middle.y *= 0.5f;

    int sd1dif = (aSide1==0)?-30:30;
    int sd2dif = (aSide2==0)?-30:30;

    if (aSide1 == aSide2)
    {
        middle.x += sd1dif;
    }

    dc.SetPen(wxPen(wxColour(1,1,1,1),2));//wxColour(0,0,0,1));
	dc.SetBrush(wxBrush(wxColour(1,1,1,1)));
    dc.DrawSpline(pt1.x, pt1.y, pt1.x + sd1dif, pt1.y, middle.x, middle.y);
    dc.DrawSpline(middle.x, middle.y, pt2.x + sd2dif, pt2.y, pt2.x, pt2.y);
    dc.DrawSpline(pt1.x, pt1.y+1, pt1.x + sd1dif, pt1.y+1, middle.x, middle.y+1);
    dc.DrawSpline(middle.x, middle.y+1, pt2.x + sd2dif, pt2.y+1, pt2.x, pt2.y+1);
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void wxGraphContainer::OnScroll(wxScrollWinEvent& event)
{
    if (event.GetOrientation() == wxHORIZONTAL)
    {
        for (unsigned int i=0;i<mNodes.size() ; i++)
        {
            int Difx = event.GetPosition()-mScrollPos.x;
            mNodes[i]->SetPosition(wxPoint(mNodes[i]->GetPosition().x - Difx ,-1));

        }
    }
    else
    if (event.GetOrientation() == wxVERTICAL)
    {
        for (unsigned int i=0;i<mNodes.size() ; i++)
        {
            int Dify = event.GetPosition()-mScrollPos.y;
            mNodes[i]->SetPosition(wxPoint(-1, mNodes[i]->GetPosition().y - Dify));

        }
    }
    Refresh();
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void wxGraphContainer::GetGraphSize(wxPoint &minPos, wxPoint &maxPos)
{
// scroll bars
    minPos = wxPoint(999999, 999999);
    maxPos = wxPoint(-999999, -999999);

    for (unsigned int i=0;i<mNodes.size() ; i++)
    {
        wxRect rc = mNodes[i]->GetRect();
        if (rc.GetLeft()-10<minPos.x) minPos.x = rc.GetLeft()-10;
        if (rc.GetTop()-10<minPos.y) minPos.y = rc.GetTop()-10;

        if (rc.GetRight()+10>maxPos.x) maxPos.x = rc.GetRight()+10;
        if (rc.GetBottom()+10>maxPos.y) maxPos.y = rc.GetBottom()+10;
    }
    // client view
    wxRect clientRect = GetRect();
    if ( (clientRect.GetWidth()+mCurrentPos.x) > maxPos.x) maxPos.x = (clientRect.GetWidth()+mCurrentPos.x);
    if ( mCurrentPos.x < minPos.x) minPos.x = mCurrentPos.y;
    if ( (clientRect.GetHeight()+mCurrentPos.y) > maxPos.y) maxPos.y = (clientRect.GetHeight()+mCurrentPos.y);
    if ( mCurrentPos.y < minPos.y) minPos.y = mCurrentPos.y;

}

///////////////////////////////////////////////////////////////////////////////////////////////////

void wxGraphContainer::OnPaint(wxPaintEvent& )
{
    wxPaintDC dc(this);
    wxRect rc= GetRect();

    dc.SetBrush(wxBrush(wxColour(77,77,77)));
    dc.DrawRectangle(-1,-1, rc.GetRight()+10, rc.GetBottom()+10);

    // draw connections
	if (!mbMoving)
	{
		for (unsigned int i=0;i<mConnections.size();i++)
		{
			wxPoint pt1 = mNodes[mConnections[i].NodeSrc]->GetPlugPointIndex(mConnections[i].SideSrc, mConnections[i].NodeSrcAnchor);
			wxPoint pt2 = mNodes[mConnections[i].NodeDst]->GetPlugPointIndex(mConnections[i].SideDst, mConnections[i].NodeDstAnchor);

			if ( (mNodes[mConnections[i].NodeSrc]->GetType() == GNT_MESSAGE) ||
				(mNodes[mConnections[i].NodeDst]->GetType() == GNT_MESSAGE) )

				DrawmSpline(dc, pt1, pt2, mConnections[i].SideSrc, mConnections[i].SideDst, wxColour(0,0,1,1));
			else
				DrawmSpline(dc, pt1, pt2, mConnections[i].SideSrc, mConnections[i].SideDst, wxColour(0,0,1,1));
		}

		// current new Connection

		if (mbCreatingNewConnection)
		{
			wxPoint pt1 = mNodes[mNewConnection.NodeSrc]->GetPlugPointIndex(mNewConnection.SideSrc, mNewConnection.NodeSrcAnchor);
			wxPoint pt2 = mNewConnectionPoint;
			DrawmSpline(dc, pt1, pt2, mNewConnection.SideSrc, 1-mNewConnection.SideSrc, wxColour(0,0,0,1));
		}
	}


/*

    // scroll bars

*/
    wxPoint minPos(999999, 999999);
    wxPoint maxPos(-999999, -999999);

    GetGraphSize(minPos, maxPos);

    wxRect clientRect = GetRect();
    mScrollPos = wxPoint(mCurrentPos.x-minPos.x, mCurrentPos.y-minPos.y);
    if (clientRect.GetWidth() >= maxPos.x-minPos.x)
        SetScrollbar(wxHORIZONTAL, mCurrentPos.x-minPos.x, clientRect.GetWidth(), clientRect.GetWidth()+1);
    else
        SetScrollbar(wxHORIZONTAL, mCurrentPos.x-minPos.x, clientRect.GetWidth(), maxPos.x-minPos.x);

    if (clientRect.GetHeight() >= maxPos.y-minPos.y)
        SetScrollbar(wxVERTICAL, mCurrentPos.y-minPos.y, clientRect.GetHeight(), clientRect.GetHeight()+1);
    else
        SetScrollbar(wxVERTICAL, mCurrentPos.y-minPos.y, clientRect.GetHeight(), maxPos.y-minPos.y);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void RefreshSelectedNode(wxGraphNode *pNode);

void wxGraphContainer::SetSelectedNode(wxGraphNode *pNode)
{
    if ((mSelectedNode)&&(pNode != mSelectedNode))
    {
		mSelectedNode->LoseFocus();
        mSelectedNode->SetSelected(false);
        mSelectedNode->Refresh();
    }

    mSelectedNode = pNode;
	RefreshSelectedNode(mSelectedNode);

    if (mSelectedNode)
    {
        mSelectedNode->SetSelected(true);
        mSelectedNode->Refresh();
    }
}

wxGraphNode *wxGraphContainer::GetSelectedNode()
{
	return mSelectedNode;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void wxGraphContainer::BeginNewConnection(wxGraphNode *pNodeSrc, unsigned int aBorder, unsigned int aPlugIndex)
{
    mbCreatingNewConnection = true;
	mNewConnectionSourceNode = pNodeSrc;

    for (unsigned int i=0;i<mNodes.size(); i++)
    {
        if (mNodes[i] == pNodeSrc)
        {
            mNewConnection.NodeSrc = i;
            mNewConnection.SideSrc = aBorder;
            mNewConnection.NodeSrcAnchor = aPlugIndex;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void wxGraphContainer::SetNewConnectionEndPoint(wxPoint aEndPoint)
{
    mNewConnectionPoint = aEndPoint;
    Refresh();
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void wxGraphContainer::EndNewConnection(wxGraphNode *pNodeDst, unsigned int aBorder, unsigned int aPlugIndex)
{

    for (unsigned int i=0;i<mNodes.size(); i++)
    {
        if (mNodes[i] == pNodeDst)
        {
            if (i != mNewConnection.NodeSrc)
            {
                mNewConnection.NodeDst = i;
                mNewConnection.SideDst = aBorder;
                mNewConnection.NodeDstAnchor= aPlugIndex;

                //mConnections.push_back(mNewConnection);
                AddConnection(mNewConnection.NodeSrc, mNewConnection.NodeDst, mNewConnection.NodeSrcAnchor, mNewConnection.NodeDstAnchor, mNewConnection.SideSrc, mNewConnection.SideDst);
            }
        }
    }


    mbCreatingNewConnection = false;
    Refresh();
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void wxGraphContainer::CancelNewConnection()
{
    mbCreatingNewConnection = false;
    Refresh();
}

///////////////////////////////////////////////////////////////////////////////////////////////////

wxGraphNode *wxGraphContainer::GetNode(wxPoint aPoint)
{

    for (unsigned int i=0;i<mNodes.size(); i++)
    {
        wxPoint nodePT = mNodes[i]->GetPosition();
        wxSize nodeSize = mNodes[i]->GetSize();
        if ( (aPoint.x>= nodePT.x) &&
            (aPoint.y>= nodePT.y) &&
            (aPoint.x<= nodePT.x + nodeSize.x) &&
            (aPoint.y<= nodePT.y + nodeSize.y) )
        {
            return mNodes[i];
        }

    }
    return NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void wxGraphContainer::DeleteSelectedNode()
{
	if (mSelectedNode->GetType() == GNT_MESSAGE)
	{
		MessageHasBeenRemoved(mSelectedNode->GetFunctionName());
	}
	std::vector<NodeConnection>::iterator iter = mConnections.begin();

	for (;iter != mConnections.end(); )
	{
        if ( (mNodes[(*iter).NodeSrc] == mSelectedNode) ||
            (mNodes[(*iter).NodeDst] == mSelectedNode) )
		{
			iter = mConnections.erase(iter);
		}
		else
			++iter;
	}

    unsigned int nodeIdx = (unsigned int )-1;
    for (std::vector<wxGraphNode*>::iterator iter = mNodes.begin();iter != mNodes.end();++iter)
    {
        if ((*iter) == mSelectedNode)
        {
            nodeIdx++;
            mNodes.erase(iter);
            break;
        }
        nodeIdx++;
    }
    // remap connections
    if (nodeIdx != (unsigned int )-1)
    {
        for (unsigned i=0;i<mConnections.size(); i++)
        {
            if (mConnections[i].NodeSrc > nodeIdx)
                mConnections[i].NodeSrc--;
            if (mConnections[i].NodeDst > nodeIdx)
                mConnections[i].NodeDst--;

        }
    }

    // free stuff
    delete mSelectedNode;
    mSelectedNode = NULL;

    SetSelectedNode(NULL);
    Refresh();
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void wxGraphContainer::RemoveConnection(wxGraphNode *pNode, unsigned int aBorder, unsigned int aPlugIndex)
{
    for (std::vector<NodeConnection>::iterator iter = mConnections.begin();iter != mConnections.end(); ++iter)
    {
        NodeConnection& con = (*iter);
        if ( ( (mNodes[con.NodeSrc] == pNode) && ( con.SideSrc == aBorder) && (con.NodeSrcAnchor == aPlugIndex)) ||
            ( (mNodes[con.NodeDst] == pNode) && ( con.SideDst == aBorder) && (con.NodeDstAnchor == aPlugIndex)) )
        {
            mConnections.erase(iter);
            Refresh();
            break;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
void wxGraphContainer::DumpConnections()
{
    for (std::vector<NodeConnection>::iterator iter = mConnections.begin();iter != mConnections.end(); ++iter)
    {
        NodeConnection& con = (*iter);
        wxLogDebug("%d,%d,%d,%d,%d,%d\n",con.NodeSrc, con.NodeDst, con.NodeSrcAnchor, con.NodeDstAnchor,
            con.SideSrc, con.SideDst);
    }
}
#endif
///////////////////////////////////////////////////////////////////////////////////////////////////

void wxGraphContainer::AddConnection(unsigned int aNodeSrc, unsigned int aNodeDst,
        unsigned int aNodeSrcAnchor, unsigned int aNodeDstAnchor,
        unsigned int aSideSrc, unsigned int aSideDst)
{
	if (aNodeSrc == aNodeDst)
		return;

    if (aSideSrc<aSideDst)
    {
        mConnections.push_back(NodeConnection(aNodeSrc, aNodeDst, aNodeSrcAnchor, aNodeDstAnchor, aSideSrc, aSideDst));
    }
    else
    {
        mConnections.push_back(NodeConnection(aNodeDst, aNodeSrc, aNodeDstAnchor, aNodeSrcAnchor, aSideDst, aSideSrc));
    }

}

///////////////////////////////////////////////////////////////////////////////////////////////////

void wxGraphContainer::AddNodePattern(const char *szPatternName, const char *szCreationScriptFunction)
{
    mPatterns.push_back(Pattern(szPatternName, szCreationScriptFunction));
}

///////////////////////////////////////////////////////////////////////////////////////////////////

wxGraphNode *wxGraphContainer::AddMessageNode()
{
    wxGraphNode *nGN = new wxGraphNodeMessage(this);
    mNodes.push_back(nGN);
    return nGN;
}
///////////////////////////////////////////////////////////////////////////////////////////////////

wxGraphNode *wxGraphContainer::AddStateNode()
{
    wxGraphNode *nGN = new wxGraphNodeState(this);
    mNodes.push_back(nGN);
    return nGN;
}

////////////////////////////////////////////////////////////////////////////////

TiXmlNode* wxGraphContainer::CreateLegacyXmlNodeWithChildren()
{
	TiXmlElement* currentXmlElement = new TiXmlElement("GraphContainer");

	// Add attributes
	currentXmlElement->SetAttribute("GraphName", mGraphName.ToUTF8());
	currentXmlElement->SetAttribute("StateEnumBase", mStateEnumBase.ToUTF8());
	currentXmlElement->SetAttribute("MessageEnumBase", mMessageEnumBase.ToUTF8());
	currentXmlElement->SetAttribute("InitialState", wxString::Format(wxT("%u"), mInitialState).ToUTF8());
	currentXmlElement->SetAttribute("RaknetMessage", mbRaknetMessage);
	currentXmlElement->SetAttribute("TickHasTime", mbTickHasTime);
	currentXmlElement->SetAttribute("Output", mOutputFileName.ToUTF8());
	currentXmlElement->SetAttribute("includes", mIncludes.ToUTF8());
	currentXmlElement->SetAttribute("membervariables", mMemberVariables.ToUTF8());
	currentXmlElement->SetAttribute("memberinit", mMembersInit.ToUTF8());

	// Add children
	for (std::vector<wxGraphNode*>::iterator it = mNodes.begin(), end = mNodes.end() ; it != end ; ++it)
	{
		currentXmlElement->LinkEndChild((*it)->CreateLegacyXmlNodeWithChildren());
	}

	// Add connections
	for (std::vector<NodeConnection>::iterator it = mConnections.begin(), end = mConnections.end() ; it != end ; ++it)
	{
		TiXmlElement* connectionXmlElement = new TiXmlElement("Connection");

		connectionXmlElement->SetAttribute("nodesrc", wxString::Format(wxT("%u"), it->NodeSrc).ToUTF8());
		connectionXmlElement->SetAttribute("nodedst", wxString::Format(wxT("%u"), it->NodeDst).ToUTF8());
		connectionXmlElement->SetAttribute("nodesrcanchor", wxString::Format(wxT("%u"), it->NodeSrcAnchor).ToUTF8());
		connectionXmlElement->SetAttribute("nodedstanchor", wxString::Format(wxT("%u"), it->NodeDstAnchor).ToUTF8());
		connectionXmlElement->SetAttribute("sidesrc", wxString::Format(wxT("%u"), it->SideSrc).ToUTF8());
		connectionXmlElement->SetAttribute("sidedst", wxString::Format(wxT("%u"), it->SideDst).ToUTF8());

		currentXmlElement->LinkEndChild(connectionXmlElement);
	}

	return currentXmlElement;
}


////////////////////////////////////////////////////////////////////////////////

TiXmlNode* wxGraphContainer::CreateXmlNodeWithChildren()
{
	TiXmlElement* currentXmlElement = new TiXmlElement("finiteStateMachine");

	// Add attributes
	currentXmlElement->SetAttribute("name", mGraphName.ToUTF8());
	currentXmlElement->SetAttribute("stateEnumBase", mStateEnumBase.ToUTF8());
	currentXmlElement->SetAttribute("messageEnumBase", mMessageEnumBase.ToUTF8());
	currentXmlElement->SetAttribute("initialState", wxString::Format(wxT("%u"), mInitialState).ToUTF8());
	currentXmlElement->SetAttribute("TickHasTime", mbTickHasTime);
	currentXmlElement->SetAttribute("hasRaknetMessage", mbRaknetMessage);
	currentXmlElement->SetAttribute("outputFileName", mOutputFileName.ToUTF8());

	// Add includes
	if (!GetIncludes()->IsEmpty())
	{
		TiXmlElement* codeXmlElement = new TiXmlElement("includes");
		TiXmlText* codeXmlText = new TiXmlText(mIncludes.ToUTF8());
		codeXmlText->SetCDATA(true);
		codeXmlElement->LinkEndChild(codeXmlText);

		currentXmlElement->LinkEndChild(codeXmlElement);
	}

	// Add variables
	if (!GetMemberVariables()->IsEmpty())
	{
		TiXmlElement* codeXmlElement = new TiXmlElement("variables");
		TiXmlText* codeXmlText = new TiXmlText(mMemberVariables.ToUTF8());
		codeXmlText->SetCDATA(true);
		codeXmlElement->LinkEndChild(codeXmlText);

		currentXmlElement->LinkEndChild(codeXmlElement);
	}

	// Add initializations
	if (!GetMembersInit()->IsEmpty())
	{
		TiXmlElement* codeXmlElement = new TiXmlElement("initializations");
		TiXmlText* codeXmlText = new TiXmlText(mMembersInit.ToUTF8());
		codeXmlText->SetCDATA(true);
		codeXmlElement->LinkEndChild(codeXmlText);

		currentXmlElement->LinkEndChild(codeXmlElement);
	}

	// Add children
	for (std::vector<wxGraphNode*>::iterator it = mNodes.begin(), end = mNodes.end() ; it != end ; ++it)
	{
		currentXmlElement->LinkEndChild((*it)->CreateXmlNodeWithChildren());
	}

	// Add connections
	for (std::vector<NodeConnection>::iterator it = mConnections.begin(), end = mConnections.end() ; it != end ; ++it)
	{
		TiXmlElement* connectionXmlElement = new TiXmlElement("connection");

		connectionXmlElement->SetAttribute("nodeSrc", wxString::Format(wxT("%u"), it->NodeSrc).ToUTF8());
		connectionXmlElement->SetAttribute("nodeDst", wxString::Format(wxT("%u"), it->NodeDst).ToUTF8());
		connectionXmlElement->SetAttribute("nodeSrcAnchor", wxString::Format(wxT("%u"), it->NodeSrcAnchor).ToUTF8());
		connectionXmlElement->SetAttribute("nodeDstAnchor", wxString::Format(wxT("%u"), it->NodeDstAnchor).ToUTF8());
		connectionXmlElement->SetAttribute("sideSrc", wxString::Format(wxT("%u"), it->SideSrc).ToUTF8());
		connectionXmlElement->SetAttribute("sideDst", wxString::Format(wxT("%u"), it->SideDst).ToUTF8());

		currentXmlElement->LinkEndChild(connectionXmlElement);
	}

	return currentXmlElement;
}

void wxGraphContainer::ParseXmlElement(TiXmlElement *aRootElement)
{
	TiXmlHandle elementHandle(aRootElement);


	// Read attribute ----
	if (aRootElement->Attribute("name"))
		mGraphName = wxString::FromUTF8(aRootElement->Attribute("name"));
	else
		mGraphName = wxT("");

	if (aRootElement->Attribute("stateEnumBase"))
		mStateEnumBase = wxString::FromUTF8(aRootElement->Attribute("stateEnumBase"));
	else
		mStateEnumBase = wxT("");

	if (aRootElement->Attribute("messageEnumBase"))
		mMessageEnumBase = wxString::FromUTF8(aRootElement->Attribute("messageEnumBase"));
	else
		mMessageEnumBase = wxT("");

	if (aRootElement->Attribute("initialState"))
	{
		std::stringstream strStream(std::string(aRootElement->Attribute("initialState")));
		strStream >> mInitialState;
	}
	else
		mInitialState = 0;

	if (aRootElement->Attribute("TickHasTime"))
		mbTickHasTime = !wxString::FromUTF8(aRootElement->Attribute("TickHasTime")).IsSameAs(wxT("0"));
	else
		mbTickHasTime = false;

	if (aRootElement->Attribute("hasRaknetMessage"))
		mbRaknetMessage = !wxString::FromUTF8(aRootElement->Attribute("hasRaknetMessage")).IsSameAs(wxT("0"));
	else
		mbRaknetMessage = false;


	if (aRootElement->Attribute("outputFileName"))
		mOutputFileName = wxString::FromUTF8(aRootElement->Attribute("outputFileName"));
	else
		mOutputFileName = wxT("res.h");

	// Read includes ----
	TiXmlText* includesText = elementHandle.FirstChild("includes").FirstChild().ToText();
	if (includesText)
		mIncludes = wxString::FromUTF8(includesText->Value());
	else
		mIncludes = wxT("");

	// Read variables ----
	TiXmlText* variablesText = elementHandle.FirstChild("variables").FirstChild().ToText();
	if (variablesText)
		mMemberVariables = wxString::FromUTF8(variablesText->Value());
	else
		mMemberVariables = wxT("");

	// Read initializations ----
	TiXmlText* initializationsText = elementHandle.FirstChild("initalizations").FirstChild().ToText();
	if (initializationsText)
		mMembersInit = wxString::FromUTF8(initializationsText->Value());
	else
		mMembersInit = wxT("");


	// Create nodes ----
	TiXmlElement* nodeElement = aRootElement->FirstChildElement("node");
	while(nodeElement)
	{
		wxGraphNode* newNode = NULL;
		const std::string nodeType(nodeElement->Attribute("type"));
		if (nodeType.compare("STATE") == 0)
			newNode = AddStateNode();
		else
			newNode = AddMessageNode();

		newNode->ParseXmlElement(nodeElement);

		nodeElement = nodeElement->NextSiblingElement("node");
	}


	// Create connections ----
	TiXmlElement* connectionElement = aRootElement->FirstChildElement("connection");
	while(connectionElement != NULL)
	{
		int nodeSrc = 0;
		connectionElement->Attribute("nodeSrc", &nodeSrc);
		int nodeDst = 0;
		connectionElement->Attribute("nodeDst", &nodeDst);
		int nodeSrcAnchor = 0;
		connectionElement->Attribute("nodeSrcAnchor", &nodeSrcAnchor);
		int nodeDstAnchor = 0;
		connectionElement->Attribute("nodeDstAnchor", &nodeDstAnchor);
		int sideSrc = 0;
		connectionElement->Attribute("sideSrc", &sideSrc);
		int sideDst = 0;
		connectionElement->Attribute("sideDst", &sideDst);


		NodeConnection connection(nodeSrc, nodeDst, nodeSrcAnchor, nodeDstAnchor, sideSrc, sideDst);
		if (!ConnectionExists(connection))
		{
			mConnections.push_back(connection);

			if  ( ( (mNodes[nodeSrc]->GetType() == GNT_STATE) && (mNodes[nodeDst]->GetType() == GNT_MESSAGE) ) ||
				( (mNodes[nodeDst]->GetType() == GNT_STATE) && (mNodes[nodeSrc]->GetType() == GNT_MESSAGE) ) )
			{
				mNodes[nodeSrc]->OnAddNewConnection(mNodes[nodeDst]);
			}
		}
		connectionElement = connectionElement->NextSiblingElement("connection");
	}
}


///////////////////////////////////////////////////////////////////////////////////////////////////

void wxGraphContainer::ParseLegacyXmlElement(TiXmlElement* aRootElement)
{

	if (aRootElement->Attribute("includes"))
		*GetIncludes() = wxString::FromUTF8(aRootElement->Attribute("includes"));
	else
		*GetIncludes() = wxT("");


	if (aRootElement->Attribute("membervariables"))
		*GetMemberVariables() = wxString::FromUTF8(aRootElement->Attribute("membervariables"));
	else
		*GetMemberVariables() = wxT("");

	if (aRootElement->Attribute("memberinit"))
		*GetMembersInit() = wxString::FromUTF8(aRootElement->Attribute("memberinit"));
	else
		*GetMembersInit() = wxT("");


	if (aRootElement->Attribute("GraphName"))
		mGraphName = wxString::FromUTF8(aRootElement->Attribute("GraphName"));
	else
		mGraphName = wxT("");

	if (aRootElement->Attribute("StateEnumBase"))
		mStateEnumBase = wxString::FromUTF8(aRootElement->Attribute("StateEnumBase"));
	else
		mStateEnumBase = wxT("");

	if (aRootElement->Attribute("MessageEnumBase"))
		mMessageEnumBase = wxString::FromUTF8(aRootElement->Attribute("MessageEnumBase"));
	else
		mMessageEnumBase = wxT("");

	if (aRootElement->Attribute("RaknetMessage"))
		mbRaknetMessage = !wxString::FromUTF8(aRootElement->Attribute("RaknetMessage")).IsSameAs(wxT("0"));
	else
		mbRaknetMessage = false;

	if (aRootElement->Attribute("TickHasTime"))
		mbTickHasTime = !wxString::FromUTF8(aRootElement->Attribute("TickHasTime")).IsSameAs(wxT("0"));
	else
		mbTickHasTime = false;


	if (aRootElement->Attribute("Output"))
		mOutputFileName = wxString::FromUTF8(aRootElement->Attribute("Output"));
	else
		mOutputFileName = _("res.h");

	if (aRootElement->Attribute("InitialState"))
		mInitialState = atoi(aRootElement->Attribute("InitialState"));
	else
		mInitialState = 0;

	unsigned int stateCount = 0;
	TiXmlElement* pExtraElem = aRootElement->FirstChildElement("GraphNode");
	while(pExtraElem)
	{
		wxGraphNode*pNode;
		const char *szType = pExtraElem->Attribute("Type");
		if (!strcmp(szType,"MESSAGE"))
			pNode = AddMessageNode();
		else if (!strcmp(szType,"STATE"))
		{
			pNode = AddStateNode();
			stateCount ++;
		}

		TiXmlElement* pNodeElem = pExtraElem->FirstChildElement("Node");
		if(pNodeElem)
		{
			const wxString szName = wxString::FromUTF8(pNodeElem->Attribute("name"));
			const wxString szComment = wxString::FromUTF8(pNodeElem->Attribute("comment"));
			const wxString szCode = wxString::FromUTF8(pNodeElem->Attribute("code"));

			int posx = atoi(pNodeElem->Attribute("posx"));
			int posy = atoi(pNodeElem->Attribute("posy"));
			int width = atoi(pNodeElem->Attribute("width"));
			int height = atoi(pNodeElem->Attribute("height"));

			if (pNode->GetType() == GNT_STATE)
			{
				pNode->SetSize(posx, posy, width, -1);
			}
			else
			{
				pNode->SetSize(posx, posy, width, height);
			}

			pNode->SetFunctionName(szName);
			pNode->SetCode(szCode);
			pNode->SetComment(szComment);


			// events
			TiXmlElement* pEventElem = pExtraElem->FirstChildElement("Event");
			while(pEventElem)
			{
				const wxString szEventName = wxString::FromUTF8(pEventElem->Attribute("name"));
				const wxString szEventCode = wxString::FromUTF8(pEventElem->Attribute("code"));

				if (szEventName.length()==0)
				{
					*pNode->GetCode() = szEventCode;
				}
				else
				{
					wxString onmachin = wxString(_("On "))+szEventName;
					*pNode->GetCode(onmachin.c_str()) = szEventCode;
				}
				pEventElem = pEventElem->NextSiblingElement();
			}


		}

		pExtraElem = pExtraElem->NextSiblingElement("GraphNode");
	}
	if (mInitialState >= stateCount)
	{
		mInitialState = 0;
	}

	// connections
	pExtraElem = aRootElement->FirstChildElement("Connection");
	while(pExtraElem)
	{

		int nodesrc, nodedst, nodesrcanchor, nodedstanchor, sidesrc, sidedst;
		nodesrc = atoi(pExtraElem->Attribute("nodesrc"));
		nodedst = atoi(pExtraElem->Attribute("nodedst"));
		nodesrcanchor = atoi(pExtraElem->Attribute("nodesrcanchor"));
		nodedstanchor = atoi(pExtraElem->Attribute("nodedstanchor"));
		sidesrc = atoi(pExtraElem->Attribute("sidesrc"));
		sidedst = atoi(pExtraElem->Attribute("sidedst"));


		NodeConnection nc(nodesrc, nodedst, nodesrcanchor, nodedstanchor, sidesrc, sidedst);
		if (!ConnectionExists(nc))
		{
			mConnections.push_back(nc);


			if  ( ( (mNodes[nodesrc]->GetType() == GNT_STATE) && (mNodes[nodedst]->GetType() == GNT_MESSAGE) ) ||
				( (mNodes[nodedst]->GetType() == GNT_STATE) && (mNodes[nodesrc]->GetType() == GNT_MESSAGE) ) )
			{
				mNodes[nodesrc]->OnAddNewConnection(mNodes[nodedst]);
			}
		}
		pExtraElem = pExtraElem->NextSiblingElement("Connection");
	}
}


///////////////////////////////////////////////////////////////////////////////////////////////////

void wxGraphContainer::FillNodeList(std::vector<wxString>& mList, GraphNodeType type)
{
	std::vector<wxGraphNode*>::iterator iter = mNodes.begin();
	for (;iter != mNodes.end(); ++iter)
	{
		if ((*iter)->GetType() == type)
		{
			mList.push_back( (*iter)->GetFunctionName() );
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void wxGraphContainer::FillCommentsFromNodeList(std::vector<wxString>& mList, GraphNodeType type)
{
	std::vector<wxGraphNode*>::iterator iter = mNodes.begin();
	for (;iter != mNodes.end(); ++iter)
	{
		if ((*iter)->GetType() == type)
		{
			mList.push_back( *(*iter)->GetComment() );
		}
	}
}

void wxGraphContainer::FillCodesFromNodeList(std::vector<wxString>& mList, GraphNodeType type, const wxChar *szSubItem)
{
	std::vector<wxGraphNode*>::iterator iter = mNodes.begin();
	for (;iter != mNodes.end(); ++iter)
	{
		if ((*iter)->GetType() == type)
		{
			mList.push_back( *(*iter)->GetCode(szSubItem) );
		}
	}
}


wxGraphNode *wxGraphContainer::GetNodeByName(const wxString szName)
{
	std::vector<wxGraphNode*>::iterator iter = mNodes.begin();
	for (;iter != mNodes.end(); ++iter)
	{
            if(szName.CmpNoCase((*iter)->GetFunctionName()))
//		if (!stricmp((*iter)->GetFunctionName(), szName))
            {
                return (*iter);
            }
	}
	return NULL;
}

int wxGraphContainer::GetNodeIndexByName(const wxChar *szName)
{
	int idx = 0;
	wxString name2 = szName;

	std::vector<wxGraphNode*>::iterator iter = mNodes.begin();
	for (;iter != mNodes.end(); ++iter)
	{
		wxString fnName = (*iter)->GetFunctionName();
		if (fnName == name2)
		{
			return idx;
		}
		idx ++;
	}

	return -1;
}

void wxGraphContainer::RemoveAllOutgoingConnectionFor(wxGraphNode *pNode)
{
    for (std::vector<NodeConnection>::iterator iter = mConnections.begin();iter != mConnections.end();)
    {
        NodeConnection& con = (*iter);
		if ( ( (mNodes[con.NodeSrc] == pNode) && (con.SideSrc == 1)) ||
            ( (mNodes[con.NodeDst] == pNode) && (con.SideDst == 1) ) )
        {
            iter = mConnections.erase(iter);
		}
		else
			++iter;
	}
}

void wxGraphContainer::RemoveAllIncomingConnectionFor(wxGraphNode *pNode)
{
    for (std::vector<NodeConnection>::iterator iter = mConnections.begin();iter != mConnections.end();)
    {
        NodeConnection& con = (*iter);
		if ( ( (mNodes[con.NodeSrc] == pNode) && (con.SideSrc == 0)) ||
            ( (mNodes[con.NodeDst] == pNode) && (con.SideSrc == 0) ) )
        {
            iter = mConnections.erase(iter);
		}
		else
			++iter;
	}
}


int wxGraphContainer::GetNodeIndex(wxGraphNode *pNode)
{
	int idx = 0;
	std::vector<wxGraphNode*>::iterator iter = mNodes.begin();
	for (;iter != mNodes.end(); ++iter)
	{
		if ((*iter) == pNode)
		{
			return idx;
		}
		idx ++;
	}
	return -1;
}

bool wxGraphContainer::ConnectionExists(const NodeConnection &nc)
{
	for (unsigned i = 0;i<mConnections.size(); i++)
	{
		if (mConnections[i] == nc)
			return true;
	}
	return false;
}

void wxGraphContainer::RebuildConnectionsFor(wxGraphNode *pNode)
{
	std::map<wxString, std::vector<wxString> > mNodeCodeConnection;

	RemoveAllOutgoingConnectionFor(pNode);
	pNode->GetAllCodeConnections(mNodeCodeConnection);

	std::map<wxString, std::vector<wxString> >::iterator iter = mNodeCodeConnection.begin();
	for (; iter != mNodeCodeConnection.end(); ++iter)
	{
		/*
		AddConnection(this->GetNodeIndexByName("Idle"),
		this->GetNodeIndexByName("Map_Loading"),
		this->GetNodeByName("Idle")->GetAnchorIndexByName("Tick"), 0, // 0 = always on enter
		1,0); // right to left always
		*/
		//wxString anchor = ((wxGraphNodeState*)mEditingNode)->mCurSelectedButton.Mid(3);
		const wxString& strsrc = (*iter).first;
                for (unsigned int j=0;j<(*iter).second.size();j++)
		{
			const wxString& strdst = (*iter).second[j];
			int NodeSource = this->GetNodeIndex(pNode);
			int NodeDest = this->GetNodeIndexByName(strdst.c_str());
			int AnchorSource = pNode->GetAnchorIndexByName(strsrc);

			if ( (NodeSource != -1) && (NodeDest != -1) && (AnchorSource != -1 ))
			{
				this->AddConnection(NodeSource,
					NodeDest,
					AnchorSource, 0, // 0 = always on enter
					1,0); // right to left always
			}
		}
	}

	Refresh();
}
