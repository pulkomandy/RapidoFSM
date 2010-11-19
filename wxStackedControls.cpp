///////////////////////////////////////////////////////////////////////////////////////////////////
// Zenith Engine
// File Name : wxStackedControls.cpp
// Creation : 08/11/2006
// Author : Cedric Guillemet
// Description :
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; version 2 of the License.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//#include "wxStdAfx.h"
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
#include "wxStackedControls.h"



///////////////////////////////////////////////////////////////////////////////////////////////////

BEGIN_EVENT_TABLE(wxZenStatic, wxPanel)
END_EVENT_TABLE()

///////////////////////////////////////////////////////////////////////////////////////////////////

BEGIN_EVENT_TABLE(wxZenTextEditRoot, wxPanel)
EVT_COMMAND_KILL_FOCUS(3, wxZenTextEditRoot::LostFocus)
END_EVENT_TABLE()

///////////////////////////////////////////////////////////////////////////////////////////////////

BEGIN_EVENT_TABLE(wxZenSlider, wxPanel)
EVT_SCROLL(wxZenSlider::Scroll)
END_EVENT_TABLE()

///////////////////////////////////////////////////////////////////////////////////////////////////

BEGIN_EVENT_TABLE(wxZenCheckBox, wxPanel)
EVT_CHECKBOX(2, wxZenCheckBox::OnCheck)
END_EVENT_TABLE()

///////////////////////////////////////////////////////////////////////////////////////////////////

BEGIN_EVENT_TABLE(wxZenCheckBoxBitField, wxPanel)
EVT_CHECKBOX(2, wxZenCheckBoxBitField::OnCheck)
END_EVENT_TABLE()

///////////////////////////////////////////////////////////////////////////////////////////////////

BEGIN_EVENT_TABLE(wxZenComboBox, wxPanel)
EVT_COMBOBOX(2, wxZenComboBox::OnSelChange)
END_EVENT_TABLE()

///////////////////////////////////////////////////////////////////////////////////////////////////


BEGIN_EVENT_TABLE(wxZenRadioBox, wxPanel)
EVT_RADIOBOX(2, wxZenRadioBox::OnSelChange)
END_EVENT_TABLE()

///////////////////////////////////////////////////////////////////////////////////////////////////
/*
BEGIN_EVENT_TABLE(wxZenColorPicker, wxPanel)
EVT_COLOURPICKER_CHANGED(2, wxZenColorPicker::OnColorChanged)
END_EVENT_TABLE()
*/
///////////////////////////////////////////////////////////////////////////////////////////////////

BEGIN_EVENT_TABLE(wxZenFileOpen, wxPanel)
EVT_BUTTON(2, wxZenFileOpen::OnOpen)
END_EVENT_TABLE()

///////////////////////////////////////////////////////////////////////////////////////////////////

BEGIN_EVENT_TABLE(wxZenDirectoryOpen, wxPanel)
EVT_BUTTON(2, wxZenDirectoryOpen::OnOpen)
END_EVENT_TABLE()

///////////////////////////////////////////////////////////////////////////////////////////////////

BEGIN_EVENT_TABLE(wxZenButton, wxPanel)
EVT_BUTTON(2, wxZenButton::OnClick)
END_EVENT_TABLE()

///////////////////////////////////////////////////////////////////////////////////////////////////
/*
BEGIN_EVENT_TABLE(wxZenGradient, wxPanel)
END_EVENT_TABLE()
*/
///////////////////////////////////////////////////////////////////////////////////////////////////

BEGIN_EVENT_TABLE(wxZenToggleButton, wxPanel)
EVT_TOGGLEBUTTON(2, wxZenToggleButton::OnToggle)
END_EVENT_TABLE()

///////////////////////////////////////////////////////////////////////////////////////////////////

BEGIN_EVENT_TABLE(wxZenToggleButtonPlugin, wxPanel)
EVT_TOGGLEBUTTON(2, wxZenToggleButtonPlugin::OnToggle)
END_EVENT_TABLE()

wxZenToggleButtonPlugin* wxZenToggleButtonPlugin::gLastClickedToggleButton = NULL;

///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////

BEGIN_EVENT_TABLE(wxZenTextList, wxPanel)
EVT_LISTBOX_DCLICK(2, wxZenTextList::OnDblClick)
EVT_LISTBOX(2, wxZenTextList::OnClick)
END_EVENT_TABLE()


BEGIN_EVENT_TABLE(wxZenTreeView, wxPanel)
EVT_TREE_SEL_CHANGED(2, wxZenTreeView::OnTVSelChanged)
END_EVENT_TABLE()


///////////////////////////////////////////////////////////////////////////////////////////////////

BEGIN_EVENT_TABLE(wxZenScriptEdit, wxPanel)
END_EVENT_TABLE()

wxStyledTextCtrl* wxZenScriptEdit::BuildScriptPanel(wxWindow *Parent, wxObjectEventFunction pOnNewChar)
{
    wxStyledTextCtrl* editor = new wxStyledTextCtrl(Parent, 3);
	static wxFont* mFont = NULL;
	static wxFont* mFontItalic = NULL;
	if (!mFont)
	{
                mFont       = new wxFont(10, wxNORMAL, wxNORMAL, wxNORMAL, false, _("Arial Unicode MS"));
                mFontItalic = new wxFont(10, wxNORMAL, wxFONTSTYLE_ITALIC, wxNORMAL, false, _("Courier New"));
	}

    editor->SetBufferedDraw(true);
    editor->SetFont(*mFont);
    editor->StyleSetFont(wxSTC_STYLE_DEFAULT, *mFont);
    editor->StyleClearAll();
    editor->StyleSetForeground(0,  wxColour(128, 128, 128));          // White space
    editor->StyleSetForeground(1,  wxColour(0, 127, 0));              // Comment
    editor->StyleSetFont(1, *mFontItalic);
	editor->StyleSetUnderline(1, false);
    editor->StyleSetForeground(2,  wxColour(0, 127, 0));              // Line Comment
    editor->StyleSetFont(2, *mFontItalic);                                 // Doc. Comment
	editor->StyleSetUnderline(2, false);
    editor->StyleSetForeground(3,  wxColour(127, 127, 127));          // ???
    editor->StyleSetForeground(4,  wxColour(200, 0, 0));            	 // Number
    editor->StyleSetForeground(5,  wxColour(0, 0, 255));              // Keyword
    editor->StyleSetBold(5,  false);
	editor->StyleSetUnderline(5, false);
    editor->StyleSetForeground(6,  wxColour(0, 150, 150));            // quoted string
    editor->StyleSetForeground(7,  wxColour(127, 0, 127));            // not used
    editor->StyleSetForeground(8,  wxColour(0, 127, 127));            // Literal strings
    editor->StyleSetForeground(9,  wxColour(127, 127, 0));            // Preprocessor
    editor->StyleSetForeground(10, wxColour(0, 0, 0));                // Operators
    editor->StyleSetBold(10, false);
    editor->StyleSetForeground(11, wxColour(0, 0, 0));                // Identifiers
    editor->StyleSetForeground(12, wxColour(0, 0, 0));                // Unterminated strings
    editor->StyleSetBackground(12, wxColour(224, 192, 224));
    editor->StyleSetBold(12, false);
    editor->StyleSetEOLFilled(12, true);

    editor->StyleSetForeground(13, wxColour(0, 0, 95));               // Keyword highlighting styles
    editor->StyleSetForeground(14, wxColour(0, 95, 0));
    editor->StyleSetForeground(15, wxColour(127, 0, 0));
    editor->StyleSetForeground(16, wxColour(127, 0, 95));
    editor->StyleSetForeground(17, wxColour(63, 95, 95));

    editor->StyleSetForeground(18, wxColour(0, 127, 127));            // Nested literal strings ???
    editor->StyleSetBackground(18, wxColour(240,255,255));
    editor->StyleSetForeground(19, wxColour(0, 127, 127));
    editor->StyleSetBackground(19, wxColour(224,255,255));
    editor->StyleSetForeground(20, wxColour(0, 127, 127));
    editor->StyleSetBackground(20, wxColour(192,255,255));
    editor->StyleSetForeground(21, wxColour(0, 127, 127));
    editor->StyleSetBackground(21, wxColour(176,255,255));
    editor->StyleSetForeground(22, wxColour(0, 127, 127));
    editor->StyleSetBackground(22, wxColour(160,255,255));
    editor->StyleSetForeground(23, wxColour(0, 127, 127));
    editor->StyleSetBackground(23, wxColour(144,255,255));
    editor->StyleSetForeground(24, wxColour(0, 127, 127));
    editor->StyleSetBackground(24, wxColour(128,155,255));

    editor->StyleSetForeground(32, wxColour(224, 192, 224));          // Line number
    editor->StyleSetBackground(33, wxColour(192, 192, 192));          // Brace highlight
    editor->StyleSetForeground(34, wxColour(0, 0, 255));
    editor->StyleSetBold(34, false);                                   // Brace incomplete highlight
    editor->StyleSetForeground(35, wxColour(255, 0, 0));
    editor->StyleSetBold(35, false);                                   // Indentation guides
    editor->StyleSetForeground(37, wxColour(192, 192, 192));
    editor->StyleSetBackground(37, wxColour(255, 255, 255));
    editor->SetUseTabs(false);
    editor->SetTabWidth(4);
    editor->SetIndent(4);
    editor->SetIndentationGuides(true);
    editor->SetMarginSensitive(1, true);
    editor->MarkerDefine(0/*wxBREAKPOINT_MARKER*/,   wxSTC_MARK_CIRCLE, *wxGREEN, *wxGREEN);
    editor->MarkerDefine(1/*wxCURRENT_LINE_MARKER*/, wxSTC_MARK_ARROW,  *wxRED,   *wxRED);
    editor->SetVisiblePolicy(wxSTC_VISIBLE_SLOP, 3);
    editor->SetXCaretPolicy(wxSTC_CARET_SLOP, 10);
    editor->SetYCaretPolicy(wxSTC_CARET_SLOP, 3);

	editor->SetLexer(wxSTC_LEX_CPP );
    //editor->SetCodePage(wxSTC_CP_UTF8);

    editor->Connect(-1, wxEVT_STC_CHANGE, pOnNewChar);//(wxObjectEventFunction)&wxZenScriptEdit::onNewChar);

    return editor;
}
/*
void wxZenScriptEdit::onNewChar(wxEvent& event)
{
	//wxZenScriptEditCallback* pthis = (wxZenScriptEditCallback*)(this->GetParent());
	//pthis->UpdateScriptText();//>SetNewText(pthis->mScriptEdit->GetText());
}
*/
