import sys
import os
import json
import pymel.core as pm
import random
from robolink import Robolink, COLLISION_OFF
import write
import props
import uprising_util as uutl
import curve_utils as cutl
import palette_utils as putl

import props

from studio import Studio
from brush import Brush
from paint import Paint
import const as k
import pymel.core.uitypes as gui


class exportTab(gui.FormLayout):
    def __init__(self):
        self.setNumberOfDivisions(100)
        pm.setParent(self)

        self.simple_ui = self.create_simple_ui()
        pm.setParent(self)

        self.create_action_buttons()

        pm.setParent(self)
        # self.initialize_ui()

    def create_simple_ui(self):

        frame = pm.frameLayout(
            collapsable=False,
            collapse=False,
            en=True,
            labelVisible=False,
            label="Simple validation",
        )
        # pm.scrollLayout(childResizable=True)
        pm.columnLayout(adj=True)

        pm.frameLayout(
            collapsable=False,
            collapse=False,
            en=True,
            lv=True,
            bv=True,
            label="Painting programs",
        )
        self.painting_wg = pm.checkBoxGrp(h=24, l="Painting", v1=0)
        self.dips_wg = pm.checkBoxGrp(h=24, l="Dips", v1=0)

        pm.setParent("..")

        pm.frameLayout(
            collapsable=False,
            collapse=False,
            en=True,
            lv=True,
            bv=True,
            label="Pick & place programs",
        )

        self.pp_wg = pm.radioButtonGrp(
            l="Pick & place", sl=1, nrb=4, la4=["None", "First", "Used", "All"]
        )
        self.ppx_wg = pm.checkBoxGrp(h=24, l="Pick & place exercise", v1=0)

        pm.setParent("..")

        pm.frameLayout(
            collapsable=False,
            collapse=False,
            en=True,
            lv=True,
            bv=True,
            label="Perspex programs",
        )
        self.perspex_tri_wg = pm.checkBoxGrp(h=24, l="Perspex triangulation", v1=0)
        self.perspex_cal_wg = pm.checkBoxGrp(h=24, l="Perspex calibration", v1=0)
        pm.setParent("..")

        pm.frameLayout(
            collapsable=False,
            collapse=False,
            en=True,
            lv=True,
            bv=True,
            label="Pot & holder calibration programs",
        )
        self.pot_cal_wg = pm.checkBoxGrp(h=24, l="Paintpot calibration", v1=0)
        self.holder_cal_wg = pm.checkBoxGrp(h=24, l="Holder calibration", v1=0)
        pm.setParent("..")

        pm.frameLayout(
            collapsable=False,
            collapse=False,
            en=True,
            lv=True,
            bv=True,
            label="Board calibration programs",
        )
        self.board_cal_wg = pm.checkBoxGrp(h=30, l="Board calibration", v1=0)
        pm.setParent("..")

        pm.frameLayout(
            collapsable=False,
            collapse=False,
            en=True,
            lv=True,
            bv=True,
            label="Export geometry",
        )
        pm.columnLayout(adj=True)
        self.brush_tools_wg = pm.radioButtonGrp(
            h=30,
            l="Brushes",
            sl=1,
            nrb=4,
            la4=["None", "Used", "Connected", "Selected"],
        )

        self.pots_geo_wg = pm.checkBoxGrp(h=30, l="Used paint pots", v1=0)

        self.sel_geo_wg = pm.checkBoxGrp(h=30, l="Selected geometry", v1=0)

        self.approaches_wg = pm.checkBoxGrp(h=30, l="Global approaches", v1=0)

        pm.setParent(frame)
        pm.setParent("..")

        return frame

    def create_action_buttons(self):
        pm.setParent(self)  # form

        go_but = pm.button(label="Send to RoboDK", command=pm.Callback(self.on_go))

        self.attachForm(self.simple_ui, "left", 2)
        self.attachForm(self.simple_ui, "right", 2)
        self.attachForm(self.simple_ui, "top", 2)
        self.attachControl(self.simple_ui, "bottom", 2, go_but)

        self.attachNone(go_but, "top")
        self.attachForm(go_but, "right", 2)
        self.attachForm(go_but, "left", 2)
        self.attachForm(go_but, "bottom", 2)

    def on_go(self):

        pot_handle_exercise_data = self.get_pot_handle_exercise_data()

        pp = pm.radioButtonGrp(self.pp_wg, query=True, sl=True)
        pp_value = [None, None, [0], "used", "all"][pp]

        studio_kwargs = {
            "do_painting": pm.checkBoxGrp(self.painting_wg, query=True, v1=True),
            "do_dips": pm.checkBoxGrp(self.dips_wg, query=True, v1=True),
            "do_pap_exercise": pm.checkBoxGrp(self.ppx_wg, query=True, v1=True),
            "do_perspex_triangulation": pm.checkBoxGrp(
                self.perspex_tri_wg, query=True, v1=True
            ),
            "do_perspex_calibration": pm.checkBoxGrp(
                self.perspex_cal_wg, query=True, v1=True
            ),
            "do_pot_calibration": pm.checkBoxGrp(self.pot_cal_wg, query=True, v1=True),
            "do_holder_calibration": pm.checkBoxGrp(
                self.holder_cal_wg, query=True, v1=True
            ),
            "do_board_calibration": pm.checkBoxGrp(
                self.board_cal_wg, query=True, v1=True
            ),
            "do_pots_geo": pm.checkBoxGrp(self.pots_geo_wg, query=True, v1=True),
            "do_approaches": pm.checkBoxGrp(self.approaches_wg, query=True, v1=True),
            "pick_and_place_slots": pp_value,
        }

        studio = Studio(**studio_kwargs)
        studio.write()

        brush_tools = pm.radioButtonGrp(self.brush_tools_wg, query=True, sl=True)
        if brush_tools == 2:
            Brush.write_used_brush_sets()
        elif brush_tools == 3:
            Brush.write_connected_brushes()
        elif brush_tools == 4:
            Brush.write_selected_brushes()

        if pm.checkBoxGrp(self.sel_geo_wg, query=True, v1=True):
            props.send_selected()

        if pm.checkBoxGrp(self.pots_geo_wg, query=True, v1=True):
            Paint.write_geos()

        result = {"stats": write.painting_stats(pm.PyNode("mainPaintingShape"))}

        if studio_kwargs.get("do_painting"):
            result["painting"] = studio.painting_program.validate_path()

        if studio_kwargs.get("do_dips"):
            result["dips"] = []
            for dip_program in studio.dip_programs:
                stats = dip_program.validate_path()
                result["dips"].append(stats)

        uutl.show_in_window(result, title="Test export results")

