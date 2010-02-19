/*
 * file tree_project_frame.h
*/

/*
 * This program source code file is part of KICAD, a free EDA CAD application.
 *
 * Copyright (C) 1992-2009 Jean-Pierre Charras, jean-pierre.charras@gipsa-lab.inpg.fr
 * Copyright (C) 1992-2009 Kicad Developers, see change_log.txt for contributors.
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

#ifndef TREEPRJ_FRAME_H
#define TREEPRJ_FRAME_H

/** class TREE_PROJECT_FRAME
 * Window to display the tree files
 */
class TREE_PROJECT_FRAME : public wxSashLayoutWindow
{
private:

    std::vector<wxMenu*>  m_ContextMenus;
    std::vector<wxString> m_Filters;

    wxMenu*  m_PopupMenu;
    wxCursor m_DragCursor;
    wxCursor m_Default;

protected:
    wxMenu*          GetContextMenu( int type );
    void             NewFile( TreeFileType type );
    void             NewFile( const wxString& name, TreeFileType type,
                              wxTreeItemId& root );
    TreePrjItemData* GetSelectedData();

public:
    WinEDA_MainFrame* m_Parent;
    TREEPROJECTFILES*   m_TreeProject;

    wxTreeItemId      m_root;

public:
    static wxString              GetFileExt( TreeFileType type );
    static wxString              GetFileWildcard( TreeFileType type );

    TREE_PROJECT_FRAME( WinEDA_MainFrame* parent );
    ~TREE_PROJECT_FRAME();
    void                         OnSelect( wxTreeEvent& Event );
    void                         OnRenameAsk( wxTreeEvent& Event );
    void                         OnRename( wxTreeEvent& Event );
    void                         OnDragStart( wxTreeEvent& event );
    void                         OnDragEnd( wxTreeEvent& event );
    void                         OnRight( wxTreeEvent& Event );
    void                         ReCreateTreePrj();

    void                         OnTxtEdit( wxCommandEvent& event );

    void                         OnDeleteFile( wxCommandEvent& event );
    void                         OnRenameFile( wxCommandEvent& event );

    void                         OnNewFile( wxCommandEvent& event );
    void                         OnNewDirectory( wxCommandEvent& event );
    void                         OnNewSchFile( wxCommandEvent& event );
    void                         OnNewBrdFile( wxCommandEvent& event );
    void                         OnNewPyFile( wxCommandEvent& event );
    void                         OnNewGerberFile( wxCommandEvent& event );
    void                         OnNewTxtFile( wxCommandEvent& event );
    void                         OnNewNetFile( wxCommandEvent& event );

    void                         ClearFilters();

    const std::vector<wxString>& GetFilters();
    void                         RemoveFilter( const wxString& filter );

#ifdef KICAD_PYTHON
    boost::python::object        ToWx();

    TREE_PROJECT_FRAME()
    {
    }


    TREE_PROJECT_FRAME( const TREE_PROJECT_FRAME& )
    {
    }


    void OnRunPy( wxCommandEvent& event );

    boost::python::object GetMenuPy( TreeFileType );

    boost::python::object GetFtExPy( TreeFileType ) const;

    void                  RemoveFilterPy( const boost::python::str& filter );
    void                  AddFilter( const boost::python::str& filter );

    boost::python::object GetTreeCtrl();
    TreePrjItemData*      GetItemData( const boost::python::object& item );
    void                  AddFilePy( const boost::python::str& name,
                                     boost::python::object&    root );
    void                  NewFilePy( const boost::python::str& name,
                                     TreeFileType              type,
                                     boost::python::object&    root );

    TreePrjItemData*      FindItemData( const boost::python::str& name );

    boost::python::object GetCurrentMenu();
    int                   AddStatePy( boost::python::object& bitmap );

#endif

    bool                  AddFile( const wxString& name, wxTreeItemId& root );

    DECLARE_EVENT_TABLE()
};

#endif  // TREEPRJ_FRAME_H
