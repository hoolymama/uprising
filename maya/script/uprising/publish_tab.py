import sys
import os
import pymel.core as pm
import write
import datetime
# import setup_dip
# import curve_utils as cutl
# import brush_utils as butl
# import paint_utils as putl
import uprising_util as uutl
import setup_dip
import stroke_factory_utils as sfu
import pymel.core.uitypes as gui
# import painting as pnt
from studio import Studio

import callbacks
from robolink import (
    Robolink
)


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

        self.create_description_ui()

        pm.setParent(self.column)
        min_frame = int(pm.playbackOptions(min=True, query=True))
        max_frame = int(pm.playbackOptions(max=True, query=True))

        with uutl.activatable(state=False):
            # self.pre_frame_py_tf = pm.textFieldGrp(
            #     label="Pre-frame python"
            # )

            self.pre_frame_py_tf = pm.textFieldButtonGrp(
                label='Pre-frame python',
                text='load_numbered_brush_pouch,test_set',
                buttonLabel='Go',
                columnWidth3=(
                    140,
                    200,
                    50),
                buttonCommand=pm.Callback(
                    self.test_python_callback))

        pm.rowLayout(numberOfColumns=2,
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

        self.save_maya_only_cb = pm.checkBoxGrp(
            label='Skip RoboDK',
            ann="Save Maya files only",
            value1=False)

        self.save_unfiltered_snapshot = pm.checkBoxGrp(
            label='Save unfiltered snapshot',
            ann="This will switch off filtering temporarily",
            value1=True)

        self.snap_now_if = pm.floatSliderButtonGrp(
            label='Make snapshot now',
            field=True,
            maxValue=4096,
            step=1,
            value=1024,
            symbolButtonDisplay=False,
            buttonLabel="Go",
            buttonCommand=pm.Callback(self.on_make_snapshot),
            columnWidth=(4, 60)
        )

        # self.send_paintings_cb = pm.checkBoxGrp(
        #     numberOfCheckBoxes=2,
        #     height=30,
        #     label='Send Clusters',
        #     labelArray2=['Painting', 'Dips'],
        #     valueArray2=[True, True],
        #     columnWidth3=(180, 120, 120))

        # self.send_board_cb = pm.checkBoxGrp(
        #     label='Send Board',
        #     value1=1,
        #     height=30,
        #     columnWidth2=(
        #         180,
        #         120))

        # pm.rowLayout(
        #     numberOfColumns=4, columnWidth4=(
        #         100, 80, 80, 80), adjustableColumn=1, columnAlign=(
        #         1, 'right'), columnAttach=[
        #         (1, 'both', 2), (2, 'both', 2), (3, 'both', 2), (4, 'both', 2)])

        # pm.text(label='Export to RoboDK')

        # pm.button(
        #     label='Painting only',
        #     command=pm.Callback(self.on_create_painting))

        # pm.button(
        #     label='Dips only',
        #     command=pm.Callback(self.on_create_dips))

        # pm.button(
        #     label='All',
        #     command=pm.Callback(self.on_create_all))

        # pm.setParent('..')

        # self.description_tf = pm.scrollField(
        #     wordWrap=True, text="Description...")
        # pm.button(
        #     label='Write program package',
        #     command=pm.Callback(self.write_program_package))

        # pm.button(
        #     label='Write maya package only',
        #     command=pm.Callback(self.write_maya_package))

        # pm.rowLayout(
        #     numberOfColumns=2, columnWidth2=(
        #         150, 200), adjustableColumn=1, columnAlign=(
        #         1, 'right'), columnAttach=[
        #         (1, 'both', 2), (2, 'both', 2)])

        # self.frame_if = pm.intFieldGrp(label="Frames to run", numberOfFields=2, value1=1, value2=1 )

        # pm.button(
        #     label='Export and write packages',
        #     command=pm.Callback(self.on_export_and_write_series))

    def on_current_frame_cb_change(self):
        state = pm.checkBox(self.current_frame_cb, query=True, value=True)
        pm.intFieldGrp(self.frame_if, edit=True, enable=(not state))

    def create_description_ui(self):

        form = pm.formLayout()

        self.description_tf = pm.scrollField(
            nl=5, wordWrap=True, text="Description...")

        self.ground_tf = pm.textField()
        self.medium_tf = pm.textField()
        self.palette_name_tf = pm.textField()

        load_but = pm.button(
            width=100,
            label="Load",
            command=pm.Callback(self.on_load_notes))
        save_but = pm.button(
            width=100,
            label="Save",
            command=pm.Callback(self.on_save_notes))

        form.attachForm(load_but, 'top', 2)
        form.attachPosition(load_but, 'left', 2, 80)
        form.attachPosition(load_but, 'bottom', 2, 50)
        form.attachForm(load_but, 'right', 2)

        form.attachPosition(save_but, 'top', 2, 50)
        form.attachPosition(save_but, 'left', 2, 80)
        form.attachForm(save_but, 'bottom', 2)
        form.attachForm(save_but, 'right', 2)

        form.attachNone(self.medium_tf, 'top')
        form.attachForm(self.medium_tf, 'left', 2)
        form.attachForm(self.medium_tf, 'bottom', 2)
        form.attachPosition(self.medium_tf, 'right', 2, 27)

        form.attachNone(self.palette_name_tf, 'top')
        form.attachControl(self.palette_name_tf, 'left', 2, self.medium_tf)
        form.attachForm(self.palette_name_tf, 'bottom', 2)
        form.attachPosition(self.palette_name_tf, 'right', 2, 54)

        form.attachNone(self.ground_tf, 'top')
        form.attachControl(self.ground_tf, 'left', 2, self.palette_name_tf)
        form.attachForm(self.ground_tf, 'bottom', 2)
        form.attachControl(self.ground_tf, 'right', 2, save_but)

        form.attachForm(self.description_tf, 'left', 2)
        form.attachControl(self.description_tf, 'right', 2, load_but)
        form.attachForm(self.description_tf, 'top', 2)
        form.attachControl(self.description_tf, 'bottom', 2, self.medium_tf)

    def on_load_notes(self):
        assembly = pm.PyNode("mainPaintingGroup")
        notes_att, ground_att, medium_att, palette_name_att = sfu.ensure_painting_has_notes(
            assembly)
        notes = notes_att.get()
        ground = ground_att.get()
        medium = medium_att.get()
        palette_name = palette_name_att.get()

        pm.scrollField(self.description_tf, edit=True, text=notes)
        pm.textField(self.ground_tf, edit=True, text=ground)
        pm.textField(self.medium_tf, edit=True, text=medium)
        pm.textField(self.palette_name_tf, edit=True, text=palette_name)

    def on_save_notes(self):
        assembly = pm.PyNode("mainPaintingGroup")
        notes = pm.scrollField(self.description_tf, query=True, text=True)
        medium = pm.textField(self.medium_tf, query=True, text=True)
        ground = pm.textField(self.ground_tf, query=True, text=True)
        palette_name = pm.textField(
            self.palette_name_tf, query=True, text=True)
        notes_att, ground_att, medium_att, palette_name_att = sfu.ensure_painting_has_notes(
            assembly)
        notes_att.set(notes)
        ground_att.set(ground)
        medium_att.set(medium)
        palette_name_att.set(palette_name)

    def create_action_buttons(self):
        pm.setParent(self)  # form

        cancel_but = pm.button(label='Cancel')
        go_but = pm.button(
            label='Go', command=pm.Callback(
                self.on_export_and_write_series))

        self.attachForm(self.column, 'left', 2)
        self.attachForm(self.column, 'right', 2)
        self.attachForm(self.column, 'top', 2)
        self.attachControl(self.column, 'bottom', 2, cancel_but)

        self.attachNone(cancel_but, 'top')
        self.attachForm(cancel_but, 'left', 2)
        self.attachPosition(cancel_but, 'right', 2, 50)
        self.attachForm(cancel_but, 'bottom', 2)

        self.attachNone(go_but, 'top')
        self.attachForm(go_but, 'right', 2)
        self.attachPosition(go_but, 'left', 2, 50)
        self.attachForm(go_but, 'bottom', 2)

    # def write(self, painting_node, dip_node):
    #     RL = Robolink()
    #     RL.setWindowState(-1)
    #     try:
    #         studio = Studio(painting_node, dip_node)
    #         studio.write()
    #     except Exception:
    #         t, v, tb = sys.exc_info()
    #         RL.setWindowState(2)
    #         raise t, v, tb
    #     RL.setWindowState(2)

    # def on_create_painting(self):
    #     painting_node = pm.PyNode("mainPaintingShape")
    #     self.write(painting_node, None)

    # def on_create_dips(self):
    #     dip_node = pm.PyNode("dipPaintingShape")
    #     self.write(None, dip_node)

    # def on_create_all(self):
    #     painting_node = pm.PyNode("mainPaintingShape")
    #     dip_node = pm.PyNode("dipPaintingShape")
    #     self.write(painting_node, dip_node)

    # def write_program_package(self):
    #     desc = pm.scrollField(self.description_tf, q=True, text=True)
    #     export_dir = write.choose_publish_dir()
    #     if export_dir:
    #         write.export_package(export_dir, desc)

    # def write_maya_package(self):
    #     desc = pm.scrollField(self.description_tf, q=True, text=True)
    #     export_dir = write.choose_publish_dir()
    #     if export_dir:
    #         write.export_maya_package_only(export_dir, desc)


    def test_python_callback(self):

        pre_frame_py = pm.textFieldButtonGrp(
            self.pre_frame_py_tf,
            q=True,
            text=True)

        kw = {
            "frame": pm.currentTime(q=True),
            "painting_node" : pm.PyNode("mainPaintingShape"),
            "dip_node": pm.PyNode("dipPaintingShape")
        }

        args = pre_frame_py.split(",")
        cmd = args[0]
        args = [uutl.numeric(a.strip()) for a in args[1:]]
        method = getattr(callbacks, cmd)
        res = method(*args, **kw)
        print res



    def on_export_and_write_series(self):

        export_dir = write.choose_publish_dir()
        if not export_dir:
            return

        current_only = pm.checkBox(
            self.current_frame_cb, query=True, value=True)

        if current_only:
            frame = int(pm.currentTime(query=True))
            frames = (frame, frame)
        else:
            frames = (
                pm.intFieldGrp(
                    self.frame_if,
                    query=True,
                    value1=True),
                pm.intFieldGrp(
                    self.frame_if,
                    query=True,
                    value2=True))

        desc = pm.scrollField(self.description_tf, q=True, text=True)
        painting_node = pm.PyNode("mainPaintingShape")
        dip_node = pm.PyNode("dipPaintingShape")

        maya_only = pm.checkBoxGrp(
            self.save_maya_only_cb, query=True, value1=True)
        save_unfiltered_snapshot = pm.checkBoxGrp(
            self.save_unfiltered_snapshot, query=True, value1=True)

        medium = pm.textField(self.medium_tf, query=True, text=True)
        ground = pm.textField(self.ground_tf, query=True, text=True)

        do_pre_frame_py = pm.textFieldButtonGrp(
            self.pre_frame_py_tf, q=True, en=True)
        pre_frame_py = pm.textFieldButtonGrp(
            self.pre_frame_py_tf,
            q=True,
            text=True) if do_pre_frame_py else None

        write.publish_sequence(
            export_dir,
            painting_node,
            dip_node,
            desc,
            medium,
            ground,
            frames,
            maya_only,
            save_unfiltered_snapshot,
            pre_frame_py
        )

    def on_make_snapshot(self):
        res = pm.floatSliderButtonGrp(self.snap_now_if, query=True, value=True)
        res = int(res)
        export_dir = os.path.join(pm.workspace.getPath(), 'export')
        entries = pm.fileDialog2(caption="Choose directory", okCaption="Save",
                                 dialogStyle=2, fileMode=3, dir=export_dir)
        if not entries:
            pm.displayWarning('Nothing Selected')
            return
        timestamp = datetime.datetime.now().strftime('%y%m%d_%H%M%S')
        write.write_ref_image(entries[0], timestamp, res)
