from uprising import palette_utils  

import pymel.core as pm
import pymel.core.uitypes as gui
from uprising import robo
from uprising import utils as uutl

from uprising.bot.session.dip_wipe_exercise_session import DipWipeExerciseSession


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
            numberOfColumns=4,
            columnWidth4=(200, 200, 150, 210),
            adjustableColumn=3,
            columnAlign=(1, "right"),
            columnAttach=[(1, "both", 2), (2, "both", 2), (3, "both", 2), (4, "both", 2)],
        )
        pm.button(label="Load selected brushes", command=pm.Callback(self.on_load_selected_brushes))

        pm.button(label="Load all brushes", command=pm.Callback(self.on_load_all_brushes))

        pm.button(label="Clear", command=pm.Callback(self.on_clear_brushes))

        self.do_components_cb = pm.checkBoxGrp(
            columnWidth3=(10, 100, 100),
            numberOfCheckBoxes=2,
            label="",
            valueArray2=(1, 1),
            labelArray2=("Do Dips", "Do Wipes")
        )
        pm.setParent("..")



        pm.scrollLayout(bv=True)
        self.brushes_column = pm.columnLayout(adj=True)

        pm.setParent("..")
        return frame

    def create_action_buttons(self):
        pm.setParent(self)  # form

        go_but = pm.button(label="Export", command=pm.Callback(self.on_go))
        print_but = pm.button(label="Show stats", command=pm.Callback(self.on_show))

        isolate_but = pm.button(label="Isolate", command=pm.Callback(self.on_isolate))

        self.attachForm(self.frame, "left", 2)
        self.attachForm(self.frame, "right", 2)
        self.attachForm(self.frame, "top", 2)
        self.attachControl(self.frame, "bottom", 2, go_but)

        self.attachNone(print_but, "top")
        self.attachPosition(print_but, "right", 2, 33)
        self.attachForm(print_but, "left", 2)
        self.attachForm(print_but, "bottom", 2)

        self.attachNone(isolate_but, "top")
        self.attachPosition(isolate_but, "left", 2, 33)
        self.attachPosition(isolate_but, "right", 2, 66)
        self.attachForm(isolate_but, "bottom", 2)

        self.attachNone(go_but, "top")
        self.attachForm(go_but, "right", 2)
        self.attachPosition(go_but, "left", 2, 66)
        self.attachForm(go_but, "bottom", 2)

    def on_isolate(self):

        data = self.get_pot_handle_exercise_data()
        combinations = data["combinations"]

        if not combinations:
            pm.displayWarning("No combinations")
            return

        for p in pm.ls(
            "rack|holes|holeRot*|holeTrans|*_loc|*",
            dag=True,
            leaf=True,
            type="painting",
        ):
            p.getParent().attr("visibility").set(False)

        for combo in combinations:
            for painting in pm.ls("ptg_*_b{:02d}_p{:02d}".format( combo["brush"], combo["pot"])):

                painting.attr("visibility").set(True)

                


    def on_show(self):
        data = self.get_pot_handle_exercise_data()
        uutl.show_in_window(data, title="Pot/holder  exercise stats")

    def on_go(self):

        do_dip, do_wipe = pm.checkBoxGrp(
            self.do_components_cb, query=True, valueArray2=True
        )

        kwargs = {"dip": do_dip,"wipe": do_wipe}

        data = self.get_pot_handle_exercise_data()
        robo.new()
        robo.hide()
        session = DipWipeExerciseSession(data["combinations"], **kwargs)
        session.run()
        robo.show()

    def on_load_selected_brushes(self):
        brushes = pm.ls(selection=True, dag=True, leaf=True, type="brushNode")
        self._load_brush_nodes(brushes)

    def on_load_all_brushes(self):
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
        combos = palette_utils.brush_pot_combination_ids()
        for brush in brushes:
            brush_id = self._get_main_painting_connection_id(brush)
            pot_ids = [c["pot"] for c in combos if c["brush"] == brush_id]
            pm.setParent(self.brushes_column)
            self._create_entry(brush, pot_ids, frame)
            frame += 1

    def _clear_entries(self):
        children = pm.columnLayout(self.brushes_column, q=True, ca=True)
        if children:
            pm.deleteUI(children)

    def _create_entry(self, brush, pot_ids, frame):
        text = ",".join(str(p) for p in pot_ids)
        tf = pm.textFieldGrp(columnWidth2=(300, 200), label=brush, text=text, cw2=(140, 300))
        return tf

    def get_pot_handle_exercise_data(self):

        data = {"combinations": [], "brushes": [], "pots": []}
        text_fields = pm.columnLayout(self.brushes_column, q=True, ca=True)
        for tf in text_fields:
            brush_name = pm.textFieldGrp(tf, q=True, label=True)
            brush = pm.PyNode(brush_name)

            brush_id = self._get_main_painting_connection_id(brush)

            val = pm.textFieldGrp(tf, q=True, text=True)

            for pot_id in [int(i) for i in val.split(",") if i is not None and i.isdigit()]:
                data["combinations"].append({"brush": brush_id, "pot": pot_id})
                data["brushes"].append(brush_name)

        data["pot"] = list(set([c["pot"] for c in data["combinations"]]))
        data["brushes"] = list(set(data["brushes"]))

        return data
