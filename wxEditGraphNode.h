#ifndef WXEDITGRAPHNODE_H__
#define WXEDITGRAPHNODE_H__

#include "wxEdit.h"
class wxGraphNode;

class wxZEditNode : public wxZEdit, public wxZEditHandler
{
public:

	wxZEditNode(wxWindow* parent, wxAuiManager* mgr = NULL) :
	  wxZEdit(parent,  this)
	  {
		  this->Connect(-1, 14000, (wxObjectEventFunction)&wxZEditNode::OnRefresh);

		  mEditingNode = NULL;


                  mFileExport = _("testExport");

		  

	  }

	  virtual ~wxZEditNode() {}
	  void OnRefresh(wxEvent& event) {}

	  void HandleValueChanged(unsigned int);

	  void BuildInterface(wxGraphNode *pNode);
	  void BuildInterfaceForCodeGeneration();

          wxString mNodeName;
	  wxGraphNode *mEditingNode;


          wxString mFileExport;
	 

	  


	virtual void HandleButtonClick(unsigned int aButtonID);
	virtual void HandleLostFocus(unsigned int aID);
};

#endif
