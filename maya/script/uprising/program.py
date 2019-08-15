import math
import painting as ptg
import uprising_util as uutl
import palette_utils as putl
import brush_utils as butl
from brush import Brush
import pymel.core as pm
import props
import robodk as rdk

from robolink import (
    Robolink,
    ITEM_TYPE_ROBOT,
    INSTRUCTION_COMMENT,
    INSTRUCTION_SHOW_MESSAGE,
    INSTRUCTION_CALL_PROGRAM,
    INSTRUCTION_INSERT_CODE,
    COLLISION_OFF
)

import const as k
import logging
logger = logging.getLogger('uprising')


class ProgramError(Exception):
    pass


class Program(object):
    def __init__(self, name, **kw):
        self.program_name = name
        self.program = None

    def write(self):
        self.program = uutl.create_program(self.program_name)
        self.program.ShowInstructions(False)

    def validate_path(self):
        if self.program:
            RL = Robolink()
            update_result = self.program.Update(COLLISION_OFF)
            return {
                "instructions": update_result[0],
                "time": update_result[1],
                "distance": update_result[2],
                "completion": update_result[3],
                "problems": update_result[4],
                "status": "SUCCESS" if (update_result[3] == 1.0) else "FAILURE"
            }


class MainProgram(Program):
    def __init__(self, name, use_gripper=False):
        super(MainProgram, self).__init__(name)
        self.painting = ptg.Painting(pm.PyNode("mainPaintingShape"))
        self.use_gripper = use_gripper

    def _change_tool(self, last_brush_id, cluster, studio):
        if self.use_gripper:
            if last_brush_id is not None:

                # Slightly Hacky.
                # If using the gripper, but not actually changing the brush,
                # then don't bother placing and repicking.
                if last_brush_id == cluster.brush.id:
                    return

                # put the last brush back
                place_program_name = PlaceProgram.generate_program_name(
                    last_brush_id)
                self.program.RunInstruction(
                    place_program_name, INSTRUCTION_CALL_PROGRAM)

            pick_program_name = PickProgram.generate_program_name(
                cluster.brush.id)
            self.program.RunInstruction(
                pick_program_name, INSTRUCTION_CALL_PROGRAM)
        else:
            self.program.addMoveJ(studio.tool_approach)
            self.program.RunInstruction(cluster.change_tool_message(),
                                        INSTRUCTION_SHOW_MESSAGE)
            self.program.Pause()

    def write(self, studio):
        # RL = Robolink()
        # robot = RL.Item('', ITEM_TYPE_ROBOT)
        super(MainProgram, self).write()
        self.frame = uutl.create_frame("{}_frame".format(self.program_name))

        with uutl.minimize_robodk():
            self.painting.write_brushes()
            motion = self.painting.motion

            # Make sure gripper is open to begin with.
            self.program.RunInstruction(
                "Gripper opens here", INSTRUCTION_SHOW_MESSAGE)
            self.program.RunInstruction(
                '$OUT[2]=FALSE', INSTRUCTION_INSERT_CODE)
            self.program.RunInstruction(
                '$OUT[1]=TRUE', INSTRUCTION_INSERT_CODE)
            self.program.RunInstruction(
                'WAIT FOR ($IN[2])', INSTRUCTION_INSERT_CODE)

            last_brush_id = None
            last_paint_id = None

            for cluster in self.painting.clusters:
                if cluster.reason == "tool":

                    self._change_tool(last_brush_id, cluster, studio)
                    last_brush_id = cluster.brush.id

                dip_program_name = DipProgram.generate_program_name(
                    cluster.paint.id, cluster.brush.id)
                self.program.RunInstruction(
                    dip_program_name, INSTRUCTION_CALL_PROGRAM)

                # go to the approach at the center of the painting
                # Why? On a big painting, the lower corners could be
                # lower than the rack and a joint move from the far end
                # of the rack could cause the wrist to collide with the
                # rack.
                self.program.addMoveJ(studio.dip_approach)
                cluster.write(
                    self.program,
                    self.frame,
                    motion,
                    studio.RL,
                    studio.robot)
                self.program.addMoveJ(studio.dip_approach)

            if self.use_gripper:
                place_program_name = PlaceProgram.generate_program_name(
                    last_brush_id)
                self.program.RunInstruction(
                    place_program_name, INSTRUCTION_CALL_PROGRAM)

            self.program.addMoveJ(studio.home_approach)


class PapExerciseProgram(Program):
    def __init__(self, name):
        super(PapExerciseProgram, self).__init__(name)

    def write(self, studio):
        # RL = Robolink()
        # robot = RL.Item('', ITEM_TYPE_ROBOT)
        super(PapExerciseProgram, self).write()
        self.frame = uutl.create_frame("{}_frame".format(self.program_name))

        with uutl.minimize_robodk():

            last_brush_id = None
            for brush_id in range(20):
                if last_brush_id is not None:

                    ############################
                    self.program.Pause()
                    self.program.RunInstruction(
                        "About to swap brush {:02d} for {:02d}".format(
                            last_brush_id, brush_id), INSTRUCTION_SHOW_MESSAGE)

                    # put the last brush back
                    place_program_name = PlaceProgram.generate_program_name(
                        last_brush_id)
                    self.program.RunInstruction(
                        place_program_name, INSTRUCTION_CALL_PROGRAM)
                    ########################

                pick_program_name = PickProgram.generate_program_name(brush_id)
                self.program.RunInstruction(
                    pick_program_name, INSTRUCTION_CALL_PROGRAM)

                last_brush_id = brush_id

            ###########################
            self.program.Pause()
            self.program.RunInstruction(
                "About to replace brush {:02d} and go home.".format(
                    last_brush_id),
                INSTRUCTION_SHOW_MESSAGE)

            place_program_name = PlaceProgram.generate_program_name(
                last_brush_id)
            self.program.RunInstruction(
                place_program_name, INSTRUCTION_CALL_PROGRAM)
            ###########################

            self.program.addMoveJ(studio.home_approach)


class DipProgram(Program):

    @staticmethod
    def generate_program_name(paint_id, brush_id):
        return "p{:02d}_b{:02d}".format(paint_id, brush_id)

    def __init__(self, name, dip_node, wipe_node):
        super(DipProgram, self).__init__(name)

        # print "DIP {}".format(dip_node)
        # print "WIP {}".format(wipe_node)

        self.dip_painting = ptg.Painting(dip_node)
        self.wipe_painting = ptg.Painting(wipe_node)

    def write(self, studio):
        # RL = Robolink()
        # robot = RL.Item('', ITEM_TYPE_ROBOT)

        super(DipProgram, self).write()
        self.frame = studio.dips_frame

        self.dip_painting.write_brushes()
        self.wipe_painting.write_brushes()

        with uutl.minimize_robodk():
            self.program.RunInstruction(
                "Dip with tool %s" %
                self.dip_painting.clusters[0].brush.node_name,
                INSTRUCTION_COMMENT)

            # self.program.addMoveJ(studio.dip_approach)

            for cluster in self.dip_painting.clusters:
                cluster.write(

                    self.program,
                    self.frame,
                    self.dip_painting.motion, studio.RL, studio.robot)

            for cluster in self.wipe_painting.clusters:
                cluster.write(

                    self.program,
                    self.frame,
                    self.wipe_painting.motion, studio.RL, studio.robot)

            # self.program.addMoveJ(studio.dip_approach)


class CalibrationProgram(Program):

    def __init__(self, name, use_gripper):
        super(CalibrationProgram, self).__init__(name)
        self.use_gripper = use_gripper
        self.RL = Robolink()
        self.robot = self.RL.Item('', ITEM_TYPE_ROBOT)
        self.robot.setParam("PostProcessor", "KUKA KRC4_RN")
        self.brush = self._get_probe_brush()
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

    def write(self, tool_approach, home_approach):
        super(CalibrationProgram, self).write()
        self.frame = uutl.create_frame("{}_frame".format(self.program_name))

        self.brush.write(self.RL, self.robot)
        self.tool = self.RL.Item(self.brush.name)
        if not self.tool.Valid():
            raise ProgramError(
                "No Probe Brush. Risk of damage. Can't continue.")

        if self.use_gripper:
            self.write_probe_attach_gripper()
        else:
            self.write_probe_attach_bayonet(tool_approach)

        self.program.setSpeed(k.CAL_LINEAR_SPEED, k.CAL_ANGULAR_SPEED)
        self.program.setRounding(k.CAL_ROUNDING_DISTANCE)
        self.program.setPoseTool(self.tool)
        self.program.RunInstruction(
            "Starting calibration",
            INSTRUCTION_COMMENT)

        self.write_locator_packs()

        if self.use_gripper:
            self.write_probe_detach_gripper()

        self.program.addMoveJ(home_approach)

    def write_probe_attach_gripper(self):
        pick_program_name = PickProgram.generate_program_name(0)
        self.program.RunInstruction(
            pick_program_name, INSTRUCTION_CALL_PROGRAM)

    def write_probe_detach_gripper(self):
        place_program_name = PlaceProgram.generate_program_name(0)
        self.program.RunInstruction(
            place_program_name, INSTRUCTION_CALL_PROGRAM)

    def write_probe_attach_bayonet(self, tool_approach):

        self.program.addMoveJ(tool_approach)
        self.program.Pause()
        self.program.RunInstruction(
            "Attach calibration Tool: {}".format(
                self.brush.name), INSTRUCTION_SHOW_MESSAGE)

    def write_locator_packs(self):
        raise NotImplementedError

    def _get_probe_brush(self):
        geo = butl.setup_probe_from_sheet()
        return Brush.brush_at_plug(0, geo.attr("outPaintBrush"))

    def _write_stops(self, loc_a, loc_b, facing_joints):
        if loc_a and loc_b:
            approach_a = pm.PyNode(loc_a)
            approach_b = pm.PyNode(loc_b)

            tmat = pm.dt.TransformationMatrix(
                approach_a.attr("worldMatrix[0]").get())
            tx_a = approach_a.attr("worldMatrix[0]").get().translate
            tx_b = approach_b.attr("worldMatrix[0]").get().translate

            dist = (tx_b - tx_a).length()

            if dist > k.CAL_P2P_SPAN:
                num_spans = math.ceil(dist / k.CAL_P2P_SPAN)
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
        tool_pose = uutl.maya_to_robodk_mat(mat)
        flange_pose = tool_pose * self.brush.matrix.invH()
        target = self.RL.AddTarget(name, self.frame, self.robot)
        joints = self.robot.SolveIK(flange_pose, facing_joints)
        target.setPose(tool_pose)
        target.setJoints(joints)
        return target


class PotCalibration(CalibrationProgram):

    def __init__(self, name, use_gripper):
        super(PotCalibration, self).__init__(name, use_gripper)
        # self.pot_handle_pairs = putl.get_pot_handle_pairs()
        # self.locators = self._setup_locators()

        self.locators = putl.get_pot_handle_packs()

    def write_locator_packs(self):
        last = None

        # RACK_POT_DEPTH = 3.7 # 3.5 next time
# RACK_HANDLE_HEIGHT = 3.8 #4.5 next time

        # pot_depth = pm.PyNode("rack|holes").attr("calibrationPotDepth").get()
        # handle_height = pm.PyNode("rack|holes").attr(
        #     "calibrationHandleHeight").get()

        for pack in self.locators:
            self._write_stops(
                last,
                pack["pot_approach"],
                k.FACING_RACK_JOINTS)

            self.program.RunInstruction(
                ("Moving to %s" %
                 pack["name"]),
                INSTRUCTION_COMMENT)

            # with uutl.at_height(pack["pot_base"], pot_depth):
            self._write_one_probe(pack, "pot")
            # with uutl.at_height(pack["handle_base"], handle_height):
            self._write_one_probe(pack, "handle")

            last = pack["handle_approach"]

    def _write_one_probe(self, pack, which):

        column = {
            "pot": "B",
            "handle": "C"
        }

        row = k.CAL_SHEET_FIRST_ROW + pack["index"]

        base_key = "{}_base".format(which)
        approach_key = "{}_approach".format(which)

        base_target = self._create_a_target(
            pack[base_key].attr("worldMatrix[0]").get(),
            "{}_{}_base".format(pack["name"], which),
            k.FACING_RACK_JOINTS
        )

        approach_target = self._create_a_target(
            pack[approach_key].attr("worldMatrix[0]").get(),
            "{}_{}_approach".format(pack["name"], which),
            k.FACING_RACK_JOINTS
        )

        self.program.addMoveJ(approach_target)
        self.program.addMoveL(base_target)
        self.program.Pause()
        self.program.RunInstruction(
            "{} {} in cell {}{}".format(
                k.CAL_PAUSE_MESSAGE,
                pack["name"],
                row,
                column[which]),
            INSTRUCTION_SHOW_MESSAGE)
        self.program.addMoveL(approach_target)

    # def _setup_locators(self):
    #     locators = []
    #     i = 0
    #     rot = pm.dt.Vector(0, 180, 0)
    #     for pot, handle in self.pot_handle_pairs:

    #         parent = pot.getParent().getParent()

    #         handle_x_offset = handle.getParent().attr("tx").get()
    #         pack = {"id": i}
    #         pack["name"] = "{}_{:02}".format(pot.split("|")[-1], i)

    #         pack["pot_base_loc"] = CalibrationProgram._make_locator(
    #             parent, "pot_base_loc", pm.dt.Vector(
    #                 0, 0, -k.RACK_POT_DEPTH), relative=True, rotation=rot)
    #         pack["pot_approach_loc"] = CalibrationProgram._make_locator(
    #             parent, "pot_approach_loc", pm.dt.Vector(
    #                 0, 0, k.CAL_APPROACH_HEIGHT), relative=True, rotation=rot)
    #         pack["handle_base_loc"] = CalibrationProgram._make_locator(
    #             parent, "handle_base_loc", pm.dt.Vector(
    #                 handle_x_offset, 0, k.RACK_HANDLE_HEIGHT), relative=True, rotation=rot)
    #         pack["handle_approach_loc"] = CalibrationProgram._make_locator(
    #             parent, "handle_approach_loc", pm.dt.Vector(
    #                 handle_x_offset, 0, k.CAL_APPROACH_HEIGHT), relative=True, rotation=rot)
    #         locators.append(pack)
    #         i += 1
    #     return locators


class HolderCalibration(CalibrationProgram):

    def __init__(self, name, use_gripper):
        super(HolderCalibration, self).__init__(name, use_gripper)
        self.locators = putl.get_pick_place_packs("all")

    def write_locator_packs(self):
        bids = sorted(self.locators.keys())
        for bid in bids:
            pack = self.locators[bid]
            with uutl.at_position(pack["trans_node"], k.RACK_HOLDER_DISTANCE, 0, k.RACK_HOLDER_HEIGHT):
                self._write_one_probe(self.locators[bid])

    def _write_one_probe(self, pack):

        self.program.RunInstruction(
            "Moving to holder {}".format(
                pack["brush_id"]),
            INSTRUCTION_COMMENT)

        row = k.CAL_SHEET_FIRST_ROW + int(pack["brush_id"])

        base_target = self._create_a_target(
            pack["probe"].attr("worldMatrix[0]").get(),
            "base_{:02d}".format(pack["brush_id"]),
            k.FACING_RACK_JOINTS
        )

        approach_target = self._create_a_target(
            pack["pin_ap"].attr("worldMatrix[0]").get(),
            "approach_{:02d}".format(pack["brush_id"]),
            k.FACING_RACK_JOINTS
        )

        self.program.addMoveJ(approach_target)
        self.program.addMoveL(base_target)
        self.program.RunInstruction(
            "{} {} in cell {}D +EF by eye".format(
                k.CAL_PAUSE_MESSAGE,
                pack["brush_id"],
                row),
            INSTRUCTION_SHOW_MESSAGE)
        self.program.Pause()
        self.program.addMoveL(approach_target)


class PerspexCalibration(CalibrationProgram):

    def __init__(self, name, use_gripper):
        super(PerspexCalibration, self).__init__(name, use_gripper)
        self.locators = putl.get_perspex_packs()

    def write_locator_packs(self):
        for pack in self.locators:
            self._write_one_probe(pack)

    def _write_one_probe(self, pack):

        self.program.RunInstruction(
            "Moving to probe {} {}".format(
                pack["name"], pack["index"]),
            INSTRUCTION_COMMENT)

        row = k.PERSPEX_CAL_SHEET_FIRST_ROW + int(pack["index"])

        base_target = self._create_a_target(
            pack["base"].attr("worldMatrix[0]").get(),
            "base_{}".format(pack["name"]),
            k.FACING_RACK_JOINTS
        )

        approach_target = self._create_a_target(
            pack["approach"].attr("worldMatrix[0]").get(),
            "approach_{}".format(pack["name"]),
            k.FACING_RACK_JOINTS
        )

        self.program.addMoveJ(approach_target)
        self.program.addMoveL(base_target)
        self.program.RunInstruction(
            "{} {} in cell {}E".format(
                k.CAL_PAUSE_MESSAGE,
                pack["name"],
                row),
            INSTRUCTION_SHOW_MESSAGE)
        self.program.Pause()
        self.program.addMoveL(approach_target)


class ManualTriangulation(CalibrationProgram):

    def __init__(self, name, use_gripper):
        super(ManualTriangulation, self).__init__(name, use_gripper)

    def _get_probe_brush(self):
        geo = butl.setup_probe_from_sheet()
        geo.attr("paintingParam").set(0)
        geo.attr("dipParam").set(0)
        geo.attr("wipeParam").set(0)
        return Brush.brush_at_plug(0, geo.attr("outPaintBrush"))

    def write_locator_packs(self):

        self.program.Pause()
        self.program.RunInstruction(
            "Manually record the 3 leg centers with the brush tip",
            INSTRUCTION_COMMENT)


################# CALIBRATION #################
class BoardCalibration(CalibrationProgram):

    def __init__(self, name, use_gripper):
        super(BoardCalibration, self).__init__(name, use_gripper)
        top_node = pm.PyNode("mainPaintingGroup")
        top_node.attr("zeroPosition").set(1)
        self.locators = self._setup_locators()

    def write_locator_packs(self):

        last = None
        for locator_pack in self.locators:
            self._write_stops(
                last,
                locator_pack["approach_loc"],
                k.FACING_BOARD_JOINTS)
            self.program.RunInstruction(
                ("Moving to %s" %
                 locator_pack["name"]),
                INSTRUCTION_COMMENT)
            self._write_one_probe(locator_pack)
            last = locator_pack["approach_loc"]

    def _write_one_probe(self, pack):

        row = k.CAL_SHEET_FIRST_ROW + pack["id"]

        base_target = self._create_a_target(
            pack["base_loc"].attr("worldMatrix[0]").get(),
            "{}_base".format(pack["name"]),
            k.FACING_BOARD_JOINTS
        )

        approach_target = self._create_a_target(
            pack["approach_loc"].attr("worldMatrix[0]").get(),
            "{}_approach".format(pack["name"]),
            k.FACING_BOARD_JOINTS
        )

        self.program.addMoveJ(approach_target)
        self.program.addMoveL(base_target)
        self.program.Pause()
        self.program.RunInstruction(
            "{} {} in cell {}B".format(
                k.CAL_PAUSE_MESSAGE,
                pack["name"],
                row),
            INSTRUCTION_SHOW_MESSAGE)
        self.program.addMoveL(approach_target)

    def _setup_locators(self):
        locators = []
        verts = [
            item for sublist in pm.sets(
                "probePointsSet",
                q=True) for item in sublist]
        parent = pm.PyNode("|mainPaintingGroup|jpos|probes")
        rot = pm.dt.Vector(180, 0, -90)
        for i, v in enumerate(verts):
            index = int(str(v).split("[")[1].split("]")[0])
            px, py, _ = v.getPosition(space="world")
            name = "board_cal_{:02}".format(index)
            pack = {"name": name, "id": i}
            pack["base_loc"] = CalibrationProgram._make_locator(
                parent, "base_{}".format(name), pm.dt.Vector(
                    px, py, 0.0), relative=False, rotation=rot)
            pack["approach_loc"] = CalibrationProgram._make_locator(
                parent, "approach_{}".format(name), pm.dt.Vector(
                    px, py, k.CAL_APPROACH_HEIGHT), relative=False, rotation=rot)
            locators.append(pack)
        return locators

################# PICK PLACE #################


class PickPlaceProgram(Program):

    @staticmethod
    def generate_program_name(brush_id):
        raise NotImplementedError

    def __init__(self, brush, pack, name):

        super(PickPlaceProgram, self).__init__(name)

        # self.RL = Robolink()
        # self.robot = self.RL.Item('', ITEM_TYPE_ROBOT)
        self.brush = brush
        self.pack = pack
        self.targets = {}

    def write_brush(self, studio):

        old_brush = studio.RL.Item(self.brush.name)
        if old_brush.Valid():
            return

        tool_item = studio.robot.AddTool(self.brush.matrix, self.brush.name)
        triangles = props.mesh_triangles(pm.PyNode("GRIPPER"))
        shape = studio.RL.AddShape(triangles)
        tool_item.AddGeometry(shape, rdk.eye())
        studio.robot.setPoseTool(tool_item)
        shape.Delete()

    def write(self, studio):
        self.frame = studio.pick_place_frame
        super(PickPlaceProgram, self).write()

        self.write_brush(studio)
        # self.brush.write(studio.RL, studio.robot)

        self.tool = studio.RL.Item(self.brush.name)
        if not self.tool.Valid():
            raise ProgramError(
                "No Gripper Brush. Risk of damage. Can't continue.")

        self.program.setRounding(self.pack["rounding"])
        self.program.setPoseTool(self.tool)

        self.pin_target = self._create_target_from_pack(
            "pin", studio.RL, studio.robot)
        self.pin_ap_target = self._create_target_from_pack(
            "pin_ap", studio.RL, studio.robot)
        self.clear_target = self._create_target_from_pack(
            "clear", studio.RL, studio.robot)
        self.clear_ap_target = self._create_target_from_pack(
            "clear_ap", studio.RL, studio.robot)

    def _create_target_from_pack(self, key, RL, robot):
        return self._create_a_target(
            self.pack[key].attr("worldMatrix[0]").get(),
            "pick_place_{}_{:02d}".format(key, self.pack["brush_id"]),
            k.FACING_RACK_JOINTS, RL, robot)

    def _create_a_target(self, mat, name, facing_joints, RL, robot):
        target = RL.Item(name)
        if target.Valid():
            return target
        tool_pose = uutl.maya_to_robodk_mat(mat)
        flange_pose = tool_pose * self.brush.matrix.invH()
        target = RL.AddTarget(name, self.frame, robot)
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

    def write(self, studio):
        # logger.debug("PickProgram WRITE")
        # self.program = uutl.create_program(self.program_name)
        # self.program.ShowInstructions(False)
        pause_ms = -1 if studio.wait else studio.pause
        pause_ms = int(pause_ms)
        with uutl.minimize_robodk():
            super(PickProgram, self).write(studio)

            self.program.RunInstruction("Starting pick", INSTRUCTION_COMMENT)
            self.program.addMoveJ(self.pin_ap_target)

            self.program.setSpeed(
                self.pack["precision_lin_speed"],
                self.pack["ang_speed"])
            self.program.addMoveL(self.pin_target)

            self.program.RunInstruction(
                "Gripper closes here", INSTRUCTION_SHOW_MESSAGE)
            self.program.Pause(pause_ms)
            self.program.RunInstruction(
                '$OUT[1]=FALSE', INSTRUCTION_INSERT_CODE)
            self.program.RunInstruction(
                '$OUT[2]=TRUE', INSTRUCTION_INSERT_CODE)
            self.program.RunInstruction(
                'WAIT FOR ($IN[1])', INSTRUCTION_INSERT_CODE)
            self.program.Pause(pause_ms)
            self.program.addMoveL(self.clear_target)
            self.program.setSpeed(
                self.pack["lin_speed"],
                self.pack["ang_speed"])
            self.program.addMoveL(self.clear_ap_target)


class PlaceProgram(PickPlaceProgram):

    @staticmethod
    def generate_program_name(brush_id):
        return "place_{:02d}".format(brush_id)

    def __init__(self, brush, pack):
        name = PlaceProgram.generate_program_name(pack["brush_id"])
        super(PlaceProgram, self).__init__(brush, pack, name)

    def write(self, studio):

        pause_ms = -1 if studio.wait else studio.pause
        pause_ms = int(pause_ms)
        with uutl.minimize_robodk():
            super(PlaceProgram, self).write(studio)

            self.program.RunInstruction("Starting place", INSTRUCTION_COMMENT)
            self.program.addMoveJ(self.clear_ap_target)
            self.program.setSpeed(
                self.pack["lin_speed"],
                self.pack["ang_speed"])
            self.program.addMoveL(self.clear_target)
            self.program.setSpeed(
                self.pack["precision_lin_speed"],
                self.pack["ang_speed"])
            self.program.addMoveL(self.pin_target)

            self.program.RunInstruction(
                "Gripper opens here", INSTRUCTION_SHOW_MESSAGE)
            self.program.Pause(pause_ms)
            self.program.RunInstruction(
                '$OUT[2]=FALSE', INSTRUCTION_INSERT_CODE)
            self.program.RunInstruction(
                '$OUT[1]=TRUE', INSTRUCTION_INSERT_CODE)
            self.program.RunInstruction(
                'WAIT FOR ($IN[2])', INSTRUCTION_INSERT_CODE)
            self.program.Pause(pause_ms)
            self.program.addMoveL(self.pin_ap_target)
