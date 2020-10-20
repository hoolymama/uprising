import os

import const as k
import pymel.core as pm
import pymel.core.uitypes as gui
import robo
import uprising_util as uutl
import write
# from studio import Studio
from uprising.session.brush_hang_session import BrushHangSession


class brushHangTab(gui.FormLayout):
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
            label="Brush hang program",
        )

        self.add_brushes_row = pm.rowLayout(
            numberOfColumns=4,
            columnWidth4=(125, 125, 125, 125),
            adjustableColumn=1,
            columnAlign=(1, "right"),
            columnAttach=[
                (1, "both", 2),
                (2, "both", 2),
                (3, "both", 2),
                (4, "both", 2),
            ],
        )
        pm.button(label="Load Brushes", command=pm.Callback(self.on_load_brushes))
        pm.button(label="Clear", command=pm.Callback(self.on_clear_brushes))

        pm.button(label="Twist on", command=pm.Callback(self.on_twist_on))

        pm.button(label="Twist off", command=pm.Callback(self.on_twist_off))

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
        data = self.get_brush_twist_data()
        uutl.show_in_window(
            [
                {"brush": str(b["brush"]), "id": b["id"], "twist": b["twist"]}
                for b in data
            ],
            title="Brush hang values",
        )

    def on_go(self):
        data = self.get_brush_twist_data()
        session = BrushHangSession(data)
        session.send()
        session.publish()

        # timestamp = write.get_timestamp()
        # if data:
        #     directory = os.path.join(
        #         pm.workspace.getPath(),
        #         "export",
        #         "calibrations",
        #         k.BRUSH_HANG_PROGRAM_NAME,
        #         timestamp,
        #     )
        #     uutl.mkdir_p(directory)

        #     studio = Studio(brush_hang_data=data, pause=300)
        #     studio.write()
        #     # robo.write_program(directory, k.BRUSH_HANG_PROGRAM_NAME)

        #     robo.show()
        #     src_fn, rdk_fn=write.save_prog_and_station(directory, k.BRUSH_HANG_PROGRAM_NAME)

        #     subprogram_names = []
        #     with uutl.final_position(pm.PyNode("RACK1_CONTEXT")):
        #         for i, program in enumerate(studio.pick_place_programs):
        #             name =  program.program_name
        #             print "Writing PP", name
        #             subprogram_names.append(name)
        #             write.save_prog_and_station(directory, name)
            
        #     write.insert_external_dependencies(subprogram_names,src_fn)




        # uutl.show_in_window(
        #     [
        #         {"brush": str(b["brush"]), "id": b["id"], "twist": b["twist"]}
        #         for b in data
        #     ],
        #     title="Brush hang values",
        # )

    def on_twist_on(self):
        for cb in pm.columnLayout(self.brushes_column, q=True, ca=True):
            pm.checkBoxGrp(cb, edit=True, value1=1)

    def on_twist_off(self):
        for cb in pm.columnLayout(self.brushes_column, q=True, ca=True):
            pm.checkBoxGrp(cb, edit=True, value1=0)

    def on_load_brushes(self):
        brushes = pm.ls(selection=True, dag=True, leaf=True, type="brushNode")
        if not brushes:
            brushes = pm.PyNode("mainPaintingShape").attr("brushes").connections(s=True)
        self._load_brush_nodes(brushes)

    def on_clear_brushes(self):
        self._clear_entries()

    def _get_main_painting_connection_id(self, brush):
        conns = pm.PyNode(brush).attr("outPaintBrush").connections(p=True)
        return [p.logicalIndex() for p in conns if p.node() == "mainPaintingShape"][0]

    def _load_brush_nodes(self, brushes):
        self._clear_entries()
        if not brushes:
            return
        frame = 0

        for brush in brushes:
            pm.setParent(self.brushes_column)
            self._create_entry(brush, frame)
            frame += 1

    def _clear_entries(self):
        children = pm.columnLayout(self.brushes_column, q=True, ca=True)
        if children:
            pm.deleteUI(children)

    def _create_entry(self, brush, frame):

        cb = pm.checkBoxGrp(
            columnWidth2=(300, 200),
            label=brush,
            value1=1,
            annotation="Check to also twist",
        )
        return cb

    def get_brush_twist_data(self):
        data = []
        cbs = pm.columnLayout(self.brushes_column, q=True, ca=True)
        for cb in cbs:
            brush_name = pm.checkBoxGrp(cb, q=True, label=True)
            twist = pm.checkBoxGrp(cb, q=True, value1=True)
            brush = pm.PyNode(brush_name)
            brush_id = self._get_main_painting_connection_id(brush)
            data.append({"brush": brush, "id": brush_id, "twist": twist})
        return data
