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


class MainProgram(Program):
    def __init__(self, name, node):
        super(MainProgram, self).__init__(name)
        self.painting = ptg.Painting(node)

    def write(self, studio):
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

            cluster.write(studio, self.program, self.frame, motion)
        self.program.addMoveJ(studio.home_approach)
        self.program.ShowInstructions(False)


class DipProgram(Program):

    @staticmethod
    def generate_program_name(paint_id, brush_id):
        return "p{}_b{}".format(paint_id, brush_id)

    def __init__(self, name, dip_node, wipe_node):
        super(DipProgram, self).__init__(name)

        print "DIP {}".format(dip_node)
        print "WIP {}".format(wipe_node)

        self.dip_painting = ptg.Painting(dip_node)
        self.wipe_painting = ptg.Painting(wipe_node)

    def write(self, studio):
        super(DipProgram, self).write()
        self.dip_painting.write_brushes()
        self.wipe_painting.write_brushes()

        self.program.RunInstruction(
            "Dip with tool %s" %
            self.dip_painting.clusters[0].brush.node_name, INSTRUCTION_COMMENT)

        self.program.addMoveJ(studio.dip_approach)

        for cluster in self.dip_painting.clusters:
            cluster.write(
                studio,
                self.program,
                self.frame,
                self.dip_painting.motion)

        for cluster in self.wipe_painting.clusters:
            cluster.write(
                studio,
                self.program,
                self.frame,
                self.wipe_painting.motion)

        self.program.addMoveJ(studio.dip_approach)


class RackCalibration(Program):

    @staticmethod
    def _make_locator(pot, name, pos):
        xf = pot.getParent().getParent()
        path = "{}|{}".format(xf, name)
        try:
            n = pm.PyNode(path)
            pm.delete(n)
        except BaseException:
            pass
        loc = pm.spaceLocator()
        pm.parent(loc, xf, relative=True)
        loc.rename(name)
        loc.attr("t").set(pos)
        loc.attr("ry").set(180)
        return loc

    def __init__(self, name, rack_node):
        super(RackCalibration, self).__init__(name)
        self.RL = Robolink()
        self.robot = self.RL.Item('', ITEM_TYPE_ROBOT)

        self.brush = self._get_probe_brush()
        if not self.brush:
            raise ProgramError(
                "No Probe Brush. Risk of damage. Can't continue.")
            
        self.pot_handle_pairs = putl.get_pot_handle_pairs()
        self.locators = self._setup_locators()


    def write(self, tool_approach, home_approach):
        super(RackCalibration, self).write()
        self.brush.write()
        self.tool = self.RL.Item(self.brush.name)
        if not self.tool.Valid():
            raise ProgramError(
                "No Probe Brush. Risk of damage. Can't continue.")
        self._write_setup(tool_approach)

        last = None
        for locator_pack in self.locators:
            self._write_stops(last, locator_pack["pot_approach_loc"])
            self._write_pot_and_handle(locator_pack)
            last = locator_pack["handle_approach_loc"]
        self.program.addMoveJ(home_approach)

    def _write_pot_and_handle(self, locator_pack):
  
        self.program.RunInstruction(("Moving to %s" % locator_pack["name"]),
                                    INSTRUCTION_COMMENT)

        self._write_one_probe(locator_pack, "pot")
        self._write_one_probe(locator_pack, "handle")

 
    def _write_one_probe(self, pack, which):

        column = {
            "pot": "B",
            "handle": "C"
        }

        row = k.RACK_SHEET_FIRST_ROW + int(pack["name"].split("_")[-1])



        base_key = "{}_base_loc".format(which)
        approach_key = "{}_approach_loc".format(which)
        
        base_target = self._create_a_target(
            pack[base_key].attr("worldMatrix[0]").get(), 
            "{}_{}_base".format(pack["name"], which)
            )

        approach_target = self._create_a_target(
            pack[approach_key].attr("worldMatrix[0]").get(), 
            "{}_{}_approach".format(pack["name"], which)
            )

        self.program.addMoveJ(approach_target)
        self.program.addMoveL(base_target)
        self.program.RunInstruction(
            "{} {} in the {} column, cell {}{}".format(k.RACK_PAUSE_MESSAGE, pack["name"] , which.upper(), row, column[which] ),
            INSTRUCTION_SHOW_MESSAGE)
        self.program.Pause()
        self.program.addMoveL(approach_target)



    def _write_setup(self, tool_approach):
        self.program.setSpeed(k.RACK_LINEAR_SPEED, k.RACK_ANGULAR_SPEED)
        self.program.setRounding(k.RACK_ROUNDING_DISTANCE)
        self.program.setPoseTool(self.tool)
        self.program.RunInstruction(
            "Starting rack calibration",
            INSTRUCTION_COMMENT)
        self.program.addMoveJ(tool_approach)
        self.program.RunInstruction(
            "Attach calibration Tool: {}".format(
                self.brush.name), INSTRUCTION_SHOW_MESSAGE)
        self.program.Pause()

    def _get_probe_brush(self):
        geo = butl.setup_probe_from_sheet()
        return Brush.brush_at_plug(0, geo.attr("outPaintBrush"))

    def _write_stops(self, loc_a, loc_b):
        if loc_a and loc_b:
            approach_a = pm.PyNode(loc_a)
            approach_b = pm.PyNode(loc_b)

            tmat = pm.dt.TransformationMatrix(
                approach_a.attr("worldMatrix[0]").get())
            tx_a = approach_a.attr("worldMatrix[0]").get().translate
            tx_b = approach_b.attr("worldMatrix[0]").get().translate

            dist = (tx_b - tx_a).length()

            if dist > k.RACK_P2P_SPAN:
                num_spans = math.ceil(dist / k.RACK_P2P_SPAN)
                gap = dist / num_spans
                vec = (tx_b - tx_a).normal() * gap
                for i in range(int(num_spans) - 1):
                    tx_a += vec
                    tmat.setTranslation(tx_a, "world")
                    name = "%s_stop_%d" % (loc_b, i)
                    target = self._create_a_target(
                        tmat.asMatrix(), name)
                    self.program.addMoveJ(target)

    def _create_a_target(self, mat, name):
        tool_pose = uutl.maya_to_robodk_mat(mat)
        flange_pose = tool_pose * self.brush.matrix.invH()
        target = self.RL.AddTarget(name, self.frame, self.robot)
        joints = self.robot.SolveIK(flange_pose, k.FACING_RACK_JOINTS)
        target.setPose(tool_pose)
        target.setJoints(joints)
        return target


    def _setup_locators(self):
        locators = []
        i = 0
        for pot, handle in self.pot_handle_pairs:
            handle_x_offset = handle.getParent().attr("tx").get()
            pack = {}
            pack["name"] = "{}_{:02}".format(pot.split("|")[-1], i)
            pack["pot_base_loc"] = RackCalibration._make_locator(
                pot, "pot_base_loc", pm.dt.Vector(0, 0, -k.RACK_POT_DEPTH))
            pack["pot_approach_loc"] = RackCalibration._make_locator(
                pot, "pot_approach_loc", pm.dt.Vector(0, 0, k.RACK_APPROACH_HEIGHT))
            pack["handle_base_loc"] = RackCalibration._make_locator(
                pot, "handle_base_loc", pm.dt.Vector(
                    handle_x_offset, 0, k.RACK_HANDLE_HEIGHT))
            pack["handle_approach_loc"] = RackCalibration._make_locator(
                pot, "handle_approach_loc", pm.dt.Vector(
                    handle_x_offset, 0, k.RACK_APPROACH_HEIGHT))
            locators.append(pack)
            i+=1
        return locators
