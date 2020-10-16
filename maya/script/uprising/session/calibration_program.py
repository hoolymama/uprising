from uprising import robo
from uprising.session.program import Program, ProgramError
import pymel.core as pm
import math

from uprising.brush import Brush

import uprising.uprising_util as uutl


from uprising.session.pick_place_program import PickAtHomeProgram, PlaceAtHomeProgram

from robolink import (
    INSTRUCTION_COMMENT,
    INSTRUCTION_CALL_PROGRAM
)


P2P_SPAN = 30.0
LINEAR_SPEED = 300  # mm
ANGULAR_SPEED = 70  # deg
ROUNDING_DISTANCE = 10  # mm


class CalibrationProgram(Program):
    def __init__(self, name):
        super(CalibrationProgram, self).__init__(name)
        self.brush = self._get_probe_brush()
        self.tool = None
        if not self.brush:
            raise ProgramError(
                "No Probe Brush. Risk of damage. Can't continue.")

    @staticmethod
    def _make_locator(parent, name, pos, **kw):
        rel = kw.get("relative", False)
        rot = kw.get("rotation", pm.dt.Vector(0, 180, 0))
        path = "{}|{}".format(parent, name)
        try:
            n = pm.PyNode(path)
            pm.delete(n)
        except BaseException:
            pass
        loc = pm.spaceLocator()
        loc.attr("t").set(pos)
        loc.attr("r").set(rot)
        pm.parent(loc, parent, relative=rel)
        loc.rename(name)
        return loc

    def _setup_locators(self):
        raise NotImplementedError

    def send(self):
        subprograms = []
        link = robo.link()
        super(CalibrationProgram, self).send()
        self.brush.send()
        self.tool = link.Item(self.brush.name)
        if not self.tool.Valid():
            raise ProgramError(
                "No Probe Brush. Risk of damage. Can't continue.")

        subprograms.append(self.send_pick_probe_home())

        self.program.setSpeed(LINEAR_SPEED, ANGULAR_SPEED)
        self.program.setRounding(ROUNDING_DISTANCE)
        self.program.setPoseTool(self.tool)
        self.program.RunInstruction(
            "Starting calibration", INSTRUCTION_COMMENT)

        self.send_locator_packs()
        subprograms.append(self.send_place_probe_home())
        self.program.addMoveJ(robo.home_approach)
        return subprograms

    def send_pick_probe_home(self):
        print "send_pick_probe_home PickAtHomeProgram"
        pick_program_name = PickAtHomeProgram.generate_program_name(0)
        self.program.RunInstruction(
            pick_program_name, INSTRUCTION_CALL_PROGRAM)
        return pick_program_name

    def send_place_probe_home(self):
        print "send_place_probe_home PlaceAtHomeProgram"
        place_program_name = PlaceAtHomeProgram.generate_program_name(0)
        self.program.RunInstruction(
            place_program_name, INSTRUCTION_CALL_PROGRAM)
        return place_program_name

    def send_locator_packs(self):
        raise NotImplementedError

    @staticmethod
    def _get_probe_brush():
        geo = pm.PyNode("bpx_0_utility_B0_probe_roundShape")
        with uutl.at_value(geo.attr("paintingParam"), 0.4):
            tool = Brush(0, geo.attr("outPaintBrush"))
        return tool

    def _send_stops(self, loc_a, loc_b, facing_joints):
        if loc_a and loc_b:
            approach_a = pm.PyNode(loc_a)
            approach_b = pm.PyNode(loc_b)

            tmat = pm.dt.TransformationMatrix(
                approach_a.attr("worldMatrix[0]").get())
            tx_a = approach_a.attr("worldMatrix[0]").get().translate
            tx_b = approach_b.attr("worldMatrix[0]").get().translate

            dist = (tx_b - tx_a).length()

            if dist > P2P_SPAN:
                num_spans = math.ceil(dist / P2P_SPAN)
                gap = dist / num_spans
                vec = (tx_b - tx_a).normal() * gap
                for i in range(int(num_spans) - 1):
                    tx_a += vec
                    tmat.setTranslation(tx_a, "world")
                    name = "%s_stop_%d" % (loc_b, i)
                    target = self._create_a_target(
                        tmat.asMatrix(), name, facing_joints)
                    self.program.addMoveJ(target)

    def _create_a_target(self, mat, name, facing_joints):
        robot = robo.robot()
        link = robo.link()
        tool_pose = robo.maya_to_robodk_mat(mat)
        flange_pose = tool_pose * self.brush.matrix.invH()
        target = link.AddTarget(name, robo.calibration_frame, robot)
        joints = robot.SolveIK(flange_pose, facing_joints)
        target.setPose(tool_pose)
        target.setJoints(joints)
        return target
