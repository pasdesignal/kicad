/*************************************************************/
/* Edition des Modules: Routines de modification des textes	 */
/*			 sur les MODULES								  */
/*************************************************************/

/* Fichier EDTXTMOD.CPP */

#include "fctsys.h"
#include "gr_basic.h"
#include "common.h"
#include "class_drawpanel.h"

#include "pcbnew.h"
#include "trigo.h"
#include "protos.h"


/* Routines Locales */
static void Show_MoveTexte_Module( WinEDA_DrawPanel* panel, wxDC* DC, bool erase );
static void ExitTextModule( WinEDA_DrawPanel* Panel, wxDC* DC );

/* local variables */
wxPoint MoveVector;                     // Move vector for move edge, exported to dialog_edit mod_text.cpp
static wxPoint CursorInitialPosition;   // Mouse cursor inital position for move command


/******************************************************************************/
TEXTE_MODULE* WinEDA_BasePcbFrame::CreateTextModule( MODULE* Module, wxDC* DC )
/******************************************************************************/

/* Add a new graphical text to the active module (footprint)
 *  Note there always are 2 texts: reference and value.
 *  New texts have the member TEXTE_MODULE.m_Type set to TEXT_is_DIVERS
 */
{
    TEXTE_MODULE* Text;

    Text = new TEXTE_MODULE( Module );

    /* Chainage de la nouvelle structure en tete de liste drawings */
    Module->m_Drawings.PushFront( Text );

    Text->m_Flags = IS_NEW;

    Text->m_Text = wxT( "text" );

    Text->m_Size  = ModuleTextSize;
    Text->m_Width = ModuleTextWidth;
    Text->m_Pos   = GetScreen()->m_Curseur;
    Text->SetLocalCoord();

    InstallTextModOptionsFrame( Text, NULL, wxPoint( -1, -1 ) );

    Text->m_Flags = 0;
    Text->Draw( DrawPanel, DC, GR_OR );

    Text->DisplayInfo( this );

    return Text;
}


/**************************************************************************/
void WinEDA_BasePcbFrame::RotateTextModule( TEXTE_MODULE* Text, wxDC* DC )
/**************************************************************************/
/* Rotation de 90 du texte d'un module */
{
    if( Text == NULL )
        return;

    MODULE* module = (MODULE*) Text->GetParent();

    // we expect MoveVector to be (0,0) if there is no move in progress
    Text->Draw( DrawPanel, DC, GR_XOR, MoveVector );

    Text->m_Orient += 900;
    while( Text->m_Orient >= 1800 )
        Text->m_Orient -= 1800;

    /* Redessin du Texte */
    Text->Draw( DrawPanel, DC, GR_XOR, MoveVector );

    Text->DisplayInfo( this );

    module->m_LastEdit_Time = time( NULL );
    GetScreen()->SetModify();
}


/**************************************************************************/
void WinEDA_BasePcbFrame::DeleteTextModule( TEXTE_MODULE* Text, wxDC* DC )
/**************************************************************************/

/*
 *  Supprime 1 texte sur module (si ce n'est pas la référence ou la valeur)
 */
{
    MODULE* Module;

    if( Text == NULL )
        return;

    Module = (MODULE*) Text->GetParent();

    if( Text->m_Type == TEXT_is_DIVERS )
    {
        // Text->Draw( DrawPanel, DC, GR_XOR );
        DrawPanel->PostDirtyRect( Text->GetBoundingBox() );

        /* liberation de la memoire : */
        Text->DeleteStructure();
        GetScreen()->SetModify();
        Module->m_LastEdit_Time = time( NULL );
    }
}


/*************************************************************/
static void ExitTextModule( WinEDA_DrawPanel* Panel, wxDC* DC )
/*************************************************************/

/*
 *  Routine de sortie du menu edit texte module
 *  Si un texte est selectionne, ses coord initiales sont regenerees
 */
{
    BASE_SCREEN*  screen = Panel->GetScreen();
    TEXTE_MODULE* Text   = (TEXTE_MODULE*) screen->GetCurItem();
    MODULE*       Module;

    Panel->ManageCurseur = NULL;
    Panel->ForceCloseManageCurseur = NULL;

    if( Text == NULL )
        return;

    Module = (MODULE*) Text->GetParent();

    Text->Draw( Panel, DC, GR_XOR, MoveVector );

    /* Redessin du Texte */
    // Text->Draw( Panel, DC, GR_OR );
    Panel->PostDirtyRect( Text->GetBoundingBox() );

    // leave it at (0,0) so we can use it Rotate when not moving.
    MoveVector.x = MoveVector.y = 0;

    Text->m_Flags   = 0;
    Module->m_Flags = 0;

    screen->SetCurItem( NULL );
}


/****************************************************************************/
void WinEDA_BasePcbFrame::StartMoveTexteModule( TEXTE_MODULE* Text, wxDC* DC )
/****************************************************************************/

/* Routine d'initialisation du deplacement d'un texte sur module
 */
{
    MODULE* Module;

    if( Text == NULL )
        return;

    Module = (MODULE*) Text->GetParent();

    Text->m_Flags   |= IS_MOVED;
    Module->m_Flags |= IN_EDIT;

    MoveVector.x = MoveVector.y = 0;

    CursorInitialPosition = Text->m_Pos;

    Text->DisplayInfo( this );

    SetCurItem( Text );
    DrawPanel->ManageCurseur = Show_MoveTexte_Module;
    DrawPanel->ForceCloseManageCurseur = ExitTextModule;

    DrawPanel->ManageCurseur( DrawPanel, DC, TRUE );
}


/*************************************************************************/
void WinEDA_BasePcbFrame::PlaceTexteModule( TEXTE_MODULE* Text, wxDC* DC )
/*************************************************************************/

/* Routine complementaire a StartMoveTexteModule().
 *  Place le texte en cours de deplacement ou nouvellement cree
 */
{
    if( Text != NULL )
    {
        DrawPanel->PostDirtyRect( Text->GetBoundingBox() );

        Text->m_Pos = GetScreen()->m_Curseur;
        /* mise a jour des coordonnées relatives a l'ancre */
        MODULE* Module = (MODULE*) Text->GetParent();
        if( Module )
        {
            int px = Text->m_Pos.x - Module->m_Pos.x;
            int py = Text->m_Pos.y - Module->m_Pos.y;
            RotatePoint( &px, &py, -Module->m_Orient );
            Text->m_Pos0.x  = px;
            Text->m_Pos0.y  = py;
            Text->m_Flags   = 0;
            Module->m_Flags = 0;
            Module->m_LastEdit_Time = time( NULL );
            GetScreen()->SetModify();

            /* Redessin du Texte */
            //Text->Draw( DrawPanel, DC, GR_OR );
            DrawPanel->PostDirtyRect( Text->GetBoundingBox() );
        }
    }

    // leave it at (0,0) so we can use it Rotate when not moving.
    MoveVector.x = MoveVector.y = 0;

    DrawPanel->ManageCurseur = NULL;
    DrawPanel->ForceCloseManageCurseur = NULL;
}


/********************************************************************************/
static void Show_MoveTexte_Module( WinEDA_DrawPanel* panel, wxDC* DC, bool erase )
/********************************************************************************/
{
    BASE_SCREEN*  screen = panel->GetScreen();
    TEXTE_MODULE* Text   = (TEXTE_MODULE*) screen->GetCurItem();
    MODULE*       Module;

    if( Text == NULL )
        return;

    Module = (MODULE*) Text->GetParent();
    /* effacement du texte : */

    if( erase )
        Text->Draw( panel, DC, GR_XOR, MoveVector );

    MoveVector.x = -(screen->m_Curseur.x - CursorInitialPosition.x);
    MoveVector.y = -(screen->m_Curseur.y - CursorInitialPosition.y);

    /* Redessin du Texte */
    Text->Draw( panel, DC, GR_XOR, MoveVector );
}
