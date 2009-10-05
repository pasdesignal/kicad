/*************************************************************************/
/* listboxes.cpp: class for displaying footprint list and component list */
/*************************************************************************/

#include "fctsys.h"
#include "wxstruct.h"
#include "common.h"

#include "cvpcb.h"
#include "protos.h"
#include "cvstruct.h"


/***************************************/
/* ListBox handling the footprint list */
/***************************************/

FOOTPRINTS_LISTBOX::FOOTPRINTS_LISTBOX( WinEDA_CvpcbFrame* parent,
                                        wxWindowID id, const wxPoint& loc,
                                        const wxSize& size,
                                        int nbitems, wxString choice[] ) :
    ITEMS_LISTBOX_BASE( parent, id, loc, size )
{
    m_UseFootprintFullList = true;
    m_ActiveFootprintList  = NULL;
    SetActiveFootprintList( TRUE );
}


FOOTPRINTS_LISTBOX::~FOOTPRINTS_LISTBOX()
{
}


/*
 * Return number of items
 */
int FOOTPRINTS_LISTBOX::GetCount()
{
    return m_ActiveFootprintList->Count();
}


/*
 * Change an item text
 */
void FOOTPRINTS_LISTBOX::SetString( unsigned linecount, const wxString& text )
{
    if( linecount >= m_ActiveFootprintList->Count() )
        linecount = m_ActiveFootprintList->Count() - 1;
    if( linecount >= 0 )
        (*m_ActiveFootprintList)[linecount] = text;
}


wxString FOOTPRINTS_LISTBOX::GetSelectedFootprint()
{
    wxString FootprintName;
    int      ii = GetFirstSelected();

    if( ii >= 0 )
    {
        wxString msg = (*m_ActiveFootprintList)[ii];
        msg.Trim( TRUE );
        msg.Trim( FALSE );
        FootprintName = msg.AfterFirst( wxChar( ' ' ) );
    }

    return FootprintName;
}


void FOOTPRINTS_LISTBOX::AppendLine( const wxString& text )
{
    m_ActiveFootprintList->Add( text );
    SetItemCount( m_ActiveFootprintList->Count() );
}


/*
 * Overlaid function: MUST be provided in wxLC_VIRTUAL mode
 * because real data is not handled by ITEMS_LISTBOX_BASE
 */
wxString FOOTPRINTS_LISTBOX::OnGetItemText( long item, long column ) const
{
    return m_ActiveFootprintList->Item( item );
}


/*
 * Enable or disable an item
 */
void FOOTPRINTS_LISTBOX::SetSelection( unsigned index, bool State )
{
    if( (int) index >= GetCount() )
        index = GetCount() - 1;

    if( (index >= 0)  && (GetCount() > 0) )
    {
#ifndef __WXMAC__
        Select( index, State );
#endif
        EnsureVisible( index );
#ifdef __WXMAC__
        Refresh();
#endif
    }
}


void FOOTPRINTS_LISTBOX::SetFootprintFullList( FOOTPRINT_LIST& list )
{
    wxString msg;
    int      OldSelection = GetSelection();

    m_FullFootprintList.Clear();

    BOOST_FOREACH( FOOTPRINT & footprint, list ) {
        msg.Printf( wxT( "%3d %s" ), m_FullFootprintList.GetCount() + 1,
                   footprint.m_Module.GetData() );
        m_FullFootprintList.Add( msg );
    }

    SetActiveFootprintList( TRUE );

    if( ( GetCount() == 0 )
       || ( OldSelection < 0 ) || ( OldSelection >= GetCount() ) )
        SetSelection( 0, TRUE );
    Refresh();
}


void FOOTPRINTS_LISTBOX::SetFootprintFilteredList( COMPONENT*      Component,
                                                   FOOTPRINT_LIST& list )
{
    FOOTPRINT_LIST::iterator i;
    wxString msg;
    unsigned jj;
    int      OldSelection = GetSelection();
    bool     HasItem = FALSE;

    m_FilteredFootprintList.Clear();

    BOOST_FOREACH( FOOTPRINT & footprint, list ) {
        /* Search for matching footprints */
        for( jj = 0; jj < Component->m_FootprintFilter.GetCount(); jj++ )
        {
            if( !footprint.m_Module.Matches( Component->m_FootprintFilter[jj] ) )
                continue;
            msg.Printf( wxT( "%3d %s" ), m_FilteredFootprintList.GetCount() + 1,
                       footprint.m_Module.GetData() );
            m_FilteredFootprintList.Add( msg );
            HasItem = TRUE;
        }
    }

    if( HasItem )
        SetActiveFootprintList( FALSE );
    else
        SetActiveFootprintList( TRUE );

    if( ( GetCount() == 0 ) || ( OldSelection >= GetCount() ) )
        SetSelection( 0, TRUE );

    Refresh();
}


/** Set the footprint list. We can have 2 footprint list:
 *  The full footprint list
 *  The filtered footprint list (if the current selected component has a
 * filter for footprints)
 *  @param FullList true = full footprint list, false = filtered footprint list
 *  @param Redraw = true to redraw the window
 */
void FOOTPRINTS_LISTBOX::SetActiveFootprintList( bool FullList, bool Redraw )
{
    bool old_selection = m_UseFootprintFullList;

#ifdef __WINDOWS__

    /* Workaround for a curious bug in wxWidgets:
     * if we switch from a long list of footprints to a short list (a
     * filtered footprint list), and if the selected item is near the end
     * of the long list,  the new list is not displayed from the top of
     * the list box
     */
    if( m_ActiveFootprintList )
    {
        bool new_selection;
        if( FullList )
            new_selection = TRUE;
        else
            new_selection = FALSE;
        if( new_selection != old_selection )
            SetSelection( 0, TRUE );
    }
#endif
    if( FullList )
    {
        m_UseFootprintFullList = TRUE;
        m_ActiveFootprintList  = &m_FullFootprintList;
        SetItemCount( m_FullFootprintList.GetCount() );
    }
    else
    {
        m_UseFootprintFullList = FALSE;
        m_ActiveFootprintList  = &m_FilteredFootprintList;
        SetItemCount( m_FilteredFootprintList.GetCount() );
    }

    if( Redraw )
    {
        if( !m_UseFootprintFullList
           || ( m_UseFootprintFullList != old_selection ) )
        {
            Refresh();
        }
    }

    if( !m_UseFootprintFullList || ( m_UseFootprintFullList != old_selection ) )
    {
        GetParent()->SetStatusText( wxEmptyString, 0 );
        GetParent()->SetStatusText( wxEmptyString, 1 );
    }

    wxString msg;
    if( FullList )
        msg.Printf( _( "Footprints (All): %d" ),
                   m_ActiveFootprintList->GetCount() );
    else
        msg.Printf( _( "Footprints (filtered): %d" ),
                   m_ActiveFootprintList->GetCount() );
    GetParent()->SetStatusText( msg, 2 );
}


/**************************************/
/* Event table for the footprint list */
/**************************************/

BEGIN_EVENT_TABLE( FOOTPRINTS_LISTBOX, ITEMS_LISTBOX_BASE )
EVT_SIZE( ITEMS_LISTBOX_BASE::OnSize )
EVT_CHAR( FOOTPRINTS_LISTBOX::OnChar )
END_EVENT_TABLE()


/********************************************************/
void FOOTPRINTS_LISTBOX::OnLeftClick( wxListEvent& event )
/********************************************************/
{
    FOOTPRINT* Module;
    wxString   FootprintName = GetSelectedFootprint();

    Module = GetModuleDescrByName( FootprintName, GetParent()->m_footprints );
    if( GetParent()->DrawFrame )
    {
        GetParent()->CreateScreenCmp(); /* refresh general */
    }

    if( Module )
    {
        wxString msg;
        msg = Module->m_Doc;
        GetParent()->SetStatusText( msg, 0 );

        msg  = wxT( "KeyW: " );
        msg += Module->m_KeyWord;
        GetParent()->SetStatusText( msg, 1 );
    }
}


/******************************************************/
void FOOTPRINTS_LISTBOX::OnLeftDClick( wxListEvent& event )
/******************************************************/
{
    wxString FootprintName = GetSelectedFootprint();

    GetParent()->SetNewPkg( FootprintName );
}


FOOTPRINT* GetModuleDescrByName( const wxString& FootprintName,
                                 FOOTPRINT_LIST& list )
{
    BOOST_FOREACH( FOOTPRINT & footprint, list ) {
        if( footprint.m_Module == FootprintName )
            return &footprint;
    }

    return NULL;
}


/** function OnChar
 * called on a key pressed
 * Call default handler for some special keys,
 * and for "ascii" keys, select the first footprint
 * that the name starts by the letter.
 * This is the defaut behaviour of a listbox, but because we use
 * virtual lists, the listbox does not know anything to what is displayed,
 * we must handle this behaviour here.
 * Furthermore the footprint name is not at the beginning of
 * displayed lines (the first word is the line number)
 */
void FOOTPRINTS_LISTBOX::OnChar( wxKeyEvent& event )
{
    int key = event.GetKeyCode();
    switch( key )
    {
        case WXK_LEFT:
        case WXK_NUMPAD_LEFT:
            GetParent()->m_ListCmp->SetFocus();
            return;

        case WXK_HOME:
        case WXK_END:
        case WXK_UP:
        case WXK_DOWN:
        case WXK_PAGEUP:
        case WXK_PAGEDOWN:
        case WXK_RIGHT:
        case WXK_NUMPAD_RIGHT:
            event.Skip();
            return;

        default:
            break;
    }
    // Search for an item name starting by the key code:
    key = toupper(key);
    for( unsigned ii = 0; ii < m_ActiveFootprintList->GetCount(); ii++ )
    {
        wxString text = m_ActiveFootprintList->Item(ii);
        /* search for the start char of the footprint name.
         * we must skip the line number
        */
        text.Trim(false);      // Remove leading spaces in line
        unsigned jj = 0;
        for( ; jj < text.Len(); jj++ )
        {   // skip line number
            if( text[jj] == ' ' )
                break;
        }
        for( ; jj < text.Len(); jj++ )
        {   // skip blanks
            if( text[jj] != ' ' )
                break;
        }
        int start_char = toupper(text[jj]);
        if ( key == start_char )
        {
            Focus( ii );
            SetSelection( ii, true );   // Ensure visible
            break;
        }
    }

}