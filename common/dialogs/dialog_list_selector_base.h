///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct  8 2012)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __DIALOG_LIST_SELECTOR_BASE_H__
#define __DIALOG_LIST_SELECTOR_BASE_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/intl.h>
class DIALOG_SHIM;

#include "dialog_shim.h"
#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/textctrl.h>
#include <wx/listbox.h>
#include <wx/sizer.h>
#include <wx/button.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class EDA_LIST_DIALOG_BASE
///////////////////////////////////////////////////////////////////////////////
class EDA_LIST_DIALOG_BASE : public DIALOG_SHIM
{
	private:
	
	protected:
		wxStaticText* m_filterLabel;
		wxTextCtrl* m_filterBox;
		wxStaticText* m_staticText2;
		wxListBox* m_listBox;
		wxStaticText* m_staticTextMsg;
		wxTextCtrl* m_messages;
		wxStdDialogButtonSizer* m_sdbSizer;
		wxButton* m_sdbSizerOK;
		wxButton* m_sdbSizerCancel;
		
		// Virtual event handlers, overide them in your derived class
		virtual void onClose( wxCloseEvent& event ) { event.Skip(); }
		virtual void textChangeInFilterBox( wxCommandEvent& event ) { event.Skip(); }
		virtual void onClickOnList( wxCommandEvent& event ) { event.Skip(); }
		virtual void onDClickOnList( wxCommandEvent& event ) { event.Skip(); }
		virtual void onCancelClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void onOkClick( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		EDA_LIST_DIALOG_BASE( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 329,398 ), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER ); 
		~EDA_LIST_DIALOG_BASE();
	
};

#endif //__DIALOG_LIST_SELECTOR_BASE_H__
