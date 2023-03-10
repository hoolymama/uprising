import os
import logging
from operator import ge
from uprising import progress
import pymel.core as pm
import datetime
import pymel.core.uitypes as gui
from uprising import utils, persist_ui

from uprising.pov.session.pov_session import (
    PovSession,
    RUNMODE_OFF,
    RUNMODE_OFFLINE,
    RUNMODE_ROBOT,
)

logger = logging.getLogger(__name__)
 
 
DIRECTORY_PREFIX_TEMPLATE = "{project}/export/bars"
DESCRIPTION_TEMPLATE = """export
{description}
"""
POST_TEMPLATE = 'rclone sync --fast-list --update --use-server-modtime {directory} s3uprising:uprisingdata/{directory} --exclude "._*" --exclude ".*" --exclude "*Icon*"'


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

        pm.setParent(self)

        prefix = "upov_pov"
        self.persistentWidgets = [
            persist_ui.factory(self, "stroke_chunk_if", prefix, default_value=600),
            persist_ui.factory(self, "anim_cb", prefix, default_value=[False]),
            persist_ui.factory(self, "frame_range_if", prefix, default_value=[1, 1]),
            persist_ui.factory(self, "pause_options_rb", prefix, default_value=2),
            persist_ui.factory(
                self, "sim_options_rb", prefix, default_value=RUNMODE_OFFLINE
            ),
            persist_ui.factory(
                self, "robot_file_options_cb", prefix, default_value=[False, True]
            ),
            persist_ui.factory(
                self, "maya_file_options_cb", prefix, default_value=[True, True]
            ),
            persist_ui.factory(self, "description_tf", prefix, default_value=""),
            persist_ui.factory(
                self, "directory_tf", prefix, default_value=DIRECTORY_PREFIX_TEMPLATE
            ),
            persist_ui.factory(
                self, "post_cmd_tf", prefix, default_value=POST_TEMPLATE
            ),
            persist_ui.factory(self, "do_post_cb", prefix, default_value=True),
        ]

        self.populate()
        self.on_ops_change()

    def create_export_frame(self):
        frame = pm.frameLayout(label="Export", bv=True)

        self.sim_options_rb = pm.radioButtonGrp(
            label="Simulation",
            sl=RUNMODE_OFFLINE,
            labelArray3=["Off", "Offline Sim", "Run on Robot"],
            numberOfRadioButtons=3,
            changeCommand=pm.Callback(self.on_ops_change),
        )

        self.stroke_chunk_if = pm.intFieldGrp(
            height=30,
            label="Max strokes/file",
            annotation="Max number of strokes per painting partial program",
            numberOfFields=1,
        )
        pm.separator()

        pm.rowLayout(numberOfColumns=2)
        self.anim_cb = pm.checkBoxGrp(
            numberOfCheckBoxes=1,
            label="Animated",
            changeCommand=pm.Callback(self.on_ops_change),
        )

        self.frame_range_if = pm.intFieldGrp(label="Frame range", numberOfFields=2)
        pm.setParent("..")

        self.pause_options_rb = pm.radioButtonGrp(
            label="Pause after frame",
            sl=2,
            labelArray3=["Off", "30 seconds", "Indefinite"],
            numberOfRadioButtons=3,
            changeCommand=pm.Callback(self.on_ops_change),
        )

        pm.separator()

        self.robot_file_options_cb = pm.checkBoxGrp(
            numberOfCheckBoxes=2,
            label="Save Sim",
            label1="RDK",
            label2="SRC",
            changeCommand=pm.Callback(self.on_ops_change),
        )

        self.maya_file_options_cb = pm.checkBoxGrp(
            numberOfCheckBoxes=2,
            label="Save Maya",
            label1="Scene file",
            label2="Snapshots",
            changeCommand=pm.Callback(self.on_ops_change),
        )

        self.directory_tf = pm.textFieldButtonGrp(
            adjustableColumn=2,
            label="Directory",
            buttonLabel="Browse",
            buttonCommand=pm.Callback(self.on_directory_button),
        )

        pm.rowLayout(nc=2, adj=1)
        self.post_cmd_tf = pm.textFieldGrp(label="Post Hook", adjustableColumn=2)
        self.do_post_cb = pm.checkBox(
            label="Run Post Hook", changeCommand=pm.Callback(self.on_ops_change)
        )
        pm.setParent("..")

        self.description_tf = pm.scrollField(nl=5, wordWrap=True, text="")

        pm.setParent("..")
        return frame

    def on_directory_button(self):
        directory = pm.fileDialog2(fm=3, dir=pm.workspace(q=True, rd=True))
        if directory:
            pm.textFieldButtonGrp(self.directory_tf, e=True, text=directory[0])

    def create_progress_frame(self):
        frame = pm.frameLayout(
            bv=True, collapse=False, labelVisible=True, label="Progress"
        )
        col = pm.columnLayout(adj=True)
        progress.create(col)
        return frame

    def create_action_buttons(self):
        pm.setParent(self)  # form
        save_but = pm.button(label="Save", command=pm.Callback(self.save))
        reset_but = pm.button(label="Reset", command=pm.Callback(self.reset))

        go_but = pm.button(label="Go", command=pm.Callback(self.on_go))

        self.attachForm(self.column, "left", 2)
        self.attachForm(self.column, "right", 2)
        self.attachForm(self.column, "top", 2)
        self.attachControl(self.column, "bottom", 2, save_but)

        self.attachForm(self.column, "left", 2)
        self.attachForm(self.column, "right", 2)
        self.attachForm(self.column, "top", 2)
        self.attachControl(self.column, "bottom", 2, save_but)

        self.attachNone(reset_but, "top")
        self.attachForm(reset_but, "left", 2)
        self.attachPosition(reset_but, "right", 2, 33)
        self.attachForm(reset_but, "bottom", 2)

        self.attachNone(save_but, "top")
        self.attachControl(save_but, "left", 2, reset_but)
        self.attachPosition(save_but, "right", 2, 66)
        self.attachForm(save_but, "bottom", 2)

        self.attachNone(go_but, "top")
        self.attachForm(go_but, "right", 2)
        self.attachControl(go_but, "left", 2, save_but)
        self.attachForm(go_but, "bottom", 2)
        return go_but

    ##############################################################

    def on_ops_change(self):
        """
        Manage the UI state based on the current options.
        """

        run_mode = pm.radioButtonGrp(self.sim_options_rb, q=True, sl=True)
        do_anim = pm.checkBoxGrp(self.anim_cb, q=True, value1=True)
        do_post = pm.checkBox(self.do_post_cb, q=True, value=True)

        pm.intFieldGrp(self.frame_range_if, e=True, en=do_anim)
        # 1=0ff, 2=offline, 3=robot
        pm.radioButtonGrp(
            self.pause_options_rb, e=True, en=(do_anim and (run_mode != RUNMODE_OFF))
        )

        pm.checkBoxGrp(self.robot_file_options_cb, e=True, en=run_mode != RUNMODE_OFF)
        pm.checkBoxGrp(
            self.robot_file_options_cb, e=True, en2=(run_mode != RUNMODE_ROBOT)
        )
        # we have to save rdk files if anim and run on robot.
        if do_anim and run_mode == RUNMODE_ROBOT:
            pm.checkBoxGrp(self.robot_file_options_cb, e=True, value1=True)

        if do_anim and run_mode == RUNMODE_OFFLINE:
            do_src = pm.checkBoxGrp(self.robot_file_options_cb, q=True, value2=True)
            if not do_src:
                pm.checkBoxGrp(self.robot_file_options_cb, e=True, value1=True)

        pm.intFieldGrp(self.stroke_chunk_if, e=True, en=(run_mode == RUNMODE_OFFLINE))

        saves = self.saves_something()

        pm.textFieldButtonGrp(self.directory_tf, e=True, en=saves)

        pm.textFieldGrp(self.post_cmd_tf, e=True, en=(saves and do_post))

    def saves_something(self):
        return (
            pm.checkBoxGrp(self.maya_file_options_cb, q=True, value1=True)
            or pm.checkBoxGrp(self.maya_file_options_cb, q=True, value2=True)
            or pm.checkBoxGrp(self.robot_file_options_cb, q=True, value1=True)
            or pm.checkBoxGrp(self.robot_file_options_cb, q=True, value2=True)
        )

    def on_go(self):
        self.save()
        do_anim = pm.checkBoxGrp(self.anim_cb, q=True, value1=True)

        if do_anim:
            start_frame = pm.intFieldGrp(self.frame_range_if, q=True, value1=True)
            end_frame = pm.intFieldGrp(self.frame_range_if, q=True, value2=True)
            logger.debug("Frame range: {} - {}".format(start_frame, end_frame)) 
        else:
            start_frame = int(pm.currentTime(q=True))
            end_frame = start_frame

        stroke_chunk_size = pm.intFieldGrp(
            self.stroke_chunk_if, query=True, value1=True
        )

        run_mode = pm.radioButtonGrp(self.sim_options_rb, q=True, sl=True)

        pause_index = 0
        if do_anim:
            pause_index = pm.radioButtonGrp(self.pause_options_rb, q=True, sl=True) - 1
        pause = [0, 30000, -1][pause_index]

        save_rdk = pm.checkBoxGrp(self.robot_file_options_cb, q=True, value1=True)
        save_src = pm.checkBoxGrp(self.robot_file_options_cb, q=True, value2=True)
        save_maya_scene = pm.checkBoxGrp(self.maya_file_options_cb, q=True, value1=True)
        save_snapshots = pm.checkBoxGrp(self.maya_file_options_cb, q=True, value2=True)

        saves = self.saves_something()
        directory = None
        post_cmd = None
        if saves:
            timestamp = datetime.datetime.now().strftime("%y%m%d_%H%M")
            project = pm.workspace.getPath()
            directory = pm.textFieldButtonGrp(self.directory_tf, query=True, text=True)
            directory = directory.format(project=project)
            if not directory:
                export_dir = os.path.join(project, "export")
                entries = pm.fileDialog2(
                    caption="Choose directory",
                    okCaption="Save",
                    dialogStyle=2,
                    fileMode=3,
                    dir=export_dir,
                )
                if not (entries and entries[0]):
                    pm.displayWarning("No directory Selected")
                    return
                directory = entries[0]

            directory = "{}/{}".format(directory, timestamp)

            post_cmd = pm.textFieldGrp(
                self.post_cmd_tf, query=True, en=True
            ) and pm.textFieldGrp(self.post_cmd_tf, query=True, text=True)
            if post_cmd:
                post_cmd = post_cmd.format(directory=directory)

        notes = pm.scrollField(self.description_tf, query=True, text=True) or "No notes"

        print("directory: {}".format(directory))
        print("post_cmd: {}".format(post_cmd))

        program_prefix = "pv" if run_mode == RUNMODE_OFFLINE else "ror"
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
                directory,
                post_cmd,
                program_prefix=program_prefix,
            )
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

    def reset(self):
        for persister in self.persistentWidgets:
            persister.reset()
