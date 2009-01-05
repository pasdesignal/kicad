/************************************************/
/* EDITEUR de PCB: AUTOROUTAGE: routines d'init */
/************************************************/

/* Fichier BOARD.CC */

#include "fctsys.h"
#include "gr_basic.h"

#include "common.h"
#include "pcbnew.h"
#include "autorout.h"
#include "zones.h"
#include "cell.h"

#include "protos.h"

/* routines externes : */

/* Routines definies ici: */
int         Build_Work( BOARD* Pcb, CHEVELU* pt_base_chevelu );
void        PlaceCells( BOARD* Pcb, int net_code, int flag );
int         InitBoard();
BoardCell   GetCell( int, int, int );
void        SetCell( int row, int col, int side, BoardCell x );
void        OrCell( int, int, int, BoardCell );
void        AndCell( int, int, int, BoardCell );
void        AddCell( int, int, int, BoardCell );
void        XorCell( int, int, int, BoardCell );
void        AddCell( int, int, int, BoardCell );
DistCell    GetDist( int, int, int );
void        SetDist( int, int, int, DistCell );
int         GetDir( int, int, int );
void        SetDir( int, int, int, int );

/*****************************************************************/
bool ComputeMatriceSize( WinEDA_BasePcbFrame* frame, int g_GridRoutingSize )
/*****************************************************************/

/*
 *  Calcule Nrows et Ncols, dimensions de la matrice de representation du BOARD
 *  pour les routages automatiques et calculs de zone
 */
{
    BOARD* pcb = frame->GetBoard();

    pcb->ComputeBoundaryBox();

    /* The boundary box must have its start point on routing grid: */
    pcb->m_BoundaryBox.m_Pos.x -= pcb->m_BoundaryBox.m_Pos.x % g_GridRoutingSize;
    pcb->m_BoundaryBox.m_Pos.y -= pcb->m_BoundaryBox.m_Pos.y % g_GridRoutingSize;
    /* The boundary box must have its end point on routing grid: */
    wxPoint end = pcb->m_BoundaryBox.GetEnd();
    end.x -= end.x % g_GridRoutingSize; end.x += g_GridRoutingSize;
    end.y -= end.y % g_GridRoutingSize; end.y += g_GridRoutingSize;
    pcb->m_BoundaryBox.SetEnd( end );

    Nrows = pcb->m_BoundaryBox.m_Size.y / g_GridRoutingSize;
    Ncols = pcb->m_BoundaryBox.m_Size.x / g_GridRoutingSize;
    /* get a small margin for memory allocation: */
    Ncols += 2; Nrows += 2;

    return TRUE;
}


/*******************/
/* class BOARDHEAD */
/*******************/

BOARDHEAD::BOARDHEAD()
{
    m_BoardSide[0]  = m_BoardSide[1] = NULL;
    m_DistSide[0]   = m_DistSide[1] = NULL;
    m_DirSide[0]    = m_DirSide[1] = NULL;
    m_InitBoardDone = FALSE;
    m_Layers  = 2;
    m_Nrows   = m_Ncols = 0;
    m_MemSize = 0;
}


BOARDHEAD::~BOARDHEAD()
{
}


/******************************/
int BOARDHEAD::InitBoard()
/*****************************/

/* initialize the data structures
 *  retourne la taille RAM utilisee, ou -1 si defaut
 */
{
    int ii, kk;

    if( Nrows <= 0 || Ncols <= 0 )
        return 0;
    m_Nrows = Nrows;
    m_Ncols = Ncols;

    m_InitBoardDone = TRUE; /* we have been called */

    ii = (Nrows + 1) * (Ncols + 1);

    for( kk = 0; kk < m_Layers; kk++ )
    {
        m_BoardSide[kk] = NULL;
        m_DistSide[kk]  = NULL;
        m_DirSide[kk]   = NULL;

        /* allocate Board & initialize everything to empty */
        m_BoardSide[kk] = (BoardCell*) MyZMalloc( ii * sizeof(BoardCell) );
        if( m_BoardSide[kk] == NULL )
            return -1;

        /***** allocate Distances *****/
        m_DistSide[kk] = (DistCell*) MyZMalloc( ii * sizeof(DistCell) );
        if( m_DistSide[kk] == NULL )
            return -1;

        /***** allocate Dir (chars) *****/
        m_DirSide[kk] = (char*) MyZMalloc( ii );
        if( m_DirSide[kk] == NULL )
            return -1;
    }

    m_MemSize = m_Layers * ii * ( sizeof(BoardCell) + sizeof(DistCell) + sizeof(char) );

    return m_MemSize;
}


/*********************************/
void BOARDHEAD::UnInitBoard()
/*********************************/
/* deallocation de la memoire */
{
    int ii;

    m_InitBoardDone = FALSE;

    for( ii = 0; ii < 2; ii++ )
    {
        /***** de-allocate Dir (chars) *****/
        if( m_DirSide[ii] )
        {
            MyFree( m_DirSide[ii] ); m_DirSide[ii] = NULL;
        }

        /***** de-allocate Distances *****/
        if( m_DistSide[ii] )
        {
            MyFree( m_DistSide[ii] ); m_DistSide[ii] = NULL;
        }

        /**** de-allocate Board *****/
        if( m_BoardSide[ii] )
        {
            MyFree( m_BoardSide[ii] ); m_BoardSide[ii] = NULL;
        }
    }

    m_Nrows = m_Ncols = 0;
}


/*****************************************************/
void PlaceCells( BOARD* aPcb, int net_code, int flag )
/*****************************************************/

/* Initialise les cellules du board a la valeur HOLE et VIA_IMPOSSIBLE
 *  selon les marges d'isolement
 *  les elements de net_code = net_code ne seront pas places comme occupe
 *  mais en VIA_IMPOSSIBLE uniquement
 *  Pour Routage 1 seule face:
 *      le plan BOTTOM est utilise
 *      et Route_Layer_BOTTOM = Route_Layer_TOP
 *
 *  Selon les bits = 1 du parametre flag:
 *      si FORCE_PADS : tous les pads seront places meme ceux de meme net_code
 */
{
    int             ux0 = 0, uy0 = 0, ux1, uy1, dx, dy;
    int             marge, via_marge;
    int             masque_layer;

    marge     = g_DesignSettings.m_TrackClearence + (g_DesignSettings.m_CurrentTrackWidth / 2);
    via_marge = g_DesignSettings.m_TrackClearence + (g_DesignSettings.m_CurrentViaSize / 2);

    /////////////////////////////////////
    // Placement des PADS sur le board //
    /////////////////////////////////////

    for( unsigned i=0;  i<aPcb->m_Pads.size(); ++i )
    {
        D_PAD* pad = aPcb->m_Pads[i];

        if( net_code != pad->GetNet() || (flag & FORCE_PADS) )
        {
            Place_1_Pad_Board( aPcb, pad, HOLE, marge, WRITE_CELL );
        }
        Place_1_Pad_Board( aPcb, pad, VIA_IMPOSSIBLE, via_marge, WRITE_OR_CELL );
    }

    ///////////////////////////////////////////////
    // Placement des elements de modules sur PCB //
    ///////////////////////////////////////////////
    for( MODULE* module = aPcb->m_Modules;  module;  module = module->Next() )
    {
        for( BOARD_ITEM* item = module->m_Drawings;  item;  item = item->Next() )
        {
            switch( item->Type() )
            {
            case TYPE_EDGE_MODULE:
                {
                    EDGE_MODULE* edge = (EDGE_MODULE*) item;

                    TRACK* TmpSegm = new TRACK( NULL );

                    TmpSegm->SetLayer( edge->GetLayer() );
                    if( TmpSegm->GetLayer() == EDGE_N )
                        TmpSegm->SetLayer( -1 );

                    TmpSegm->m_Start   = edge->m_Start;
                    TmpSegm->m_End     = edge->m_End;
                    TmpSegm->m_Shape   = edge->m_Shape;
                    TmpSegm->m_Width   = edge->m_Width;
                    TmpSegm->m_Param   = edge->m_Angle;
                    TmpSegm->SetNet( -1 );

                    TraceSegmentPcb( aPcb, TmpSegm, HOLE, marge, WRITE_CELL );
                    TraceSegmentPcb( aPcb, TmpSegm, VIA_IMPOSSIBLE, via_marge,
                                     WRITE_OR_CELL );
                    delete TmpSegm;
                }
                break;

            default:
                break;
            }
        }
    }

    ////////////////////////////////////////////
    // Placement des contours et segments PCB //
    ////////////////////////////////////////////
    for( BOARD_ITEM* item = aPcb->m_Drawings;  item;  item = item->Next() )
    {
        switch( item->Type() )
        {
        case TYPE_DRAWSEGMENT:
            {
                DRAWSEGMENT*    DrawSegm;

                int    type_cell = HOLE;
                TRACK* TmpSegm   = new TRACK( NULL );
                DrawSegm = (DRAWSEGMENT*) item;
                TmpSegm->SetLayer( DrawSegm->GetLayer() );
                if( DrawSegm->GetLayer() == EDGE_N )
                {
                    TmpSegm->SetLayer( -1 );
                    type_cell |= CELL_is_EDGE;
                }

                TmpSegm->m_Start   = DrawSegm->m_Start;
                TmpSegm->m_End     = DrawSegm->m_End;
                TmpSegm->m_Shape   = DrawSegm->m_Shape;
                TmpSegm->m_Width   = DrawSegm->m_Width;
                TmpSegm->m_Param   = DrawSegm->m_Angle;
                TmpSegm->SetNet( -1 );

                TraceSegmentPcb( aPcb, TmpSegm, type_cell, marge, WRITE_CELL );

    //				TraceSegmentPcb(Pcb, TmpSegm, VIA_IMPOSSIBLE, via_marge,WRITE_OR_CELL );
                delete TmpSegm;
            }
            break;

        case TYPE_TEXTE:
            TEXTE_PCB*      PtText;
            PtText = (TEXTE_PCB*) item;

            if( PtText->GetLength() == 0 )
                break;

            ux0 = PtText->m_Pos.x; uy0 = PtText->m_Pos.y;

            dx = PtText->Pitch() * PtText->GetLength();
            dy = PtText->m_Size.y + PtText->m_Width;

            /* Put bounding box (rectangle) on matrix */
            dx /= 2;
            dy /= 2;    /* dx et dy = demi dimensionx X et Y */

            ux1 = ux0 + dx;
            uy1 = uy0 + dy;

            ux0 -= dx;
            uy0 -= dy;

            masque_layer = g_TabOneLayerMask[PtText->GetLayer()];

            TraceFilledRectangle( aPcb, ux0 - marge, uy0 - marge, ux1 + marge, uy1 + marge,
                                  (int) (PtText->m_Orient),
                                  masque_layer, HOLE, WRITE_CELL );

            TraceFilledRectangle( aPcb, ux0 - via_marge, uy0 - via_marge,
                                  ux1 + via_marge, uy1 + via_marge,
                                  (int) (PtText->m_Orient),
                                  masque_layer, VIA_IMPOSSIBLE, WRITE_OR_CELL );
            break;

        default:
            break;
        }
    }

    /* Put tracks and vias on matrix */
    for( TRACK* track = aPcb->m_Track;  track;  track = track->Next() )
    {
        if( net_code == track->GetNet() )
            continue;

        TraceSegmentPcb( aPcb, track, HOLE, marge, WRITE_CELL );
        TraceSegmentPcb( aPcb, track, VIA_IMPOSSIBLE, via_marge, WRITE_OR_CELL );
    }

    // Put zone filling on matrix
    for( SEGZONE* zone = aPcb->m_Zone;  zone;  zone = zone->Next() )
    {
        if( net_code == zone->GetNet() )
            continue;

        TraceSegmentPcb( aPcb, zone, HOLE, marge, WRITE_CELL );
        TraceSegmentPcb( aPcb, zone, VIA_IMPOSSIBLE, via_marge, WRITE_OR_CELL );
    }
}


/******************************************************/
int Build_Work( BOARD* Pcb, CHEVELU* pt_base_chevelu )
/*****************************************************/
/* Build liste conn */
{
    int      ii;
    CHEVELU* pt_rats = pt_base_chevelu;
    D_PAD*   pt_pad;
    int      r1, r2, c1, c2, current_net_code;
    CHEVELU* pt_ch;
    int      demi_pas = g_GridRoutingSize / 2;
    wxString msg;

    InitWork(); /* clear work list */
    Ntotal = 0;
    for( ii = Pcb->GetNumRatsnests(); ii > 0; ii--, pt_rats++ )
    {
        /* On ne route que les chevelus actifs et routables */
        if( (pt_rats->status & CH_ACTIF) == 0 )
            continue;
        if( pt_rats->status & CH_UNROUTABLE )
            continue;
        if( (pt_rats->status & CH_ROUTE_REQ) == 0 )
            continue;
        pt_pad = pt_rats->pad_start;

        current_net_code = pt_pad->GetNet();
        pt_ch = pt_rats;

        r1 = (pt_pad->GetPosition().y - Pcb->m_BoundaryBox.m_Pos.y + demi_pas ) / g_GridRoutingSize;
        if( r1 < 0 || r1 >= Nrows )
        {
            msg.Printf( wxT( "erreur : row = %d ( padY %d pcbY %d) " ), r1,
                        pt_pad->GetPosition().y, Pcb->m_BoundaryBox.m_Pos.y );
            DisplayError( NULL, msg );
            return 0;
        }
        c1 = (pt_pad->GetPosition().x - Pcb->m_BoundaryBox.m_Pos.x + demi_pas ) / g_GridRoutingSize;
        if( c1 < 0 || c1 >= Ncols )
        {
            msg.Printf( wxT( "erreur : col = %d ( padX %d pcbX %d) " ), c1,
                        pt_pad->GetPosition().x, Pcb->m_BoundaryBox.m_Pos.x );
            DisplayError( NULL, msg );
            return 0;
        }

        pt_pad = pt_rats->pad_end;

        r2 = (pt_pad->GetPosition().y - Pcb->m_BoundaryBox.m_Pos.y + demi_pas ) / g_GridRoutingSize;
        if( r2 < 0 || r2 >= Nrows )
        {
            msg.Printf( wxT( "erreur : row = %d ( padY %d pcbY %d) " ), r2,
                        pt_pad->GetPosition().y, Pcb->m_BoundaryBox.m_Pos.y );
            DisplayError( NULL, msg );
            return 0;
        }
        c2 = (pt_pad->GetPosition().x - Pcb->m_BoundaryBox.m_Pos.x + demi_pas ) / g_GridRoutingSize;
        if( c2 < 0 || c2 >= Ncols )
        {
            msg.Printf( wxT( "erreur : col = %d ( padX %d pcbX %d) " ), c2,
                        pt_pad->GetPosition().x, Pcb->m_BoundaryBox.m_Pos.x );
            DisplayError( NULL, msg );
            return 0;
        }

        SetWork( r1, c1, current_net_code, r2, c2, pt_ch, 0 ); Ntotal++;
    }

    SortWork();
    return Ntotal;
}


/*******************************************/
BoardCell GetCell( int row, int col, int side )
/*******************************************/

/* fetch board cell :
 */
{
    BoardCell* p;

    p = Board.m_BoardSide[side];
    return p[row * Ncols + col];
}


/************************************************/
/* void SetCell(int r,int c,int s,BoardCell x ) */
/************************************************/

/* store board cell */
void SetCell( int row, int col, int side, BoardCell x )
{
    BoardCell* p;

    p = Board.m_BoardSide[side];
    p[row * Ncols + col] = x;
}


/******************************************/
/* void OrCell(int r,int c,int s,BoardCell x ) */
/******************************************/

void OrCell( int r, int c, int s, BoardCell x )
{
    BoardCell* p;

    p = Board.m_BoardSide[s];
    p[r * Ncols + c] |= x;
}


/******************************************/
/* void XorCell(int r,int c,int s,BoardCell x ) */
/******************************************/

void XorCell( int r, int c, int s, BoardCell x )
{
    BoardCell* p;

    p = Board.m_BoardSide[s];
    p[r * Ncols + c] ^= x;
}


/************************************************/
/* void AndCell(int r,int c,int s,BoardCell x ) */
/************************************************/

void AndCell( int r, int c, int s, BoardCell x )
{
    BoardCell* p;

    p = Board.m_BoardSide[s];
    p[r * Ncols + c] &= x;
}


/************************************************/
/* void AddCell(int r,int c,int s,BoardCell x ) */
/************************************************/

void AddCell( int r, int c, int s, BoardCell x )
{
    BoardCell* p;

    p = Board.m_BoardSide[s];
    p[r * Ncols + c] += x;
}


/****************************************/
/* DistCell GetDist(int r,int c,int s ) */
/****************************************/

/* fetch distance cell */
DistCell GetDist( int r, int c, int s ) /* fetch distance cell */
{
    DistCell* p;

    p = Board.m_DistSide[s];
    return p[r * Ncols + c];
}


/***********************************************/
/* void SetDist(int r,int c,int s,DistCell x ) */
/***********************************************/

/* store distance cell */
void SetDist( int r, int c, int s, DistCell x )
{
    DistCell* p;

    p = Board.m_DistSide[s];
    p[r * Ncols + c] = x;
}


/**********************************/
/* int GetDir(int r,int c,int s ) */
/**********************************/

/* fetch direction cell */
int GetDir( int r, int c, int s )
{
    char* p;

    p = Board.m_DirSide[s];
    return (int) (p[r * Ncols + c]);
}


/*****************************************/
/* void SetDir(int r,int c,int s,int x ) */
/*****************************************/

/* store direction cell */
void SetDir( int r, int c, int s, int x )
{
    char* p;

    p = Board.m_DirSide[s];
    p[r * Ncols + c] = (char) x;
}
