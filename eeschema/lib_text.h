/*
 * This program source code file is part of KiCad, a free EDA CAD application.
 *
 * Copyright (C) 2004 Jean-Pierre Charras, jaen-pierre.charras@gipsa-lab.inpg.com
 * Copyright (C) 2004-2011 KiCad Developers, see change_log.txt for contributors.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, you may find one here:
 * http://www.gnu.org/licenses/old-licenses/gpl-2.0.html
 * or you may search the http://www.gnu.org website for the version 2 license,
 * or you may write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 */

/**
 * @file lib_text.h
 */

#ifndef _LIB_TEXT_H_
#define _LIB_TEXT_H_

#include <eda_text.h>
#include <lib_draw_item.h>

/**
 * Class LIB_TEXT
 * defines a component library graphical text item.
 * <p>
 * This is only a graphical text item.  Field text like the reference designator,
 * component value, etc. are not LIB_TEXT items.  See the #LIB_FIELD class for the
 * field item definition.
 * </p>
 */
class LIB_TEXT : public LIB_ITEM, public EDA_TEXT
{
    wxString m_savedText;         ///< Temporary storage for the string when edition.
    bool m_rotate;                ///< Flag to indicate a rotation occurred while editing.
    bool m_updateText;            ///< Flag to indicate text change occurred while editing.

    void drawGraphic( EDA_DRAW_PANEL* aPanel, wxDC* aDC, const wxPoint& aOffset,
                      int aColor, int aDrawMode, void* aData, const TRANSFORM& aTransform );

    void calcEdit( const wxPoint& aPosition );

public:
    LIB_TEXT( LIB_COMPONENT * aParent );

    // Do not create a copy constructor.  The one generated by the compiler is adequate.

    ~LIB_TEXT() { }

    wxString GetClass() const
    {
        return wxT( "LIB_TEXT" );
    }

    /**
     * Sets the text item string to \a aText.
     *
     * This method does more than just set the set the text string.  There are special
     * cases when changing the text string alone is not enough.  If the text item is
     * being moved, the name change must be delayed until the next redraw to prevent
     * drawing artifacts.
     *
     * @param aText - New text value.
     */
    void SetText( const wxString& aText );

    bool Save( OUTPUTFORMATTER& aFormatter );

    bool Load( LINE_READER& aLineReader, wxString& aErrorMsg );

    bool HitTest( const wxPoint& aPosition );

    bool HitTest( wxPoint aPosition, int aThreshold, const TRANSFORM& aTransform );

    bool HitTest( EDA_RECT& aRect )
    {
        return TextHitTest( aRect );
    }


    int GetPenSize( ) const;

    void DisplayInfo( EDA_DRAW_FRAME* aFrame );

    EDA_RECT GetBoundingBox() const;

    void Rotate();

    void BeginEdit( int aEditMode, const wxPoint aStartPoint = wxPoint( 0, 0 ) );

    bool ContinueEdit( const wxPoint aNextPoint );

    void EndEdit( const wxPoint& aPosition, bool aAbort = false );

    void SetOffset( const wxPoint& aOffset );

    bool Inside( EDA_RECT& aRect ) const;

    void Move( const wxPoint& aPosition );

    wxPoint GetPosition() const { return m_Pos; }

    void MirrorHorizontal( const wxPoint& aCenter );

    void MirrorVertical( const wxPoint& aCenter );

    void Rotate( const wxPoint& aCenter, bool aRotateCCW = true );

    void Plot( PLOTTER* aPlotter, const wxPoint& aOffset, bool aFill,
               const TRANSFORM& aTransform );

    int GetWidth() const { return m_Thickness; }

    void SetWidth( int aWidth ) { m_Thickness = aWidth; }

    wxString GetSelectMenuText() const;

    BITMAP_DEF GetMenuImage() const { return  add_text_xpm; }

    EDA_ITEM* Clone() const;

private:

    /**
     * @copydoc LIB_ITEM::compare()
     *
     * The text specific sort order is as follows:
     *      - Text string, case insensitive compare.
     *      - Text horizontal (X) position.
     *      - Text vertical (Y) position.
     *      - Text width.
     *      - Text height.
     */
    int compare( const LIB_ITEM& aOther ) const;
};


#endif    // _LIB_TEXT_H_
