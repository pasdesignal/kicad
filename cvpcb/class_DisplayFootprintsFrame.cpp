/**********************/
/** displayframe.cpp **/
/**********************/

#include "fctsys.h"
#include "appl_wxstruct.h"
#include "common.h"
#include "class_drawpanel.h"
#include "confirm.h"
#include "macros.h"

#include "cvpcb.h"
#include "bitmaps.h"
#include "protos.h"
#include "cvstruct.h"
#include "class_DisplayFootprintsFrame.h"

/*
 * NOTE: There is something in 3d_viewer.h that causes a compiler error in
 *       <boost/foreach.hpp> in Linux so move it after cvpcb.h where it is
 *       included to prevent the error from occurring.
 */
#include "3d_viewer.h"



BEGIN_EVENT_TABLE( DISPLAY_FOOTPRINTS_FRAME, WinEDA_BasePcbFrame )
    EVT_CLOSE( DISPLAY_FOOTPRINTS_FRAME::OnCloseWindow )
    EVT_SIZE( DISPLAY_FOOTPRINTS_FRAME::OnSize )
    EVT_TOOL_RANGE( ID_ZOOM_IN, ID_ZOOM_PAGE, DISPLAY_FOOTPRINTS_FRAME::OnZoom )
    EVT_TOOL( ID_OPTIONS_SETUP, DISPLAY_FOOTPRINTS_FRAME::InstallOptionsDisplay )
    EVT_TOOL( ID_CVPCB_SHOW3D_FRAME, DISPLAY_FOOTPRINTS_FRAME::Show3D_Frame )
END_EVENT_TABLE()


/***************************************************************************/
/* DISPLAY_FOOTPRINTS_FRAME: the frame to display the current focused footprint */
/***************************************************************************/

DISPLAY_FOOTPRINTS_FRAME::DISPLAY_FOOTPRINTS_FRAME( WinEDA_CvpcbFrame* father,
                                          const wxString& title,
                                          const wxPoint& pos,
                                          const wxSize& size, long style ) :
    WinEDA_BasePcbFrame( father, CVPCB_DISPLAY_FRAME, title, pos,
                         size, style )
{
    m_FrameName = wxT( "CmpFrame" );

    // Give an icon
#ifdef __WINDOWS__
    SetIcon( wxICON( a_icon_cvpcb ) );
#else
    SetIcon( wxICON( icon_cvpcb ) );
#endif
    SetTitle( title );

    SetBoard( new BOARD( NULL, this ) );
    SetBaseScreen( new PCB_SCREEN() );

    LoadSettings();
    // Internalize grid id to a default value if not found in config or bad:
    if( (m_LastGridSizeId <= 0) ||
        (m_LastGridSizeId > (ID_POPUP_GRID_USER - ID_POPUP_GRID_LEVEL_1000)) )
        m_LastGridSizeId = ID_POPUP_GRID_LEVEL_500 - ID_POPUP_GRID_LEVEL_1000;
    GetScreen()->SetGrid( ID_POPUP_GRID_LEVEL_1000 + m_LastGridSizeId );

    // Initialize some display options
    DisplayOpt.DisplayPadIsol = false;      // Pad clearance has no meaning here
    DisplayOpt.ShowTrackClearanceMode = 0;  /* tracks and vias clearance has
                                             * no meaning here. */


    SetSize( m_FramePos.x, m_FramePos.y, m_FrameSize.x, m_FrameSize.y );
    ReCreateHToolbar();
    ReCreateVToolbar();

    m_auimgr.SetManagedWindow( this );

    wxAuiPaneInfo horiz;
    horiz.Gripper( false );
    horiz.DockFixed( true );
    horiz.Movable( false );
    horiz.Floatable( false );
    horiz.CloseButton( false );
    horiz.CaptionVisible( false );

    wxAuiPaneInfo vert( horiz );

    vert.TopDockable( false ).BottomDockable( false );
    horiz.LeftDockable( false ).RightDockable( false );

    m_auimgr.AddPane( m_HToolBar,
                      wxAuiPaneInfo( horiz ).Name( wxT( "m_HToolBar" ) ).Top().
                     Row( 0 ) );

    if( m_VToolBar )    // Currently, no vertical right toolbar.
        m_auimgr.AddPane( m_VToolBar,
                      wxAuiPaneInfo( vert ).Name( wxT( "m_VToolBar" ) ).Right() );

    m_auimgr.AddPane( DrawPanel,
                      wxAuiPaneInfo().Name( wxT( "DrawFrame" ) ).CentrePane() );

    m_auimgr.AddPane( MsgPanel,
                      wxAuiPaneInfo( horiz ).Name( wxT( "MsgPanel" ) ).Bottom() );

    m_auimgr.Update();

    Show( TRUE );
}


DISPLAY_FOOTPRINTS_FRAME::~DISPLAY_FOOTPRINTS_FRAME()
{
    delete GetBoard();

    delete GetBaseScreen();
    SetBaseScreen( 0 );

    ( (WinEDA_CvpcbFrame*) wxGetApp().GetTopWindow() )->DrawFrame = NULL;
}

/* Called when the frame is closed
 *  Save current settings (frame position and size
 */
void DISPLAY_FOOTPRINTS_FRAME::OnCloseWindow( wxCloseEvent& event )
{
    wxPoint pos;
    wxSize  size;

    size = GetSize();
    pos  = GetPosition();

    SaveSettings();
    Destroy();
}


void DISPLAY_FOOTPRINTS_FRAME::ReCreateVToolbar()
{
    // Currently, no vertical right toolbar.
    // So do nothing
}


void DISPLAY_FOOTPRINTS_FRAME::ReCreateHToolbar()
{
    if( m_HToolBar != NULL )
        return;

    m_HToolBar = new WinEDA_Toolbar( TOOLBAR_MAIN, this, ID_H_TOOLBAR, TRUE );

    SetToolBar( (wxToolBar*) m_HToolBar );

    m_HToolBar->AddTool( ID_OPTIONS_SETUP, wxEmptyString,
                         wxBitmap( display_options_xpm ),
                         _( "Display Options" ) );

    m_HToolBar->AddSeparator();

    m_HToolBar->AddTool( ID_ZOOM_IN, wxEmptyString,
                         wxBitmap( zoom_in_xpm ),
                         _( "Zoom in (F1)" ) );

    m_HToolBar->AddTool( ID_ZOOM_OUT, wxEmptyString,
                         wxBitmap( zoom_out_xpm ),
                         _( "Zoom out (F2)" ) );

    m_HToolBar->AddTool( ID_ZOOM_REDRAW, wxEmptyString,
                         wxBitmap( zoom_redraw_xpm ),
                         _( "Redraw view (F3)" ) );

    m_HToolBar->AddTool( ID_ZOOM_PAGE, wxEmptyString,
                         wxBitmap( zoom_auto_xpm ),
                         _( "Zoom auto (Home)" ) );

    m_HToolBar->AddSeparator();
    m_HToolBar->AddTool( ID_CVPCB_SHOW3D_FRAME, wxEmptyString,
                         wxBitmap( show_3d_xpm ),
                         _( "3D Display" ) );

    // after adding the buttons to the toolbar, must call Realize() to reflect
    // the changes
    m_HToolBar->Realize();
}


void DISPLAY_FOOTPRINTS_FRAME::SetToolbars()
{
}


void DISPLAY_FOOTPRINTS_FRAME::OnLeftClick( wxDC* DC, const wxPoint& MousePos )
{
}


void DISPLAY_FOOTPRINTS_FRAME::OnLeftDClick( wxDC* DC, const wxPoint& MousePos )
{
}


bool DISPLAY_FOOTPRINTS_FRAME::OnRightClick( const wxPoint& MousePos,
                                        wxMenu* PopMenu )
{
    return true;
}


void DISPLAY_FOOTPRINTS_FRAME::GeneralControle( wxDC* DC, wxPoint Mouse )
{
    wxRealPoint  delta;
    int     flagcurseur = 0;
    wxPoint curpos, oldpos;
    wxCommandEvent cmd( wxEVT_COMMAND_MENU_SELECTED );
    cmd.SetEventObject( this );

    curpos = DrawPanel->CursorRealPosition( Mouse );
    oldpos = GetScreen()->m_Curseur;

    delta = GetScreen()->GetGridSize();

#ifdef USE_WX_ZOOM
    delta.x = DC->LogicalToDeviceXRel( wxRound( delta.x ) );
    delta.y = DC->LogicalToDeviceYRel( wxRound( delta.y ) );
    Mouse = DrawPanel->CalcUnscrolledPosition( Mouse );
#else
    GetScreen()->Scale( delta );
#endif

    if( delta.x <= 0 )
        delta.x = 1;
    if( delta.y <= 0 )
        delta.y = 1;

    switch( g_KeyPressed )
    {
    case WXK_F1:
        cmd.SetId( ID_POPUP_ZOOM_IN );
        GetEventHandler()->ProcessEvent( cmd );
        flagcurseur = 2;
        curpos = GetScreen()->m_Curseur;
        break;

    case WXK_F2:
        cmd.SetId( ID_POPUP_ZOOM_OUT );
        GetEventHandler()->ProcessEvent( cmd );
        flagcurseur = 2;
        curpos = GetScreen()->m_Curseur;
        break;

    case WXK_F3:
        cmd.SetId( ID_ZOOM_REDRAW );
        GetEventHandler()->ProcessEvent( cmd );
        flagcurseur = 2;
        break;

    case WXK_F4:
        cmd.SetId( ID_POPUP_ZOOM_CENTER );
        GetEventHandler()->ProcessEvent( cmd );
        flagcurseur = 2;
        curpos = GetScreen()->m_Curseur;
        break;

    case WXK_HOME:
        cmd.SetId( ID_ZOOM_PAGE );
        GetEventHandler()->ProcessEvent( cmd );
        flagcurseur = 2;
        curpos = GetScreen()->m_Curseur;
        break;

    case ' ':
        GetScreen()->m_O_Curseur = GetScreen()->m_Curseur;
        break;

    case WXK_NUMPAD8:       /* cursor moved up */
    case WXK_UP:
        Mouse.y -= wxRound(delta.y);
        DrawPanel->MouseTo( Mouse );
        break;

    case WXK_NUMPAD2:       /* cursor moved down */
    case WXK_DOWN:
        Mouse.y += wxRound(delta.y);
        DrawPanel->MouseTo( Mouse );
        break;

    case WXK_NUMPAD4:       /*  cursor moved left */
    case WXK_LEFT:
        Mouse.x -= wxRound(delta.x);
        DrawPanel->MouseTo( Mouse );
        break;

    case WXK_NUMPAD6:      /*  cursor moved right */
    case WXK_RIGHT:
        Mouse.x += wxRound(delta.x);
        DrawPanel->MouseTo( Mouse );
        break;
    }

    GetScreen()->m_Curseur = curpos;
    /* Put cursor on grid */
    PutOnGrid( &GetScreen()->m_Curseur );

    if( GetScreen()->IsRefreshReq() )
    {
        flagcurseur = 2;
        Refresh();
    }

    if( oldpos != GetScreen()->m_Curseur )
    {
        if( flagcurseur != 2 )
        {
            curpos = GetScreen()->m_Curseur;
            GetScreen()->m_Curseur = oldpos;
            DrawPanel->CursorOff( DC );

            GetScreen()->m_Curseur = curpos;
            DrawPanel->CursorOn( DC );
        }

        if( DrawPanel->ManageCurseur )
        {
            DrawPanel->ManageCurseur( DrawPanel, DC, 0 );
        }
    }

    UpdateStatusBar();    /* Display new cursor coordinates */
}


/**
 * Display 3D frame of current footprint selection.
 */
void DISPLAY_FOOTPRINTS_FRAME::Show3D_Frame( wxCommandEvent& event )
{
    if( m_Draw3DFrame )
    {
        DisplayInfoMessage( this, _( "3D Frame already opened" ) );
        return;
    }

    m_Draw3DFrame = new WinEDA3D_DrawFrame( this, _( "3D Viewer" ),
                                            KICAD_DEFAULT_3D_DRAWFRAME_STYLE |
                                            wxFRAME_FLOAT_ON_PARENT );
    m_Draw3DFrame->Show( TRUE );
}

/* Virtual function needed by the PCB_SCREEN class derived from BASE_SCREEN
* this is a virtual pure function in BASE_SCREEN
* do nothing in cvpcb
* could be removed later
*/
void PCB_SCREEN::ClearUndoORRedoList(UNDO_REDO_CONTAINER&, int )
{
}

/** Function IsGridVisible() , virtual
 * @return true if the grid must be shown
 */
bool DISPLAY_FOOTPRINTS_FRAME::IsGridVisible()
{
    return true;
}

/** Function SetGridVisibility() , virtual
 * It may be overloaded by derived classes
 * if you want to store/retrieve the grid visiblity in configuration.
 * @param aVisible = true if the grid must be shown
 */
void DISPLAY_FOOTPRINTS_FRAME::SetGridVisibility(bool aVisible)
{
    // Currently do nothing because there is no option to hide/show grid
}

/** Function GetGridColor() , virtual
 * @return the color of the grid
 */
int DISPLAY_FOOTPRINTS_FRAME::GetGridColor()
{
    return DARKGRAY;
}

