import logging
import math
import datetime
import time
import os
import tarfile
import pymel.core as pm

# from uprising.pov.session.pov_target import PovTarget

from uprising import utils
from uprising import robo
from uprising import progress
from uprising.common.session.session import Session
from uprising.pov.session.pov_program import PovProgram
from uprising.pov.session.pov_target import PovTarget

import maya.api.OpenMaya as om
from uprising.brush import Brush

logger = logging.getLogger("uprising")


class PovSession(Session):
    def __init__(self, 
        stroke_chunk_size, 
        run_on_robot, 
        save_rdk, 
        save_src,
        start_frame, 
        end_frame,
        program_prefix="pv"):

        self.frame_range=range(start_frame, end_frame+1)
        self.program_prefix = program_prefix
        self.stroke_chunk_size = stroke_chunk_size

        self.run_on_robot = run_on_robot
        self.save_rdk = save_rdk
        self.save_src = save_src

        self.directory = None
        if self.save_rdk or self.save_src:
            self.directory = self.choose_session_dir()
            if not self.directory: 
                print("No file chosen. Aborted")
                raise ValueError("No valid directory chosen.")

        self.painting_node = pm.PyNode("lightPaintingShape")

        self.stats = {}

    def run(self):
        timer_start = time.time()

        all_program_names = []
        
        for f in self.frame_range:
            pm.currentTime(f)
            stroke_count = pm.lightPaintingQuery(self.painting_node, sc=True)
            if self.save_rdk or self.save_src:
                stroke_chunk_size = self.stroke_chunk_size or stroke_count
            else:
                stroke_chunk_size = stroke_count

            program_prefix = "{}_f{:03d}".format(self.program_prefix, f)
            robo.new()
            robo.clean("kr8")
            robo.hide()
            program = PovProgram(program_prefix, self.run_on_robot)
            if not (program and program.painting and program.painting.strokes):
                raise ValueError("Invalid bot_program. No painting/clusters")

            program.configure()

            # SEND
            program_names = self.send_and_publish_pov_program(program, stroke_chunk_size)
            all_program_names.extend(program_names)
            
        if len(all_program_names) > 1:
            self.orchestrate(self.directory, all_program_names)

        duration = int(time.time() - timer_start)
        progress.update(
            major_line="Total time: {}".format(str(datetime.timedelta(seconds=duration))),
            minor_line="",
            major_progress=0,
            minor_progress=0,
        )

        self.stats = {}

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


PROGRAM_NAME = "povtest"
X = 120
Y = -5
Z = 10
FRONT = pm.dt.Vector(1, 0, 0)
SIDE = pm.dt.Vector(0, 1, 0)
UP = pm.dt.Vector(0, 0, -1)


class PovTestSession(Session):
    def __init__(self):

        self.directory = "/Volumes/xtr/gd/pov/export/test"
        self.brush = Brush(0, pm.PyNode("fbx_0_slot_00_fo1_roundShape").attr("outPaintBrush"))

        robo.new()
        robo.clean("kr8")
        robo.hide()
        _id = "{}_{}_{}".format(X, Y, Z)
        mat = om.MMatrix([list(UP) + [0], list(SIDE) + [0], list(FRONT) + [0], [X, Y, Z, 1]])

        mtmat = om.MTransformationMatrix(mat)

        position = mtmat.translation(om.MSpace.kWorld)
        xyz_rotation = mtmat.rotation(False)
        zyx_rotation = xyz_rotation.reorder(om.MEulerRotation.kZYX)
        try:
            t = PovTarget(
                _id, list(position * 10), list(zyx_rotation), pm.dt.Color(1, 0, 0, 0.5), self.brush
            )
        except utils.StrokeError:
            return
        print("Making Frame for", PROGRAM_NAME)
        frame = robo.create_frame("POV_frame")
        print("Making Program for", PROGRAM_NAME)
        program = robo.create_program(PROGRAM_NAME)
        print("Made Program for", PROGRAM_NAME)

        self.brush.send()
        t.configure("001")
        t.send("test_", program, frame, None)

        self.save_program(self.directory, PROGRAM_NAME)
        self.save_station(self.directory, PROGRAM_NAME)
        robo.show()
