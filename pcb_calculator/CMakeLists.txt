
include_directories(BEFORE ${INC_BEFORE})
include_directories(
    bitmaps
    dialogs
    transline
    attenuators
    ../polygon
    ../common
    ${INC_AFTER}
    )

set(PCB_CALCULATOR_SRCS
    attenuators.cpp
    board_classes_values.cpp
    colorcode.cpp
    electrical_spacing_values.cpp
    params_read_write.cpp
    pcb_calculator.cpp
    pcb_calculator_frame.cpp
    datafile_read_write.cpp
    regulators_funct.cpp
    tracks_width_versus_current.cpp
    transline_ident.cpp
    UnitSelector.cpp
    pcb_calculator_datafile_keywords.cpp
    transline/transline.cpp
    transline/c_microstrip.cpp
    transline/microstrip.cpp
    transline/coplanar.cpp
    transline/coax.cpp
    transline/rectwaveguide.cpp
    transline/stripline.cpp
    transline/twistedpair.cpp
    transline_dlg_funct.cpp
    attenuators/attenuator_classes.cpp
    dialogs/pcb_calculator_frame_base.cpp
    dialogs/dialog_regulator_data_base.cpp
    )

if(WIN32)
    if(MINGW)
        # PCB_CALCULATOR_RESOURCES variable is set by the macro.
        mingw_resource_compiler(pcb_calculator)
    else(MINGW)
        set(PCB_CALCULATOR_RESOURCES pcb_calculator.rc)
    endif(MINGW)
endif(WIN32)


if(APPLE)
    set(PCB_CALCULATOR_RESOURCES pcb_calculator.icns)
    set_source_files_properties("${CMAKE_CURRENT_SOURCE_DIR}/pcb_calculator.icns"
                                PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
    set(MACOSX_BUNDLE_ICON_FILE pcb_calculator.icns)
    set(MACOSX_BUNDLE_GUI_IDENTIFIER org.kicad-eda.pcb_calculator)
    set(MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/Info.plist)
endif(APPLE)

# auto-generate pcb_calculator_datafile.h and pcb_calculator_datafile_keywords.cpp
# for the storage data file format.
make_lexer(
   ${CMAKE_CURRENT_SOURCE_DIR}/pcb_calculator_datafile.keywords
   ${CMAKE_CURRENT_SOURCE_DIR}/pcb_calculator_datafile_lexer.h
   ${CMAKE_CURRENT_SOURCE_DIR}/pcb_calculator_datafile_keywords.cpp
   PCBCALC_DATA_T
   )


add_executable(pcb_calculator WIN32 MACOSX_BUNDLE
    ${PCB_CALCULATOR_SRCS}
    ${PCB_CALCULATOR_RESOURCES}
    )


if(APPLE)
    set_target_properties(pcb_calculator PROPERTIES MACOSX_BUNDLE_INFO_PLIST
        ${CMAKE_CURRENT_SOURCE_DIR}/Info.plist)
endif(APPLE)

target_link_libraries( pcb_calculator
    common
    bitmaps
    polygon
    ${wxWidgets_LIBRARIES}
    )

install(TARGETS pcb_calculator
        DESTINATION ${KICAD_BIN}
        COMPONENT binary)

