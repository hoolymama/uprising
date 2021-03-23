
from uprising.target import Target
from uprising.utils import StrokeError
import maya.api.OpenMaya as om
import logging

import pymel.core as pm
from uprising import robo
from uprising.session.session import Session
from uprising.brush import Brush
logger = logging.getLogger("uprising")

maya_useNewAPI = True


class GamutSession(Session):

    PROGRAM_NAME = "pv"

    def __init__(self, minbb, maxbb, gap, z_axis, x_axis, brush_name, create_locator, create_target):

        self.brush = Brush(0, pm.PyNode(brush_name).attr("outPaintBrush"))

        self.minbb = minbb
        self.maxbb = maxbb
        self.gap = gap
        self.create_locator = create_locator
        self.create_target = create_target
        self.x_axis = x_axis
        # self.locator = pm.spaceLocator()

        if z_axis == "camera":
            self.cam_pos = pm.xform(
                pm.PyNode("CAM"), q=True, rp=True,  ws=True)
            self.z_axis = None
        else:
            self.z_axis = z_axis
            self.cam_pos = None

    def run(self):
        robo.new()
        robo.show()
        robo.clean("kr8")

        if self.create_target:
            self.frame = robo.create_frame("lattice")

        z = self.minbb[2]

        while z <= self.maxbb[2]:
            y = self.minbb[1]
            while y <= self.maxbb[1]:
                x = self.minbb[0]
                while x <= self.maxbb[0]:
                    self.create_one_target(x, y, z)
                    # print x, y, z

                    # self.create_locator(x, y, z)

                    x = x + self.gap
                y = y + self.gap
            z = z + self.gap

    def create_one_target(self, x, y, z):

        _id = "{}_{}_{}".format(x, y, z)
        front = pm.dt.Vector(self.z_axis).normal()
        side = front.cross(pm.dt.Vector(self.x_axis)).normal()
        up = side.cross(front).normal()

        mat = om.MMatrix([list(up)+[0], list(side)+[0],
                          list(front)+[0], [x, y, z, 1]])

        mtmat = om.MTransformationMatrix(mat)

        position = mtmat.translation(om.MSpace.kWorld)
        xyz_rotation = mtmat.rotation(False)
        zyx_rotation = xyz_rotation.reorder(om.MEulerRotation.kZYX)
        try:
            t = Target(_id, list(position * 10),
                       list(zyx_rotation), self.brush)
        except StrokeError:
            return

        if self.create_locator:
            loc = pm.spaceLocator()
            loc.attr("t").set(position)
            loc.attr("r").set([pm.mel.rad_to_deg(a) for a in xyz_rotation])
            loc.attr("s").set(mtmat.scale(om.MSpace.kWorld))
        if self.create_target:
            t.configure()
            t.send("lattice_", None, self.frame)

# def create_locator(x, y, z, z_axis, x_axis):
#     loc = pm.spaceLocator()

#     front = pm.dt.Vector(z_axis).normal()
#     side = front.cross(pm.dt.Vector(x_axis)).normal()
#     up = side.cross(front).normal()

#     mat = om.MMatrix([list(up)+[0], list(side)+[0],
#                       list(front)+[0], [x, y, z, 1]])

#     mtmat = om.MTransformationMatrix(mat)

#     loc.attr("t").set(mtmat.translation(om.MSpace.kWorld))
#     loc.attr("r").set([pm.mel.rad_to_deg(a) for a in mtmat.rotation()])
#     loc.attr("s").set(result.scale(om.MSpace.kWorld))

#     return mtmat


# result = create_locator(150, 50, 20, z_axis=(1, 0, 0), x_axis=(0, 0, 1))
# print result.translation(om.MSpace.kWorld)
# print result.rotation()
# print result.scale(om.MSpace.kWorld)

# self.directory = directory or self.choose_session_dir()
# if not self.directory:
#     return

# self.painting_node = pm.PyNode("mainPaintingShape")
# self.program_names = []

# self.cluster_count = pm.paintingQuery(self.painting_node, cc=True)
# self.cluster_chunk_size = cluster_chunk_size or self.cluster_count

# timer_start = time.time()
# # PAINTING
# self.pov_program = self.build_pov_program()
# self.send_and_publish_pov_program()

# duration = int(time.time() - timer_start)
# progress.update(
#     major_line="Total time: {}".format(
#         str(datetime.timedelta(seconds=duration))),
#     minor_line="", major_progress=0, minor_progress=0,
# )

# self.stats = {
#     "painting_stats": stats.stats(),
#     "camera_stats": {
#         "camera_top": list(pm.PyNode("CAM").attr("topLocator").get()),
#         "camera_base": list(pm.PyNode("CAM").attr("baseLocator").get()),
#         "focal_length": pm.PyNode("CAMShape").attr("focalLength").get()
#     }
# }

# def build_pov_program(self):
#     self.init_progress()

#     robo.clean("kr8")
#     with uutl.final_position(pm.PyNode("mainPaintingShape")):
#         result = PovProgram(self.PROGRAM_NAME)
#     return result

# def send_and_publish_pov_program(self):
#     cluster_count = len(self.pov_program.painting.clusters)
#     num_chunks = int(
#         math.ceil(cluster_count / float(self.cluster_chunk_size)))

#     progress.update(
#         major_max=num_chunks,
#         header="Writing {} main program chunks".format(num_chunks)
#     )

#     for i in range(num_chunks):
#         robo.clean("kr8")
#         progress.update(
#             major_progress=i, major_line="Writing {:d} of {:d} chunks".format(i+1, num_chunks))

#         self.pov_program.send(
#             chunk_id=i, chunk_length=self.cluster_chunk_size)

#         self.save_program(
#             self.directory, self.pov_program.program_name)

#         self.save_station(
#             self.directory, self.pov_program.program_name)

#         self.program_names.append(self.pov_program.program_name)

#         progress.update(major_progress=num_chunks, major_line="Done")

#     if len(self.program_names) > 1:
#         self.orchestrate(self.directory, self.program_names)

# def init_progress(self):
#     progress.update(
#         header="Creating main painting",
#         major_line="{} clusters in chunks of {}".format(
#             self.cluster_count, self.cluster_chunk_size),
#         major_progress=0,
#         minor_progress=0
#     )

# def show_stats(self):
#     uutl.show_in_window(self.stats, title="Painting stats")

# def write_stats(self):
#     self.json_report(self.directory, "stats", self.stats)

# def make_src_tar(self):
#     src_folder = os.path.join(self.directory, "src")
#     with tarfile.open("{}.tar.gz".format(src_folder), "w:gz") as tar:
#         tar.add(src_folder, arcname=os.path.sep)
