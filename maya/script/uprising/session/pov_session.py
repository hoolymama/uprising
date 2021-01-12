
import logging
import math
import datetime
import time
import os
import tarfile
import pymel.core as pm
from uprising.target import PovTarget
from uprising.uprising_util import StrokeError
from uprising import uprising_util as uutl
from uprising import robo
from uprising import stats
from uprising import progress
from uprising.session.session import Session
from uprising.session.pov_program import PovProgram
import maya.api.OpenMaya as om
from uprising.brush import Brush

logger = logging.getLogger("uprising")


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
        except StrokeError:
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


class PovSession(Session):

    PROGRAM_NAME = "pv"

    def __init__(self, cluster_chunk_size=None, directory=None):

        self.directory = directory or self.choose_session_dir()
        if not self.directory:
            return

        self.painting_node = pm.PyNode("mainPaintingShape")
        self.program_names = []

        robo.new()

        robo.clean("kr8")
        robo.hide()


        self.cluster_count = pm.paintingQuery(self.painting_node, cc=True)
        self.cluster_chunk_size = cluster_chunk_size or self.cluster_count

        timer_start = time.time()
        # PAINTING
        self.pov_program = self.build_pov_program()
        self.send_and_publish_pov_program()

        duration = int(time.time() - timer_start)
        progress.update(
            major_line="Total time: {}".format(
                str(datetime.timedelta(seconds=duration))),
            minor_line="", major_progress=0, minor_progress=0,
        )

        self.stats = {
            "painting_stats": stats.stats(),
            "camera_stats": {
                "camera_top": list(pm.PyNode("CAM").attr("topLocator").get()),
                "camera_base": list(pm.PyNode("CAM").attr("baseLocator").get()),
                "focal_length": pm.PyNode("CAMShape").attr("focalLength").get()
            }
        }
        robo.show()

    def build_pov_program(self):
        self.init_progress()

        robo.clean("kr8")
        with uutl.final_position(pm.PyNode("mainPaintingShape")):
            result = PovProgram(self.PROGRAM_NAME)
        return result

    def send_and_publish_pov_program(self):
        cluster_count = len(self.pov_program.painting.clusters)
        num_chunks = int(
            math.ceil(cluster_count / float(self.cluster_chunk_size)))

        progress.update(
            major_max=num_chunks,
            header="Writing {} main program chunks".format(num_chunks)
        )

        for i in range(num_chunks):
            robo.clean("kr8")
            progress.update(
                major_progress=i, major_line="Writing {:d} of {:d} chunks".format(i+1, num_chunks))

            self.pov_program.send(
                chunk_id=i, chunk_length=self.cluster_chunk_size)

            self.save_program(
                self.directory, self.pov_program.program_name)

            self.save_station(
                self.directory, self.pov_program.program_name)

            self.program_names.append(self.pov_program.program_name)

            progress.update(major_progress=num_chunks, major_line="Done")

        if len(self.program_names) > 1:
            self.orchestrate(self.directory, self.program_names)

    def init_progress(self):
        progress.update(
            header="Creating main painting",
            major_line="{} clusters in chunks of {}".format(
                self.cluster_count, self.cluster_chunk_size),
            major_progress=0,
            minor_progress=0
        )

    def show_stats(self):
        uutl.show_in_window(self.stats, title="Painting stats")

    def write_stats(self):
        self.json_report(self.directory, "stats", self.stats)

    def make_src_tar(self):
        src_folder = os.path.join(self.directory, "src")
        with tarfile.open("{}.tar.gz".format(src_folder), "w:gz") as tar:
            tar.add(src_folder, arcname=os.path.sep)
