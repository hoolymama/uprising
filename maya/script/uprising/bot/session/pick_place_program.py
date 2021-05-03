
import pymel.core as pm
from uprising import props
import robodk as rdk
from uprising import robo
from uprising.brush import Brush
from uprising.common.session.program import Program, ProgramError
import uprising.utils as uutl

from robolink import (
    INSTRUCTION_COMMENT,
    INSTRUCTION_SHOW_MESSAGE,
    INSTRUCTION_INSERT_CODE
)

from uprising import const as k
import logging

logger = logging.getLogger("uprising")


class PickPlaceProgram(Program):
    @staticmethod
    def generate_program_name(brush_id):
        raise NotImplementedError

    def __init__(self, brush, pack, name):

        super(PickPlaceProgram, self).__init__(name)

        self.brush = brush
        self.pack = pack
        self.targets = {}

    def send_brush(self):
        robot = robo.robot()
        link = robo.link()
        existing_brush = link.Item(self.brush.name)
        if existing_brush.Valid():
            return
        print "self.brush.name", self.brush.name
        tool_item = robot.AddTool(self.brush.matrix, self.brush.name)
        triangles = props.mesh_triangles(pm.PyNode("GRIPPER"))
        shape = link.AddShape(triangles)
        tool_item.AddGeometry(shape, rdk.eye())
        robot.setPoseTool(tool_item)
        shape.Delete()

    def send(self):
        link = robo.link()
        super(PickPlaceProgram, self).send()

        self.send_brush()

        self.tool = link.Item(self.brush.name)
        if not self.tool.Valid():
            raise ProgramError(
                "No Gripper Brush. Risk of damage. Can't continue.")

        self.program.setRounding(self.pack["rounding"])
        self.program.setPoseTool(self.tool)

        self.pin_target = self._create_target_from_pack("pin")
        self.pin_ap_target = self._create_target_from_pack("pin_ap")
        self.clear_target = self._create_target_from_pack("clear")
        self.clear_ap_target = self._create_target_from_pack("clear_ap")

    def _create_target_from_pack(self, key):

        return self._create_a_target(
            self.pack[key].attr("worldMatrix[0]").get(),
            "pick_place_{}_{:02d}".format(key, self.pack["brush_id"]),
            k.FACING_RACK_JOINTS
        )

    def _create_a_target(self, mat, name, facing_joints):
        link = robo.link()
        robot = robo.robot()
        target = link.Item(name)
        if target.Valid():
            return target
        tool_pose = robo.maya_to_robodk_mat(mat)
        flange_pose = tool_pose * self.brush.matrix.invH()
        target = link.AddTarget(name, robo.pick_place_frame, robot)
        joints = robot.SolveIK(flange_pose, facing_joints)
        target.setPose(tool_pose)
        target.setJoints(joints)
        return target


class PickProgram(PickPlaceProgram):
    @staticmethod
    def generate_program_name(brush_id):
        return "pick_{:02d}".format(brush_id)

    def __init__(self, brush, pack):
        name = PickProgram.generate_program_name(pack["brush_id"])
        super(PickProgram, self).__init__(brush, pack, name)

    def send(self):

        super(PickProgram, self).send()

        self.program.RunInstruction("Starting pick", INSTRUCTION_COMMENT)
        self.program.addMoveJ(self.pin_ap_target)

        self.program.setSpeed(
            self.pack["precision_lin_speed"], self.pack["ang_speed"]
        )
        self.program.addMoveL(self.pin_target)

        self.program.RunInstruction(
            "Gripper closes here", INSTRUCTION_SHOW_MESSAGE)
        self.program.Pause(k.GRIPPER_OPEN_CLOSE_PAUSE)
        self.program.RunInstruction("$OUT[1]=FALSE", INSTRUCTION_INSERT_CODE)
        self.program.RunInstruction("$OUT[2]=TRUE", INSTRUCTION_INSERT_CODE)
        self.program.RunInstruction(
            "WAIT FOR ($IN[1])", INSTRUCTION_INSERT_CODE)
        self.program.RunInstruction(
            "Gripper open. Make sure it dropped the tool properly.",
            INSTRUCTION_SHOW_MESSAGE,
        )
        self.program.Pause(k.GRIPPER_OPEN_CLOSE_PAUSE)
        self.program.addMoveL(self.clear_target)
        self.program.setSpeed(self.pack["lin_speed"], self.pack["ang_speed"])
        self.program.addMoveL(self.clear_ap_target)


class PlaceProgram(PickPlaceProgram):
    @staticmethod
    def generate_program_name(brush_id):
        return "place_{:02d}".format(brush_id)

    def __init__(self, brush, pack):
        name = PlaceProgram.generate_program_name(pack["brush_id"])
        super(PlaceProgram, self).__init__(brush, pack, name)

    def send(self):

        super(PlaceProgram, self).send()

        self.program.RunInstruction("Starting place", INSTRUCTION_COMMENT)
        self.program.addMoveJ(self.clear_ap_target)
        self.program.setSpeed(self.pack["lin_speed"], self.pack["ang_speed"])
        self.program.addMoveL(self.clear_target)
        self.program.setSpeed(
            self.pack["precision_lin_speed"], self.pack["ang_speed"]
        )
        self.program.addMoveL(self.pin_target)

        self.program.RunInstruction(
            "Gripper opens here", INSTRUCTION_SHOW_MESSAGE)
        self.program.Pause(k.GRIPPER_OPEN_CLOSE_PAUSE)
        self.program.RunInstruction("$OUT[2]=FALSE", INSTRUCTION_INSERT_CODE)
        self.program.RunInstruction("$OUT[1]=TRUE", INSTRUCTION_INSERT_CODE)
        self.program.RunInstruction(
            "WAIT FOR ($IN[2])", INSTRUCTION_INSERT_CODE)
        self.program.Pause(k.GRIPPER_OPEN_CLOSE_PAUSE)
        self.program.addMoveL(self.pin_ap_target)


class PickAtHomeProgram(PickPlaceProgram):
    @staticmethod
    def generate_program_name(brush_id):
        return "pick_{:02d}".format(brush_id)

    def __init__(self, brush, pack):
        name = PickAtHomeProgram.generate_program_name(pack["brush_id"])
        super(PickAtHomeProgram, self).__init__(brush, pack, name)

    def send(self):

        super(PickAtHomeProgram, self).send()

        self.program.RunInstruction("Starting pick", INSTRUCTION_COMMENT)

        self.program.addMoveJ(robo.home_approach)
        self.program.RunInstruction(
            "Continue when you are ready for the robot to grip the probe.",
            INSTRUCTION_SHOW_MESSAGE,
        )
        self.program.Pause(-1)
        self.program.RunInstruction("$OUT[1]=FALSE", INSTRUCTION_INSERT_CODE)
        self.program.RunInstruction("$OUT[2]=TRUE", INSTRUCTION_INSERT_CODE)
        self.program.RunInstruction(
            "WAIT FOR ($IN[1])", INSTRUCTION_INSERT_CODE)

        self.program.RunInstruction(
            "Did it grip properly? If so hit continue.",
            INSTRUCTION_SHOW_MESSAGE
        )
        self.program.Pause(-1)


class PlaceAtHomeProgram(PickPlaceProgram):
    @staticmethod
    def generate_program_name(brush_id):
        return "place_{:02d}".format(brush_id)

    def __init__(self, brush, pack):
        name = PlaceAtHomeProgram.generate_program_name(pack["brush_id"])
        super(PlaceAtHomeProgram, self).__init__(brush, pack, name)

    def send(self):

        super(PlaceAtHomeProgram, self).send()

        self.program.RunInstruction("Starting place", INSTRUCTION_COMMENT)
        self.program.addMoveJ(robo.home_approach)

        self.program.RunInstruction(
            "Continue when you are ready for the robot to release the probe.",
            INSTRUCTION_SHOW_MESSAGE,
        )
        self.program.Pause(-1)
        self.program.RunInstruction("$OUT[2]=FALSE", INSTRUCTION_INSERT_CODE)
        self.program.RunInstruction("$OUT[1]=TRUE", INSTRUCTION_INSERT_CODE)
        self.program.RunInstruction(
            "WAIT FOR ($IN[2])", INSTRUCTION_INSERT_CODE)
        self.program.RunInstruction(
            "Did you catch it? Hit continue to end the program.",
            INSTRUCTION_SHOW_MESSAGE,
        )


class PickPlaceCollection(object):

    def __init__(self, brush_ids):
        self.is_caibration = brush_ids == "calibration"
        self.brush_ids = [0] if self.is_caibration else brush_ids
        self.packs = self.get_packs()
        self.programs = self._build()

    def get_packs(self):
        result = {}

        holders_node = pm.PyNode("RACK1_CONTEXT|j1|rack|holders")
        path_attributes = {
            "lin_speed": holders_node.attr("linearSpeed").get() * 10,
            "precision_lin_speed": holders_node.attr("precisionLinearSpeed").get() * 10,
            "ang_speed": holders_node.attr("angularSpeed").get(),
            "rounding": holders_node.attr("approximationDistance").get() * 10,
        }
        for bid in self.brush_ids:
            key = "b{:02d}".format(bid)
            trans = "holderRot{:02d}|holderTrans".format(bid)
            result[key] = {
                "trans_node": pm.PyNode(trans),
                "brush_id": bid,
                "probe": pm.PyNode("{}|probe_loc".format(trans)),
                "pin": pm.PyNode("{}|pin_loc".format(trans)),
                "pin_ap": pm.PyNode("{}|pin_approach_loc".format(trans)),
                "clear": pm.PyNode("{}|clear_loc".format(trans)),
                "clear_ap": pm.PyNode("{}|clear_approach_loc".format(trans)),
            }
            result[key].update(path_attributes)
        return result

    def _build(self):
 
        gripper_geo = pm.PyNode("bpx_0_utility_B0_gripper_roundShape")
        gripper = Brush(0, gripper_geo.attr("outPaintBrush"))
        if not gripper:
            raise ProgramError(
                "No Gripper. Risk of damage. Can't continue.")

        result = []
        for p in self.packs:
            pack = self.packs[p]
            if self.is_caibration:
                pick_prg = PickAtHomeProgram(gripper, pack)
                place_prg = PlaceAtHomeProgram(gripper, pack)
            else:
                pick_prg = PickProgram(gripper, pack)
                place_prg = PlaceProgram(gripper, pack)

            result.append({"pick": pick_prg, "place": place_prg})
        return result

    def send(self):
        for pair in self.programs:
            pair["pick"].send()
            pair["place"].send()

    def program_names(self):
        result = []
        for pair in self.programs:
            result.append(pair["pick"].program_name)
            result.append(pair["place"].program_name)
        return result
