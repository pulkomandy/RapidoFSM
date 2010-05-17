///////////////////////////////////////////////////////////////////////////////////////////////////
// Zenith Engine
// File Name : wxNavigator.h
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

#ifndef WXSTACKEDCONTROLS_H__
#define WXSTACKEDCONTROLS_H__

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
#include <wx/tokenzr.h>
#include <wx/clrpicker.h>
#include <wx/tglbtn.h>
#include "wx/foldbar/foldpanelbar.h"
#include "wx/laywin.h"
#include <wx/dirdlg.h>
#include <wx/stc/stc.h>
//#include "wxGradientControl.h"
#include "ZString.h"
///////////////////////////////////////////////////////////////////////////////////////////////////

class wxZEditHandler
{
public:
	wxZEditHandler() {}
	~wxZEditHandler() {}

	virtual void HandleButtonClick(unsigned int aButtonID) = 0;
	virtual void HandleValueChanged(unsigned int aID) = 0;
	virtual void HandleLostFocus(unsigned int aID) = 0;
	//virtual void HandleGradientSelChanged(wxGradientControl *pControl) = 0;
};

class wxZEditStackedControl
{
public:
	wxZEditStackedControl() {}
	~wxZEditStackedControl() {}

	virtual void UpdateControlFromValue() {}
	virtual unsigned int GetID() { return mID; }
protected:
	unsigned int mID;
};

///////////////////////////////////////////////////////////////////////////////////////////////////

template <typename T> class wxZenNumEditRoot : public wxPanel
{
public:

	wxZenNumEditRoot(wxWindow* parent, wxZEditHandler * aHandler, const wxString& Lib, T aMin, T aMax, T *pValue, T aIncr=1.0f, unsigned int aID = 0)
		: wxPanel(parent, -1, wxDefaultPosition, wxDefaultSize, wxNO_BORDER)
	{

		mValue = pValue;
		wxSizer *mSizer = new wxBoxSizer(wxHORIZONTAL);
		mID = aID;

		mTxt = new wxTextCtrl(this, 3, "", wxDefaultPosition, wxDefaultSize, wxTE_RIGHT,
			wxTextValidator(wxFILTER_NUMERIC));
		mTxt->SetBackgroundColour(wxColour(197, 197, 197));

		mLib = NULL;
		if (!Lib.IsEmpty())
			mLib = new wxStaticText(this, wxID_ANY, Lib, wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT);

		mSpin = new wxSpinButton(this, 2);

		if (mLib)
		{
			mSizer->Add(mLib, 1, wxEXPAND + wxALL, 0);
			mSizer->Add(mTxt, 2, wxEXPAND + wxALL, 0);
		}
		else
			mSizer->Add(mTxt, 1, wxEXPAND + wxALL, 0);

		mSizer->Add(mSpin, 0, wxEXPAND + wxALL, 0);
		SetSizer(mSizer);

		mMin = aMin;
		mMax = aMax;
		mIncrement = aIncr;

		UpdateValue();
		mTxt->Connect(-1, wxEVT_KILL_FOCUS, (wxObjectEventFunction)&wxZenNumEditRoot::LostFocus);
		mHandler = aHandler;

		Connect(wxEVT_SCROLL_LINEUP, (wxObjectEventFunction)&wxZenNumEditRoot<T>::SpinUp);
		Connect(wxEVT_SCROLL_LINEDOWN, (wxObjectEventFunction)&wxZenNumEditRoot<T>::SpinDown);
		Connect(wxEVT_KILL_FOCUS, (wxObjectEventFunction)&wxZenNumEditRoot<T>::LostFocus);
	}

	~wxZenNumEditRoot() {}



	virtual unsigned int GetID() { return mID;}
protected:

	//DECLARE_EVENT_TABLE();


	wxTextCtrl *mTxt;
	wxStaticText *mLib;
	wxSpinButton *mSpin;
	T mIncrement;
	T mMin, mMax;
	T *mValue;
	unsigned int mID;

	void SpinUp(wxSpinEvent&)
	{
		FromText();
		*mValue += mIncrement;
		UpdateValue();
	}

	void SpinDown(wxSpinEvent&)
	{
		FromText();
		*mValue -= mIncrement;
		UpdateValue();
	}

	void LostFocus(wxCommandEvent& focusEvent)
	{
		((wxZenNumEditRoot*)((wxTextCtrl*)this)->GetParent())->FromText();
	}

	void FromText()
	{
		double nValue = mMin;
		if (mTxt->GetValue().ToDouble(&nValue))
		{
			*mValue = (T)nValue;
			if (mHandler) mHandler->HandleValueChanged(this->GetID());
			UpdateValue();
		}
	}

	void UpdateValue()
	{
		if (*mValue<mMin) *mValue = mMin;
		if (*mValue>mMax) *mValue = mMax;

		char tmps[512];
		sprintf(tmps,"%5.2f",(float)*mValue);
		mTxt->SetValue(tmps);
	}

	wxZEditHandler *mHandler;
};

typedef wxZenNumEditRoot<float> wxZenNumEditF;
typedef wxZenNumEditRoot<char> wxZenNumEditI8;
typedef wxZenNumEditRoot<unsigned char> wxZenNumEditUI8;
typedef wxZenNumEditRoot<short> wxZenNumEditI16;
typedef wxZenNumEditRoot<unsigned short> wxZenNumEditUI16;
typedef wxZenNumEditRoot<long> wxZenNumEditI32;
typedef wxZenNumEditRoot<unsigned long> wxZenNumEditUI32;
typedef wxZenNumEditRoot<unsigned int> wxZenNumEditUI;

template <typename T> class wxZenNumEdit : public wxZenNumEditRoot<T> , public wxZEditStackedControl
{
public:
	wxZenNumEdit(wxWindow* parent, wxZEditHandler * aHandler, const wxString& Lib, T aMin, T aMax, T *pValue, T aIncr=1.0f, unsigned int aID = 0)
		: wxZenNumEditRoot<T>(parent, aHandler, Lib, aMin, aMax, pValue, aIncr, aID)
	{
		wxZenNumEditRoot<T>::mID = aID;
		wxZEditStackedControl::mID = aID;
	}
	virtual ~wxZenNumEdit() {}
	virtual void UpdateControlFromValue()
	{
		wxZenNumEditRoot<T>::UpdateValue();
	}
protected:

};

///////////////////////////////////////////////////////////////////////////////////////////////////

class wxZenTextEditRoot : public wxPanel
{
public:

        wxZenTextEditRoot(wxWindow* parent, wxZEditHandler * aHandler, const wxString& Lib, wxString *Str, unsigned int aID = 0)
		: wxPanel(parent, -1, wxDefaultPosition, wxDefaultSize, wxNO_BORDER)
	{
		wxSizer *mSizer = new wxBoxSizer(wxHORIZONTAL);
		mID = aID;
                mTxt = new wxTextCtrl(this, 3, wxString::FromUTF8(""), wxDefaultPosition, wxDefaultSize, wxTE_LEFT);
		mTxt->SetBackgroundColour(wxColour(197, 197, 197));

		mStr = Str;

		mLib = NULL;
		if (!Lib.IsEmpty())
			mLib = new wxStaticText(this, wxID_ANY, Lib, wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT);

		if (mLib)
		{
			mSizer->Add(mLib, 1, wxEXPAND + wxALL, 0);
			mSizer->Add(mTxt, 2, wxEXPAND + wxALL, 0);
		}
		else
			mSizer->Add(mTxt, 1, wxEXPAND + wxALL, 0);

		SetSizer(mSizer);

		UpdateValue();
		mTxt->Connect(3, wxEVT_KILL_FOCUS, (wxObjectEventFunction)&wxZenTextEditRoot::LostFocus);
		mHandler = aHandler;
	}

	virtual ~wxZenTextEditRoot()
	{
	}
	virtual unsigned int GetID() { return mID; }

protected:

	DECLARE_EVENT_TABLE();


	wxTextCtrl *mTxt;
	wxStaticText *mLib;
        wxString *mStr;
	unsigned int mID;

	void LostFocus(wxCommandEvent& focusEvent)
	{
		((wxZenTextEditRoot*)((wxTextCtrl*)this)->GetParent())->FromText();
	}

	void FromText()
	{

		*mStr = mTxt->GetValue().c_str();
		if (mHandler) mHandler->HandleValueChanged(this->GetID());
		UpdateValue();

	}

	void UpdateValue()
	{
            mTxt->SetValue(*mStr);
	}

	wxZEditHandler *mHandler;
};

class wxZenTextEdit : public wxZenTextEditRoot, public wxZEditStackedControl
{
public:
        wxZenTextEdit(wxWindow* parent, wxZEditHandler * aHandler, const wxString& Lib, wxString *Str, unsigned int aID = 0)
		: wxZenTextEditRoot(parent, aHandler, Lib, Str, aID)
	{
		wxZenTextEditRoot::mID = aID;
		wxZEditStackedControl::mID = aID;
	}
	virtual ~wxZenTextEdit() {}
	virtual void UpdateControlFromValue()
	{
		UpdateValue();
	}
protected:


};





///////////////////////////////////////////////////////////////////////////////////////////////////

class wxZenTextList : public wxPanel, public wxZEditStackedControl
{
public:

	wxZenTextList(wxWindow* parent, wxZEditHandler * aHandler, const wxString& Lib, const wxString& strings, unsigned int *Value, unsigned int aID = 0, bool bHandleDblClick = true)
		: wxPanel(parent, -1, wxDefaultPosition, wxDefaultSize, wxNO_BORDER)
	{
		wxSizer *mSizer = new wxBoxSizer(wxVERTICAL);
		mID = aID;
		mLB = new wxListBox(this, 2);
		mbHandleDblClick = bHandleDblClick;
		SetSize(-1, 300);
		mValue = Value;
		wxArrayString mArray;
		wxStringTokenizer tkz(strings, wxT("|"));
		while ( tkz.HasMoreTokens() )
		{
			mArray.Add(tkz.GetNextToken());
		}
		mLB->InsertItems(mArray, 0);
		if (mArray.GetCount())
			mLB->SetSelection(*mValue);

		mLib = NULL;
		if (!Lib.IsEmpty())
			mLib = new wxStaticText(this, wxID_ANY, Lib, wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT);

		if (mLib)
		{
			mSizer->Add(mLib, 0, wxEXPAND + wxALL, 0);
			mSizer->Add(mLB, 2, wxEXPAND + wxALL, 0);
		}
		else
			mSizer->Add(mLB, 1, wxEXPAND + wxALL, 0);

		SetSizer(mSizer);
		mHandler = aHandler;
	}

	~wxZenTextList() {}

private:

	DECLARE_EVENT_TABLE();

	virtual void OnDblClick(wxCommandEvent& event)
	{
		if (mbHandleDblClick)
		{
			*mValue = mLB->GetSelection();
			if (mHandler) mHandler->HandleValueChanged(mID);
		}
	}

	virtual void OnClick(wxCommandEvent& event)
	{
		if (!mbHandleDblClick)
		{
			*mValue = mLB->GetSelection();
			if (mHandler) mHandler->HandleValueChanged(mID);
		}
	}

	wxListBox *mLB;
	wxStaticText *mLib;
	unsigned int *mValue;
	wxZEditHandler *mHandler;
	bool mbHandleDblClick;
};



///////////////////////////////////////////////////////////////////////////////////////////////////

class wxZenSlider : public wxPanel, public wxZEditStackedControl
{
public:

	wxZenSlider(wxWindow* parent, wxZEditHandler * aHandler, const wxString& Lib, unsigned int *Value, unsigned int aMin, unsigned int aMax, unsigned int aID = 0)
		: wxPanel(parent, -1, wxDefaultPosition, wxDefaultSize, wxNO_BORDER)
	{
		wxSizer *mSizer = new wxBoxSizer(wxHORIZONTAL);
		mID = aID;
		mSld = new wxSlider(this, 2, *Value, aMin, aMax);
		mValue = Value;


		mLib = NULL;
		if (!Lib.IsEmpty())
			mLib = new wxStaticText(this, wxID_ANY, Lib, wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT);

		if (mLib)
		{
			mSizer->Add(mLib, 1, wxEXPAND + wxALL, 0);
			mSizer->Add(mSld, 2, wxEXPAND + wxALL, 0);
		}
		else
			mSizer->Add(mSld, 1, wxEXPAND + wxALL, 0);

		SetSizer(mSizer);
		mHandler = aHandler;
	}

	~wxZenSlider() {}
	void Scroll(wxScrollEvent& event)
	{
		*mValue = event.GetPosition();
		if (mHandler) mHandler->HandleValueChanged(mID);
	}

private:

	DECLARE_EVENT_TABLE();


	wxSlider *mSld;
	wxStaticText *mLib;
	unsigned int *mValue;
	wxZEditHandler *mHandler;
};

///////////////////////////////////////////////////////////////////////////////////////////////////

class wxZenStatic : public wxPanel, public wxZEditStackedControl
{
public:

	wxZenStatic(wxWindow* parent, const wxString& Lib)
		: wxPanel(parent, -1, wxDefaultPosition, wxDefaultSize, wxNO_BORDER)
	{
		wxSizer *mSizer = new wxBoxSizer(wxHORIZONTAL);
		mLib = new wxStaticText(this, wxID_ANY, Lib, wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT);

		mSizer->Add(mLib, 1, wxEXPAND + wxALL, 0);

		SetSizer(mSizer);
	}

	~wxZenStatic() {}


private:

	DECLARE_EVENT_TABLE();
	wxStaticText *mLib;
};

///////////////////////////////////////////////////////////////////////////////////////////////////

class wxZenCheckBox : public wxPanel, public wxZEditStackedControl
{
public:

	wxZenCheckBox(wxWindow* parent, wxZEditHandler * aHandler, const wxString& Lib, bool *Value, unsigned int aID = 0)
		: wxPanel(parent, -1, wxDefaultPosition, wxDefaultSize, wxNO_BORDER)
	{
		mID = aID;
		mValue = Value;
		mBox = new wxCheckBox(this, 2, Lib, wxDefaultPosition, wxDefaultSize, wxCHK_2STATE);
		mBox->SetValue(*Value);
		mHandler = aHandler;
	}

	~wxZenCheckBox() {}


private:

	DECLARE_EVENT_TABLE();

	void OnCheck(wxCommandEvent& event)
	{
		*mValue = mBox->GetValue();
		if (mHandler) mHandler->HandleValueChanged(mID);
	}


	wxCheckBox *mBox;
	bool *mValue;
	wxZEditHandler *mHandler;
};

///////////////////////////////////////////////////////////////////////////////////////////////////

class wxZenCheckBoxBitField : public wxPanel, public wxZEditStackedControl
{
public:

	wxZenCheckBoxBitField(wxWindow* parent, wxZEditHandler * aHandler, const wxString& Lib, unsigned long *Value, int aBitNumber, unsigned int aID = 0)
		: wxPanel(parent, -1, wxDefaultPosition, wxDefaultSize, wxNO_BORDER)
	{
		mID = aID;
		mValue = Value;
		mBox = new wxCheckBox(this, 2, Lib, wxDefaultPosition, wxDefaultSize, wxCHK_2STATE);
		mMask = 1<<aBitNumber;
		mBox->SetValue((*Value & mMask) != 0);
		mHandler = aHandler;
	}

	~wxZenCheckBoxBitField() {}


private:

	DECLARE_EVENT_TABLE();

	void OnCheck(wxCommandEvent& event)
	{
		if (mBox->GetValue())
			*mValue |= mMask;
		else
			*mValue &= ~mMask;
	}


	wxCheckBox *mBox;
	unsigned long *mValue;
	unsigned long mMask;
	wxZEditHandler *mHandler;
};

///////////////////////////////////////////////////////////////////////////////////////////////////

class wxZenComboBox : public wxPanel, public wxZEditStackedControl
{
public:

	wxZenComboBox(wxWindow* parent, wxZEditHandler * aHandler, const wxString& Lib, const wxString& strValues, unsigned int *Value, unsigned int aID = 0)
		: wxPanel(parent, -1, wxDefaultPosition, wxDefaultSize, wxNO_BORDER)
	{
		wxSizer *mSizer = new wxBoxSizer(wxHORIZONTAL);

		mValue = Value;
		mValueC = NULL;
		mID = aID;
		wxStringTokenizer tkz(strValues, wxT("|"));
		while ( tkz.HasMoreTokens() )
		{
			mArray.Add(tkz.GetNextToken());
		}

		mLib = NULL;
		if (!Lib.IsEmpty())
			mLib = new wxStaticText(this, wxID_ANY, Lib, wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT);


                mBox = new wxComboBox(this, 2, _(""), wxDefaultPosition, wxDefaultSize, mArray, wxCB_READONLY);
		if (mArray.GetCount())
			mBox->SetValue(mArray[*Value]);
		mBox->SetBackgroundColour(wxColour(197, 197, 197));

		if (mLib)
		{
			mSizer->Add(mLib, 1, wxEXPAND + wxALL, 0);
			mSizer->Add(mBox, 2, wxEXPAND + wxALL, 0);
		}
		else
			mSizer->Add(mBox, 1, wxEXPAND + wxALL, 0);

		SetSizer(mSizer);
		mHandler = aHandler;
		SetSize(-1, mBox->GetSize().y);

	}

	wxZenComboBox(wxWindow* parent, wxZEditHandler * aHandler, const wxString& Lib, const wxString& strValues, unsigned char *Value, unsigned int aID = 0)
		: wxPanel(parent, -1, wxDefaultPosition, wxDefaultSize, wxNO_BORDER)
	{
		wxSizer *mSizer = new wxBoxSizer(wxHORIZONTAL);
		mID = aID;
		mValue = NULL;
		mValueC = Value;

		wxStringTokenizer tkz(strValues, wxT("|"));
		while ( tkz.HasMoreTokens() )
		{
			mArray.Add(tkz.GetNextToken());
		}

		mLib = NULL;
		if (!Lib.IsEmpty())
			mLib = new wxStaticText(this, wxID_ANY, Lib, wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT);


                mBox = new wxComboBox(this, 2, _(""), wxDefaultPosition, wxDefaultSize, mArray, wxCB_READONLY);
		mBox->SetValue(mArray[*Value]);
		mBox->SetBackgroundColour(wxColour(197, 197, 197));

		if (mLib)
		{
			mSizer->Add(mLib, 1, wxEXPAND + wxALL, 0);
			mSizer->Add(mBox, 2, wxEXPAND + wxALL, 0);
		}
		else
			mSizer->Add(mBox, 1, wxEXPAND + wxALL, 0);

		SetSizer(mSizer);
		mHandler = aHandler;
		SetSize(-1, mBox->GetSize().y);

	}

	~wxZenComboBox() {}


private:

	DECLARE_EVENT_TABLE();

	void OnSelChange(wxCommandEvent& event)
	{
		if (mValue)
			*mValue = mArray.Index(mBox->GetValue());
		else
			*mValueC = (unsigned char)mArray.Index(mBox->GetValue());
		if (mHandler) mHandler->HandleValueChanged(mID);
	}


	wxComboBox *mBox;
	unsigned int *mValue;
	unsigned char *mValueC;
	wxArrayString mArray;
	wxStaticText *mLib;
	wxZEditHandler *mHandler;
};

///////////////////////////////////////////////////////////////////////////////////////////////////

class wxZenRadioBox : public wxPanel, public wxZEditStackedControl
{
public:

	wxZenRadioBox(wxWindow* parent, wxZEditHandler * aHandler, const wxString& Lib, const wxString& strValues, unsigned int *Value, unsigned int aID = 0)
		: wxPanel(parent, -1, wxDefaultPosition, wxDefaultSize, wxNO_BORDER)
	{
		wxSizer *mSizer = new wxBoxSizer(wxHORIZONTAL);
		mID = aID;
		mValue = Value;
		wxArrayString mArray;
		wxStringTokenizer tkz(strValues, wxT("|"));
		while ( tkz.HasMoreTokens() )
		{
			mArray.Add(tkz.GetNextToken());
		}

                mBox = new wxRadioBox(this, 2, wxString::FromUTF8(""), wxDefaultPosition, wxDefaultSize, mArray, 1, wxRA_SPECIFY_COLS);
		mBox->SetSelection(*Value);
		mSizer->Add(mBox, 1, wxEXPAND + wxALL, 0);

		SetSizer(mSizer);

		mLib = NULL;
		if (!Lib.IsEmpty())
			mLib = new wxStaticText(this, wxID_ANY, Lib, wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT);

		SetSize(-1, mBox->GetSize().y);
		mHandler = aHandler;

	}

	~wxZenRadioBox() {}


private:

	DECLARE_EVENT_TABLE();

	void OnSelChange(wxCommandEvent& event)
	{
		*mValue = mBox->GetSelection();
		if (mHandler) mHandler->HandleValueChanged(mID);
	}


	wxRadioBox *mBox;
	unsigned int *mValue;


	wxStaticText *mLib;
	wxZEditHandler *mHandler;
};

///////////////////////////////////////////////////////////////////////////////////////////////////
/*
inline wxColour tcolorTowxColour(const tcolor &tcol)
{
	//tulong ucol = tcol.ConvToBGRA();
	return wxColour((unsigned char)(tcol.r*255)&0xFF,
		(unsigned char)(tcol.g*255)&0xFF,
		(unsigned char)(tcol.b*255)&0xFF,
		(unsigned char)(tcol.a*255)&0xFF);
}

inline tcolor wxColourTotColor(const wxColour &wcol)
{
	return tcolor(wcol.Red()*(1.f/255.f), wcol.Green()*(1.f/255.f), wcol.Blue()*(1.f/255.f), wcol.Alpha()*(1.f/255.f)  );
}


class wxZenColorPicker : public wxPanel, public wxZEditStackedControl
{
public:

	wxZenColorPicker(wxWindow* parent, wxZEditHandler * aHandler, const wxString& Lib, tcolor *pColor, unsigned int aID = 0)
		: wxPanel(parent, -1, wxDefaultPosition, wxDefaultSize, wxNO_BORDER)
	{
		wxSizer *mSizer = new wxBoxSizer(wxHORIZONTAL);

		tulong col = pColor->ConvToBGRA();
		mCol = new wxColourPickerCtrl(this, 2, tcolorTowxColour(*pColor));
		mID = aID;
		mColor = pColor;

		mLib = NULL;
		if (!Lib.IsEmpty())
			mLib = new wxStaticText(this, wxID_ANY, Lib, wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT);

		if (mLib)
		{
			mSizer->Add(mLib, 1, wxEXPAND + wxALL, 0);
			mSizer->Add(mCol, 2, wxEXPAND + wxALL, 0);
		}
		else
			mSizer->Add(mCol, 1, wxEXPAND + wxALL, 0);

		SetSizer(mSizer);
		mHandler = aHandler;

	}

	~wxZenColorPicker() {}

	virtual void UpdateControlFromValue()
	{
		mCol->SetColour(tcolorTowxColour(*mColor));
	}

private:
	void OnColorChanged(wxColourPickerEvent &event)
	{
		*mColor = wxColourTotColor(mCol->GetColour());
		if (mHandler) mHandler->HandleValueChanged(mID);
	}

	DECLARE_EVENT_TABLE();

	wxColourPickerCtrl *mCol;
	wxStaticText *mLib;
	tcolor *mColor;
	wxZEditHandler *mHandler;
};
*/
///////////////////////////////////////////////////////////////////////////////////////////////////

class wxZenFileOpen : public wxPanel, public wxZEditStackedControl
{
public:

        wxZenFileOpen(wxWindow* parent, wxZEditHandler * aHandler, const wxString& Lib, const wxString& aExtensions, wxString *pFileName, bool aForOpening = true, unsigned int aID = 0)
		: wxPanel(parent, -1, wxDefaultPosition, wxDefaultSize, wxNO_BORDER)
	{
		wxSizer *mSizer = new wxBoxSizer(wxHORIZONTAL);
		mID = aID;

                if (pFileName->Length()) {
                    wxFileName location (*pFileName);
                    mBut = new wxButton(this, 2, location.GetName());
                } else
                        mBut = new wxButton(this, 2, _("..."));

		mFileName = pFileName;
		mLib = NULL;
		mExtensions = aExtensions;
		mbForOpening = aForOpening;

		if (!Lib.IsEmpty())
			mLib = new wxStaticText(this, wxID_ANY, Lib, wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT);

		if (mLib)
		{
			mSizer->Add(mLib, 1, wxEXPAND + wxALL, 0);
			mSizer->Add(mBut, 2, wxEXPAND + wxALL, 0);
		}
		else
			mSizer->Add(mBut, 1, wxEXPAND + wxALL, 0);

		SetSizer(mSizer);
		mHandler = aHandler;
	}

	~wxZenFileOpen() {}

private:

	void OnOpen(wxCommandEvent &event)
	{
                wxFileDialog fDialog(GetParent(), _("Choose a file"), _(""), _(""), mExtensions,
			mbForOpening?(wxFD_OPEN|wxFD_FILE_MUST_EXIST):(wxFD_SAVE|wxFD_OVERWRITE_PROMPT));

		if (fDialog.ShowModal() == wxID_OK)
		{
                        *mFileName = fDialog.GetPath();
			mBut->SetLabel(fDialog.GetFilename());
			if (mHandler) mHandler->HandleValueChanged(mID);
		}
	}


	DECLARE_EVENT_TABLE();

	wxButton *mBut;
	wxStaticText *mLib;
	//tcolor *mColor;
	bool mbForOpening;
	wxString mExtensions;
        wxString *mFileName;
	wxZEditHandler *mHandler;
};

///////////////////////////////////////////////////////////////////////////////////////////////////

class wxZenDirectoryOpen : public wxPanel, public wxZEditStackedControl
{
public:

        wxZenDirectoryOpen(wxWindow* parent, wxZEditHandler * aHandler, const wxString& Lib, wxString *pDirName, unsigned int aID = 0)
		: wxPanel(parent, -1, wxDefaultPosition, wxDefaultSize, wxNO_BORDER)
	{
		wxSizer *mSizer = new wxBoxSizer(wxHORIZONTAL);
		mID = aID;

		if (pDirName->Length())
                    mBut = new wxButton(this, 2, *pDirName);
		else
                        mBut = new wxButton(this, 2, _("..."));

		mDirName = pDirName;
		mLib = NULL;

		if (!Lib.IsEmpty())
			mLib = new wxStaticText(this, wxID_ANY, Lib, wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT);

		if (mLib)
		{
			mSizer->Add(mLib, 1, wxEXPAND + wxALL, 0);
			mSizer->Add(mBut, 2, wxEXPAND + wxALL, 0);
		}
		else
			mSizer->Add(mBut, 1, wxEXPAND + wxALL, 0);

		SetSizer(mSizer);
		mHandler = aHandler;

	}

	~wxZenDirectoryOpen() {}

private:

	void OnOpen(wxCommandEvent &event)
	{
		/*
		wxDirDialog fDialog(GetParent(), "Choose a directory", "", wxDD_DEFAULT_STYLE|wxDD_DIR_MUST_EXIST);

		if (fDialog.ShowModal() == wxID_OK)
		{
		*mDirName = fDialog.GetPath().mb_str();
		mBut->SetLabel(fDialog.GetPath());
		}
		*/
		if (mHandler) mHandler->HandleValueChanged(mID);
	}


	DECLARE_EVENT_TABLE();

	wxButton *mBut;
	wxStaticText *mLib;
	//tcolor *mColor;
	bool mbForOpening;
	wxString mExtensions;
        wxString *mDirName;
	wxZEditHandler *mHandler;
};

///////////////////////////////////////////////////////////////////////////////////////////////////


class wxZenButton : public wxPanel, public wxZEditStackedControl
{
public:

	wxZenButton(wxWindow* parent, wxZEditHandler * aHandler, unsigned int aID, const wxString& Lib)
		: wxPanel(parent, -1, wxDefaultPosition, wxDefaultSize, wxNO_BORDER)
	{
		wxSizer *mSizer = new wxBoxSizer(wxHORIZONTAL);
		mBut = new wxButton(this, 2, Lib);
		mID = aID;
		mHandler = aHandler;
		mSizer->Add(mBut, 1, wxEXPAND + wxALL, 0);
		SetSizer(mSizer);
		SetSize(-1, mBut->GetSize().y);
	}

	~wxZenButton() {}

private:

	void OnClick(wxCommandEvent &event)
	{
		if (mHandler) mHandler->HandleButtonClick(mID);
	}


	DECLARE_EVENT_TABLE();

	wxButton *mBut;
	wxZEditHandler *mHandler;
};

class wxZenScriptEdit : public wxPanel, public wxZEditStackedControl
{
public:
	wxZenScriptEdit()
	{
	}

	wxZenScriptEdit(wxWindow* parent, wxZEditHandler * aHandler, wxString *pToEdit, unsigned int aID)
		: wxPanel(parent, -1, wxDefaultPosition, wxDefaultSize, wxNO_BORDER)
	{
		wxSizer *mSizer = new wxBoxSizer(wxHORIZONTAL);
		mScriptEdit = BuildScriptPanel(this, (wxObjectEventFunction)&wxZenScriptEdit::onNewChar);
		mID = aID;
		mToEdit = pToEdit;
		mHandler = aHandler;
		mSizer->Add(mScriptEdit, 1, wxEXPAND + wxALL, 0);
		SetSizer(mSizer);
		SetSize(-1, mScriptEdit->GetSize().y);
		mScriptEdit->SetText(mToEdit->c_str());
		//mScriptEdit->Connect(-1, wxEVT_KILL_FOCUS, (wxObjectEventFunction)&wxZenScriptEdit::LostFocus);

	}

	virtual ~wxZenScriptEdit() {}

	static wxStyledTextCtrl* BuildScriptPanel(wxWindow *Parent, wxObjectEventFunction pOnNewChar);


	void onNewChar(wxEvent& event)
	{
		wxZenScriptEdit* pthis = (wxZenScriptEdit*)this->GetParent();
		*pthis->mToEdit = pthis->mScriptEdit->GetText();//text; 
	}
private:

	/*void OnClick(wxCommandEvent &event)
	{
		if (mHandler) mHandler->HandleButtonClick(mID);
	}
	*/
	wxZEditHandler *mHandler;

	wxString *mToEdit;

	

	/*
	void LostFocus(wxEvent& focusEvent)
	{
		wxZenScriptEdit *pse = ((wxZenScriptEdit*)this->GetParent());
		if (pse->mHandler) 
			pse->mHandler->HandleLostFocus(mID);
	}
*/
	DECLARE_EVENT_TABLE();

	wxStyledTextCtrl* mScriptEdit;
	//void onNewChar(wxEvent& event);
	//wxZEditHandler *mHandler;
};

///////////////////////////////////////////////////////////////////////////////////////////////////

/*
class wxZenGradient : public wxPanel, public wxZEditStackedControl
{
public:

	wxZenGradient(wxWindow* parent, wxZEditHandler * aHandler,
		ZAnimationTrack<tcolor> *animColor = NULL,
		ZAnimationTrack<tfloat> *animSize = NULL,
		ZAnimationTrack<tfloat> *animRot = NULL,
		tfloat *aLifeTime = NULL, unsigned int aID = 0)
		: wxPanel(parent, -1, wxDefaultPosition, wxDefaultSize, wxNO_BORDER)
	{
		mID = aID;
		wxSizer *mSizer = new wxBoxSizer(wxHORIZONTAL);
		mGrad = new wxGradientControl(this, aHandler, animColor, animSize, animRot, aLifeTime, 2);
		mHandler = aHandler;
		mSizer->Add(mGrad, 1, wxEXPAND + wxALL, 0);
		SetSizer(mSizer);
		SetSize(-1, 80);
	}

	~wxZenGradient() {}
	wxGradientControl *GetGradientControl() { return mGrad; }

private:
	DECLARE_EVENT_TABLE();

	wxGradientControl *mGrad;
	wxZEditHandler *mHandler;
};

*/
///////////////////////////////////////////////////////////////////////////////////////////////////

class wxZenToggleButton : public wxPanel, public wxZEditStackedControl
{
public:

	wxZenToggleButton(wxWindow* parent, wxZEditHandler * aHandler, const wxString& Lib, unsigned int aID = 0)
		: wxPanel(parent, -1, wxDefaultPosition, wxDefaultSize, wxNO_BORDER)
	{
		mID = aID;
		//mValue = Value;
		wxSizer *mSizer = new wxBoxSizer(wxHORIZONTAL);
		mButton = new wxToggleButton(this, 2, Lib, wxDefaultPosition, wxDefaultSize);
		mSizer->Add(mButton, 1, wxEXPAND + wxALL, 0);
		SetSizer(mSizer);
		SetSize(-1, mButton->GetSize().y);
		//mBox->SetValue(*Value);
		mHandler = aHandler;
	}

	~wxZenToggleButton() {}


private:

	DECLARE_EVENT_TABLE();

	virtual void OnToggle(wxCommandEvent& event)
	{
		//        *mValue = mBox->GetValue();
		if (mHandler) mHandler->HandleValueChanged(mID);
	}


	wxToggleButton *mButton;
	//bool *mValue;
	wxZEditHandler *mHandler;
};

///////////////////////////////////////////////////////////////////////////////////////////////////

class wxZenToggleButtonPlugin : public wxZenToggleButton
{
public:

	wxZenToggleButtonPlugin(wxWindow* parent, wxZEditHandler * aHandler, const wxString& Lib, unsigned int aID = 0)
		: wxZenToggleButton(parent, aHandler, Lib, aID)
	{
	}

	~wxZenToggleButtonPlugin() {}

	static void UntogglePluginButton()
	{
		if (gLastClickedToggleButton)
		{
			gLastClickedToggleButton->mButton->SetValue(false);
		}
	}
private:

	DECLARE_EVENT_TABLE();

	virtual void OnToggle(wxCommandEvent& event)
	{
		wxZenToggleButton::OnToggle(event);
		UntogglePluginButton();
		mButton->SetValue(true);
		gLastClickedToggleButton = this;
	}

	static wxZenToggleButtonPlugin* gLastClickedToggleButton;
};
///////////////////////////////////////////////////////////////////////////////////////////////////

class wxZenTreeView : public wxPanel, public wxZEditStackedControl
{
public:

	wxZenTreeView(wxWindow* parent, wxZEditHandler * aHandler, const wxString& Lib, unsigned int aID = 0)
		: wxPanel(parent, -1, wxDefaultPosition, wxDefaultSize, wxNO_BORDER)
	{
		mID = aID;
		//mValue = Value;
		wxSizer *mSizer = new wxBoxSizer(wxHORIZONTAL);
		mTV = new wxTreeCtrl(this, 2, wxDefaultPosition, wxSize(100,160));
		mSizer->Add(mTV, 1, wxEXPAND + wxALL, 0);
		SetSizer(mSizer);
		SetSize(-1, mTV->GetSize().y);
		//mBox->SetValue(*Value);
		mHandler = aHandler;

                mRootID = mTV->AddRoot(_(""));
	}

	~wxZenTreeView() {}

        virtual wxTreeItemId AddItem(const wxChar *szStr)
	{
                wxTreeItemId ret = mTV->AppendItem(mRootID, szStr);
		mTV->ExpandAll();
		return ret;

	}

        virtual wxTreeItemId AddItem(const wxChar *szStr, wxTreeItemId rt)
	{
                wxTreeItemId ret = mTV->AppendItem(rt, szStr);
		mTV->ExpandAll();
		return ret;
	}

	virtual void SetItemData(wxTreeItemId rt, void *pData)
	{
		mTV->SetItemData(rt, (wxTreeItemData*)pData);
	}
	virtual void* GetItemData(wxTreeItemId rt)
	{
		return mTV->GetItemData(rt);
	}

	virtual wxTreeItemId GetSelection()
	{
		return mTV->GetSelection();
	}

private:

	DECLARE_EVENT_TABLE();

	virtual void OnTVSelChanged(wxTreeEvent& event)
	{
		if (mHandler) mHandler->HandleValueChanged(mID);
	}





	wxTreeCtrl *mTV;
	wxZEditHandler *mHandler;
	wxTreeItemId mRootID;
};

///////////////////////////////////////////////////////////////////////////////////////////////////


#endif
