import math
import painting as ptg
import uprising_util as uutl
import palette_utils as putl
import brush_utils as butl
from brush import Brush
import pymel.core as pm

from robolink import (
    Robolink,
    ITEM_TYPE_ROBOT,
    INSTRUCTION_COMMENT,
    INSTRUCTION_SHOW_MESSAGE,
    INSTRUCTION_CALL_PROGRAM
)

import const as k


class ProgramError(Exception):
    pass


class Program(object):
    def __init__(self, name, **kw):
        self.program_name = name

    def write(self):
        self.program = uutl.create_program(self.program_name)
        self.frame = uutl.create_frame("{}_frame".format(self.program_name))
        # self.program.ShowInstructions(False)

class MainProgram(Program):
    def __init__(self, name, node):
        super(MainProgram, self).__init__(name)
        self.painting = ptg.Painting(node)

    def write(self, studio):
        RL = Robolink()
        robot = RL.Item('', ITEM_TYPE_ROBOT)

        super(MainProgram, self).write()
        self.painting.write_brushes()

        motion = self.painting.motion
        for cluster in self.painting.clusters:

            if cluster.reason == "tool":
                self.program.addMoveJ(studio.tool_approach)
                self.program.RunInstruction(cluster.change_tool_message(),
                                            INSTRUCTION_SHOW_MESSAGE)
                self.program.Pause()

            dip_program_name = DipProgram.generate_program_name(
                cluster.paint.id, cluster.brush.id)

            self.program.RunInstruction(
                dip_program_name, INSTRUCTION_CALL_PROGRAM)

            cluster.write(self.program, self.frame, motion, RL, robot)

        self.program.addMoveJ(studio.home_approach)
        # self.program.ShowInstructions(False)


class DipProgram(Program):

    @staticmethod
    def generate_program_name(paint_id, brush_id):
        return "p{:02d}_b{:02d}".format(paint_id, brush_id)

    def __init__(self, name, dip_node, wipe_node):
        super(DipProgram, self).__init__(name)

        print "DIP {}".format(dip_node)
        print "WIP {}".format(wipe_node)

        self.dip_painting = ptg.Painting(dip_node)
        self.wipe_painting = ptg.Painting(wipe_node)

    def write(self, studio):
        RL = Robolink()
        robot = RL.Item('', ITEM_TYPE_ROBOT)

        super(DipProgram, self).write()
        self.dip_painting.write_brushes()
        self.wipe_painting.write_brushes()

        self.program.RunInstruction(
            "Dip with tool %s" %
            self.dip_painting.clusters[0].brush.node_name, INSTRUCTION_COMMENT)

        self.program.addMoveJ(studio.dip_approach)

        for cluster in self.dip_painting.clusters:
            cluster.write(

                self.program,
                self.frame,
                self.dip_painting.motion, RL, robot)

        for cluster in self.wipe_painting.clusters:
            cluster.write(

                self.program,
                self.frame,
                self.wipe_painting.motion, RL, robot)

        self.program.addMoveJ(studio.dip_approach)


class CalibrationProgram(Program):

    def __init__(self, name):
        super(CalibrationProgram, self).__init__(name)

        self.RL = Robolink()
        self.robot = self.RL.Item('', ITEM_TYPE_ROBOT)
        self.brush = self._get_probe_brush()
        if not self.brush:
            raise ProgramError(
                "No Probe Brush. Risk of damage. Can't continue.")

    @staticmethod
    def _make_locator(parent, name, pos, **kw):
        rel = kw.get("relative", False)
        rot = kw.get("rotation", pm.dt.Vector(0,180,0))
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
        self.brush.write()
        self.tool = self.RL.Item(self.brush.name)
        if not self.tool.Valid():
            raise ProgramError(
                "No Probe Brush. Risk of damage. Can't continue.")

        self.program.setSpeed(k.CAL_LINEAR_SPEED, k.CAL_ANGULAR_SPEED)
        self.program.setRounding(k.CAL_ROUNDING_DISTANCE)
        self.program.setPoseTool(self.tool)
        self.program.RunInstruction(
            "Starting calibration",
            INSTRUCTION_COMMENT)
        self.program.addMoveJ(tool_approach)
        self.program.RunInstruction(
            "Attach calibration Tool: {}".format(
                self.brush.name), INSTRUCTION_SHOW_MESSAGE)
        self.program.Pause()

        self.write_locator_packs()

        self.program.addMoveJ(home_approach)

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


class RackCalibration(CalibrationProgram):

    def __init__(self, name):
        super(RackCalibration, self).__init__(name)
        self.pot_handle_pairs = putl.get_pot_handle_pairs()
        self.locators = self._setup_locators()

    def write_locator_packs(self):
        last = None
        for locator_pack in self.locators:
            self._write_stops(
                last,
                locator_pack["pot_approach_loc"],
                k.FACING_RACK_JOINTS)

            self.program.RunInstruction(
                ("Moving to %s" %
                 locator_pack["name"]),
                INSTRUCTION_COMMENT)
            self._write_one_probe(locator_pack, "pot")
            self._write_one_probe(locator_pack, "handle")
            last = locator_pack["handle_approach_loc"]

    def _write_one_probe(self, pack, which):

        column = {
            "pot": "B",
            "handle": "C"
        }

        row = k.CAL_SHEET_FIRST_ROW + pack["id"]

        base_key = "{}_base_loc".format(which)
        approach_key = "{}_approach_loc".format(which)

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
        self.program.RunInstruction(
            "{} {} in the {} column, cell {}{}".format(
                k.CAL_PAUSE_MESSAGE,
                pack["name"],
                which.upper(),
                row,
                column[which]),
            INSTRUCTION_SHOW_MESSAGE)
        self.program.Pause()
        self.program.addMoveL(approach_target)

    def _setup_locators(self):
        locators = []
        i = 0
        rot = pm.dt.Vector(0,180, 0)
        for pot, handle in self.pot_handle_pairs:

            parent = pot.getParent().getParent()

            handle_x_offset = handle.getParent().attr("tx").get()
            pack = {"id": i}
            pack["name"] = "{}_{:02}".format(pot.split("|")[-1], i)
            pack["pot_base_loc"] = CalibrationProgram._make_locator(
                parent, "pot_base_loc", pm.dt.Vector(
                    0, 0, -k.RACK_POT_DEPTH), relative=True, rotation=rot)
            pack["pot_approach_loc"] = CalibrationProgram._make_locator(
                parent, "pot_approach_loc", pm.dt.Vector(
                    0, 0, k.CAL_APPROACH_HEIGHT), relative=True, rotation=rot)
            pack["handle_base_loc"] = CalibrationProgram._make_locator(
                parent, "handle_base_loc", pm.dt.Vector(
                    handle_x_offset, 0, k.RACK_HANDLE_HEIGHT), relative=True, rotation=rot)
            pack["handle_approach_loc"] = CalibrationProgram._make_locator(
                parent, "handle_approach_loc", pm.dt.Vector(
                    handle_x_offset, 0, k.CAL_APPROACH_HEIGHT), relative=True, rotation=rot)
            locators.append(pack)
            i += 1
        return locators


class BoardCalibration(CalibrationProgram):

    def __init__(self, name):
        super(BoardCalibration, self).__init__(name)
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
        self.program.RunInstruction(
            "{} {} in cell B {}".format(
                k.CAL_PAUSE_MESSAGE,
                pack["name"],
                row),
            INSTRUCTION_SHOW_MESSAGE)
        self.program.Pause()
        self.program.addMoveL(approach_target)

    def _setup_locators(self):
        locators = []
        verts = [
            item for sublist in pm.sets(
                "probePointsSet",
                q=True) for item in sublist]
        parent = pm.PyNode("|mainPaintingGroup|jpos|probes")
        rot = pm.dt.Vector(180,0, -90)
        for i, v in enumerate(verts):
            index = int(str(v).split("[")[1].split("]")[0])
            px, py, _ = v.getPosition(space="world")
            name = "board_cal_{:02}".format(index)
            pack = {"name": name, "id": i}
            pack["base_loc"] = CalibrationProgram._make_locator(
                parent, "base_{}".format(name), pm.dt.Vector(px, py, 0.0) , relative=False, rotation=rot)
            pack["approach_loc"] = CalibrationProgram._make_locator(
                parent, "approach_{}".format(name), pm.dt.Vector(
                    px, py, k.CAL_APPROACH_HEIGHT),relative=False, rotation=rot)
            locators.append(pack)
        return locators
