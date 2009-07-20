/*
 * This program source code file is part of KICAD, a free EDA CAD application.
 *
 * Copyright (C) 2009 Jea-Pierre.Charras, jaen-pierre.charras@gipsa-lab.inpg.com
 * Copyright (C) 2009 Kicad Developers, see change_log.txt for contributors.
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

#ifndef _ERC_H
#define _ERC_H

/* For ERC markers: error types (used in diags, and to set the color):
*/
enum errortype
{
    OK  = 0,
    WAR,        // Error: severity = warning
    ERR,        // Error: severity = error
    UNC         // Error: unconnected pin
};


/// DRC error codes:
#define ERCE_UNSPECIFIED          0
#define ERCE_DUPLICATE_SHEET_NAME 1                                 //duplicate sheet names within a given sheet
#define ERCE_PIN_NOT_CONNECTED    2                                 //pin not connected and not no connect symbol
#define ERCE_PIN_NOT_DRIVEN       3                                 //pin connected to some others pins but no pin to drive it
#define ERCE_PIN_TO_PIN_WARNING   4                                 //pin connected to an other pin: warning level
#define ERCE_PIN_TO_PIN_ERROR     5                                 //pin connected to an other pin: error level
#define ERCE_HIERACHICAL_LABEL    6                                 //mismatch between hierarchical labels and pins sheets
#define ERCE_NOCONNECT_CONNECTED  7                                 //a no connect symbol is connected to more than 1 pin


#endif  // _ERC_H