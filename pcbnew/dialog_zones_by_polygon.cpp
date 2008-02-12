/////////////////////////////////////////////////////////////////////////////

// Name:        zones.cpp
// Purpose:
// Author:      jean-pierre Charras
// Modified by:
// Created:     25/01/2006 11:35:19
// RCS-ID:
// Copyright:   GNU License
// Licence:     GNU License
/////////////////////////////////////////////////////////////////////////////

// Generated by DialogBlocks (unregistered), 25/01/2006 11:35:19

#if defined (__GNUG__) && !defined (NO_GCC_PRAGMA)
#pragma implementation "zones.h"
#endif

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

////@begin includes
////@end includes

#include "dialog_zones_by_polygon.h"

////@begin XPM images
////@end XPM images

/*!
 * WinEDA_ZoneFrame type definition
 */

IMPLEMENT_DYNAMIC_CLASS( WinEDA_ZoneFrame, wxDialog )

/*!
 * WinEDA_ZoneFrame event table definition
 */

BEGIN_EVENT_TABLE( WinEDA_ZoneFrame, wxDialog )

////@begin WinEDA_ZoneFrame event table entries
    EVT_BUTTON( wxID_OK, WinEDA_ZoneFrame::OnOkClick )

    EVT_BUTTON( wxID_CANCEL, WinEDA_ZoneFrame::OnCancelClick )

    EVT_RADIOBOX( ID_NET_SORTING_OPTION, WinEDA_ZoneFrame::OnNetSortingOptionSelected )

////@end WinEDA_ZoneFrame event table entries

END_EVENT_TABLE()

/*!
 * WinEDA_ZoneFrame constructors
 */

WinEDA_ZoneFrame::WinEDA_ZoneFrame()
{
}


WinEDA_ZoneFrame::WinEDA_ZoneFrame( WinEDA_PcbFrame* parent,
									ZONE_CONTAINER * zone_container,
                                    wxWindowID       id,
                                    const wxString&  caption,
                                    const wxPoint&   pos,
                                    const wxSize&    size,
                                    long             style )
{
    m_Parent = parent;
	m_Zone_Container = zone_container;
	if( m_Parent->m_Parent->m_EDA_Config )
    {
        m_NetSorting = m_Parent->m_Parent->m_EDA_Config->Read( ZONE_NET_SORT_OPTION_KEY, (long) BOARD::PAD_CNT_SORT );
	}

    Create( parent, id, caption, pos, size, style );
	SetReturnCode(ZONE_ABORT);	// Will be changed on buttons click
}


/*!
 * WinEDA_ZoneFrame creator
 */

bool WinEDA_ZoneFrame::Create( wxWindow*       parent,
                               wxWindowID      id,
                               const wxString& caption,
                               const wxPoint&  pos,
                               const wxSize&   size,
                               long            style )
{
////@begin WinEDA_ZoneFrame member initialisation
    m_GridCtrl = NULL;
    m_ClearanceValueTitle = NULL;
    m_ZoneClearanceCtrl = NULL;
    m_OutlineAppearanceCtrl = NULL;
    m_FillOpt = NULL;
    m_OrientEdgesOpt = NULL;
    m_NetSortingOption = NULL;
    m_NetNameFilter = NULL;
    m_ListNetNameSelection = NULL;
    m_LayerSelectionCtrl = NULL;
////@end WinEDA_ZoneFrame member initialisation
	
////@begin WinEDA_ZoneFrame creation
    SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
    wxDialog::Create( parent, id, caption, pos, size, style );

    CreateControls();
    if (GetSizer())
    {
        GetSizer()->SetSizeHints(this);
    }
    Centre();
////@end WinEDA_ZoneFrame creation

    return true;
}


/*!
 * Control creation for WinEDA_ZoneFrame
 */

void WinEDA_ZoneFrame::CreateControls()
{
    SetFont( *g_DialogFont );

////@begin WinEDA_ZoneFrame content construction
    // Generated by DialogBlocks, 24/01/2008 11:39:58 (unregistered)

    WinEDA_ZoneFrame* itemDialog1 = this;

    wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
    itemDialog1->SetSizer(itemBoxSizer2);

    wxBoxSizer* itemBoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
    itemBoxSizer2->Add(itemBoxSizer3, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

    wxBoxSizer* itemBoxSizer4 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer3->Add(itemBoxSizer4, 0, wxGROW|wxALL, 5);

    wxArrayString m_GridCtrlStrings;
    m_GridCtrlStrings.Add(_("0.00000"));
    m_GridCtrlStrings.Add(_("0.00000"));
    m_GridCtrlStrings.Add(_("0.00000"));
    m_GridCtrlStrings.Add(_("0.00000"));
    m_GridCtrl = new wxRadioBox( itemDialog1, ID_RADIOBOX_GRID_SELECTION, _("Grid Size for Filling:"), wxDefaultPosition, wxDefaultSize, m_GridCtrlStrings, 1, wxRA_SPECIFY_COLS );
    m_GridCtrl->SetSelection(0);
    itemBoxSizer4->Add(m_GridCtrl, 0, wxGROW|wxALL, 5);

    m_ClearanceValueTitle = new wxStaticText( itemDialog1, wxID_STATIC, _("Zone clearance value (mm):"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer4->Add(m_ClearanceValueTitle, 0, wxALIGN_LEFT|wxLEFT|wxRIGHT|wxTOP|wxADJUST_MINSIZE, 5);

    m_ZoneClearanceCtrl = new wxTextCtrl( itemDialog1, ID_TEXTCTRL1, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer4->Add(m_ZoneClearanceCtrl, 0, wxGROW|wxLEFT|wxRIGHT|wxBOTTOM, 5);

    wxArrayString m_OutlineAppearanceCtrlStrings;
    m_OutlineAppearanceCtrlStrings.Add(_("Line"));
    m_OutlineAppearanceCtrlStrings.Add(_("Hatched Outline"));
    m_OutlineAppearanceCtrlStrings.Add(_("Full Hatched"));
    m_OutlineAppearanceCtrl = new wxRadioBox( itemDialog1, ID_RADIOBOX_OUTLINES_OPTION, _("Outlines Appearance"), wxDefaultPosition, wxDefaultSize, m_OutlineAppearanceCtrlStrings, 1, wxRA_SPECIFY_COLS );
    m_OutlineAppearanceCtrl->SetSelection(0);
    itemBoxSizer4->Add(m_OutlineAppearanceCtrl, 0, wxGROW|wxALL, 5);

    itemBoxSizer3->Add(5, 5, 0, wxGROW|wxALL, 5);

    wxBoxSizer* itemBoxSizer10 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer3->Add(itemBoxSizer10, 0, wxGROW|wxALL, 5);

    wxArrayString m_FillOptStrings;
    m_FillOptStrings.Add(_("Include Pads"));
    m_FillOptStrings.Add(_("Thermal"));
    m_FillOptStrings.Add(_("Exclude Pads"));
    m_FillOpt = new wxRadioBox( itemDialog1, ID_RADIOBOX4, _("Pad options:"), wxDefaultPosition, wxDefaultSize, m_FillOptStrings, 1, wxRA_SPECIFY_COLS );
    m_FillOpt->SetSelection(0);
    itemBoxSizer10->Add(m_FillOpt, 0, wxGROW|wxALL, 5);

    itemBoxSizer10->Add(5, 5, 1, wxGROW|wxALL, 5);

    wxArrayString m_OrientEdgesOptStrings;
    m_OrientEdgesOptStrings.Add(_("Any"));
    m_OrientEdgesOptStrings.Add(_("H , V and 45 deg"));
    m_OrientEdgesOpt = new wxRadioBox( itemDialog1, ID_RADIOBOX5, _("Zone edges orient:"), wxDefaultPosition, wxDefaultSize, m_OrientEdgesOptStrings, 1, wxRA_SPECIFY_COLS );
    m_OrientEdgesOpt->SetSelection(0);
    itemBoxSizer10->Add(m_OrientEdgesOpt, 0, wxGROW|wxALL, 5);

    itemBoxSizer3->Add(5, 5, 0, wxGROW|wxALL, 5);

    wxBoxSizer* itemBoxSizer15 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer3->Add(itemBoxSizer15, 0, wxGROW|wxALL, 5);

    wxButton* itemButton16 = new wxButton( itemDialog1, wxID_OK, _("OK"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer15->Add(itemButton16, 0, wxGROW|wxALL, 5);

    wxButton* itemButton17 = new wxButton( itemDialog1, wxID_CANCEL, _("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
    itemButton17->SetForegroundColour(wxColour(0, 0, 255));
    itemBoxSizer15->Add(itemButton17, 0, wxGROW|wxALL, 5);

    itemBoxSizer15->Add(5, 5, 1, wxGROW|wxALL, 5);

    wxArrayString m_NetSortingOptionStrings;
    m_NetSortingOptionStrings.Add(_("Alphabetic"));
    m_NetSortingOptionStrings.Add(_("Advanced"));
    m_NetSortingOption = new wxRadioBox( itemDialog1, ID_NET_SORTING_OPTION, _("Net sorting:"), wxDefaultPosition, wxDefaultSize, m_NetSortingOptionStrings, 1, wxRA_SPECIFY_COLS );
    m_NetSortingOption->SetSelection(0);
    itemBoxSizer15->Add(m_NetSortingOption, 0, wxGROW|wxALL, 5);

    wxStaticText* itemStaticText20 = new wxStaticText( itemDialog1, wxID_STATIC, _("Filter"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer15->Add(itemStaticText20, 0, wxALIGN_LEFT|wxLEFT|wxRIGHT, 5);

    m_NetNameFilter = new wxTextCtrl( itemDialog1, ID_TEXTCTRL_NETNAMES_FILTER, _T(""), wxDefaultPosition, wxDefaultSize, 0 );
    if (WinEDA_ZoneFrame::ShowToolTips())
        m_NetNameFilter->SetToolTip(_("Do not list net names which match with this text, in advanced mode"));
    itemBoxSizer15->Add(m_NetNameFilter, 0, wxGROW|wxLEFT|wxRIGHT|wxBOTTOM, 5);

    wxBoxSizer* itemBoxSizer22 = new wxBoxSizer(wxVERTICAL);
    itemBoxSizer2->Add(itemBoxSizer22, 0, wxGROW|wxALL, 5);

    wxStaticText* itemStaticText23 = new wxStaticText( itemDialog1, wxID_STATIC, _("Net:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer22->Add(itemStaticText23, 0, wxALIGN_LEFT|wxLEFT|wxRIGHT|wxTOP, 5);

    wxArrayString m_ListNetNameSelectionStrings;
    m_ListNetNameSelection = new wxListBox( itemDialog1, ID_NETNAME_SELECTION, wxDefaultPosition, wxDefaultSize, m_ListNetNameSelectionStrings, wxLB_SINGLE|wxSUNKEN_BORDER );
    itemBoxSizer22->Add(m_ListNetNameSelection, 0, wxGROW|wxLEFT|wxRIGHT|wxBOTTOM, 5);

    wxStaticText* itemStaticText25 = new wxStaticText( itemDialog1, wxID_LAYER_SELECTION, _("Layer:"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer22->Add(itemStaticText25, 0, wxALIGN_LEFT|wxLEFT|wxRIGHT|wxTOP, 5);

    wxArrayString m_LayerSelectionCtrlStrings;
    m_LayerSelectionCtrl = new wxListBox( itemDialog1, ID_LAYER_CHOICE, wxDefaultPosition, wxDefaultSize, m_LayerSelectionCtrlStrings, wxLB_SINGLE );
    itemBoxSizer22->Add(m_LayerSelectionCtrl, 0, wxGROW|wxALL, 5);

    // Set validators
    m_NetSortingOption->SetValidator( wxGenericValidator(& m_NetSorting) );
////@end WinEDA_ZoneFrame content construction

	// Initialise options
	wxString title = _( "Zone clearance value:" ) + ReturnUnitSymbol( g_UnitMetric );
    m_ClearanceValueTitle->SetLabel( title );

    title = _( "Grid :" ) + ReturnUnitSymbol( g_UnitMetric );;
    m_GridCtrl->SetLabel( title );

    if( g_DesignSettings.m_ZoneClearence == 0 )
        g_DesignSettings.m_ZoneClearence = g_DesignSettings.m_TrackClearence;
    title = ReturnStringFromValue( g_UnitMetric,
                                   g_DesignSettings.m_ZoneClearence,
                                   m_Parent->m_InternalUnits );
    m_ZoneClearanceCtrl->SetValue( title );

    if( Zone_45_Only )
        m_OrientEdgesOpt->SetSelection( 1 );

    static const int GridList[4] = { 25, 50, 100, 250 };
    int selection = 0;

    for( unsigned ii = 0; ii < (unsigned) m_GridCtrl->GetCount(); ii++ )
    {
        wxString msg = ReturnStringFromValue( g_UnitMetric,
                                              GridList[ii],
                                              m_Parent->m_InternalUnits );
        m_GridCtrl->SetString( ii, msg );
        if( g_GridRoutingSize == GridList[ii] )
            selection = ii;
    }

    m_GridCtrl->SetSelection( selection );

    switch( s_Zone_Pad_Options )
    {
        case ZONE_CONTAINER::PAD_NOT_IN_ZONE:		// Pads are not covered
            m_FillOpt->SetSelection( 2 );
			break;
		case ZONE_CONTAINER::THERMAL_PAD:			// Use thermal relief for pads
            m_FillOpt->SetSelection( 1 );
			break;
		case ZONE_CONTAINER::PAD_IN_ZONE:			// pads are covered by copper
            m_FillOpt->SetSelection( 0 );
			break;
    }		

	if ( m_Zone_Container )
		s_Zone_Hatching = m_Zone_Container->m_Poly->GetHatchStyle();
	else
        s_Zone_Hatching = m_Parent->m_Parent->m_EDA_Config->Read( ZONE_NET_OUTLINES_HATCH_OPTION_KEY,
			(long) CPolyLine::DIAGONAL_EDGE );

    switch( s_Zone_Hatching )
    {
		case CPolyLine::NO_HATCH:
			m_OutlineAppearanceCtrl->SetSelection(0);
			break;

		case CPolyLine::DIAGONAL_EDGE:
			m_OutlineAppearanceCtrl->SetSelection(1);
			break;

		case CPolyLine::DIAGONAL_FULL:
		   m_OutlineAppearanceCtrl->SetSelection(2);
			break;
	}

    int           layer_cnt = g_DesignSettings.m_CopperLayerCount;
    for( int ii = 0; ii < g_DesignSettings.m_CopperLayerCount; ii++ )
    {
        wxString msg;
        int      layer_number = COPPER_LAYER_N;
        if( layer_cnt == 0 || ii < layer_cnt - 1 )
            layer_number = ii;
        else  if( ii == layer_cnt - 1 )
            layer_number = LAYER_CMP_N;
        m_LayerId[ii] = layer_number;
        msg = ReturnPcbLayerName( layer_number ).Trim();
        m_LayerSelectionCtrl->InsertItems( 1, &msg, ii );
		if ( m_Zone_Container )
		{
			if( m_Zone_Container->GetLayer() == layer_number )
				m_LayerSelectionCtrl->SetSelection( ii );
		}
		else
		{
			if( ((PCB_SCREEN*)(m_Parent->GetScreen()))->m_Active_Layer == layer_number )
				m_LayerSelectionCtrl->SetSelection( ii );
		}
    }

	m_NetSortingOption->SetSelection(m_NetSorting == BOARD::ALPHA_SORT ? 0 : 1 );
	wxString NetNameFilter;
	if( m_Parent->m_Parent->m_EDA_Config )
    {
	    NetNameFilter = m_Parent->m_Parent->m_EDA_Config->Read( ZONE_NET_FILTER_STRING_KEY, wxT("N_0*") );
	}
	m_NetNameFilter->SetValue(NetNameFilter);
    wxArrayString ListNetName;
    m_Parent->m_Pcb->ReturnSortedNetnamesList( ListNetName,
		m_NetSorting == 0	? BOARD::ALPHA_SORT : BOARD::PAD_CNT_SORT );
	if ( m_NetSorting != 0 )
	{
		wxString Filter  = m_NetNameFilter->GetValue();
		for (unsigned ii = 0; ii < ListNetName.GetCount(); ii ++ )
		{
			if (  ListNetName[ii].Matches(Filter.GetData() ) )
			{
				ListNetName. RemoveAt(ii);
				ii--;
			}
		}
	}
	
    m_ListNetNameSelection->InsertItems( ListNetName, 0 );

    // Select net:
	int net_select = g_HightLigth_NetCode;
	if ( m_Zone_Container )
		net_select = m_Zone_Container->GetNet();
    if( net_select > 0 )
    {
        EQUIPOT* equipot = m_Parent->m_Pcb->FindNet( net_select );
        if( equipot )  // Search net in list and select it
        {
            for( unsigned ii = 0; ii < ListNetName.GetCount(); ii++ )
            {
                if( ListNetName[ii] == equipot->m_Netname )
                {
                    m_ListNetNameSelection->SetSelection( ii );
                    break;
                }
            }
        }
    }
}


/*!
 * Should we show tooltips?
 */

bool WinEDA_ZoneFrame::ShowToolTips()
{
    return true;
}


/*!
 * Get bitmap resources
 */

wxBitmap WinEDA_ZoneFrame::GetBitmapResource( const wxString& name )
{
    // Bitmap retrieval
////@begin WinEDA_ZoneFrame bitmap retrieval
    wxUnusedVar(name);
    return wxNullBitmap;
////@end WinEDA_ZoneFrame bitmap retrieval
}


/*!
 * Get icon resources
 */

wxIcon WinEDA_ZoneFrame::GetIconResource( const wxString& name )
{
    // Icon retrieval
////@begin WinEDA_ZoneFrame icon retrieval
    wxUnusedVar(name);
    return wxNullIcon;
////@end WinEDA_ZoneFrame icon retrieval
}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_CANCEL
 */

void WinEDA_ZoneFrame::OnCancelClick( wxCommandEvent& event )
{
    EndModal( ZONE_ABORT );
}


/**********************************************************/
bool WinEDA_ZoneFrame::AcceptOptions(bool aPromptForErrors)
/**********************************************************/
/** Function WinEDA_ZoneFrame::AcceptOptions(
 * @return false if incorrect options, true if Ok.
 */
{
    switch( m_FillOpt->GetSelection() )
    {
    case 2:
        s_Zone_Pad_Options = ZONE_CONTAINER::PAD_NOT_IN_ZONE;		// Pads are not covered
        break;

    case 1:
        s_Zone_Pad_Options = ZONE_CONTAINER::THERMAL_PAD;			// Use thermal relief for pads
        break;

    case 0:
        s_Zone_Pad_Options = ZONE_CONTAINER::PAD_IN_ZONE;			// pads are covered by copper
        break;
    }

    switch( m_OutlineAppearanceCtrl->GetSelection() )
    {
    case 0:
        s_Zone_Hatching = CPolyLine::NO_HATCH;
        break;

    case 1:
        s_Zone_Hatching = CPolyLine::DIAGONAL_EDGE;
        break;

    case 2:
        s_Zone_Hatching = CPolyLine::DIAGONAL_FULL;
        break;
    }

    if( m_Parent->m_Parent->m_EDA_Config )
    {
		m_Parent->m_Parent->m_EDA_Config->Write( ZONE_NET_OUTLINES_HATCH_OPTION_KEY, (long)s_Zone_Hatching);
	}	

    switch( m_GridCtrl->GetSelection() )
    {
    case 0:
        g_GridRoutingSize = 25;
        break;

    case 1:
        g_GridRoutingSize = 50;
        break;

	default:
    case 2:
        g_GridRoutingSize = 100;
        break;

    case 3:
        g_GridRoutingSize = 250;
        break;
    }

    wxString txtvalue = m_ZoneClearanceCtrl->GetValue();
    g_DesignSettings.m_ZoneClearence =
        ReturnValueFromString( g_UnitMetric, txtvalue, m_Parent->m_InternalUnits );
    if( m_OrientEdgesOpt->GetSelection() == 0 )
        Zone_45_Only = FALSE;
    else
        Zone_45_Only = TRUE;

    /* Get the layer selection for this zone */
    int ii = m_LayerSelectionCtrl->GetSelection();
    if( ii < 0 && aPromptForErrors )
    {
        DisplayError( this, _( "Error : you must choose a layer" ) );
        return false;
    }
    s_Zone_Layer = m_LayerId[ii];

    /* Get the net name selection for this zone */
    ii = m_ListNetNameSelection->GetSelection();
    if( ii < 0 && aPromptForErrors )
    {
        DisplayError( this, _( "Error : you must choose a net name" ) );
        return false;
    }

    wxString net_name = m_ListNetNameSelection->GetString( ii );

    /* Search net_code for this net */
    EQUIPOT* net;
    s_NetcodeSelection = 0;
    for( net = m_Parent->m_Pcb->m_Equipots; net;  net = net->Next() )
    {
        if( net->m_Netname == net_name )
        {
            s_NetcodeSelection = net->GetNet();
            break;
        }
    }
	
	return true;
}


/*!
 * wxEVT_COMMAND_RADIOBOX_SELECTED event handler for ID_NET_SORTING_OPTION
 */

void WinEDA_ZoneFrame::OnNetSortingOptionSelected( wxCommandEvent& event )
{
    wxArrayString ListNetName;
	m_NetSorting = m_NetSortingOption->GetSelection();
    m_Parent->m_Pcb->ReturnSortedNetnamesList( ListNetName,
		m_NetSorting == 0 ? BOARD::ALPHA_SORT : BOARD::PAD_CNT_SORT );
	if ( m_NetSorting != 0 )
	{
		wxString Filter  = m_NetNameFilter->GetValue();
		for (unsigned ii = 0; ii < ListNetName.GetCount(); ii ++ )
		{
			if (  ListNetName[ii].Matches(Filter.GetData() ) )
			{
				ListNetName. RemoveAt(ii);
				ii--;
			}
		}
	}
	m_ListNetNameSelection->Clear();
    m_ListNetNameSelection->InsertItems( ListNetName, 0 );
    if( m_Parent->m_Parent->m_EDA_Config )
    {
        m_Parent->m_Parent->m_EDA_Config->Write( ZONE_NET_SORT_OPTION_KEY, (long) m_NetSorting );
        m_Parent->m_Parent->m_EDA_Config->Write( ZONE_NET_FILTER_STRING_KEY, m_NetNameFilter->GetValue() );
	}	

}


/*!
 * wxEVT_COMMAND_BUTTON_CLICKED event handler for wxID_OK
 */

void WinEDA_ZoneFrame::OnOkClick( wxCommandEvent& event )
{
	if ( AcceptOptions(true) )
		EndModal( ZONE_OK );
}

