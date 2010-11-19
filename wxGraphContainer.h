///////////////////////////////////////////////////////////////////////////////////////////////////
// Zenith Engine
// File Name : wxGraphContainer.h
// Creation : 22/06/2007
// Author : Cedric Guillemet
// Description : 
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef WXGRAPHCONTAINER_H__
#define WXGRAPHCONTAINER_H__

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
#include "wxGraphNode.h"
class TiXmlElement;

///////////////////////////////////////////////////////////////////////////////////////////////////

class wxGraphContainer : public wxNotebookPage //wxPanel //, public wxGraphContainer
{
public:
    wxGraphContainer(wxWindow* parent);
    virtual ~wxGraphContainer() 
    {
		//wxString mString = BuildGraphString();
/*

		FILE *fp = fopen("res.xml","wt");
		if (fp)
		{
			fwrite(mString.c_str(), mString.Len(), 1, fp);
			fflush(fp);
			fclose(fp);
		}
		*/

		// clear
        for (unsigned int i=0;i<mNodes.size();i++)
            delete mNodes[i];
    }

	void Clear()
	{
        for (unsigned int i=0;i<mNodes.size();i++)
            delete mNodes[i];
		mNodes.clear();
		mConnections.clear();
		mSelectedNode = NULL;
		Refresh();

	}

	wxGraphNode *GetSelectedNode() const { return mSelectedNode; }

    wxGraphNode *AddNode()
    {
        wxGraphNode *nGN = new wxGraphNode(this);
        mNodes.push_back(nGN);
        return nGN;
    }

    wxGraphNode *AddMessageNode();
	wxGraphNode *AddStateNode();

    void SetSelectedNode(wxGraphNode *pNode);
	wxGraphNode *GetSelectedNode();

    void BeginNewConnection(wxGraphNode *pNodeSrc, unsigned int aBorder, unsigned int aPlugIndex);
    void SetNewConnectionEndPoint(wxPoint aEndPoint);
    void EndNewConnection(wxGraphNode *pNodeDst, unsigned int aBorder, unsigned int aPlugIndex);
    void CancelNewConnection();

    wxGraphNode *GetNode(wxPoint aPoint);
        wxGraphNode *GetNodeByName(const wxString szName);
    void DeleteSelectedNode();

    void RemoveConnection(wxGraphNode *pNode, unsigned int aBorder, unsigned int aPlugIndex);

    void SetfinalNode(wxGraphNode *pNode)
    {
        mFinalNode = pNode;
    }

        unsigned int mInitialState;

	// for generation

	void FillNodeList(std::vector<wxString>& mList, GraphNodeType type);
	void FillCommentsFromNodeList(std::vector<wxString>& mList, GraphNodeType type);
        void FillCodesFromNodeList(std::vector<wxString>& mList, GraphNodeType type, const wxChar *szSubItem = _(""));

	// --
#ifdef _DEBUG
    virtual void DumpConnections();
#endif


    unsigned int GetPatternCount() { return mPatterns.size(); }
    const char * GetPatternName(unsigned int idx) { return mPatterns[idx].mPatternName; }
    const char * GetPatternScriptFunction(unsigned int idx) { return mPatterns[idx].mCreationScriptFunction; }

	wxString BuildGraphString();
        void ReadString(const wxString pszFileName);

	// global stuff
	wxString *GetIncludes() { return &mIncludes; }
	wxString *GetMemberVariables() { return &mMemberVariables; }
	wxString *GetMembersInit() { return &mMembersInit; }

        int GetNodeIndexByName(const wxChar *szName);
	int GetNodeIndex(wxGraphNode *pNode);

	void RemoveAllOutgoingConnectionFor(wxGraphNode *pNode);
	void RemoveAllIncomingConnectionFor(wxGraphNode *pNode);

    void AddConnection(unsigned int aNodeSrc, unsigned int aNodeDst,
            unsigned int aNodeSrcAnchor, unsigned int aNodeDstAnchor,
            unsigned int aSideSrc, unsigned int aSideDst);

	void RebuildConnectionsFor(wxGraphNode *pNode);
private:
	wxString mIncludes, mMemberVariables, mMembersInit;
    DECLARE_EVENT_TABLE()

    void OnPaint(wxPaintEvent& event);
    void OnEraseBack(wxEraseEvent& ) { }
    void OnScroll(wxScrollWinEvent& event);
    void OnSize(wxSizeEvent &event);
    void OnLButtonDown(wxMouseEvent& event);
    void OnLButtonUp(wxMouseEvent& event);
    void OnMouseMotion(wxMouseEvent& event);
    std::vector<wxGraphNode*> mNodes;
    bool mbMoving;
    wxPoint mSvgPtr;

    void GetGraphSize(wxPoint &minPos, wxPoint &maxPos);

    wxPoint mCurrentPos;
    wxPoint mScrollPos;

    typedef struct NodeConnection
    {
        NodeConnection() {}

        NodeConnection(unsigned int aNodeSrc, unsigned int aNodeDst,
            unsigned int aNodeSrcAnchor, unsigned int aNodeDstAnchor,
            unsigned int aSideSrc, unsigned int aSideDst)
        {
            NodeSrc = aNodeSrc;
            NodeDst = aNodeDst;
            NodeSrcAnchor = aNodeSrcAnchor;
            NodeDstAnchor = aNodeDstAnchor;
            SideSrc = aSideSrc;
            SideDst = aSideDst;
        }
		bool operator == (const NodeConnection & nc)
		{
			if ( (NodeSrc == nc.NodeSrc) &&
				(NodeDst == nc.NodeDst) &&
				(NodeSrcAnchor == nc.NodeSrcAnchor) &&
				(NodeDstAnchor == nc.NodeDstAnchor) &&
				(SideSrc == nc.SideSrc) &&
				(SideDst == nc.SideDst) )
				return true;
			return false;
		}


        unsigned int NodeSrc, NodeDst;
        unsigned int NodeSrcAnchor, NodeDstAnchor; // 0 left 1 right
        unsigned int SideSrc, SideDst;
    } NodeConnection;

    std::vector<NodeConnection> mConnections;
    wxGraphNode *mSelectedNode;

	bool ConnectionExists(const NodeConnection &nc);

    bool mbCreatingNewConnection;
    NodeConnection mNewConnection;
    wxPoint mNewConnectionPoint;

    wxGraphNode *mFinalNode;


    wxString BuildScript();
    bool GetConnected(wxGraphNode *pNodeDst, wxGraphNode **pNodeSrc, unsigned int aPlugDst, unsigned int & aPlugSrc);
    wxString BuildCall(wxGraphNode *pNode);




    typedef struct Pattern
    {
        Pattern(const char *aPatternName, const char *aCreationScriptFunction)
        {
            mPatternName = aPatternName;
            mCreationScriptFunction = aCreationScriptFunction;
        }

        const char *mPatternName;
        const char *mCreationScriptFunction;

    } Pattern;
    std::vector<Pattern> mPatterns;
    void AddNodePattern(const char *szPatternName, const char *szCreationScriptFunction);

public:
	wxGraphNode *mNewConnectionSourceNode;
	void ParseGraphString(TiXmlElement* pRootElem);//const wxString &aStr);

/*
	const wxString& GetGraphName() { return mGraphName; }
	void SetGraphName(const wxString& gname) { mGraphName = gname; }
*/
	wxString mGraphName;
        wxString mOutputFileName;

        wxString mStateEnumBase, mMessageEnumBase;
	bool mbRaknetMessage, mbTickHasTime;

        virtual void ChangeMessageName(const wxChar *szPrevious, const wxChar *szNew)
	{
                for (unsigned int i=0;i<mNodes.size();i++)
		{
			mNodes[i]->ChangeMessageName(szPrevious, szNew);
		}
	}
        virtual void MessageHasBeenRemoved(const wxChar *szMessageName)
	{
                for (unsigned int i=0;i<mNodes.size();i++)
		{
			mNodes[i]->MessageHasBeenRemoved(szMessageName);
		}
	}
};

#endif
