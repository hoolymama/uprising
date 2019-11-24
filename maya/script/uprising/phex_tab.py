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


class phexTab(gui.FormLayout):
    def __init__(self):
        self.setNumberOfDivisions(100)
        pm.setParent(self)

        self.frame = self.create_ui()
        pm.setParent(self)

        self.create_action_buttons()

        pm.setParent(self)
 

    def create_ui(self):

        
        frame = pm.frameLayout(
            collapsable=False,
            collapse=False,
            en=True,
            lv=True,
            bv=True,
            label="Pot/handle exercise",
        )

        self.add_brushes_row = pm.rowLayout(
            numberOfColumns=3,
            columnWidth3=(200, 200, 150),
            adjustableColumn=3,
            columnAlign=(1, "right"),
            columnAttach=[(1, "both", 2), (2, "both", 2), (2, "both", 2)],
        )
        pm.button(
            label="Load selected brushes", command=pm.Callback(self.on_load_selected_brushes)
        )

        pm.button(label="Load all brushes", command=pm.Callback(self.on_load_all_brushes))

        pm.button(label="Clear", command=pm.Callback(self.on_clear_brushes))


        pm.setParent("..")

        pm.scrollLayout(bv=True)
        self.brushes_column = pm.columnLayout(adj=True)

        pm.setParent("..")
        return frame
 

    def create_action_buttons(self):
        pm.setParent(self)  # form

        go_but = pm.button(label="Export", command=pm.Callback(self.on_go))
        print_but = pm.button(label="Show stats", command=pm.Callback(self.on_show))

        self.attachForm(self.frame, "left", 2)
        self.attachForm(self.frame, "right", 2)
        self.attachForm(self.frame, "top", 2)
        self.attachControl(self.frame, "bottom", 2, go_but)

        self.attachNone(go_but, "top")
        self.attachForm(go_but, "right", 2)
        self.attachPosition(go_but, "left", 2, 50)
        self.attachForm(go_but, "bottom", 2)

        self.attachNone(print_but, "top")
        self.attachControl(print_but, "right", 2, go_but)
        self.attachForm(print_but, "left", 2)
        self.attachForm(print_but, "bottom", 2)

    def on_show(self):
        data = self.get_pot_handle_exercise_data()
        uutl.show_in_window(data, title="Pot/holder  exercise stats")

    def on_go(self):
        data = self.get_pot_handle_exercise_data()
       
        timestamp =  write.get_timestamp()
        if data["combinations"]:
            exercise_dir = os.path.join(
                pm.workspace.getPath(),
                'export',
                'calibrations',
                k.POT_HANDLE_EXERCISE_PROGRAM_NAME,
                write.get_timestamp())
            uutl.mkdir_p(exercise_dir)

            studio = Studio(pot_handle_exercise_data=data["combinations"])
            studio.write()
            write.write_program(Robolink(), exercise_dir, k.POT_HANDLE_EXERCISE_PROGRAM_NAME, timestamp)
        uutl.show_in_window(data, title="Pot/holder  exercise stats")


    def on_load_selected_brushes(self):
        brushes = pm.ls(selection=True, dag=True, leaf=True, type="brushNode")
        self._load_brush_nodes(brushes)

    def on_load_all_brushes(self):
        brushes = pm.PyNode("mainPaintingShape").attr("brushes").connections(s=True)
        self._load_brush_nodes(brushes)

    def on_clear_brushes(self):
        self._clear_entries()


    def _get_main_painting_connection_id(self,brush):
        conns = pm.PyNode(brush).attr("outPaintBrush").connections(p=True)
        return [p.logicalIndex() for p in conns if p.node() == "mainPaintingShape"][0]


    def _load_brush_nodes(self, brushes):
        self._clear_entries()
        if not brushes:
            return
        frame = 0
        painting_node = pm.PyNode("mainPaintingShape")
        combos = putl.dip_combination_ids()
        print "COMBOS", combos
        for brush in brushes:
            conns = pm.PyNode(brush).attr("outPaintBrush").connections(p=True)
            brush_id = self._get_main_painting_connection_id(brush)
            paint_ids =  [c["paint"] for c in combos if  c["brush"] == brush_id]

            print "PAINT_IDS",paint_ids

            pm.setParent(self.brushes_column)
            self._create_entry(brush, paint_ids, frame)
            frame += 1

    def _clear_entries(self):
        children = pm.columnLayout(self.brushes_column, q=True, ca=True)
        if children:
            pm.deleteUI(children)

    def _create_entry(self, brush,paint_ids, frame):
        text = ",".join(str(p) for p in paint_ids)
        tf = pm.textFieldGrp(
            columnWidth2=(300, 200), label=brush, text=text, cw2=(140, 300)
        )
        return tf

    def get_pot_handle_exercise_data(self):
 
        data = {"combinations": [], "brushes": []}
        tfs = pm.columnLayout(self.brushes_column, q=True, ca=True)
        for tf in tfs:
            brush_name = pm.textFieldGrp(tf, q=True, label=True)
            brush = pm.PyNode(brush_name)

            brush_id = self._get_main_painting_connection_id(brush)

            val = pm.textFieldGrp(tf, q=True, text=True)

            for paint_id in [
                int(i) for i in val.split(",") if i is not None and i.isdigit()
            ]:
                data["combinations"].append({"brush": brush_id, "paint": paint_id})
                data["brushes"].append(brush_name)

        data["paints"] = list(set([c["paint"] for c in data["combinations"]]))
        data["brushes"] = list(set(data["brushes"]))

        return data
