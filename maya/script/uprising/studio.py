
import pymel.core as pm
import uprising_util as uutl

from robolink import (
    Robolink,
    ITEM_TYPE_ROBOT,
    INSTRUCTION_COMMENT,
    INSTRUCTION_SHOW_MESSAGE)
from paint import Paint
import props
from brush import Brush
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

    def __init__(self, painting_node, dip_node, **kw):
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

        self.calibration = kw.get("calibration", False)

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
                canvas = pm.PyNode("mainPaintingGroup|jpos|canvas")
                if canvas:
                    canvas_frame = uutl.create_frame("cx_frame")
                    props.send(canvas, canvas_frame)

        if self.dip:
            self.dip_frame = uutl.create_frame("dx_frame")
            self.trays_frame = uutl.create_frame("tx_frame")

        self.approaches_frame = uutl.create_frame("ax_frame")

        self.dip_approach = self._create_joint_target(
            pm.PyNode(DIP_TARGET), "dip_approach")
        self.tool_approach = self._create_joint_target(
            pm.PyNode(TOOL_TARGET), "tool_approach")
        self.home_approach = self._create_joint_target(
            pm.PyNode(HOME_TARGET), "home_approach")

        if self.dip:
            self.dip.write(self)
            with uutl.final_position(self.dip.node):
                Paint.write_trays(self.dip.node, self.trays_frame, self.RL)


        if self.painting:
            self.painting.write(self)

        self.painting_program.addMoveJ(self.home_approach)
 
        if self.calibration and self.painting:
            with uutl.final_position(self.painting.node):
                self.write_calibration()


    def _create_joint_target(self, obj, name):
        mat = obj.attr("worldMatrix[0]").get()
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

    def _create_calibration_target(self, obj, name, frame):
        mat = obj.attr("worldMatrix[0]").get()
        mat = uutl.maya_to_robodk_mat(mat)
        target = self.RL.AddTarget(name, frame, self.robot)
        target.setPose(mat)
        return target

    def write_calibration(self):
        cal_program = uutl.create_program("xx")
        calibration_frame = uutl.create_frame("xx_frame")

        # use brush 0 because there should always be one and
        # it should be smallest.
        # we write it now, because the painting hasn't
        # run yet. (This might not be necessary)
        brush = Brush.brush_at_index(self.painting.node, 0)
        brush.write(self)

        probes = {}
        for p in ["TL", "BL", "BR", "C", "TR"]:
            probe_name = "probe%s" % p
            probe_node = pm.PyNode(probe_name)
            probe_target = self._create_calibration_target(
                probe_node, probe_name, calibration_frame)

            approach_name = "probe%sApproach" % p
            approach_node = pm.PyNode(approach_name)
            approach_target = self._create_calibration_target(
                approach_node, approach_name, calibration_frame)

            probes[p] = {
                "probe": {
                    "name": probe_name,
                    "node": probe_node,
                    "target": probe_target
                },
                "approach": {
                    "name": approach_name,
                    "node": approach_node,
                    "target": approach_target
                }
            }
        probes["TL"]["nice"] = "Top Left"
        probes["TR"]["nice"] = "Top Right"
        probes["BL"]["nice"] = "Bottom Left"
        probes["BR"]["nice"] = "Bottom Right"
        probes["C"]["nice"] = "Center"
        


        motion = self.painting.motion
        cal_program.setSpeed(
            motion["linear_speed"],
            motion["angular_speed"])
        cal_program.setRounding(motion["rounding"])

        # set tool
        tool = self.RL.Item(brush.name)
        if not tool.Valid():
            raise StudioError(
                "SERIOUS RISK OF DAMAGE! Can't find valid tool!")
        cal_program.setPoseTool(tool)

        cal_program.RunInstruction("Starting calibration",
                                   INSTRUCTION_COMMENT)
        cal_program.addMoveJ(self.tool_approach)
        cal_program.RunInstruction(
            "Attach calibration Tool: %s PID:(%d)" %
            (brush.name, brush.physical_id),
            INSTRUCTION_SHOW_MESSAGE)
        cal_program.Pause()

        self.do_one_probe(cal_program, probes["TL"])
        self.do_one_probe(cal_program, probes["TR"])
        self.do_one_probe(cal_program, probes["BR"])
        self.do_one_probe(cal_program, probes["BL"])
        self.do_one_probe(cal_program, probes["C"])

        cal_program.addMoveJ(self.home_approach)

    def do_one_probe(self, program, probe):
        program.RunInstruction("Moving to Top Left",
                               INSTRUCTION_COMMENT)
        program.addMoveJ(probe["approach"]["target"])
        program.addMoveL(probe["probe"]["target"])
        program.RunInstruction(
            "Brush TCP should now be touching the board",
            INSTRUCTION_SHOW_MESSAGE)
        program.Pause()
        program.addMoveL(probe["approach"]["target"])
