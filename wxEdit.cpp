///////////////////////////////////////////////////////////////////////////////////////////////////
// Zenith Engine
// File Name : wxNavigator.cpp
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
#include <wx/stc/stc.h>
#include <wx/aui/aui.h>
#include "wxEdit.h"



///////////////////////////////////////////////////////////////////////////////////////////////////

BEGIN_EVENT_TABLE(wxZEdit, wxSashLayoutWindow)
    EVT_SIZE(wxZEdit::OnSize)
    EVT_SCROLLWIN(wxZEdit::OnScroll)
END_EVENT_TABLE()

///////////////////////////////////////////////////////////////////////////////////////////////////

wxZEdit::wxZEdit(wxWindow* parent, wxZEditHandler *aHandler)
                     : wxSashLayoutWindow(parent, -1, wxDefaultPosition, wxDefaultSize, wxNO_BORDER)
{
    m_pnl = NULL;
    mCurItem = NULL;
    mHandler = aHandler;
    //SetBackgroundColour(wxColour(157, 157, 157));
}

///////////////////////////////////////////////////////////////////////////////////////////////////

wxZEdit::~wxZEdit()
{
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void wxZEdit::BeginPanel()
{
    //SAFE_DELETE(m_pnl);
	if (m_pnl)
		delete m_pnl;
	m_pnl = NULL;
    mStackControls.clear();

    m_pnl = new wxFoldPanelBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxFPB_DEFAULT_STYLE, ~wxFPB_COLLAPSE_TO_BOTTOM);//fpb_flags);
    //m_pnl->SetBackgroundColour(wxColour(157, 157, 157));
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void wxZEdit::AddFolder(const wxString &Lib)
{
    //SAFE_DELETE(mCurItem);
	if (mCurItem)
		delete mCurItem;
	mCurItem = NULL;
    mCurItem = new wxFoldPanel(m_pnl->AddFoldPanel(Lib, false));
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void wxZEdit::EndPanel()
{
    wxCaptionBarStyle style;
    wxColour col1(193,208,233), col2(49,106,197);
    style.SetFirstColour(col1);
    style.SetSecondColour(col2);
    style.SetCaptionStyle(wxCAPTIONBAR_FILLED_RECTANGLE);
    m_pnl->ApplyCaptionStyleAll(style);

    // ask for a resize
    wxSizeEvent mevent( GetSize()+wxSize(1,1), GetId() );
    mevent.SetEventObject( this );
    ProcessEvent( mevent );

}

///////////////////////////////////////////////////////////////////////////////////////////////////
#if 0
void wxZEdit::EditClass(SerializableField *pFields, const char *className, void *ptr, bool bCreateFolder, bool aHandleParentClass)
{
    if (bCreateFolder)
    {
        if ( (className[0] == 'P') && (className[1] == 'S') && (className[2] == 'M') && (className[3] == '_'))
            AddFolder(className+4);
        else
            AddFolder(className);
    }

    while (pFields->type != SerializableEnd)
    {
        if (pFields->type == SerializableBaseClass)
        {
            //printf("%s+ %s \n",getTabDepth(depth),pFields->name);
            if (aHandleParentClass)
                EditClass((SerializableField *)pFields->size_fieldsOffset, pFields->name, (void*)((((char*)ptr)+pFields->offset)), /*depth, */false, false);
        }
        else
        {
            // agregated class?
            if (pFields->type == SerializableAgregat)
            {
                if (pFields->count>1)
                {
                    /*
                    for (unsigned int i= 0;i<pFields->count;i++)
                    {
                        printf("%s%s[%d]\n",getTabDepth(depth),pFields->name,i);
                        DumpObjectValuesFn((SerializableField *)pFields->size_fieldsOffset,
                            pFields->name,
                            (void*)((((char*)ptr)+pFields->offset+(i*gSerializableClasses[pFields->classID].mClassSize))),
                            depth);
                    }
                    */
                }
                else
                {
                    /*
                    if (pFields->classID == tcolor::mClassID)
                    {
                        AddColorPicker(pFields->name, (tcolor*)((((char*)ptr)+pFields->offset)));
                    }
                    else
                    {
                    }
                    */

                    //printf("%s%s\n",getTabDepth(depth),pFields->name);
                    //DumpObjectValuesFn((SerializableField *)pFields->size_fieldsOffset, pFields->name, (void*)((((char*)ptr)+pFields->offset)), depth);
                }
            }
            else
            {
/*
                if (pFields->count>1)
                {
                    printf("%s%s[%d] = {",getTabDepth(depth),pFields->name,pFields->count);
                }
                else
                    printf("%s%s = ",getTabDepth(depth),pFields->name);
*/
                const char *aFieldName = pFields->name;
                if (aFieldName[0] == 'm') aFieldName++;

                for (unsigned int i= 0;i<pFields->count;i++)
                {
                    char *ptrOfset = (((char*)ptr)+pFields->offset+(i*pFields->size_fieldsOffset));
                    // base
                    switch (pFields->classID)
                    {
                    case SB_UNDEFINED:    /*printf("(undefined type)");*/                    break;
                    case SB_I8:
                        {
                            if (pFields->mEnumStr)
                            {
                                AddCombo(aFieldName, wxString(pFields->mEnumStr), (unsigned char*)ptrOfset);
                            }
                            else
                            {
                                AddNumEdit<char>(aFieldName, -128, 127, (char*)ptrOfset, 1);
                            }
                        }
                        break;
                    case SB_BOOL:
                        AddCheckBox(aFieldName, (bool*)ptrOfset);
                        break;
                    case SB_UI8:
                        AddNumEdit<unsigned char>(aFieldName, 0, 255, (unsigned char*)ptrOfset, 1);
                        break;
                    case SB_I16:
                        AddNumEdit<short>(aFieldName, -32768, 32767, (short*)ptrOfset, 1);
                        break;
                    case SB_UI16:
                        AddNumEdit<unsigned short>(aFieldName, 0, 65535, (unsigned short*)ptrOfset, 1);
                        break;
                    case SB_I32:
                        AddNumEdit<long>(aFieldName, -2147483647, 2147483647, (long*)ptrOfset, 1);
                        break;
                    case SB_UI32:
                        {
                            if (pFields->mEnumStr)
                            {
                                // bit field
                                unsigned long aBitNb = 0;
                                wxStringTokenizer tkz(pFields->mEnumStr, wxT("|"));
                                while ( tkz.HasMoreTokens() )
                                {
                                    wxString bitFieldName = tkz.GetNextToken();
                                    if (!bitFieldName.IsEmpty())
                                    {
                                        AddCheckBoxBitField(bitFieldName, (unsigned long*)ptrOfset, aBitNb);
                                    }
                                    aBitNb++;
                                }
                            }
                            else
                            {
                                // normal ui32
                                AddNumEdit<unsigned long>(aFieldName, 0, 4294967295, (unsigned long*)ptrOfset, 1);
                            }
                        }
                        break;
                    case SB_FLOAT:
                        AddNumEdit<float>(aFieldName, -10000000, 10000000, (float*)ptrOfset, 0.1f);
                        break;
                    case SB_DOUBLE:        /*printf("%f",*(double*) ptrOfset);*/            break;
                    case SB_STRING:        /*printf("%s",(char*) ptrOfset);*/
                        if (!_stricmp(aFieldName,"name"))
                            AddTextEdit(aFieldName, (tstring*)ptrOfset, 10);
                        else
                            AddTextEdit(aFieldName, (tstring*)ptrOfset);
                        break;
                    case SB_WSTRING:
                        //printf("%s%s = %d \n",getTabDepth(depth),pFields->name,*(int*) (((char*)ptr)+pFields->offset));
                        break;

                    }
                    //if ((i!=(pFields->count-1))&&(pFields->count>1)) printf(", ");

                }
                /*
                if (pFields->count>1)
                    printf("}\n");
                else
                    printf("\n");
                    */
            }
        }

        pFields++;
    }
}
#endif
///////////////////////////////////////////////////////////////////////////////////////////////////

void wxZEdit::UpdateControlFromValue(unsigned int aControlID)
{
    tlist<wxZEditStackedControl*>::iterator iter = mStackControls.begin();
    for (;iter != mStackControls.end(); ++iter)
    {
        if ( (*iter)->GetID() == aControlID)
        {
            (*iter)->UpdateControlFromValue();
        }

    }

}

///////////////////////////////////////////////////////////////////////////////////////////////////
