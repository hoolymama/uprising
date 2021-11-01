
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
    stroke_chunk_size=None, 
    directory=None,
    program_prefix="pv"
    ):

        self.directory = directory or self.choose_session_dir()
        if not self.directory:
            return

        self.painting_node = pm.PyNode("lightPaintingShape")
        self.program_names = []
        self.program_prefix = program_prefix


        self.stroke_count = pm.lightPaintingQuery(self.painting_node, sc=True)
        self.stroke_chunk_size = stroke_chunk_size or self.stroke_count

        self.stats = {}
        robo.show()

    def run(self):
        timer_start = time.time()
        # CONFIGURE EVERYTHING
        self.program = self._build_pov_program()
        self.program.configure()

        # SEND
        self.send_and_publish_pov_program()

        duration = int(time.time() - timer_start)
        progress.update(
            major_line="Total time: {}".format(
                str(datetime.timedelta(seconds=duration))),
            minor_line="",
            major_progress=0,
            minor_progress=0,
        )

        self.stats = {}

    def _build_pov_program(self):
        # self.init_progress()
        robo.new()
        robo.clean("kr8")
        program = PovProgram(self.program_prefix)
        if not (program and program.painting and program.painting.strokes):
            raise ValueError("Invalid bot_program. No painting/clusters")
        return program


    def send_and_publish_pov_program(self):
        stroke_count = len(self.program.painting.strokes)
        # num_chunks = int(
        #     math.ceil(stroke_count / float(self.stroke_chunk_size)))

        # progress.update(
        #     major_max=1,
        #     header="Writing {} main program chunks".format(num_chunks)
        # )

        # for i in range(num_chunks):

        robo.clean("kr8")
        robo.hide()

        # progress.update(
        #     major_progress=i, major_line="Writing {:d} of {:d} chunks".format(i+1, num_chunks))

        # self.program.send(0, self.stroke_chunk_size)
        self.program.send(0, 99999)

        self.save_program(
            self.directory, self.program.program_name)

        self.save_station(
            self.directory, self.program.program_name)

        self.program_names.append(self.program.program_name)

        # progress.update(major_progress=num_chunks, major_line="Done")

        # if len(self.program_names) > 1:
        #     self.orchestrate(self.directory, self.program_names)

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
        self.brush = Brush(0, pm.PyNode(
            "fbx_0_slot_00_fo1_roundShape").attr("outPaintBrush"))

        robo.new()
        robo.clean("kr8")
        robo.hide()
        _id = "{}_{}_{}".format(X, Y, Z)
        mat = om.MMatrix([list(UP)+[0], list(SIDE)+[0],
                          list(FRONT)+[0], [X, Y, Z, 1]])

        mtmat = om.MTransformationMatrix(mat)

        position = mtmat.translation(om.MSpace.kWorld)
        xyz_rotation = mtmat.rotation(False)
        zyx_rotation = xyz_rotation.reorder(om.MEulerRotation.kZYX)
        try:
            t = PovTarget(_id, list(position * 10),
                          list(zyx_rotation),  pm.dt.Color(1, 0, 0, 0.5), self.brush)
        except utils.StrokeError:
            return
        print "Making Frame for", PROGRAM_NAME
        frame = robo.create_frame("POV_frame")
        print "Making Program for", PROGRAM_NAME
        program = robo.create_program(PROGRAM_NAME)
        print "Made Program for", PROGRAM_NAME

        self.brush.send()
        t.configure("001")
        t.send("test_", program, frame, None)

        self.save_program(self.directory, PROGRAM_NAME)
        self.save_station(self.directory, PROGRAM_NAME)
        robo.show()
