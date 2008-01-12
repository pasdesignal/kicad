/*
 * This program source code file is part of KICAD, a free EDA CAD application.
 *
 * Copyright (C) 2007 Dick Hollenbeck, dick@softplc.com
 * Copyright (C) 2007 Kicad Developers, see change_log.txt for contributors.
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

 
/*************************************************/
/* class_drc_item.cpp - DRC_ITEM class functions */
/*************************************************/
#include "fctsys.h"
#include "common.h"

#include "pcbnew.h"

wxString DRC_ITEM::GetErrorText() const
{
    switch( m_ErrorCode )
    {
    case DRCE_UNCONNECTED_PADS:
        return wxString( _("Unconnected pads") );
    case DRCE_TRACK_NEAR_THROUGH_HOLE:
        return wxString( _("Track near thru-hole") ); 
    case DRCE_TRACK_NEAR_PAD:
        return wxString( _("Track near pad") );
    case DRCE_TRACK_NEAR_VIA:
        return wxString( _("Track near via") );
    case DRCE_VIA_NEAR_VIA:
        return wxString( _("Via near via") );
    case DRCE_VIA_NEAR_TRACK:
        return wxString( _("Via near track") );
    case DRCE_TRACK_ENDS1:
    case DRCE_TRACK_ENDS2:
    case DRCE_TRACK_ENDS3:
    case DRCE_TRACK_ENDS4:
    case DRCE_ENDS_PROBLEM1: 
    case DRCE_ENDS_PROBLEM2:
    case DRCE_ENDS_PROBLEM3:
    case DRCE_ENDS_PROBLEM4:
    case DRCE_ENDS_PROBLEM5:
        return wxString( _("Two track ends") );
    case DRCE_TRACK_UNKNOWN1:
        return wxString( _("This looks bad") );  ///< @todo check source code and change this comment
    case DRCE_TRACKS_CROSSING:
        return wxString( _("Tracks crossing") );
    case DRCE_PAD_NEAR_PAD1:
        return wxString( _("Pad near pad") );
    case DRCE_VIA_HOLE_BIGGER:
        return wxString( _("Via hole > diameter"));
    case DRCE_MICRO_VIA_INCORRECT_LAYER_PAIR:
        return wxString( _("Micro Via: incorrect layer pairs (not adjacent)"));
    case COPPERAREA_INSIDE_COPPERAREA:
        return wxString( _("Copper area inside copper area"));
    case COPPERAREA_CLOSE_TO_COPPERAREA:
        return wxString( _("Copper areas intersect or too close"));
        
    default:
        return wxString( wxT("PROGRAM BUG, PLEASE LEAVE THE ROOM.") );
    }
}


wxString DRC_ITEM::ShowCoord( const wxPoint& aPos )
{
    wxString temp;
    wxString ret;

    ret << wxT("@(") << valeur_param( aPos.x, temp ); 
    ret << wxT(",")  << valeur_param( aPos.y, temp );
    ret << wxT(")");

    return ret;
}

