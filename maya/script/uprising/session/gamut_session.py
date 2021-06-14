
from uprising.common.session.target import Target
from uprising.utils import StrokeError
import maya.api.OpenMaya as om
import logging

import pymel.core as pm
from uprising import robo
from uprising.common.session.session import Session
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
