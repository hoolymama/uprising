from uprising import palette_utils

import pymel.core as pm
import pymel.core.uitypes as gui

# from uprising import robo
from uprising import utils as utils
from uprising import const as k


# from uprising.bot.session.dip_wipe_exercise_session import DipWipeExerciseSession


class segmentTab(gui.FormLayout):
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
            label="Connect segments",
        )

        self.add_buttons_row = pm.rowLayout(
            numberOfColumns=4,
            columnWidth4=(200, 200, 200, 200),
            adjustableColumn=2,
            columnAlign=(1, "right"),
            columnAttach=[(1, "both", 2), (2, "both", 2), (2, "both", 2), (2, "both", 2)],
        )
        pm.button(label="Load cImgSplit", command=pm.Callback(self.on_load_split_node))

        self.splitField = pm.nameField(en=False)

        pm.button(label="Load cImgMerge nodes", command=pm.Callback(self.on_load_merges))

        pm.button(label="Clear", command=pm.Callback(self.on_clear))

        pm.setParent("..")

        pm.scrollLayout(bv=True)
        self.merges_column = pm.columnLayout(adj=True)

        pm.setParent("..")
        return frame

    def create_action_buttons(self):
        pm.setParent(self)  # form

        go_but = pm.button(label="Connect all", command=pm.Callback(self.on_go_all))

        self.attachForm(self.frame, "left", 2)
        self.attachForm(self.frame, "right", 2)
        self.attachForm(self.frame, "top", 2)
        self.attachControl(self.frame, "bottom", 2, go_but)

        self.attachNone(go_but, "top")
        self.attachForm(go_but, "right", 2)
        self.attachForm(go_but, "left", 2)
        self.attachForm(go_but, "bottom", 2)

    def on_go_all(self):
        for tf in pm.columnLayout(self.merges_column, q=True, ca=True):
            self.on_go_one(tf)


    def on_go_one(self, textField):
        split = pm.PyNode(pm.nameField(self.splitField, q=True, o=True))
        merge_name = pm.textFieldButtonGrp(textField, q=True, label=True)
        for att in pm.PyNode(merge_name).attr("input"):
            pm.removeMultiInstance(att, b=True)
            print("Removing multi instance", att)
            

        val = pm.textFieldButtonGrp(textField, q=True, text=True)
        indices = [int(i) for i in val.split(",") if i is not None and i.isdigit()]
        for i, index in enumerate(indices):
            src = split.attr("output[{}].outputImage".format(index))
            dest = pm.PyNode(merge_name).attr("input[{}]".format(i))
            print("Connecting {} to {}".format(src, dest))
            src >> dest

 
    def on_load_split_node(self):
        try:
            split = pm.ls(selection=True, type="cImgFileSplit")[0]
        except IndexError:
            split = pm.PyNode("cImgFileSplit_m")
        pm.nameField(self.splitField, e=True, o=split)

    def on_load_merges(self):
 
        merges = sorted(pm.ls(selection=True, type="cImgMergeDilate"))
        if not merges:
            merges = pm.ls("*cImgMergeDilate_m_*")
        self._load_merges(merges)

    def on_clear(self):
        self._clear_entries()

    def _load_merges(self, merges):
        self._clear_entries()
        if not merges:
            return
        frame = 0
        pm.setParent(self.merges_column)
        for merge in merges:
            conns = merge.attr("input").connections(s=True, c=True, p=True, d=False)
            src_indices = [conn[1].parent().index() for conn in conns]
            self._create_entry(merge, src_indices, frame)
            frame += 1
        pm.setParent(self)  # form

    def _clear_entries(self):
        children = pm.columnLayout(self.merges_column, q=True, ca=True)
        if children:
            pm.deleteUI(children)

    def _create_entry(self, merge, src_indices, frame):
        text = ",".join(str(i) for i in src_indices)
        tf = pm.textFieldButtonGrp(
            columnWidth3=(300, 200,100), 
            label=merge, 
            text=text,
            buttonLabel="Connect")

        pm.textFieldButtonGrp(tf, e=True, buttonCommand=pm.Callback(self.on_go_one, tf))

 