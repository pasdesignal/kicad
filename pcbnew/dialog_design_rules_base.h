///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Sep  9 2009)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __dialog_design_rules_base__
#define __dialog_design_rules_base__

#include <wx/intl.h>

#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/font.h>
#include <wx/grid.h>
#include <wx/gdicmn.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/choice.h>
#include <wx/listctrl.h>
#include <wx/html/htmlwin.h>
#include <wx/dialog.h>

///////////////////////////////////////////////////////////////////////////

#define wxID_ADD_NETCLASS 1000
#define wxID_REMOVE_NETCLASS 1001
#define ID_LEFT_CHOICE_CLICK 1002
#define ID_LEFT_TO_RIGHT_COPY 1003
#define ID_RIGHT_TO_LEFT_COPY 1004
#define ID_RIGHT_CHOICE_CLICK 1005

///////////////////////////////////////////////////////////////////////////////
/// Class DIALOG_DESIGN_RULES_BASE
///////////////////////////////////////////////////////////////////////////////
class DIALOG_DESIGN_RULES_BASE : public wxDialog 
{
	private:
	
	protected:
		wxGrid* m_grid;
		wxButton* m_addButton;
		wxButton* m_removeButton;
		wxButton* m_moveUpButton;
		wxChoice* m_leftClassChoice;
		wxListCtrl* m_leftListCtrl;
		wxButton* m_buttonRightToLeft;
		wxButton* m_buttonLeftToRight;
		wxButton* m_buttonLeftSelAll;
		wxButton* m_buttonRightSelAll;
		wxChoice* m_rightClassChoice;
		wxListCtrl* m_rightListCtrl;
		wxHtmlWindow* m_MessagesList;
		wxStdDialogButtonSizer* m_sdbSizer1;
		wxButton* m_sdbSizer1OK;
		wxButton* m_sdbSizer1Cancel;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnNetClassesGridLeftClick( wxGridEvent& event ) { event.Skip(); }
		virtual void OnNetClassesGridRightClick( wxGridEvent& event ) { event.Skip(); }
		virtual void OnAddNetclassClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnRemoveNetclassClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnLeftCBSelection( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnRightToLeftCopyButton( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnLeftToRightCopyButton( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnLeftSelectAllButton( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnRightSelectAllButton( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnRightCBSelection( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnCancelButtonClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnOkButtonClick( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		DIALOG_DESIGN_RULES_BASE( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Design Rules Editor"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 750,520 ), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER );
		~DIALOG_DESIGN_RULES_BASE();
	
};

#endif //__dialog_design_rules_base__
