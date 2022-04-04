
from operator import ge
from uprising import progress
import pymel.core as pm
import pymel.core.uitypes as gui
from uprising import utils, persist_ui

from uprising.pov.session.pov_session import PovSession, RUNMODE_OFF, RUNMODE_OFFLINE, RUNMODE_ROBOT


class PovPublishTab(gui.FormLayout):
    def __init__(self):
        self.setNumberOfDivisions(100)
        pm.setParent(self)
        self.column = pm.columnLayout()
        self.column.adjustableColumn(True)

        self.export_frame = self.create_export_frame()
        
        self.progress_frame = self.create_progress_frame()

        pm.setParent(self)
        self.go_but = self.create_action_buttons()


        prefix =  "upov_pov"
        self.persistentWidgets = [
            persist_ui.factory(self, "stroke_chunk_if", prefix, default_value=600),
            persist_ui.factory(self, "anim_cb", prefix, default_value=[False]),
            persist_ui.factory(self, "sim_options_rb", prefix, default_value=RUNMODE_OFFLINE),
            persist_ui.factory(self, "robot_file_options_cb", prefix, default_value=[False, True]),
            persist_ui.factory(self, "maya_file_options_cb", prefix, default_value=[True, True]),
            persist_ui.factory(self, "description_tf", prefix, default_value="")
        ]

        pm.setParent(self)


        self.populate()
        self.on_ops_change()

    def create_export_frame(self):

        frame = pm.frameLayout(label="Export", bv=True)

        self.stroke_chunk_if = pm.intFieldGrp(
            height=30,
            label="Max strokes/file",
            annotation="Max number of strokes per painting partial program",
            numberOfFields=1
        )

        self.anim_cb =  pm.checkBoxGrp(
            numberOfCheckBoxes=1,
            label="Animated",
            changeCommand=pm.Callback(self.on_ops_change)
        )

        self.sim_options_rb = pm.radioButtonGrp(
            label='Simulation', sl=RUNMODE_OFFLINE,
            labelArray3=[
                "Off",
                "Offline Sim",
                "Run on Robot"
                ],
            numberOfRadioButtons=3,
            changeCommand=pm.Callback(self.on_ops_change)
        )

        self.pause_options_rb = pm.radioButtonGrp(
            label='Pause after frame', sl=2,
            labelArray3=[
                "Off",
                "30 seconds",
                "Indefinite"
                ],
            numberOfRadioButtons=3,
            changeCommand=pm.Callback(self.on_ops_change)
        )


        self.robot_file_options_cb =  pm.checkBoxGrp(
            numberOfCheckBoxes=2,
            label="Save Sim",
            label1="RDK",
            label2="SRC",
            changeCommand=pm.Callback(self.on_ops_change)
        )
        
        self.maya_file_options_cb =  pm.checkBoxGrp(
            numberOfCheckBoxes=2,
            label="Save Maya",
            label1="Scene file",
            label2="Snapshots",
            changeCommand=pm.Callback(self.on_ops_change)
        )

        self.description_tf = pm.scrollField(
            nl=5, wordWrap=True, text="")

        pm.setParent("..")
        return frame

    def create_progress_frame(self):
        frame = pm.frameLayout(
            bv=True, collapse=False, labelVisible=True, label="Progress"
        )
        col = pm.columnLayout(adj=True)
        progress.create(col)
        return frame

    def create_action_buttons(self):
        pm.setParent(self)  # form
        save_but = pm.button(label='Save', command=pm.Callback(self.save))
        go_but = pm.button(label="Go", command=pm.Callback(self.on_go))

        self.attachForm(self.column, "left", 2)
        self.attachForm(self.column, "right", 2)
        self.attachForm(self.column, "top", 2)
        self.attachControl(self.column, "bottom", 2, save_but)


        self.attachNone(save_but, 'top')
        self.attachForm(save_but, 'left', 2)
        self.attachPosition(save_but, 'right', 2, 50)
        self.attachForm(save_but, 'bottom', 2)


        self.attachNone(go_but, "top")
        self.attachForm(go_but, "right", 2)
        self.attachPosition(go_but, 'left', 2, 50)
        self.attachForm(go_but, "bottom", 2)
        return go_but

    ##############################################################

    def on_ops_change(self):

        run_mode = pm.radioButtonGrp(self.sim_options_rb, q=True , sl=True)
        # 1=0ff, 2=offline, 3=robot

        pm.checkBoxGrp(self.robot_file_options_cb, e=True, en=run_mode==RUNMODE_OFFLINE)

        do_anim = pm.checkBoxGrp(self.anim_cb, q=True, value1=True)
        pm.radioButtonGrp(self.pause_options_rb, e=True , en=(do_anim and (run_mode!=RUNMODE_OFF)))

    def on_go(self):



        self.save()
        do_anim = pm.checkBoxGrp(self.anim_cb, q=True, value1=True)

        if do_anim:
            start_frame = int(pm.playbackOptions(q=True, min=True))
            end_frame = int(pm.playbackOptions(q=True, max=True))
        else:
            start_frame = int(pm.currentTime(q=True))
            end_frame = start_frame



        stroke_chunk_size = pm.intFieldGrp(
            self.stroke_chunk_if, query=True, value1=True
        )

        run_mode = pm.radioButtonGrp(self.sim_options_rb, q=True , sl=True)

        pause_index = 0
        if do_anim:
            pause_index = pm.radioButtonGrp(self.pause_options_rb, q=True , sl=True) -1
        pause = [0, 30000, -1][pause_index]


        save_rdk = pm.checkBoxGrp(self.robot_file_options_cb, q=True , value1=True)
        save_src = pm.checkBoxGrp(self.robot_file_options_cb, q=True , value2=True)

        save_maya_scene = pm.checkBoxGrp(self.maya_file_options_cb, q=True , value1=True)
        save_snapshots = pm.checkBoxGrp(self.maya_file_options_cb, q=True , value2=True)

        notes = pm.scrollField(self.description_tf, query=True, text=True)
        if (len(notes) < 6) and (save_rdk or save_src or save_maya_scene or save_snapshots):
            raise ValueError("You must enter some notes if you are saving any files.")


        try:
            pov_session = PovSession(
                stroke_chunk_size, 
                run_mode, 
                save_rdk, 
                save_src,
                save_maya_scene,
                save_snapshots,
                notes,
                start_frame,
                end_frame,
                pause,
                program_prefix="pv")
            pov_session.run()
        except ValueError:
            print("PovSession aborted")
            raise

        

    def populate(self):
        for persister in self.persistentWidgets:
            persister.populate()

    def save(self):
        for persister in self.persistentWidgets:
            persister.save()

