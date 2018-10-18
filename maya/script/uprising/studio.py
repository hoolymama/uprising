
import pymel.core as pm
import uprising_util as uutl

from robolink import Robolink, ITEM_TYPE_ROBOT
from paint import Paint
import props

from painting import Painting
# from contextlib import contextmanager
# import stroke_factory_utils as sfu


DIP_TARGET = "dipTarget"
TOOL_TARGET = "toolChangeTarget"
HOME_TARGET = "homeTarget"


class StudioError(Exception):
    pass


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

        self.dip = None
        self.painting = None

        if dip_node:
            with uutl.final_position(dip_node):
                self.dip = Painting(dip_node)
                self.dip.create_clusters("dip", self.robot)
        if painting_node:
            with uutl.final_position(painting_node):
                self.painting = Painting(painting_node)
                self.painting.create_clusters("painting", self.robot)

    def write(self):
        """Clean up and make parent objects etc."""

        # uutl.delete_tools()
        if self.painting and self.dip:
            uutl.delete_programs()
        if self.painting:
            self.painting_program = uutl.create_program("px")
            self.painting_program.ShowInstructions(False)
            self.painting_frame = uutl.create_frame("px_frame")

            with uutl.final_position(self.painting.node):
                top_node = uutl.assembly(self.painting.node)
                canvas = [
                    d for d in pm.listRelatives(
                        top_node,
                        ad=True,
                        type="transform") if d.name() == "canvas"][0]
                if canvas:
                    canvas_frame = uutl.create_frame("cx_frame")
                    props.send(canvas, canvas_frame)

        if self.dip:
            self.dip_frame = uutl.create_frame("dx_frame")
            self.trays_frame = uutl.create_frame("tx_frame")

        self.approaches_frame = uutl.create_frame("ax_frame")

        self.dip_approach = self._create_approach(
            DIP_TARGET, "dip_approach")
        self.tool_approach = self._create_approach(
            TOOL_TARGET, "tool_approach")
        self.home_approach = self._create_approach(
            HOME_TARGET, "home_approach")

        if self.dip:
            self.dip.write(self)
            with uutl.final_position(self.dip.node):
                Paint.write_trays(self.dip.node, self.trays_frame, self.RL)

        if self.painting:
            self.painting.write(self)

        self.painting_program.addMoveJ(self.home_approach)
        

    def _create_approach(self, object_name, name):
        mat = pm.PyNode(object_name).attr("worldMatrix[0]").get()
        mat = uutl.maya_to_robodk_mat(mat)
        joint_poses = uutl.config_000_poses(mat)
        if not joint_poses:
            raise StudioError(
                "No configs for approach mat. Try repositioning.")
        joints = joint_poses[0]

        old_approach = self.RL.Item(name)
        if old_approach.Valid():
            old_approach.Delete()
        target = self.RL.AddTarget(name, self.approaches_frame, self.robot)
        target.setAsJointTarget()
        target.setJoints(joints)
        return target
