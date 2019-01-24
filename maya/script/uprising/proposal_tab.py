import sys
import os
import pymel.core as pm
import write
import datetime

import stroke_factory_utils as sfu
import pymel.core.uitypes as gui
 

class ProposalTab(gui.FormLayout):

    def __init__(self):
        self.setNumberOfDivisions(100)
        pm.setParent(self)
        self.column = pm.columnLayout()
        self.column.adjustableColumn(True)
        self.create_ui()
        self.create_action_buttons()

        try:
            self.on_load_notes()
        except:
            pass

        self.on_current_frame_cb_change()

    def create_ui(self):
        pm.setParent(self.column)

        self.create_description_ui()

        pm.setParent(self.column)
        min_frame = int(pm.playbackOptions(min=True, query=True))
        max_frame = int(pm.playbackOptions(max=True, query=True))



        self.clean_top_view_cb = pm.checkBoxGrp(
            numberOfCheckBoxes=1,
            h=30,
            label='Force clean top view',
            value1=1,
            annotation='Make sure the view is clean and framed on the painting from above'
            )

        self.make_top = pm.button(
            h=50,
            label='MAKE CLEAN TOP',
            command=pm.Callback(self.on_make_clean_top)
            )

 
        pm.rowLayout(
             h=30,
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



    

        # self.save_maya_only_cb = pm.checkBoxGrp(
        #     label='Skip RoboDK',
        #     ann="Save Maya files only",
        #     value1=False)

        # self.save_unfiltered_snapshot = pm.checkBoxGrp(
        #     label='Save unfiltered snapshot',
        #     ann="This will switch off filtering temporarily",
        #     value1=True)

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

 
    def on_current_frame_cb_change(self):
        state = pm.checkBox(self.current_frame_cb, query=True, value=True)
        pm.intFieldGrp(self.frame_if, edit=True, enable=(not state))

    def create_description_ui(self):

        form = pm.formLayout()

        self.description_tf = pm.scrollField(
            nl=5, wordWrap=True, text="Description...")

        # self.ground_tf = pm.textField()
        # self.medium_tf = pm.textField()
        # self.palette_name_tf = pm.textField()

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

        # form.attachNone(self.medium_tf, 'top')
        # form.attachForm(self.medium_tf, 'left', 2)
        # form.attachForm(self.medium_tf, 'bottom', 2)
        # form.attachPosition(self.medium_tf, 'right', 2, 27)

        # form.attachNone(self.palette_name_tf, 'top')
        # form.attachControl(self.palette_name_tf, 'left', 2, self.medium_tf)
        # form.attachForm(self.palette_name_tf, 'bottom', 2)
        # form.attachPosition(self.palette_name_tf, 'right', 2, 54)

        # form.attachNone(self.ground_tf, 'top')
        # form.attachControl(self.ground_tf, 'left', 2, self.palette_name_tf)
        # form.attachForm(self.ground_tf, 'bottom', 2)
        # form.attachControl(self.ground_tf, 'right', 2, save_but)

        form.attachForm(self.description_tf, 'left', 2)
        form.attachControl(self.description_tf, 'right', 2, load_but)
        form.attachForm(self.description_tf, 'top', 2)
        form.attachForm(self.description_tf, 'bottom', 2)

    def on_load_notes(self):
        assembly= pm.PyNode("mainPaintingGroup")
        notes_att, _, _, _ = sfu.ensure_painting_has_notes(assembly)
        notes = notes_att.get()
 
        pm.scrollField(self.description_tf, edit=True, text=notes)
 

    def on_save_notes(self):
        assembly= pm.PyNode("mainPaintingGroup")
        notes = pm.scrollField(self.description_tf, query=True, text=True)
 
        notes_att, _, _, _ = sfu.ensure_painting_has_notes(assembly)
        notes_att.set(notes)
 

    def create_action_buttons(self):
        pm.setParent(self)  # form

        save_but = pm.button(label='Save', command=pm.Callback(self.save))
        go_but = pm.button(
            label='Go', command=pm.Callback(
                self.on_write))

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
        # val = "default"
        # if "up_setup_palette_name" in pm.optionVar:
        #     val = pm.optionVar["up_setup_palette_name"]
        # print val
        # pm.textFieldGrp(self.setup_paints_tf, e=True, text=val)

    def save(self):
        pass
        # val = pm.textFieldButtonGrp(self.setup_paints_tf, q=True, text=True)
        # pm.optionVar["up_setup_palette_name"] = val
     


    def on_make_clean_top(self):
        painting_node = pm.PyNode("mainPaintingShape")
        write.setup_clean_top(painting_node)

    def on_write(self):

        proposals_dir = write.choose_proposal_dir()
        if not proposals_dir:
            return

        current_only = pm.checkBox(self.current_frame_cb, query=True, value=True)

        clean_top = pm.checkBoxGrp(self.clean_top_view_cb, query=True, value1=True)





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

        # maya_only = pm.checkBoxGrp(
        #     self.save_maya_only_cb, query=True, value1=True)
        # save_unfiltered_snapshot = pm.checkBoxGrp(
        #     self.save_unfiltered_snapshot, query=True, value1=True)

        # medium = pm.textField(self.medium_tf, query=True, text=True)
        # ground = pm.textField(self.ground_tf, query=True, text=True)

        write.publish_proposal(

            proposals_dir,
            painting_node,
            desc,
            frames,
            clean_top)


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
