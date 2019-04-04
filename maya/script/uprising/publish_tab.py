import sys
import os
import re
import pymel.core as pm
import write
import datetime
import uprising_util as uutl
import stroke_factory_utils as sfu
import pymel.core.uitypes as gui

import callbacks


class PublishTab(gui.FormLayout):

    def __init__(self):
        self.setNumberOfDivisions(100)
        pm.setParent(self)
        self.column = pm.columnLayout()
        self.column.adjustableColumn(True)
        self.create_ui()
        self.create_action_buttons()

        try:
            self.on_load_notes()
        except BaseException:
            pass

        self.on_current_frame_cb_change()
 
    def create_ui(self):
        pm.setParent(self.column)

        self.context_rb = pm.radioButtonGrp(
            height=30,
            label='Context',
            labelArray3=[
                "Bayonet painting",
                "Gripper painting",
                "Snapshot only"],
            sl=1,
            numberOfRadioButtons=3
            # ,changeCommand=pm.Callback(  self.on_context_rb_change   )

        )


        pm.setParent(self.column)
        min_frame = int(pm.playbackOptions(min=True, query=True))
        max_frame = int(pm.playbackOptions(max=True, query=True))

 
        self.anim_row = pm.rowLayout(height=30,
                                     numberOfColumns=2,
                                     columnWidth2=(
                                         (390), 100),
                                     # adjustableColumn=1,
                                     columnAlign=(1, 'right'),
                                     columnAttach=[(1, 'both', 2), (2, 'both', 2)])

        self.frame_if = pm.intFieldGrp(
     
            label="Frames to run",
            numberOfFields=2,
            value1=min_frame,
            value2=max_frame)

        self.current_frame_cb = pm.checkBox(
            label='Current',
            value=1,
            annotation='Use current frame only',
            changeCommand=pm.Callback(self.on_current_frame_cb_change)
        )

        pm.setParent('..')

 
        self.save_unfiltered_snapshot = pm.checkBoxGrp(
            height=30,
            label='Save unfiltered snapshot',
            ann="This will switch off filtering temporarily",
            value1=False)

        self.snap_size_if = pm.intFieldGrp(
            height=30,
            label='Snapshot size',
            value1=1024
        )

    def on_current_frame_cb_change(self):
        state = pm.checkBox(self.current_frame_cb, query=True, value=True)
        pm.intFieldGrp(self.frame_if, edit=True, enable=(not state))
 

    def create_action_buttons(self):
        pm.setParent(self)  # form

        save_but = pm.button(label='Save', command=pm.Callback(self.save))
        go_but = pm.button(
            label='Go', command=pm.Callback(
                self.on_go))

        self.attachForm(self.column, 'left', 2)
        self.attachForm(self.column, 'right', 2)
        self.attachForm(self.column, 'top', 2)
        self.attachControl(self.column, 'bottom', 2, save_but)

        self.attachNone(save_but, 'top')
        self.attachForm(save_but, 'left', 2)
        self.attachPosition(save_but, 'right', 2, 50)
        self.attachForm(save_but, 'bottom', 2)

        self.attachNone(go_but, 'top')
        self.attachForm(go_but, 'right', 2)
        self.attachPosition(go_but, 'left', 2, 50)
        self.attachForm(go_but, 'bottom', 2)

    def populate(self):
        pass
 
    def save(self):
        pass
 

    def _get_frames(self):
        current_only = pm.checkBox(
            self.current_frame_cb, query=True, value=True)

        if current_only:
            frame = int(pm.currentTime(query=True))
            return (frame, frame)

        return (
            pm.intFieldGrp(
                self.frame_if,
                query=True,
                value1=True),
            pm.intFieldGrp(
                self.frame_if,
                query=True,
                value2=True))

    def on_go(self):

        context = pm.radioButtonGrp(
            self.context_rb, query=True, sl=True)

        if context == 3:
            # snapshot
            self.make_snapshot()
            return

        export_dir = write.choose_publish_dir()
        if not export_dir:
            return

        painting_node = pm.PyNode("mainPaintingShape")

 
        current_only = pm.checkBox(
            self.current_frame_cb, query=True, value=True)
        frames = self._get_frames()

        save_unfiltered_snapshot = pm.checkBoxGrp(
            self.save_unfiltered_snapshot, query=True, value1=True)

        auto = (context == 2)
        write.publish_sequence(
            export_dir,
            frames,
            auto,
            save_unfiltered_snapshot
        )

    def make_snapshot(self):
        res = pm.intFieldGrp(self.snap_size_if, query=True, value1=True)
        export_dir = os.path.join(pm.workspace.getPath(), 'export')
        entries = pm.fileDialog2(caption="Choose directory", okCaption="Save",
                                 dialogStyle=2, fileMode=3, dir=export_dir)
        if not entries:
            pm.displayWarning('Nothing Selected')
            return
        timestamp = datetime.datetime.now().strftime('%y%m%d_%H%M%S')
        write.write_ref_image(entries[0], timestamp, res)
