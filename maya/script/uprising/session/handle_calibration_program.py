
import logging
import pymel.core as pm
from uprising import robo
from uprising import utils
from uprising.brush import Brush
from uprising.common.session.program import Program, ProgramError
from uprising.bot.session.pick_place_program import PickProgram, PlaceProgram
from robolink import (
    INSTRUCTION_COMMENT,
    INSTRUCTION_SHOW_MESSAGE,
    INSTRUCTION_CALL_PROGRAM,

)
from uprising import const as k

from uprising import utils

P2P_SPAN = 30.0
LINEAR_SPEED = 300  # mm
ANGULAR_SPEED = 70  # deg
ROUNDING_DISTANCE = 10  # mm


logger = logging.getLogger(__name__)


class HandleCalibrationProgram(Program):
    def __init__(self, name):
        super(HandleCalibrationProgram, self).__init__(name)
    
        node = pm.PyNode("bpx_00_slot_00_davinci2_roundShape")
        with utils.at_value(node.attr("dipParam"), 0.0):
            self.brush = Brush.brush_set_at_index(0)["outDipBrush"]




    def send(self):
        
        super(HandleCalibrationProgram, self).send()
    
        packs = []
        for i, base in enumerate(pm.ls("RACK1_CONTEXT|j1|rack|holes|holeRot*|holeTrans|handle_base_loc")):
 
            packs.append(
                {
                    "base": base,
                    "approach": base.getChildren(type="transform")[0],
                    "name": "handle_{:02d}".format(i),
                    "index": i
                }
            )

        link = robo.link()
        pick_program_name = PickProgram.generate_program_name(0)
        self.program.RunInstruction(
                pick_program_name, INSTRUCTION_CALL_PROGRAM)

        self.brush.send(with_geo=True)
        tool = link.Item(self.brush.name)
        if not tool.Valid():
            raise ProgramError(
                "Brush is not valid. Risk of damage. Can't continue.")

        self.program.setSpeed(k.CAL_LINEAR_SPEED)
        self.program.setSpeedJoints( k.CAL_ANGULAR_SPEED)
        self.program.setRounding(k.CAL_ROUNDING_DISTANCE)
        self.program.setPoseTool(tool)

        self._send_hangs(packs)

        place_program_name = PlaceProgram.generate_program_name(0)
        self.program.RunInstruction(
            place_program_name, INSTRUCTION_CALL_PROGRAM)

        self.program.addMoveJ(robo.home_approach)

    def _send_hangs(self, packs):

        with utils.at_value(pm.PyNode("RACK1_CONTEXT|j1|rack|holes").attr("calibrationHandleHeight"), 4.7):
            for pack in packs:
                self.program.RunInstruction(
                        "Moving to {}".format(pack["name"]), INSTRUCTION_COMMENT
                    )

                base_target = self._create_a_target(
                    pack["base"].attr("worldMatrix[0]").get(),
                    "{}_base".format(pack["name"]),
                    k.FACING_RACK_JOINTS,
                )

                approach_target = self._create_a_target(
                    pack["approach"].attr("worldMatrix[0]").get(),
                    "{}_approach".format(pack["name"]),
                    k.FACING_RACK_JOINTS,
                )

                self.program.addMoveJ(approach_target)
                self.program.addMoveL(base_target)
                self.program.Pause()
                self.program.RunInstruction(
                    "Record Y offset (to the right) for {}".format(pack["name"]),
                    INSTRUCTION_SHOW_MESSAGE,
                )
                self.program.addMoveL(approach_target)



        # mats = {
        #     "A": pm.PyNode("hangLocal|approach_loc").attr("worldMatrix[0]").get(),
        #     "N": pm.PyNode("hangLocal|loc_N").attr("worldMatrix[0]").get(),
        #     "E": pm.PyNode("hangLocal|loc_E").attr("worldMatrix[0]").get(),
        #     "W": pm.PyNode("hangLocal|loc_W").attr("worldMatrix[0]").get(),
        #     "S": pm.PyNode("hangLocal|loc_S").attr("worldMatrix[0]").get()
        # }
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

    def _send_one_hang(self, pack, mats):
        link = robo.link()

        targets = {}
        for key in mats:
            targets[key] = self._create_target_for_brush(
                pack["brush"],
                mats[key],
                "hang_{}_{:02d}".format(key, pack["id"])
            )

        # write the brush
        pack["brush"].send()
        tool = link.Item(pack["brush"].name)
        if not tool.Valid():
            raise ProgramError(
                "Brush is not valid. Risk of damage. Can't continue.")

        self.program.setSpeed(k.CAL_LINEAR_SPEED)
        self.program.setSpeedJoints( k.CAL_ANGULAR_SPEED)
        
        self.program.setRounding(k.CAL_ROUNDING_DISTANCE)
        self.program.setPoseTool(tool)

        msg = "Record BACK(X) and RIGHT(Y) DOWN(Z) offsets for {}".format(pack["id"])
        if pack["twist"]:
            msg = "Watch closely for ARC center (using brush{})".format(
                pack["id"])

        self.program.addMoveJ(targets["A"])
        self.program.addMoveL(targets["N"])
        self.program.RunInstruction(msg, INSTRUCTION_SHOW_MESSAGE,)
        self.program.Pause()

        if pack["twist"]:
            self.program.addMoveL(targets["E"])
            self.program.addMoveL(targets["S"])
            self.program.addMoveL(targets["E"])
            self.program.addMoveL(targets["N"])
            self.program.addMoveL(targets["W"])
            self.program.addMoveL(targets["S"])
            self.program.addMoveL(targets["W"])
            self.program.addMoveL(targets["N"])

            self.program.RunInstruction(
                "Mark the ARC center point {}".format(pack["id"]),
                INSTRUCTION_SHOW_MESSAGE,
            )
            self.program.Pause()

        self.program.addMoveL(targets["A"])

    def _create_target_for_brush(self, brush, mat, name):
        robot = robo.robot()
        link = robo.link()
        tool_pose = robo.maya_to_robodk_mat(mat)
        flange_pose = tool_pose * brush.matrix.invH()
        target = link.AddTarget(name, robo.calibration_frame, robot)
        joints = robot.SolveIK(flange_pose, k.FACING_RACK_JOINTS)
        target.setPose(tool_pose)
        target.setJoints(joints)
        return target
