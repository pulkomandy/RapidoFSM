/*
OK pouvoir creer 1 nouveau tab
OK pouvoir renomer 1 tab
OK pouvoir fermer 1 tab (supprimer)
? pouvoir juste sauver 1 tab ?
OK parcourir tous les tabs (pour SVG)
OK recuperer le tab courant (et update mscrollv)
OK charger un projet
? merger un tab
OK members, membersinit dans le graphedit et pas dans l'edit de message/state
- ne pas pouvoir avoir de lien d'une sortie de state vers 1 entree du meme state
ÒK ne pas dessiner les liens pendant 1 déplacement de node ou de scroll
- ne pas ralentir tant que ca quand on tape du texte (trop de redraw)
- on peux mettre 1 message dans le 1er leftanchor d'un etat
- 2 couleurs pour les lignes: 1 pour message->state, 1 pour state->state
OK on ne peut pas supprimer la liaison entre 2 etats en enlevant le code 'setstate' ni en cliquant sur la croix

OK quand on supprime 1 message, ca ne supprime pas le bouton 'on machin'
VIRE l'edition d'un nom de state (et de message?) rajoute des 0 
OK link message vers state, puis renomage du message-> on update pas le bouton
OK Renommer un état/message après avoir fait les liens fout la merde

OK On ne peut pas supprimer un lien ‘message vers state’. Il faut supprimer le message et le recréer -> ajout d'un bouton X dans l'editeur de script. On selection le "on message" qu'on veut degager et on click sur X.
OK On peut ajouter plusieurs fois le même message à un état
OK Le nom de fichier pour le code généré est toujours ‘res.h’
VIRE On peut avoir plusieurs états/messages avec le même nom -> a l'utilisateur de se demerder
OK Pas de sauvegarde/restauration du layout de fenêtres
OK La perte de focus foire un peu (le curseur peut apparaitre dans plusieurs edition de texte)
OK La touche Suppr dans l’editeur de texte (code, commentaire,…) supprime le noeud selectionné
OK Pas de coloration C/C++ et zone d’edition trop petites
OK Les paramètres du paneau de génération ne sont pas sauvés dans l’xml
OK Pas de demande de confirmation quand on quitte avec un fichier modifié (ou quand on en ouvre un autre)
OK Ajouter un bouton dans la toolbar pour générer le code (ne plus passer par le panneau de génération de code)
OK A l’écriture/chargement du XML du modèle, remplacer les < > ” et autres trucs qui foutent le Bronx
OK Ca crash quand on quitte
OK Affichage du nom de fichier courant dans la barre système + ajouter une * quand il y a modification

*/




/////////////////////////////////////////////////////////////////////////////
// Name:        minimal.cpp
// Purpose:     Minimal wxWidgets sample
// Author:      Julian Smart
// Modified by:
// Created:     04/01/98
// RCS-ID:      $Id: minimal.cpp,v 1.1.1.1 2007-07-01 19:25:05 u39417655 Exp $
// Copyright:   (c) Julian Smart
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#include <iostream>
#include <wx/ffile.h>

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWidgets headers)
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

#include "wxGraphContainer.h"

unsigned short ClassIDZFx;
unsigned short ClassIDZTexture;
unsigned short ClassIDZMeshInstance;
// ----------------------------------------------------------------------------
// private classes
// ----------------------------------------------------------------------------
#include <wx/aui/aui.h>
#include <wx/aui/dockart.h>

#include "wxEditGraphNode.h"
#include "wxScriptEditPanel.h"
#include "tinyxml.h"

// Define a new application type, each program should derive a class from wxApp
class MyApp : public wxApp
{
public:
	// override base class virtuals
	// ----------------------------

	// this one is called on application startup and is a good place for the app
	// initialization (doing it here and not in the ctor allows to have an error
	// return: if OnInit() returns false, the application terminates)
	virtual bool OnInit();
};
class wxZEditNode;
// Define a new frame type: this is going to be our main frame
wxGraphContainer *mScrollV;
wxNotebook * myNotebook;
wxScriptEditPanel *mEditScriptPanel;
wxZEditNode * mEditNode;
void SetScriptToEdit(const wxChar *szName, wxString* pCode)
{
	mEditScriptPanel->SetScriptToEdit(szName, pCode);
}
class MyFrame : public wxFrame
{
public:
	// ctor(s)
	MyFrame(const wxString& title);
	virtual ~MyFrame()
	{
		wxConfigBase *pConfig = wxConfigBase::Get();
		wxString myPerspect = m_mgr.SavePerspective();
		pConfig->Write(_T("/Rapido/AUIPerspective"), myPerspect);

		wxString myWinState;
                myWinState = wxString::Format(_("%d,%d,%d,%d"),GetSize().x, GetSize().y, GetPosition().x, GetPosition().y);
		pConfig->Write(_T("/Rapido/MainWinState"), myWinState);

		exit(0);
		//delete mScrollV;
	}
	// event handlers (these functions should _not_ be virtual)
	void OnQuit(wxCommandEvent& event);
	void OnAbout(wxCommandEvent& event);
	void OnDelSelected(wxCommandEvent& event);
	void OnNewNodeSelected(wxCommandEvent& event);

	void OnNewNodeMessage(wxCommandEvent& event);
	void OnNewNodeState(wxCommandEvent& event);
	void OnGenerate(wxCommandEvent& event);

	void OnFileSaveProject(wxCommandEvent& event);
	void OnFileOpenProject(wxCommandEvent& event);
	void OnFileOpenFSM(wxCommandEvent& event);
	void OnFileSaveProjectAs(wxCommandEvent& event);
	void OnFileNewTab(wxCommandEvent& event);
	void OnFileNewProject(wxCommandEvent& event);
	void OnNotebookChange(wxNotebookEvent& event);
	void OnCloseTab(wxCommandEvent& event);
	void OnFileCloseProject(wxCommandEvent& event);
	void OnFileSaveFSMAs(wxCommandEvent& event);
	void OnCompileAll(wxCommandEvent& event);

private:
	// any class wishing to process wxWidgets events must use this macro
	DECLARE_EVENT_TABLE()
	
	wxAuiManager m_mgr;
public:
	
	
	wxString mFileName;
	bool CloseProject();
	void DoClearProject();
	void UpdateTitle();
	void DoAddNewTab();
	bool mbModified;
};



// ----------------------------------------------------------------------------
// constants
// ----------------------------------------------------------------------------

// IDs for the controls and the menu commands
enum
{
	// menu items
	Minimal_Quit = wxID_EXIT,

	// it is important for the id corresponding to the "About" command to have
	// this standard value as otherwise it won't be handled properly under Mac
	// (where it is special and put into the "Apple" menu)
	Minimal_About = wxID_ABOUT,
	Minimal_Delete =10008 ,
};

enum
{
	wxNode_NewMessage = 12345,
	wxNode_NewState,
	wxNode_Generate,
	wxNode_CompileAll,
	Minimal_SaveProject,
	Minimal_OpenProject,
	Minimal_SaveProjectAs,
	Minimal_NewTab,
	Minimal_NewProject,
	Minimal_CloseTab,
	Minimal_OpenFSM,
	Minimal_CloseProject,
	Minimal_SaveFSMAs

};

// ----------------------------------------------------------------------------
// event tables and other macros for wxWidgets
// ----------------------------------------------------------------------------
#define IDC_NOTEBOOK 23445
// the event tables connect the wxWidgets events with the functions (event
// handlers) which process them. It can be also done at run-time, but for the
// simple menu events like this the static method is much simpler.
BEGIN_EVENT_TABLE(MyFrame, wxFrame)
EVT_MENU(Minimal_Quit,  MyFrame::OnQuit)
EVT_MENU(Minimal_About, MyFrame::OnAbout)
EVT_MENU(Minimal_Delete, MyFrame::OnDelSelected)
EVT_MENU_RANGE(wxNode_NewMessage,wxNode_NewMessage, MyFrame::OnNewNodeMessage)
EVT_MENU_RANGE(wxNode_NewState,wxNode_NewState, MyFrame::OnNewNodeState)
EVT_MENU_RANGE(wxNode_Generate,wxNode_Generate, MyFrame::OnGenerate)
EVT_MENU(wxNode_CompileAll, MyFrame::OnCompileAll)

EVT_MENU(Minimal_SaveProject, MyFrame::OnFileSaveProject)
EVT_MENU(Minimal_OpenProject, MyFrame::OnFileOpenProject)
EVT_MENU(Minimal_SaveProjectAs, MyFrame::OnFileSaveProjectAs)
EVT_MENU(Minimal_NewTab, MyFrame::OnFileNewTab)
EVT_MENU(Minimal_NewProject, MyFrame::OnFileNewProject)
EVT_NOTEBOOK_PAGE_CHANGED(IDC_NOTEBOOK, MyFrame::OnNotebookChange)
EVT_MENU(Minimal_CloseTab, MyFrame::OnCloseTab)
EVT_MENU(Minimal_OpenFSM, MyFrame::OnFileOpenFSM)
EVT_MENU(Minimal_SaveFSMAs, MyFrame::OnFileSaveFSMAs)
END_EVENT_TABLE()

// Create a new application object: this macro will allow wxWidgets to create
// the application object during program execution (it's better than using a
// static object for many reasons) and also implements the accessor function
// wxGetApp() which will return the reference of the right type (i.e. MyApp and
// not wxApp)
IMPLEMENT_APP(MyApp)

// ============================================================================
// implementation
// ============================================================================

// ----------------------------------------------------------------------------
// the application class
// ----------------------------------------------------------------------------
MyFrame *GMYframe;
const wxChar *RapidoTitleBar = _("Rapido! v0.1.9");
// 'Main program' equivalent: the program execution "starts" here
bool MyApp::OnInit()
{
	// call the base class initialization method, currently it only parses a
	// few common command-line options but it could be do more in the future
	if ( !wxApp::OnInit() )
		return false;

	// create the main application window
	MyFrame *frame = new MyFrame(RapidoTitleBar);
	

	// and show it (the frames, unlike simple controls, are not shown when
	// created initially)
	frame->Show(true);




	return true;
}


// ----------------------------------------------------------------------------
// main frame
// ----------------------------------------------------------------------------

// frame constructor
MyFrame::MyFrame(const wxString& title)
: wxFrame(NULL, wxID_ANY, title)
{
	GMYframe = this;
	mbModified = false;
	// set the frame icon
	SetIcon(wxICON(sample));

	m_mgr.SetManagedWindow(this);

#if wxUSE_MENUS
	// create a menu bar
	wxMenu *fileMenu = new wxMenu;
	wxMenu *nodeMenu = new wxMenu;
	// the "About" item should be in the help menu
	wxMenu *helpMenu = new wxMenu;
	helpMenu->Append(Minimal_About, _T("&About...\tF1"), _T("Show about dialog"));
	

	fileMenu->Append(Minimal_NewProject, _T("New Project"), _T("New FSM collection"));
	fileMenu->Append(Minimal_NewTab, _T("New FSM\tCTRL+N"), _T("New FSM"));
	fileMenu->AppendSeparator();
	fileMenu->Append(Minimal_OpenProject, _T("Open\tCTRL+O"), _T("Open a Project"));
	fileMenu->Append(Minimal_OpenFSM, _T("Merge FSM"), _T("Open a Project"));
	fileMenu->AppendSeparator();
	fileMenu->Append(Minimal_CloseTab, _T("Close FSM"), _T("Close FSM"));
	fileMenu->Append(Minimal_CloseProject, _T("Close Project"), _T("Close Project"));

	fileMenu->AppendSeparator();
	fileMenu->Append(Minimal_SaveProject, _T("Save Project\tCTRL+S"), _T("Save graph file"));
	fileMenu->Append(Minimal_SaveProjectAs, _T("Save Project As ..."), _T("Save graph file as"));
	fileMenu->Append(Minimal_SaveFSMAs, _T("Save FSM As ..."), _T("Save graph file as"));

	nodeMenu->AppendSeparator();
	fileMenu->Append(Minimal_Quit, _T("E&xit\tAlt-F4"), _T("Quit this program"));

	// now append the freshly created menu to the menu bar...
	wxMenuBar *menuBar = new wxMenuBar();
	menuBar->Append(fileMenu, _T("&File"));
	menuBar->Append(nodeMenu, _T("Nodes"));
	menuBar->Append(helpMenu, _T("&Help"));




	// ... and attach this menu bar to the frame
	SetMenuBar(menuBar);
#endif // wxUSE_MENUS

#if wxUSE_STATUSBAR
	// create a status bar just for fun (by default with 1 pane only)
	CreateStatusBar(2);
	SetStatusText(_T("Welcome to Rapido!"));
#endif // wxUSE_STATUSBAR

	SetSize(wxSize(1024,768));

	// --------------------------------------------------------------------------------



	
	myNotebook = new wxNotebook( this, IDC_NOTEBOOK, wxDefaultPosition, wxSize(500, 500), 0 );
	
	/*
	LUA()->Init();
	LUA()->DoFile("script1.lua");
	//LUA()->CallFunction("DeclareNodes");
	//delete LUA();

	for (unsigned int i = 0;i<mScrollV->GetPatternCount();i++)
	{
	nodeMenu->Append(12345+i, wxString(mScrollV->GetPatternName(i)), _T(""));
	}
	*/

        nodeMenu->Append(wxNode_NewMessage, wxString(_("New Message")), _T(""));
        nodeMenu->Append(wxNode_NewState, wxString(_("New State")), _T(""));
	nodeMenu->AppendSeparator();
	nodeMenu->Append(Minimal_Delete, _T("Delete Selected node"));
	/*
	m_mgr.AddPane(mScrollV, wxAuiPaneInfo().
		Name(wxT("Nodes")).Caption(wxT("Nodes")).
		CenterPane());
		*/
	m_mgr.AddPane(myNotebook, wxAuiPaneInfo().
		Name(wxT("Nodes")).Caption(wxT("Nodes")).
		CenterPane());

	

	mEditNode = new wxZEditNode(this, &m_mgr);
	m_mgr.AddPane(mEditNode, wxAuiPaneInfo().Name(wxT("Edit Node")).Caption(wxT("Edit Node")).Right());

	mEditScriptPanel = new wxScriptEditPanel(this);//, &m_mgr);
	m_mgr.AddPane(mEditScriptPanel, wxAuiPaneInfo().Name(wxT("Edit Script")).Caption(wxT("Edit Script")).Bottom());
	// toolbar

	wxToolBar* mToolBarEdit = new wxToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTB_FLAT | wxTB_NODIVIDER);
        mToolBarEdit->SetToolBitmapSize(wxSize(22,22));

        wxBitmap tb_newMessage(_("TBNewMessage.bmp"), wxBITMAP_TYPE_BMP);
        wxBitmap tb_newState(_("TBNewState.bmp"), wxBITMAP_TYPE_BMP);
        wxBitmap tb_generate(_("TBGenerate.bmp"), wxBITMAP_TYPE_BMP);
        wxBitmap tb_compileAll(_("TBCompileAll.bmp"), wxBITMAP_TYPE_BMP);

    mToolBarEdit->AddTool(wxNode_NewMessage, wxT("New Message"), tb_newMessage);
	mToolBarEdit->AddTool(wxNode_NewState, wxT("New State"), tb_newState);
	mToolBarEdit->AddSeparator();
	//mToolBarEdit->AddTool(wxNode_Generate, wxT("Generate"), tb_generate);
	mToolBarEdit->AddTool(wxNode_CompileAll, wxT("Compile"), tb_compileAll);

    mToolBarEdit->Realize();

    m_mgr.AddPane(mToolBarEdit, wxAuiPaneInfo().
        Name(wxT("tb_edit")).Caption(wxT("Edit Bar")).
        ToolbarPane().Top().Row(1).
        LeftDockable(false).RightDockable(false));



	m_mgr.Update();

            wxConfigBase *pConfig = wxConfigBase::Get();
            wxString appLayout = pConfig->Read(_T("/Rapido/AUIPerspective"), _(""));
            if (!appLayout.IsEmpty())
            {
                m_mgr.LoadPerspective(appLayout);
            }
            wxString appWinState = pConfig->Read(_T("/Rapido/MainWinState"), _(""));
            if (!appWinState.IsEmpty())
            {
                wxSize appSize;
                wxPoint appPos;
                sscanf(appWinState.mb_str(), "%d,%d,%d,%d",&appSize.x, &appSize.y, &appPos.x, &appPos.y);


                SetSize(appSize);
                SetPosition(appPos);
            }

			DoAddNewTab();
}


// event handlers

void MyFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
	// true is to force the frame to close
	Close(true);
}

void MyFrame::OnDelSelected(wxCommandEvent& WXUNUSED(event))
{
	mScrollV->DeleteSelectedNode();
}

void MyFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
	/*
	wxMessageBox(wxString::Format(
	_T("Welcome to %s!\n")
	_T("\n")
	_T("This is the minimal wxWidgets sample\n")
	_T("running under %s."),
	wxVERSION_STRING,
	wxGetOsDescription().c_str()
	),
	_T("About wxWidgets minimal sample"),
	wxOK | wxICON_INFORMATION,
	this);
	*/
#ifdef _DEBUG
	mScrollV->DumpConnections();
#endif
}

void MyFrame::OnNewNodeSelected(wxCommandEvent& event)
{
	//int idx = event.GetId() - 12345;
	//LUA()->CallFunction(mScrollV->GetPatternScriptFunction(idx));
}

void RefreshSelectedNode(wxGraphNode *pNode)
{
	mEditNode->BuildInterface(pNode);
}

void MyFrame::OnNewNodeMessage(wxCommandEvent& event)
{
	wxGraphNode * nGraphNode8 = mScrollV->AddMessageNode();
        nGraphNode8->SetFunctionName(_("NewMessage"));
	nGraphNode8->SetPosition(wxPoint(40, 40));
}

void MyFrame::OnNewNodeState(wxCommandEvent& event)
{
	wxGraphNode * nGraphNode4b = mScrollV->AddStateNode();
        nGraphNode4b->SetFunctionName(_("NewState"));
	nGraphNode4b->SetPosition(wxPoint( 40, 40));
}

void MyFrame::OnGenerate(wxCommandEvent& event)
{
	mScrollV->SetSelectedNode(NULL);
	mEditNode->BuildInterfaceForCodeGeneration();
}
/*
void MyFrame::OnFileSaveFSM(wxCommandEvent& event)
{
	wxString fileName;
	if (!mScrollV->GetGraphName().empty())
	{
		fileName = mScrollV->GetGraphName();
	}
	else
	{
		OnFileSaveFSMAs(event);
	}


	if (!fileName.empty())
	{
		wxString mString = mScrollV->BuildGraphString();

		FILE *fp = fopen(fileName.c_str(),"wt");
		if (fp)
		{
			fwrite(mString.c_str(), mString.Len(), 1, fp);
			fflush(fp);
			fclose(fp);
		}
	}
}
*/
void MyFrame::OnFileOpenFSM(wxCommandEvent& event)
{
	bool mbForOpening = true;
        wxFileDialog fDialog(GetParent(), _("Choose a file"), _(""), _(""), _("*.xml"),
		mbForOpening?(wxFD_OPEN|wxFD_FILE_MUST_EXIST):(wxFD_SAVE|wxFD_OVERWRITE_PROMPT));

	if (fDialog.ShowModal() == wxID_OK)
	{
		mScrollV = new wxGraphContainer(myNotebook);

		myNotebook->AddPage(mScrollV, fDialog.GetFilename().c_str() );
		myNotebook->SetSelection(myNotebook->GetPageCount()-1);
		myNotebook->Layout();

		mScrollV->Clear();
		mScrollV->ReadString(fDialog.GetFilename().c_str());
		//mScrollV->SetGraphName(fDialog.GetFilename());
	}


	
}

wxString LoadStringFromFile(const wxChar *pszFileName)
{
	wxString res;
        wxFFile fp(pszFileName,"rt");
        if (fp.IsOpened())
	{
            fp.ReadAll(&res);
        } else {
            wxMessageDialog m(NULL, _("Unable to open file!"));
            m.ShowModal();
        }
	return res;
}


void MyFrame::OnFileOpenProject(wxCommandEvent& event)
{
    if (!CloseProject()) {
        return;
    }

    
	bool mbForOpening = true;
        wxFileDialog fDialog(GetParent(), _("Choose a file"), _(""), _(""), _("*.xml"),
            mbForOpening?(wxFD_OPEN|wxFD_FILE_MUST_EXIST):(wxFD_SAVE|wxFD_OVERWRITE_PROMPT));

	if (fDialog.ShowModal() == wxID_OK)
	{
		DoClearProject();
		mFileName = fDialog.GetPath();
        wxString res = LoadStringFromFile(mFileName);
		
		TiXmlDocument pXmlDoc;
                if (!pXmlDoc.Parse(res.mb_str()))
		{
                    wxMessageDialog m(this, res );
                    m.ShowModal();
                    return ;
		}
			
                TiXmlElement* pRootElem = pXmlDoc.RootElement();
                if (pRootElem == NULL) std::cout << "Not an XML file ?" << std::endl;
		while(pRootElem)
		{
                    std::cout << "adding FSMs..." << std::endl;
			DoAddNewTab();
			mScrollV->ParseGraphString(pRootElem);
			myNotebook->SetPageText(myNotebook->GetSelection(), mScrollV->mGraphName);
			pRootElem = pRootElem->NextSiblingElement("GraphContainer");
		}

		RefreshSelectedNode(NULL);
		UpdateTitle();

		/*
		mScrollV = new wxGraphContainer(myNotebook);

		myNotebook->AddPage(mScrollV, fDialog.GetFilename().c_str() );
		myNotebook->SetSelection(myNotebook->GetPageCount()-1);
		myNotebook->Layout();

		mScrollV->Clear();
		mScrollV->ReadString(fDialog.GetFilename().c_str());
		mScrollV->SetGraphName(fDialog.GetFilename());
		*/
	}


	
}

void MyFrame::OnFileSaveProject(wxCommandEvent& event)
{
	if (mFileName.empty())
	{
		OnFileSaveProjectAs(event);
	}
	else
	{
		wxString mString;
		for (unsigned int i=0;i<myNotebook->GetPageCount();i++)
		{
			wxGraphContainer *pCont = (wxGraphContainer *)myNotebook->GetPage(i);
			/*
			mString += "<";
			mString += pCont->mGraphName;
			mString += ">\n";

*/
			mString += pCont->BuildGraphString();

                        mString += _("\n");
			/*
			mString += pCont->mGraphName;
			mString += ">\n";
			*/

		}
                FILE *fp = fopen(mFileName.mb_str(),"wt");
		if (fp)
		{
			fwrite(mString.c_str(), mString.Len(), 1, fp);
			fflush(fp);
			fclose(fp);
		}
		mbModified = false;
		UpdateTitle();

		/*
		bool mbForOpening = false;
		wxFileDialog fDialog(GetParent(), "Choose a file", "", "", "*.xml",
			mbForOpening?(wxFD_OPEN|wxFD_FILE_MUST_EXIST):(wxFD_SAVE|wxFD_OVERWRITE_PROMPT));

		if (fDialog.ShowModal() == wxID_OK)
		{
			mFileName = fDialog.GetFilename();
			//mScrollV->SetGraphName(fDialog.GetFilename());
			OnFileSaveProject(event);
		}
		*/
	}
}

void MyFrame::OnFileSaveProjectAs(wxCommandEvent& event)
{
	bool mbForOpening = false;
        wxFileDialog fDialog(GetParent(), _("Choose a file"), _(""), _(""), _("*.xml"),
		mbForOpening?(wxFD_OPEN|wxFD_FILE_MUST_EXIST):(wxFD_SAVE|wxFD_OVERWRITE_PROMPT));

	if (fDialog.ShowModal() == wxID_OK)
	{
		//mScrollV->SetGraphName(fDialog.GetFilename());
		mFileName = fDialog.GetPath();
		OnFileSaveProject(event);
	}
}

void MyFrame::DoAddNewTab()
{
	mScrollV = new wxGraphContainer(myNotebook);

	myNotebook->AddPage(mScrollV, L"Unnamed" );
	myNotebook->SetSelection(myNotebook->GetPageCount()-1);
	myNotebook->Layout();
	RefreshSelectedNode(NULL);
}

void MyFrame::OnFileNewTab(wxCommandEvent& event)
{
	DoAddNewTab();

	mbModified = true;
	UpdateTitle();

}

void MyFrame::OnFileNewProject(wxCommandEvent& event)
{
	OnFileCloseProject(event);
	
}

void MyFrame::OnNotebookChange(wxNotebookEvent& event)
{
	mScrollV = (wxGraphContainer*)myNotebook->GetCurrentPage();
	RefreshSelectedNode(NULL);
}

void MyFrame::OnCloseTab(wxCommandEvent& event)
{
        if (wxMessageBox(_("Are you sure?\nModifications will be lost"), _(""), wxYES_NO) == wxYES)
		myNotebook->DeletePage(myNotebook->GetSelection());
}

void MyFrame::OnFileCloseProject(wxCommandEvent& event)
{
	if (CloseProject())
	{
		DoClearProject();
		DoAddNewTab();
		UpdateTitle();
	}
}

void MyFrame::OnFileSaveFSMAs(wxCommandEvent& event)
{
	bool mbForOpening = false;
        wxFileDialog fDialog(GetParent(), _("Choose a file"), _(""), _(""), _("*.xml"),
		mbForOpening?(wxFD_OPEN|wxFD_FILE_MUST_EXIST):(wxFD_SAVE|wxFD_OVERWRITE_PROMPT));

	if (fDialog.ShowModal() == wxID_OK)
	{
		wxString mString = mScrollV->BuildGraphString();

                FILE *fp = fopen(mFileName.mb_str(),"wt");
		if (fp)
		{
			fwrite(mString.c_str(), mString.Len(), 1, fp);
			fflush(fp);
			fclose(fp);
		}
	}
}

bool MyFrame::CloseProject()
{
	if (mbModified)
	{
                if (wxMessageBox(_("Are you sure?\nModifications will be lost"), _(""), wxYES_NO) != wxYES)
			return false;
	}
	DoAddNewTab();
	UpdateTitle();
	return true;
}

void MyFrame::UpdateTitle()
{
	wxString title = RapidoTitleBar;
        title += _(" - ");
	if (mFileName.empty())
                title += _("Unnamed Project");
	else
		title += mFileName.c_str();
	if (mbModified)
                title += _(" * ");
	this->SetTitle(title);

}

void MyFrame::DoClearProject()
{
	mbModified = false;
	myNotebook->DeleteAllPages();
        mFileName = _("");

}

wxString GenerateCPP();

void MyFrame::OnCompileAll(wxCommandEvent& event)
{
	OnFileSaveProject(event);

	wxString res;
	wxGraphContainer *pCursv = mScrollV;
	for (unsigned int i=0;i<myNotebook->GetPageCount();i++)
	{
		wxGraphContainer *pCont = (wxGraphContainer *)myNotebook->GetPage(i);
		mScrollV = pCont;
		res += GenerateCPP();
	}
	mScrollV = pCursv;


        wxMessageBox(res, _("Code Generation"));
}
