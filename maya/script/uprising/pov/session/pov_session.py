import logging
import math
import datetime
import time
import os
import tarfile
import pymel.core as pm
import subprocess
import shlex

# from uprising.pov.session.pov_target import PovTarget

from uprising import utils
from uprising import robo
from uprising import progress
from uprising.common.session.session import Session
from uprising.pov.session.pov_program import PovProgram
from uprising.pov.session.pov_target import PovTarget

import maya.api.OpenMaya as om
from uprising.brush import Brush

RUNMODE_OFF = 1
RUNMODE_OFFLINE = 2
RUNMODE_ROBOT = 3

logger = logging.getLogger(__name__)


class PovSession(Session):
    def __init__(
        self,
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
        do_post_cmd,
        remote_cmd,
        program_prefix="pv",
    ):
        self.frame_range = list(range(start_frame, end_frame + 1))
        self.program_prefix = program_prefix
        self.stroke_chunk_size = stroke_chunk_size
        self.pause = pause

        self.run_mode = run_mode
        self.save_rdk = save_rdk and run_mode != RUNMODE_OFF
        self.save_src = save_src and run_mode == RUNMODE_OFFLINE
        self.save_maya_scene = save_maya_scene
        self.save_snapshots = save_snapshots
        self.notes = notes
        self.directory = directory
        self.post_cmd = post_cmd
        self.do_post_cmd = do_post_cmd
        self.remote_cmd = remote_cmd

        self.painting_node = pm.PyNode("lightPaintingShape")

        self.stats = {}

    def run(self):
        timer_start = time.time()
        all_program_names = []
        if self.directory:
            if self.save_maya_scene:
                self.write_maya_scene(self.directory, "scene")
            if self.notes:
                self.write_text(self.directory, "notes", self.notes)
            if self.remote_cmd:
                remote_text =f"# POST_CMD IS:\n# {self.post_cmd}\n\n{self.remote_cmd}"
                self.write_sh(self.directory, "run_robo_batch", remote_text)
                

        robo.new()
        robo.hide()
        run_on_robot = self.run_mode == RUNMODE_ROBOT
        for frame in self.frame_range:
            pm.currentTime(frame)
            program_prefix = "{}_f{:03d}".format(self.program_prefix, frame)
            if self.save_snapshots:
                kwargs = {}
                self.write_snapshot(self.directory, "snapshot", **kwargs)

            if self.run_mode == RUNMODE_OFF:
                continue
            robo.new()
            robo.clean("kr8")
            logger.debug("Clean scene")
            # robo.hide()
            # robo.clean("kr8")

            #  Don't chunk if we are running on robot. We need all the instructions in one RDK file.
            stroke_count = pm.lightPaintingQuery(self.painting_node, sc=True)
            # if self.run_mode == RUNMODE_OFFLINE and (self.save_rdk or self.save_src):
            #     stroke_chunk_size = self.stroke_chunk_size or stroke_count
            # else:
            #     stroke_chunk_size = stroke_count
            stroke_chunk_size = self.stroke_chunk_size or stroke_count
            
            logger.debug("Stroke count: {}".format(stroke_count))

            program = PovProgram(program_prefix, run_on_robot, self.pause)
            if not (program and program.painting and program.painting.strokes):
                raise ValueError("Invalid pov_program. No painting")

            program.configure()
            logger.debug("Program configured")
            # SEND
            program_names = self.send_and_publish_pov_program(
                program, stroke_chunk_size
            )
            all_program_names.extend(program_names)
            logger.debug("End of frame {}".format(frame))

        if len(all_program_names) > 1  and self.save_src:
            self.orchestrate(self.directory, all_program_names)

        if self.do_post_cmd:
            logger.info("Running post command: '{}'".format(self.post_cmd))
            args = shlex.split(self.post_cmd)
            completed = subprocess.run(args, check=True)
            logger.info(
                "Post command completed with return code {}".format(
                    completed.returncode
                )
            )
            if completed.stdout:
                logger.info("Post command stdout:\n{}".format(completed.stdout))
            if completed.stderr:
                logger.warning("Post command Error:\n{}".format(completed.stderr))

        duration = int(time.time() - timer_start)
        progress.update(
            major_line="Total time: {}".format(
                str(datetime.timedelta(seconds=duration))
            ),
            minor_line="",
            major_progress=0,
            minor_progress=0,
        )

        self.stats = {}
        if not self.run_mode == RUNMODE_OFF:
            robo.show()

    def send_and_publish_pov_program(self, program, stroke_chunk_size):
        stroke_count = len(program.painting.strokes)
        num_chunks = int(math.ceil(stroke_count / float(stroke_chunk_size)))

        program_names = []

        # progress.update(
        #     major_max=1,
        #     header="Writing {} main program chunks".format(num_chunks)
        # )

        for i in range(num_chunks):
            robo.clean("kr8")

            # progress.update(
            #     major_progress=i, major_line="Writing {:d} of {:d} chunks".format(i+1, num_chunks))

            # self.program.send(0, self.stroke_chunk_size)
            program.send(i, stroke_chunk_size)
            if self.save_src:
                self.save_program(self.directory, program.program_name)
            if self.save_rdk:
                self.save_station(self.directory, program.program_name)

            program_names.append(program.program_name)

        return program_names
        # progress.update(major_progress=num_chunks, major_line="Done")

    # def init_progress(self):
    #     progress.update(
    #         header="Creating main painting",
    #         major_line="{} strokes in chunks of {}".format(
    #             self.stroke_count, self.stroke_chunk_size),
    #         major_progress=0,
    #         minor_progress=0
    #     )

    # def show_stats(self):
    #     utils.show_in_window(self.stats, title="Painting stats")

    # def write_stats(self):
    #     self.json_report(self.directory, "stats", self.stats)

    def make_src_tar(self):
        src_folder = os.path.join(self.directory, "src")
        with tarfile.open("{}.tar.gz".format(src_folder), "w:gz") as tar:
            tar.add(src_folder, arcname=os.path.sep)
