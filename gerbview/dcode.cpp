/***************************/
/**** class D_CODE ****/
/***************************/

#include "fctsys.h"
#include "common.h"
#include "class_drawpanel.h"
#include "confirm.h"
#include "macros.h"
#include "trigo.h"

#include "gerbview.h"
#include "class_gerber_draw_item.h"

#define DEFAULT_SIZE 100

/* Format Gerber: NOTES:
 * Tools and D_CODES
 *   tool number (identification of shapes)
 *   1 to 999
 *
 * D_CODES:
 *   D01 ... D9 = action codes:
 *   D01 = activating light (lower pen) when di �� placement
 *   D02 = light extinction (lift pen) when di �� placement
 *   D03 Flash
 *   D09 = VAPE Flash
 *   D10 ... = Indentification Tool (Shape id)
 *
 * For tools:
 * DCode min = D10
 * DCode max = 999
 */


/***************/
/* Class DCODE */
/***************/


D_CODE::D_CODE( int num_dcode )
{
    m_Num_Dcode = num_dcode;
    Clear_D_CODE_Data();
}


D_CODE::~D_CODE()
{
}


void D_CODE::Clear_D_CODE_Data()
{
    m_Size.x     = DEFAULT_SIZE;
    m_Size.y     = DEFAULT_SIZE;
    m_Shape      = APT_CIRCLE;
    m_Drill.x    = m_Drill.y = 0;
    m_DrillShape = 0;
    m_InUse      = FALSE;
    m_Defined    = FALSE;
    m_Macro      = 0;
    m_Rotation   = 0.0;
}


const wxChar* D_CODE::ShowApertureType( APERTURE_T aType )
{
    const wxChar* ret;

    switch( aType )
    {
    case APT_CIRCLE:
        ret = wxT( "Round" );   break;

    case APT_LINE:
        ret = wxT( "Line" );    break;

    case APT_RECT:
        ret = wxT( "Rect" );    break;

    case APT_OVAL:
        ret = wxT( "Oval" );    break;

    case APT_POLYGON:
        ret = wxT( "Poly" );    break;

    case APT_MACRO:
        ret = wxT( "Macro" );   break;

    default:
        ret = wxT( "???" );     break;
    }

    return ret;
}


int WinEDA_GerberFrame::Read_D_Code_File( const wxString& D_Code_FullFileName )
{
    int      current_Dcode, ii, dcode_scale;
    char*    ptcar;
    int      dimH, dimV, drill, dummy;
    float    fdimH, fdimV, fdrill;
    char     c_type_outil[256];
    char     line[GERBER_BUFZ];
    wxString msg;
    D_CODE*  dcode;
    FILE*    dest;
    int      layer = GetScreen()->m_Active_Layer;
    int      type_outil;

    if( g_GERBER_List[layer] == NULL )
    {
        g_GERBER_List[layer] = new GERBER( layer );
    }

    GERBER* gerber = g_GERBER_List[layer];


    /* Updating gerber scale: */
    dcode_scale   = 10; /* By uniting dCode = mil, internal unit = 0.1 mil
                         * -> 1 unite dcode = 10 unit PCB */
    current_Dcode = 0;

    if( D_Code_FullFileName.IsEmpty() )
        return 0;

    dest = wxFopen( D_Code_FullFileName, wxT( "rt" ) );
    if( dest == 0 )
    {
        msg = _( "File " ) + D_Code_FullFileName + _( " not found" );
        DisplayError( this, msg, 10 );
        return -1;
    }

    gerber->InitToolTable();

    while( fgets( line, sizeof(line) - 1, dest ) != NULL )
    {
        if( *line == ';' )
            continue;

        if( strlen( line ) < 10 )
            continue;                       /* Skip blank line. */

        dcode = NULL;
        current_Dcode = 0;

        /* Determine of the type of file from D_Code. */
        ptcar = line;
        ii    = 0;
        while( *ptcar )
            if( *(ptcar++) == ',' )
                ii++;

        if( ii >= 6 )   /* valeurs en mils */
        {
            sscanf( line, "%d,%d,%d,%d,%d,%d,%d", &ii,
                    &dimH, &dimV, &drill,
                    &dummy, &dummy,
                    &type_outil );

            dimH  = (int) ( (dimH * dcode_scale) + 0.5 );
            dimV  = (int) ( (dimV * dcode_scale) + 0.5 );
            drill = (int) ( (drill * dcode_scale) + 0.5 );
            if( ii < 1 )
                ii = 1;
            current_Dcode = ii - 1 + FIRST_DCODE;
        }
        else        /* Values in inches are converted to mils. */
        {
            fdrill = 0;
            current_Dcode = 0;

            sscanf( line, "%f,%f,%1s", &fdimV, &fdimH, c_type_outil );
            ptcar = line;
            while( *ptcar )
            {
                if( *ptcar == 'D' )
                {
                    sscanf( ptcar + 1, "%d,%f", &current_Dcode, &fdrill );
                    break;
                }
                else
                    ptcar++;
            }

            dimH  = (int) ( (fdimH * dcode_scale * 1000) + 0.5 );
            dimV  = (int) ( (fdimV * dcode_scale * 1000) + 0.5 );
            drill = (int) ( (fdrill * dcode_scale * 1000) + 0.5 );

            if( strchr( "CLROP", c_type_outil[0] ) )
                type_outil = (APERTURE_T) c_type_outil[0];
            else
            {
                fclose( dest );
                return -2;
            }
        }

        /* Update the list of d_codes if consistant. */
        if( current_Dcode < FIRST_DCODE )
            continue;

        if( current_Dcode >= TOOLS_MAX_COUNT )
            continue;

        dcode = gerber->GetDCODE( current_Dcode );
        dcode->m_Size.x  = dimH;
        dcode->m_Size.y  = dimV;
        dcode->m_Shape   = (APERTURE_T) type_outil;
        dcode->m_Drill.x = dcode->m_Drill.y = drill;
        dcode->m_Defined = TRUE;
    }

    fclose( dest );

    return 1;
}


/* Set Size Items (Lines, Flashes) from DCodes List
 */
void WinEDA_GerberFrame::CopyDCodesSizeToItems()
{
    static D_CODE dummy( 999 );   //Used if D_CODE not found in list

    BOARD_ITEM*   item = GetBoard()->m_Drawings;

    for( ; item; item = item->Next() )
    {
        GERBER_DRAW_ITEM* gerb_item = (GERBER_DRAW_ITEM*) item;
        D_CODE*           dcode     = gerb_item->GetDcodeDescr();
        wxASSERT( dcode );
        if( dcode == NULL )
            dcode = &dummy;

        dcode->m_InUse = TRUE;

        gerb_item->m_Size = dcode->m_Size;
        if(                                             // Line Item
            (gerb_item->m_Shape == GBR_SEGMENT )        /* rectilinear segment */
            || (gerb_item->m_Shape == GBR_ARC )         /* segment arc (rounded tips) */
            || (gerb_item->m_Shape == GBR_CIRCLE )      /* segment in a circle (ring) */
            )
        {
        }
        else        // Spots ( Flashed Items )
        {
            switch( dcode->m_Shape )
            {
            case APT_LINE:          // might not appears here, but some broken
            // gerber files use it
            case APT_CIRCLE:        /* spot round (for GERBER) */
                gerb_item->m_Shape = GBR_SPOT_CIRCLE;
                break;

            case APT_OVAL:          /* spot oval (for GERBER)*/
                gerb_item->m_Shape = GBR_SPOT_OVAL;
                break;

            default:                /* spot rect (for GERBER)*/
                gerb_item->m_Shape = GBR_SPOT_RECT;
                break;
            }
        }
    }
}


/** function DrawFlashedShape
 * Draw the dcode shape for flashed items.
 * When an item is flashed, the DCode shape is the shape of the item
 */
void D_CODE::DrawFlashedShape( EDA_Rect* aClipBox, wxDC* aDC, int aColor,
                               wxPoint aShapePos, bool aFilledShape )
{
    int radius;

    switch( m_Shape )
    {
    case APT_CIRCLE:
        radius = m_Size.x >> 1;
        if( !aFilledShape )
            GRCircle( aClipBox, aDC, aShapePos.x, aShapePos.y, radius, aColor );
        else
            if( m_DrillShape == 0 )
                GRFilledCircle( aClipBox, aDC, aShapePos, radius, aColor );
            else if( m_DrillShape == 1 )    // round hole
            {
                int width = (m_Size.x - m_Drill.x ) / 2;
                GRCircle( aClipBox, aDC, aShapePos, radius - (width / 2), width, aColor );
            }
            else                            // rectangular hole
            {
                if( m_PolyCorners.size() == 0 )
                    ConvertShapeToPolygon();
                DrawFlashedPolygon( aClipBox, aDC, aColor, aFilledShape, aShapePos );
            }
        break;

    case APT_LINE:
        // not used for flashed items
        break;

    case APT_RECT:
    {
        wxPoint start;
        start.x = aShapePos.x - m_Size.x/2;
        start.y = aShapePos.y - m_Size.y/2;
        wxPoint end = start + m_Size;
        if( !aFilledShape )
        {
            GRRect( aClipBox, aDC, start.x, start.y, end.x, end.y ,
                    0, aColor );
        }
        else if( m_DrillShape == 0 )
        {
            GRFilledRect( aClipBox, aDC, start.x, start.y, end.x, end.y,
                          0, aColor, aColor );
        }
        else
        {
            if( m_PolyCorners.size() == 0 )
                ConvertShapeToPolygon();
            DrawFlashedPolygon( aClipBox, aDC, aColor, aFilledShape, aShapePos );
        }
    }
        break;

    case APT_OVAL:
    {
        wxPoint start = aShapePos;
        wxPoint end   = aShapePos;
        if( m_Size.x > m_Size.y )   // horizontal oval
        {
            int delta = (m_Size.x - m_Size.y) / 2;
            start.x -= delta;
            end.x   += delta;
            radius   = m_Size.y;
        }
        else   // horizontal oval
        {
            int delta = (m_Size.y - m_Size.x) / 2;
            start.y -= delta;
            end.y   += delta;
            radius   = m_Size.x;
        }
        if( !aFilledShape )
        {
            GRCSegm( aClipBox, aDC, start.x, start.y,
                     end.x, end.y, radius, aColor );
        }
        else if( m_DrillShape == 0 )
        {
            GRFillCSegm( aClipBox, aDC, start.x,
                         start.y, end.x, end.y, radius, aColor );
        }
        else
        {
            if( m_PolyCorners.size() == 0 )
                ConvertShapeToPolygon();
            DrawFlashedPolygon( aClipBox, aDC, aColor, aFilledShape, aShapePos );
        }
    }
    break;

    case APT_POLYGON:
        if( m_PolyCorners.size() == 0 )
            ConvertShapeToPolygon();
        DrawFlashedPolygon( aClipBox, aDC, aColor, aFilledShape, aShapePos );
        break;
    
    case APT_MACRO:
        // TODO
        break;
    }
}


/** function DrawFlashedPolygon
 * a helper function used id ::Draw to draw the polygon stored ion m_PolyCorners
 * Draw some Apertures shapes when they are defined as filled polygons.
 * APT_POLYGON is always a polygon, but some complex shapes are also converted to
 * polygons (shapes with holes)
 */
void D_CODE::DrawFlashedPolygon( EDA_Rect* aClipBox, wxDC* aDC,
                                 int aColor, bool aFilled,
                                 const wxPoint& aPosition )
{
    if( m_PolyCorners.size() == 0 )
        return;

    std::vector<wxPoint> points;
    points = m_PolyCorners;
    for( unsigned ii = 0; ii < points.size(); ii++ )
    {
        points[ii] += aPosition;
    }

    GRClosedPoly( aClipBox, aDC, points.size(), &points[0], aFilled, aColor, aColor );
}


/** function ConvertShapeToPolygon
 * convert a shape to an equivalent polygon.
 * Arcs and circles are approximated by segments
 * Useful when a shape is not a graphic primitive (shape with hole,
 * Rotated shape ... ) and cannot be easily drawn.
 */
void D_CODE::ConvertShapeToPolygon()
{
    #define SEGS_CNT 32     // number of segments to approximate a circle
    wxPoint initialpos;
    wxPoint currpos;;
    m_PolyCorners.clear();

    switch( m_Shape )
    {
    case APT_CIRCLE:        // creates only a circle with rectangular hole
        currpos.x  = m_Size.x >> 1;
        initialpos = currpos;
        for( unsigned ii = 0; ii <= SEGS_CNT; ii++ )
        {
            currpos = initialpos;
            RotatePoint( &currpos, ii * 3600 / SEGS_CNT );
            m_PolyCorners.push_back( currpos );
        }
        if( m_DrillShape == 1 )
        {
            for( unsigned ii = 0 ; ii <= SEGS_CNT; ii++ )
            {
                currpos.x = m_Drill.x / 2;
                currpos.y = 0;
                RotatePoint( &currpos, ii * 3600 / SEGS_CNT );
                m_PolyCorners.push_back( currpos );
            }
            m_PolyCorners.push_back( initialpos );  // link to outline
        }
        if( m_DrillShape == 2 )  // Create rectangular hole
        {
            currpos.x = m_Drill.x / 2;
            currpos.y = m_Drill.y / 2;
            m_PolyCorners.push_back( currpos );     // link to hole and begin hole
            currpos.x -= m_Drill.x;
            m_PolyCorners.push_back( currpos );
            currpos.y -= m_Drill.y;
            m_PolyCorners.push_back( currpos );
            currpos.x += m_Drill.x;
            m_PolyCorners.push_back( currpos );
            currpos.y += m_Drill.y;
            m_PolyCorners.push_back( currpos );     // close hole
            m_PolyCorners.push_back( initialpos );  // link to outline
        }
        break;

    case APT_LINE:
        // Not used for flashed shapes
        break;

    case APT_RECT:
        currpos.x = m_Size.x / 2;
        currpos.y = m_Size.y / 2;
        initialpos = currpos;
        m_PolyCorners.push_back( currpos );
        currpos.x -= m_Size.x;
        m_PolyCorners.push_back( currpos );
        currpos.y -= m_Size.y;
        m_PolyCorners.push_back( currpos );
        currpos.x += m_Size.x;
        m_PolyCorners.push_back( currpos );
        currpos.y += m_Size.y;
        m_PolyCorners.push_back( currpos );         // close polygon
        if( m_DrillShape == 1 )                     // build a round hole
        {
            for( int ii = 0 ; ii <= SEGS_CNT; ii++ )
            {
                currpos.x = 0;
                currpos.y = m_Drill.x / 2;      // m_Drill.x / 2 is the radius of the hole
                RotatePoint( &currpos, ii * 3600 / SEGS_CNT );
                m_PolyCorners.push_back( currpos );
            }
            m_PolyCorners.push_back( initialpos );  // link to outline
        }
        if( m_DrillShape == 2 )  // Create rectangular hole
        {
            currpos.x = m_Drill.x / 2;
            currpos.y = m_Drill.y / 2;
            m_PolyCorners.push_back( currpos );     // link to hole and begin hole
            currpos.x -= m_Drill.x;
            m_PolyCorners.push_back( currpos );
            currpos.y -= m_Drill.y;
            m_PolyCorners.push_back( currpos );
            currpos.x += m_Drill.x;
            m_PolyCorners.push_back( currpos );
            currpos.y += m_Drill.y;
            m_PolyCorners.push_back( currpos );     // close hole
            m_PolyCorners.push_back( initialpos );  // link to outline
        }
        break;

    case APT_OVAL:
    {
        int delta, radius;

        // we create an horizontal oval shape. then rotate if needed
        if( m_Size.x > m_Size.y )   // horizontal oval
        {
            delta = (m_Size.x - m_Size.y) / 2;
            radius = m_Size.y / 2;
        }
        else   // vertical oval
        {
            delta  = (m_Size.y - m_Size.x) / 2;
            radius = m_Size.x / 2;
        }
        currpos.y  = radius;
        initialpos = currpos;
        m_PolyCorners.push_back( currpos );

        // build the right arc of the shape
        unsigned ii = 0;
        for( ; ii <= SEGS_CNT / 2; ii++ )
        {
            currpos = initialpos;
            RotatePoint( &currpos, ii * 3600 / SEGS_CNT );
            currpos.x += delta;
            m_PolyCorners.push_back( currpos );
        }

        // build the left arc of the shape
        for( ii = SEGS_CNT / 2; ii <= SEGS_CNT; ii++ )
        {
            currpos = initialpos;
            RotatePoint( &currpos, ii * 3600 / SEGS_CNT );
            currpos.x -= delta;
            m_PolyCorners.push_back( currpos );
        }
        m_PolyCorners.push_back( initialpos );      // close outline
        if( m_Size.y > m_Size.x )   // vertical oval, rotate polygon.
        {
            for( unsigned jj = 0; jj < m_PolyCorners.size(); jj++ )
                RotatePoint( &m_PolyCorners[jj], 900 );
        }
        if( m_DrillShape == 1 )                     // build a round hole
        {
            for( ii = 0 ; ii <= SEGS_CNT; ii++ )
            {
                currpos.x = 0;
                currpos.y = m_Drill.x / 2;      // m_Drill.x / 2 is the radius of the hole
                RotatePoint( &currpos, ii * 3600 / SEGS_CNT );
                m_PolyCorners.push_back( currpos );
            }
            m_PolyCorners.push_back( initialpos );  // link to outline
        }
        if( m_DrillShape == 2 )  // Create rectangular hole
        {
            currpos.x = m_Drill.x / 2;
            currpos.y = m_Drill.y / 2;
            m_PolyCorners.push_back( currpos );     // link to hole and begin hole
            currpos.x -= m_Drill.x;
            m_PolyCorners.push_back( currpos );
            currpos.y -= m_Drill.y;
            m_PolyCorners.push_back( currpos );
            currpos.x += m_Drill.x;
            m_PolyCorners.push_back( currpos );
            currpos.y += m_Drill.y;
            m_PolyCorners.push_back( currpos );     // close hole
            m_PolyCorners.push_back( initialpos );  // link to outline
        }
    }
        break;

    case APT_POLYGON:
        // TODO
        break;
    
    case APT_MACRO:
        // TODO
        break;
    }
}
