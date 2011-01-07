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
// File Name : wxEdit.h
// Creation : 08/11/2006
// Author : Cedric Guillemet
// Description :
//
////////////////////////////////////////////////////////////////////////////////

#ifndef WXZEDIT_H__
#define WXZEDIT_H__

#include <list>

#include <wx/laywin.h>
#include <wx/foldbar/foldpanelbar.h>
#include "wxStackedControls.h"

#define ZEEditClass(x) EditClass(x->mInfosSerializableFields, x->mInfosSerializableName, x)

///////////////////////////////////////////////////////////////////////////////////////////////////

class wxZEdit : public wxSashLayoutWindow
{
public:

    wxZEdit(wxWindow* parent, wxZEditHandler *aHandler = NULL);

    ~wxZEdit();

    // --
    void AddSeparator() { m_pnl->AddFoldPanelSeperator(*mCurItem); }

    // static lib
    void AddStaticLib(const wxString &Lib)
    {
        m_pnl->AddFoldPanelWindow(*mCurItem, new wxStaticText(mCurItem->GetParent(), wxID_ANY, Lib), wxFPB_ALIGN_WIDTH, 2, 5);
    }
    // num edit

    template<typename type> void AddNumEdit(const wxString &Lib, type aMin, type aMax, type *pValue, type increment = 0.01f, unsigned int aID = 2)
    {
        wxZenNumEdit<type> * aItem = new wxZenNumEdit<type>(mCurItem->GetParent(), mHandler, Lib, aMin, aMax, pValue, increment, aID);
        mStackControls.push_back(aItem);
        m_pnl->AddFoldPanelWindow(*mCurItem, aItem, wxFPB_ALIGN_WIDTH, 0, 5);
    }


	// static

    void AddStatic(const wxString &Lib)
    {
        wxZenStatic *aItem = new wxZenStatic(mCurItem->GetParent(), Lib);
        mStackControls.push_back(aItem);
        m_pnl->AddFoldPanelWindow(*mCurItem, aItem, wxFPB_ALIGN_WIDTH, 0, 5);
    }

    // button
    void AddButton(const wxString &Lib, unsigned int ID)
    {
        wxZenButton *aItem = new wxZenButton(mCurItem->GetParent(), mHandler, ID, Lib);
        mStackControls.push_back(aItem);
        m_pnl->AddFoldPanelWindow(*mCurItem, aItem, wxFPB_ALIGN_WIDTH, 0, 5);
    }

	void AddScriptEdit(wxString *pToEdit, unsigned int ID)
	{
        wxZenScriptEdit *aItem = new wxZenScriptEdit(mCurItem->GetParent(), mHandler, pToEdit, ID);
        mStackControls.push_back(aItem);
        m_pnl->AddFoldPanelWindow(*mCurItem, aItem, wxFPB_ALIGN_WIDTH, 0, 5);
	}

    // toggle button
    void AddToggleButton(const wxString &Lib, unsigned int ID)
    {
        wxZenToggleButton *aItem = new wxZenToggleButton(mCurItem->GetParent(), mHandler, Lib, ID);
        mStackControls.push_back(aItem);
        m_pnl->AddFoldPanelWindow(*mCurItem, aItem, wxFPB_ALIGN_WIDTH, 0, 5);
    }
    // toggle plugin button
    void AddTogglePluginButton(const wxString &Lib, unsigned int ID)
    {
        wxZenToggleButtonPlugin *aItem = new wxZenToggleButtonPlugin(mCurItem->GetParent(), mHandler, Lib, ID);
        mStackControls.push_back(aItem);
        m_pnl->AddFoldPanelWindow(*mCurItem, aItem, wxFPB_ALIGN_WIDTH, 0, 5);
    }
    // color
	/*
    void AddColorPicker(const wxString &Lib, tcolor* pColor, unsigned int aID = 2)
    {
        wxZenColorPicker *aItem = new wxZenColorPicker(mCurItem->GetParent(), mHandler, Lib, pColor, aID);
        mStackControls.push_back(aItem);
        m_pnl->AddFoldPanelWindow(*mCurItem, aItem, wxFPB_ALIGN_WIDTH, 0, 5);
    }
	*/
    // slider
    void AddSlider(const wxString &Lib, unsigned int aMin, unsigned int aMax, unsigned int *pValue, unsigned int aID = 2)
    {
        wxZenSlider *aItem = new wxZenSlider(mCurItem->GetParent(), mHandler, Lib, pValue, aMin, aMax, aID);
        mStackControls.push_back(aItem);
        m_pnl->AddFoldPanelWindow(*mCurItem, aItem, wxFPB_ALIGN_WIDTH, 0, 5);
    }
    // text edit
    void AddTextEdit(const wxString &Lib, wxString* pValue, unsigned int aID = 2)
    {
        wxZenTextEdit * aItem = new wxZenTextEdit(mCurItem->GetParent(), mHandler, Lib, pValue, aID);
        mStackControls.push_back(aItem);
        m_pnl->AddFoldPanelWindow(*mCurItem, aItem, wxFPB_ALIGN_WIDTH, 0, 5);
    }
    // check box
    void AddCheckBox(const wxString &Lib, bool *pValue, unsigned int aID = 2)
    {
        wxZenCheckBox *aItem = new wxZenCheckBox(mCurItem->GetParent(), mHandler, Lib, pValue, aID);
        mStackControls.push_back(aItem);
        m_pnl->AddFoldPanelWindow(*mCurItem, aItem, wxFPB_ALIGN_WIDTH, 0, 5);
    }
    // bit field check box
    void AddCheckBoxBitField(const wxString &Lib, unsigned long *pValue, int aBitNumber, unsigned int aID = 2)
    {
        wxZenCheckBoxBitField * aItem = new wxZenCheckBoxBitField(mCurItem->GetParent(), mHandler, Lib, pValue, aBitNumber, aID);
        mStackControls.push_back(aItem);
        m_pnl->AddFoldPanelWindow(*mCurItem, aItem, wxFPB_ALIGN_WIDTH, 0, 5);
    }
    // radio
    void AddRadios(const wxString &Lib, const wxString& aRadioStrings, unsigned int *pValue, unsigned int aID = 2)
    {
        wxZenRadioBox *aItem = new wxZenRadioBox(mCurItem->GetParent(), mHandler, Lib, aRadioStrings, pValue, aID);
        mStackControls.push_back(aItem);
        m_pnl->AddFoldPanelWindow(*mCurItem, aItem, wxFPB_ALIGN_WIDTH, 0, 5);
    }
    // file
    void AddFileOpener(const wxString &Lib, const wxString & aExtensions, wxString *pValue, unsigned int aID = 2)
    {
        wxZenFileOpen *aItem = new wxZenFileOpen(mCurItem->GetParent(), mHandler, Lib, aExtensions, pValue, true, aID);
        mStackControls.push_back(aItem);
        m_pnl->AddFoldPanelWindow(*mCurItem, aItem, wxFPB_ALIGN_WIDTH, 0, 5);
    }
    // directory
    void AddDirectoryChooser(const wxString &Lib, wxString *pValue, unsigned int aID = 2)
    {
        wxZenDirectoryOpen *aItem = new wxZenDirectoryOpen(mCurItem->GetParent(), mHandler, Lib, pValue, aID);
        mStackControls.push_back(aItem);
        m_pnl->AddFoldPanelWindow(*mCurItem, aItem, wxFPB_ALIGN_WIDTH, 0, 5);
    }
    // combo
    void AddCombo(const wxString &Lib, const wxString& Items, unsigned int *pValue, unsigned int aID = 2)
    {
        wxZenComboBox *aItem = new wxZenComboBox(mCurItem->GetParent(), mHandler, Lib, Items, pValue, aID);
        mStackControls.push_back(aItem);
        m_pnl->AddFoldPanelWindow(*mCurItem, aItem, wxFPB_ALIGN_WIDTH, 0, 5);
    }
    void AddCombo(const wxString &Lib, const wxString& Items, unsigned char *pValue, unsigned int aID = 2)
    {
        wxZenComboBox *aItem = new wxZenComboBox(mCurItem->GetParent(), mHandler, Lib, Items, pValue, aID);
        mStackControls.push_back(aItem);
        m_pnl->AddFoldPanelWindow(*mCurItem, aItem, wxFPB_ALIGN_WIDTH, 0, 5);
    }
    void AddTextList(const wxString &Lib, const wxString& Items, unsigned int *pValue, unsigned int aID = 2, bool bHandleDblClick = true)
    {
        wxZenTextList *aItem = new wxZenTextList(mCurItem->GetParent(), mHandler, Lib, Items, pValue, aID, bHandleDblClick);
        mStackControls.push_back(aItem);
        m_pnl->AddFoldPanelWindow(*mCurItem, aItem, wxFPB_ALIGN_WIDTH, 0, 5);
    }

    wxZenTreeView * AddTreeView(const wxString &Lib, unsigned int aID = 2)
    {
        wxZenTreeView *aItem = new wxZenTreeView(mCurItem->GetParent(), mHandler, Lib, aID);
        mStackControls.push_back(aItem);
        m_pnl->AddFoldPanelWindow(*mCurItem, aItem, wxFPB_ALIGN_WIDTH, 0, 5);
		return aItem;
    }
	/*
    // gradient
    wxZenGradient*  AddGradient(ZAnimationTrack<tcolor> *animColor = NULL,
                    ZAnimationTrack<tfloat> *animSize = NULL,
					ZAnimationTrack<tfloat> *animRot = NULL,
                    tfloat *aLifeTime = NULL)
    {
        wxZenGradient* aItem = new wxZenGradient(mCurItem->GetParent(), mHandler, animColor, animSize, animRot, aLifeTime);
        mStackControls.push_back(aItem);
        m_pnl->AddFoldPanelWindow(*mCurItem, aItem, wxFPB_ALIGN_WIDTH, 0, 5);
		return aItem;
    }
	*/
    //
    void BeginPanel();
    void AddFolder(const wxString &Lib);
    void EndPanel();

    // class
//    void EditClass(SerializableField *pFields, const char *className, void *ptr, bool bCreateFolder = true, bool aHandleParentClass = false);

    // update
    void UpdateControlFromValue(unsigned int aControlID);
private:

    DECLARE_EVENT_TABLE();

    void OnSize(wxSizeEvent& event)
    {
        wxSashLayoutWindow::OnSize(event);
        if (m_pnl)
        {
            wxSize mSize = GetClientSize();
            SetScrollbar(wxVERTICAL, 0, mSize.y, GetPanelHeight());

        }
    }

    void OnScroll(wxScrollWinEvent& event)
    {
        wxSize mSize = GetClientSize();
        int panelHeight = GetPanelHeight();


        if (mSize.y>=panelHeight)
        {
            SetScrollbar(wxVERTICAL,0,panelHeight-1,panelHeight);
            return;
        }
        SetScrollbar(wxVERTICAL, event.GetPosition(), mSize.y, panelHeight);

        if (m_pnl)
        {


            m_pnl->SetPosition(wxPoint(0, -event.GetPosition()));
            m_pnl->SetSize(wxSize(m_pnl->GetSize().x, panelHeight));
        }
    }

    int GetPanelHeight()
    {
        if (!m_pnl) return 0;

        int sum = 0;
        for (unsigned int i =0;i<m_pnl->GetCount();i++)
        {
            sum +=m_pnl->Item(i).GetItem()->GetPanelLength();
        }
        return sum;
    }

    wxFoldPanel *mCurItem;
    wxFoldPanelBar *m_pnl;
    wxZEditHandler *mHandler;

	std::list<wxZEditStackedControl* >mStackControls;
};

///////////////////////////////////////////////////////////////////////////////////////////////////
#endif
