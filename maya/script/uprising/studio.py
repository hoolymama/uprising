import pymel.core as pm
import uprising_util as uutl

from robolink import Robolink, ITEM_TYPE_ROBOT

 
from painting import Painting
from cluster import (PaintingCluster, DipCluster)
from contextlib import contextmanager
import stroke_factory_utils as sfu

DIP_TARGET = "dipTarget"
TOOL_TARGET = "toolChangeTarget"
HOME_TARGET = "homeTarget"

class StudioError(Exception):
    pass


@contextmanager
def final_position(node):
    assembly = sfu.assembly(node)
    zpos = assembly.attr("zeroPosition").get()
    assembly.attr("zeroPosition").set(False)
    yield
    assembly.attr("zeroPosition").set(zpos)

class Studio(object):
    """Glue together the entire studio."""

    def __init__(self, painting_node, dip_node):
        self.RL = Robolink()
        self.robot = self.RL.Item('', ITEM_TYPE_ROBOT)
        self.painting_program = None
        self.painting_frame = None
        self.approaches_frame = None

        self.dip_approach = None
        self.tool_approach = None
        self.home_approach = None

        with final_position(dip_node):
            self.dip = Painting(dip_node)
            self.dip.create_clusters("dip", self.robot)
        with final_position(painting_node):
            self.painting = Painting(painting_node)
            self.painting.create_clusters("painting", self.robot)


    def write(self):
        """Clean up and make parent objects etc."""
        self.painting_program = uutl.create_program("px")
        self.painting_frame = uutl.create_frame("px_frame")
        self.approaches_frame = uutl.create_frame("ax_frame")

        self.dip_approach = self._create_approach(
            DIP_TARGET, "dip_approach")
        self.tool_approach = self._create_approach(
            TOOL_TARGET, "tool_approach")
        self.home_approach = self._create_approach(
            HOME_TARGET, "home_approach")

        self.dip.write(self)
        self.painting.write(self)


    def _create_approach(self, object_name, name):
        mat = pm.PyNode(object_name).attr("worldMatrix[0]").get()
        mat = uutl.maya_to_robodk_mat(mat)
        configs = uutl.config_map(mat, "000")
        if configs and "000" in configs:
            joints = configs["000"][0]
        else:
            raise StudioError("No configs for approach mat. Try repositioning.")
        
        old_approach = self.RL.Item(name)
        if old_approach.Valid():
            old_approach.Delete()
        target = self.RL.AddTarget(name, self.approaches_frame, self.robot)
        target.setAsJointTarget()
        target.setJoints(joints)
        return target
