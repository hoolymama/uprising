
import logging
import pymel.core as pm
from uprising import robo
import uprising.utils as uutl
from uprising.brush import Brush
from uprising.common.session.program import Program, ProgramError
from uprising.session.pick_place_program import PickProgram, PlaceProgram
from robolink import (
    INSTRUCTION_SHOW_MESSAGE,
    INSTRUCTION_CALL_PROGRAM,

)
from uprising import const as k
WATER_POT_ID = 9
WATER_REPEATS = 2

RETARDANT_POT_ID = 19


logger = logging.getLogger("uprising")


class BrushHangProgram(Program):
    def __init__(self, name, data):
        super(BrushHangProgram, self).__init__(name)
        self.brush_data = self._get_zero_tip_brushes(data)

    def _get_zero_tip_brushes(self, data):
        result = []
        for b in data:
            node = pm.PyNode(b["brush"])
            with uutl.at_value(node.attr("dipParam"), 0.0):
                result.append({
                    "brush": Brush.brush_set_at_index(b["id"])["outDipBrush"],
                    "twist": b["twist"],
                    "id": b["id"]
                }
                )

        return result

    def send(self):

        super(BrushHangProgram, self).send()
        mats = {
            "A": pm.PyNode("hangLocal|approach_loc").attr("worldMatrix[0]").get(),
            "N": pm.PyNode("hangLocal|loc_N").attr("worldMatrix[0]").get(),
            "E": pm.PyNode("hangLocal|loc_E").attr("worldMatrix[0]").get(),
            "W": pm.PyNode("hangLocal|loc_W").attr("worldMatrix[0]").get(),
            "S": pm.PyNode("hangLocal|loc_S").attr("worldMatrix[0]").get()
        }

        last_brush_id = None

        for brush_pack in self.brush_data:
            brush_id = brush_pack["id"]
            if last_brush_id is not None:

                # put the last brush back
                place_program_name = PlaceProgram.generate_program_name(
                    last_brush_id
                )
                self.program.RunInstruction(
                    place_program_name, INSTRUCTION_CALL_PROGRAM
                )

            pick_program_name = PickProgram.generate_program_name(brush_id)
            self.program.RunInstruction(
                pick_program_name, INSTRUCTION_CALL_PROGRAM)

            last_brush_id = brush_id

            # brush (is a PyNode), id, twist (bool)
            ###########################
            self._send_one_hang(brush_pack, mats)
            ###########################

        place_program_name = PlaceProgram.generate_program_name(last_brush_id)
        self.program.RunInstruction(
            place_program_name, INSTRUCTION_CALL_PROGRAM)
        ###########################

        self.program.addMoveJ(robo.home_approach)

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

        self.program.setSpeed(k.CAL_LINEAR_SPEED, k.CAL_ANGULAR_SPEED)
        self.program.setRounding(k.CAL_ROUNDING_DISTANCE)
        self.program.setPoseTool(tool)

        msg = "Record BACK(X) and RIGHT(Y) offsets for {}".format(pack["id"])
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
