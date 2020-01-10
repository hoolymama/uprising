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
    COLLISION_OFF,
)

import const as k
import logging

logger = logging.getLogger("uprising")


class ProgramError(Exception):
    pass


class Program(object):
    def __init__(self, name):
        self.program_name = name
        self.program_name_prefix = name
        self.program = None

    def write(self):
        self.program = uutl.create_program(self.program_name)
        self.program.ShowInstructions(False)

    def validate_path(self):
        if self.program:
            update_result = self.program.Update(COLLISION_OFF)
            return {
                "name": self.program_name,
                "instructions": update_result[0],
                "time": update_result[1],
                "distance": update_result[2],
                "completion": update_result[3],
                "problems": update_result[4],
                "status": "SUCCESS" if (update_result[3] == 1.0) else "FAILURE",
            }
    def ensure_gripper_open(self):
        self.program.RunInstruction("Gripper opens here", INSTRUCTION_SHOW_MESSAGE)
        self.program.RunInstruction("$OUT[2]=FALSE", INSTRUCTION_INSERT_CODE)
        self.program.RunInstruction("$OUT[1]=TRUE", INSTRUCTION_INSERT_CODE)
        self.program.RunInstruction("WAIT FOR ($IN[2])", INSTRUCTION_INSERT_CODE)

class MainProgram(Program):
    def __init__(self, name, **kw):
        super(MainProgram, self).__init__(name)
        
        self.painting = ptg.Painting(pm.PyNode("mainPaintingShape"))
        self.do_water_dip = kw.get("do_water_dip")
        self.do_retardant_dip = kw.get("do_retardant_dip")
        self.pause_brush_list = kw.get("pause_brushes", [])

    def bump_program_name(self, suffix):
        self.program_name = "{}_{:02d}".format(self.program_name_prefix, suffix)

    def write(self, studio, **kw):
        if not self.painting.clusters:
            return
        num_clusters = len(self.painting.clusters) 
        chunk_id = kw.get("chunk_id", 0)
        chunk_length = kw.get("chunk_length", num_clusters )
        start = chunk_id*chunk_length
        end = start+chunk_length
        end = min(end, num_clusters)
        last_chunk = (end >= num_clusters)
        first_chunk = start == 0
        
        self.bump_program_name(chunk_id)

        super(MainProgram, self).write()

        self.frame = uutl.create_frame("{}_frame".format(self.program_name))

        # with uutl.minimize_robodk():
        self.painting.write_brushes()
   
        last_brush_id = None if start == 0 else  self.painting.clusters[(start-1)].brush.id
        if first_chunk:
            self.ensure_gripper_open()

        
        for cluster in self.painting.clusters[start:end]:
            this_brush_id = cluster.brush.id
            dip_repeats = 1
            if cluster.reason == "tool":
                did_change_brush = last_brush_id != this_brush_id

                if did_change_brush:
                    if last_brush_id is not None:
                        self._on_end_tool(last_brush_id)
                    self._on_start_tool(this_brush_id)
                    dip_repeats = studio.first_dip_repeats
                last_brush_id = this_brush_id
            self._write_dip_and_cluster(cluster, dip_repeats, studio)

        if last_chunk:
            self._on_end_tool(last_brush_id)
            self.program.addMoveJ(studio.home_approach)


        # return True

    def _on_start_tool(self, brush_id):
        pick_program_name = PickProgram.generate_program_name(brush_id)

        self.program.RunInstruction(pick_program_name, INSTRUCTION_CALL_PROGRAM)

        if brush_id in self.pause_brush_list:
            self.program.Pause()
            self.program.RunInstruction(
                "New bush, press continue when ready. ID: {:02d}".format(brush_id),
                INSTRUCTION_SHOW_MESSAGE
            )
        if self.do_water_dip:
            self.program.RunInstruction(
                WaterProgram.generate_program_name(brush_id),
                INSTRUCTION_CALL_PROGRAM)

    def _on_end_tool(self, brush_id):
        if self.do_retardant_dip:
            retardant_program_name = RetardantProgram.generate_program_name(brush_id)
            self.program.RunInstruction(retardant_program_name, INSTRUCTION_CALL_PROGRAM)

        place_program_name = PlaceProgram.generate_program_name(brush_id)
        self.program.RunInstruction(place_program_name, INSTRUCTION_CALL_PROGRAM)


    def _write_dip_and_cluster(self, cluster, dip_repeats, studio):
        dip_program_name = DipProgram.generate_program_name(
            cluster.paint.id, cluster.brush.id
        )
        for repeat in range(dip_repeats):
            self.program.RunInstruction(dip_program_name, INSTRUCTION_CALL_PROGRAM)

        self.program.addMoveJ(studio.dip_approach)
        cluster.write(self.program, self.frame, self.painting.motion, studio.RL, studio.robot)
        self.program.addMoveJ(studio.dip_approach)


class PapExerciseProgram(Program):
    def __init__(self, name):
        super(PapExerciseProgram, self).__init__(name)

    def write(self, studio):

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
                            last_brush_id, brush_id
                        ),
                        INSTRUCTION_SHOW_MESSAGE,
                    )

                    # put the last brush back
                    place_program_name = PlaceProgram.generate_program_name(
                        last_brush_id
                    )
                    self.program.RunInstruction(
                        place_program_name, INSTRUCTION_CALL_PROGRAM
                    )
                    ########################

                pick_program_name = PickProgram.generate_program_name(brush_id)
                self.program.RunInstruction(pick_program_name, INSTRUCTION_CALL_PROGRAM)

                last_brush_id = brush_id

            ###########################
            self.program.Pause()
            self.program.RunInstruction(
                "About to replace brush {:02d} and go home.".format(last_brush_id),
                INSTRUCTION_SHOW_MESSAGE,
            )

            place_program_name = PlaceProgram.generate_program_name(last_brush_id)
            self.program.RunInstruction(place_program_name, INSTRUCTION_CALL_PROGRAM)
            ###########################

            self.program.addMoveJ(studio.home_approach)



class PotHandleExerciseProgram(Program):
    def __init__(self, name, data):
        super(PotHandleExerciseProgram, self).__init__(name)
        self.data=data

    def write(self, studio):

        super(PotHandleExerciseProgram, self).write()
        self.frame = uutl.create_frame("{}_frame".format(self.program_name))

        with uutl.minimize_robodk():

            last_brush_id = None

            brush_ids = list(set([b["brush"] for b in self.data]))

            for brush_id in brush_ids:
                if last_brush_id is not None:

                    # put the last brush back
                    place_program_name = PlaceProgram.generate_program_name(
                        last_brush_id
                    )
                    self.program.RunInstruction(
                        place_program_name, INSTRUCTION_CALL_PROGRAM
                    )

                pick_program_name = PickProgram.generate_program_name(brush_id)
                self.program.RunInstruction(pick_program_name, INSTRUCTION_CALL_PROGRAM)

                last_brush_id = brush_id

            ###########################
                for paint_id in  [p["paint"] for p in  self.data if p["brush"] == brush_id]:
                    dip_program_name = DipProgram.generate_program_name(
                        paint_id,brush_id
                    )
                    self.program.RunInstruction(dip_program_name, INSTRUCTION_CALL_PROGRAM)

            place_program_name = PlaceProgram.generate_program_name(last_brush_id)
            self.program.RunInstruction(place_program_name, INSTRUCTION_CALL_PROGRAM)
            ###########################

            self.program.addMoveJ(studio.home_approach)

class BrushHangProgram(Program):
    def __init__(self, name, data):
        super(BrushHangProgram, self).__init__(name)
        self.RL = Robolink()
        self.robot = self.RL.Item("", ITEM_TYPE_ROBOT)
        self.robot.setParam("PostProcessor", "KUKA KRC4_RN")
        self.brush_data = self._get_zero_tip_brushes(data)

    def _get_zero_tip_brushes(self, data):
        result = []
        for b in data:
            node = pm.PyNode(b["brush"])
            with uutl.at_value(node.attr("dipParam") , 0.0):
                result.append({
                    "brush": Brush.brush_set_at_index(b["id"])["outDipBrush"],
                    "twist": b["twist"],
                    "id": b["id"]
                    }
                    )

        return result


    def write(self, studio):

        super(BrushHangProgram, self).write()
        self.frame = uutl.create_frame("{}_frame".format(self.program_name))

        mats = {
            "A": pm.PyNode("hangLocal|approach_loc").attr("worldMatrix[0]").get(),
            "N": pm.PyNode("hangLocal|loc_N").attr("worldMatrix[0]").get(),
            "E": pm.PyNode("hangLocal|loc_E").attr("worldMatrix[0]").get(),
            "W": pm.PyNode("hangLocal|loc_W").attr("worldMatrix[0]").get(),
            "S": pm.PyNode("hangLocal|loc_S").attr("worldMatrix[0]").get()
        }

        with uutl.minimize_robodk():

            last_brush_id = None

            for brush_pack in self.brush_data :
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
                self.program.RunInstruction(pick_program_name, INSTRUCTION_CALL_PROGRAM)

                last_brush_id = brush_id

                # brush (is a PyNode), id, twist (bool)
                ###########################
                self._write_one_hang(brush_pack, mats)
                ###########################


            place_program_name = PlaceProgram.generate_program_name(last_brush_id)
            self.program.RunInstruction(place_program_name, INSTRUCTION_CALL_PROGRAM)
            ###########################

            self.program.addMoveJ(studio.home_approach)

    def _write_one_hang(self, pack, mats):

        targets = {}
        for key in mats:
            targets[key] =  self._create_target_for_brush(
            pack["brush"],
            mats[key],
            "hang_{}_{:02d}".format(key, pack["id"])
        )

        #write the brush
        pack["brush"].write(self.RL, self.robot)
        tool = self.RL.Item(pack["brush"].name)
        if not tool.Valid():
            raise ProgramError("Brush is not valid. Risk of damage. Can't continue.")

        self.program.setSpeed(k.CAL_LINEAR_SPEED, k.CAL_ANGULAR_SPEED)
        self.program.setRounding(k.CAL_ROUNDING_DISTANCE)
        self.program.setPoseTool(tool)

        msg = "Record BACK(X) and RIGHT(Y) offsets for {}".format(pack["id"])
        if pack["twist"]:
            msg = "Watch closely for ARC center (using brush{})".format(pack["id"])

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
        tool_pose = uutl.maya_to_robodk_mat(mat)
        flange_pose = tool_pose * brush.matrix.invH()
        target = self.RL.AddTarget(name, self.frame, self.robot)
        joints = self.robot.SolveIK(flange_pose, k.FACING_RACK_JOINTS)
        target.setPose(tool_pose)
        target.setJoints(joints)
        return target





class DipProgram(Program):
    @staticmethod
    def generate_program_name(paint_id, brush_id):
        return "p{:02d}_b{:02d}".format(paint_id, brush_id)

    def __init__(self, pack):
        name = DipProgram.generate_program_name(pack["paint_id"], pack["brush_id"])

        super(DipProgram, self).__init__(name)
        self.dip_painting = ptg.Painting(pack["dip"])
        self.wipe_painting = ptg.Painting(pack["wipe"])

    def write(self, studio):

        if not (self.dip_painting.clusters and self.wipe_painting.clusters):
            return

        super(DipProgram, self).write()
        self.frame = studio.dips_frame

        self.dip_painting.write_brushes()
        self.wipe_painting.write_brushes()

        with uutl.minimize_robodk():
            self.program.RunInstruction(
                "Dip with tool %s" % self.dip_painting.clusters[0].brush.node_name,
                INSTRUCTION_COMMENT,
            )


            for cluster in self.dip_painting.clusters:
                cluster.write(
                    self.program,
                    self.frame,
                    self.dip_painting.motion,
                    studio.RL,
                    studio.robot,
                )

            for cluster in self.wipe_painting.clusters:
                cluster.write(
                    self.program,
                    self.frame,
                    self.wipe_painting.motion,
                    studio.RL,
                    studio.robot,
                )






class WashProgram(Program):

    @staticmethod
    def generate_program_name(brush_id):
        raise NotImplementedError

    def __init__(self, pack):
        name = self.generate_program_name(pack["brush_id"])
        super(WashProgram, self).__init__(name)
        self.dip_painting = ptg.Painting(pack["dip"])
        self.wipe_painting = ptg.Painting(pack["wipe"])
        self.frame = None
        self.repeats = 0

    def _valid(self):
        return (self.dip_painting.clusters and self.wipe_painting.clusters)

    def write(self, studio):

        super(WashProgram, self).write()

        self.dip_painting.write_brushes()
        self.wipe_painting.write_brushes()

        with uutl.minimize_robodk():
            self.program.RunInstruction(
                "{} dip with tool {}".format(self.__class__.__name__[:-7], self.dip_painting.clusters[0].brush.node_name) ,
                INSTRUCTION_COMMENT,
            )

            for cluster in self.dip_painting.clusters:
                cluster.write(
                    self.program,
                    self.frame,
                    self.dip_painting.motion,
                    studio.RL,
                    studio.robot
                    )

            for repeat in range(self.repeats):
                for cluster in self.wipe_painting.clusters:
                    cluster.write(
                        self.program,
                        self.frame,
                        self.wipe_painting.motion,
                        studio.RL,
                        studio.robot,
                    )


class WaterProgram(WashProgram):
    @staticmethod
    def generate_program_name(brush_id):
        return "water_b{:02d}".format( brush_id)

    def __init__(self, pack, repeats):
        super(WaterProgram, self).__init__(pack)
        self.repeats = repeats

    def write(self, studio):
        if not self._valid:
            return
        self.frame = studio.wash_frame

        super(WaterProgram, self).write(studio)

class RetardantProgram(WashProgram):

    @staticmethod
    def generate_program_name(brush_id):
        return "retardant_b{:02d}".format( brush_id)

    def __init__(self, pack):
        super(RetardantProgram, self).__init__(pack)
        self.repeats = 0

    def write(self, studio):
        if not self._valid:
            return
        self.frame = studio.wash_frame
        super(RetardantProgram, self).write(studio)



class CalibrationProgram(Program):
    def __init__(self, name):
        super(CalibrationProgram, self).__init__(name)
        self.RL = Robolink()
        self.robot = self.RL.Item("", ITEM_TYPE_ROBOT)
        self.robot.setParam("PostProcessor", "KUKA KRC4_RN")
        self.brush = self._get_probe_brush()
        if not self.brush:
            raise ProgramError("No Probe Brush. Risk of damage. Can't continue.")

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
            raise ProgramError("No Probe Brush. Risk of damage. Can't continue.")

        self.write_probe_attach_gripper()

        self.program.setSpeed(k.CAL_LINEAR_SPEED, k.CAL_ANGULAR_SPEED)
        self.program.setRounding(k.CAL_ROUNDING_DISTANCE)
        self.program.setPoseTool(self.tool)
        self.program.RunInstruction("Starting calibration", INSTRUCTION_COMMENT)

        self.write_locator_packs()

        self.write_probe_detach_gripper()

        self.program.addMoveJ(home_approach)


    def write_probe_attach_gripper(self):
        print "write_probe_attach_gripper PickAtHomeProgram"
        pick_program_name = PickAtHomeProgram.generate_program_name(0)
        self.program.RunInstruction(pick_program_name, INSTRUCTION_CALL_PROGRAM)

    def write_probe_detach_gripper(self):
        print "write_probe_detach_gripper PlaceAtHomeProgram"
        place_program_name = PlaceAtHomeProgram.generate_program_name(0)
        self.program.RunInstruction(place_program_name, INSTRUCTION_CALL_PROGRAM)

    def write_locator_packs(self):
        raise NotImplementedError

    def _get_probe_brush(self):
        geo = butl.setup_probe_from_sheet()
        return Brush.brush_at_plug(0, geo.attr("outPaintBrush"))

    def _write_stops(self, loc_a, loc_b, facing_joints):
        if loc_a and loc_b:
            approach_a = pm.PyNode(loc_a)
            approach_b = pm.PyNode(loc_b)

            tmat = pm.dt.TransformationMatrix(approach_a.attr("worldMatrix[0]").get())
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
                    target = self._create_a_target(tmat.asMatrix(), name, facing_joints)
                    self.program.addMoveJ(target)

    def _create_a_target(self, mat, name, facing_joints):
        tool_pose = uutl.maya_to_robodk_mat(mat)
        flange_pose = tool_pose * self.brush.matrix.invH()
        target = self.RL.AddTarget(name, self.frame, self.robot)
        joints = self.robot.SolveIK(flange_pose, facing_joints)
        target.setPose(tool_pose)
        target.setJoints(joints)
        return target



class PotHolderCalibration(Program):
    def __init__(self, name):
        super(PotHolderCalibration, self).__init__(name)

    def write(self):
        super(PotHolderCalibration, self).write()
        self.program.RunInstruction(k.POT_CALIBRATION_PROGRAM_NAME, INSTRUCTION_CALL_PROGRAM)
        self.program.RunInstruction(k.HOLDER_CALIBRATION_PROGRAM_NAME, INSTRUCTION_CALL_PROGRAM)


class HolderCalibration(CalibrationProgram):
    def __init__(self, name):
        super(HolderCalibration, self).__init__(name)
        self.locators = putl.get_pick_place_packs("all")

    def write_locator_packs(self):
        bids = sorted(self.locators.keys())
        for bid in bids:
            pack = self.locators[bid]
            with uutl.at_position(
                pack["trans_node"], k.RACK_HOLDER_DISTANCE, 0, k.RACK_HOLDER_HEIGHT
            ):
                self._write_one_probe(self.locators[bid])

    def _write_one_probe(self, pack):

        self.program.RunInstruction(
            "Moving to holder {}".format(pack["brush_id"]), INSTRUCTION_COMMENT
        )

        row = k.CAL_SHEET_FIRST_ROW + int(pack["brush_id"])

        base_target = self._create_a_target(
            pack["probe"].attr("worldMatrix[0]").get(),
            "base_{:02d}".format(pack["brush_id"]),
            k.FACING_RACK_JOINTS,
        )

        approach_target = self._create_a_target(
            pack["pin_ap"].attr("worldMatrix[0]").get(),
            "approach_{:02d}".format(pack["brush_id"]),
            k.FACING_RACK_JOINTS,
        )

        self.program.addMoveJ(approach_target)
        self.program.addMoveL(base_target)
        self.program.RunInstruction(
            "{} {} in cell {}D +EF by eye".format(
                k.CAL_PAUSE_MESSAGE, pack["brush_id"], row
            ),
            INSTRUCTION_SHOW_MESSAGE,
        )
        self.program.Pause()
        self.program.addMoveL(approach_target)


class PotCalibration(CalibrationProgram):
    def __init__(self, name):
        super(PotCalibration, self).__init__(name)
        self.locators = putl.get_pot_handle_packs()

    def write_locator_packs(self):
        last = None

        for pack in self.locators:
            self._write_stops(last, pack["pot_approach"], k.FACING_RACK_JOINTS)

            self.program.RunInstruction(
                ("Moving to %s" % pack["name"]), INSTRUCTION_COMMENT
            )

            self._write_one_probe(pack, "pot")

            self._write_one_probe(pack, "handle")

            last = pack["handle_approach"]

    def _write_one_probe(self, pack, which):

        column = {"pot": "B", "handle": "C"}

        row = k.CAL_SHEET_FIRST_ROW + pack["index"]

        base_key = "{}_base".format(which)
        approach_key = "{}_approach".format(which)

        base_target = self._create_a_target(
            pack[base_key].attr("worldMatrix[0]").get(),
            "{}_{}_base".format(pack["name"], which),
            k.FACING_RACK_JOINTS,
        )

        approach_target = self._create_a_target(
            pack[approach_key].attr("worldMatrix[0]").get(),
            "{}_{}_approach".format(pack["name"], which),
            k.FACING_RACK_JOINTS,
        )

        self.program.addMoveJ(approach_target)
        self.program.addMoveL(base_target)
        self.program.Pause()
        self.program.RunInstruction(
            "{} {} in cell {}{}".format(
                k.CAL_PAUSE_MESSAGE, pack["name"], row, column[which]
            ),
            INSTRUCTION_SHOW_MESSAGE,
        )
        self.program.addMoveL(approach_target)


class PerspexCalibration(CalibrationProgram):
    def __init__(self, name):
        super(PerspexCalibration, self).__init__(name)
        self.locators = putl.get_perspex_packs()

    def write_locator_packs(self):
        for pack in self.locators:
            self._write_one_probe(pack)

    def _write_one_probe(self, pack):

        self.program.RunInstruction(
            "Moving to probe {} {}".format(pack["name"], pack["index"]),
            INSTRUCTION_COMMENT,
        )

        row = k.PERSPEX_CAL_SHEET_FIRST_ROW + int(pack["index"])

        base_target = self._create_a_target(
            pack["base"].attr("worldMatrix[0]").get(),
            "base_{}".format(pack["name"]),
            k.FACING_RACK_JOINTS,
        )

        approach_target = self._create_a_target(
            pack["approach"].attr("worldMatrix[0]").get(),
            "approach_{}".format(pack["name"]),
            k.FACING_RACK_JOINTS,
        )

        self.program.addMoveJ(approach_target)
        self.program.addMoveL(base_target)
        self.program.RunInstruction(
            "{} {} in cell G{}".format(k.CAL_PAUSE_MESSAGE, pack["name"], row),
            INSTRUCTION_SHOW_MESSAGE,
        )
        self.program.Pause()
        self.program.addMoveL(approach_target)


class ManualTriangulation(CalibrationProgram):
    # def __init__(self, name):
    #     super(ManualTriangulation, self).__init__(name)

    def _get_probe_brush(self):
        geo = butl.setup_probe_from_sheet()
        geo.attr("paintingParam").set(0)
        geo.attr("dipParam").set(0)
        geo.attr("wipeParam").set(0)
        return Brush.brush_at_plug(0, geo.attr("outPaintBrush"))

    def write_locator_packs(self):

        self.program.Pause()
        self.program.RunInstruction(
            "Manually record the 3 leg centers with the brush tip", INSTRUCTION_COMMENT
        )


################# CALIBRATION #################
class BoardCalibration(CalibrationProgram):
    def __init__(self, name):
        super(BoardCalibration, self).__init__(name)
        top_node = pm.PyNode("mainPaintingGroup")
        top_node.attr("zeroPosition").set(1)
        self.locators = self._setup_locators()

    def write_locator_packs(self):

        last = None
        for locator_pack in self.locators:
            self._write_stops(last, locator_pack["approach_loc"], k.FACING_BOARD_JOINTS)
            self.program.RunInstruction(
                ("Moving to %s" % locator_pack["name"]), INSTRUCTION_COMMENT
            )
            self._write_one_probe(locator_pack)
            last = locator_pack["approach_loc"]

    def _write_one_probe(self, pack):

        row = k.CAL_SHEET_FIRST_ROW + pack["id"]

        base_target = self._create_a_target(
            pack["base_loc"].attr("worldMatrix[0]").get(),
            "{}_base".format(pack["name"]),
            k.FACING_BOARD_JOINTS,
        )

        approach_target = self._create_a_target(
            pack["approach_loc"].attr("worldMatrix[0]").get(),
            "{}_approach".format(pack["name"]),
            k.FACING_BOARD_JOINTS,
        )

        self.program.addMoveJ(approach_target)
        self.program.addMoveL(base_target)
        self.program.Pause()
        self.program.RunInstruction(
            "{} {} in cell {}B".format(k.CAL_PAUSE_MESSAGE, pack["name"], row),
            INSTRUCTION_SHOW_MESSAGE,
        )
        self.program.addMoveL(approach_target)

    def _setup_locators(self):
        locators = []
        verts = [
            item for sublist in pm.sets("probePointsSet", q=True) for item in sublist
        ]
        parent = pm.PyNode("|mainPaintingGroup|jpos|probes")
        rot = pm.dt.Vector(180, 0, -90)
        for i, v in enumerate(verts):
            index = int(str(v).split("[")[1].split("]")[0])
            px, py, _ = v.getPosition(space="world")
            name = "board_cal_{:02}".format(index)
            pack = {"name": name, "id": i}
            pack["base_loc"] = CalibrationProgram._make_locator(
                parent,
                "base_{}".format(name),
                pm.dt.Vector(px, py, 0.0),
                relative=False,
                rotation=rot,
            )
            pack["approach_loc"] = CalibrationProgram._make_locator(
                parent,
                "approach_{}".format(name),
                pm.dt.Vector(px, py, k.CAL_APPROACH_HEIGHT),
                relative=False,
                rotation=rot,
            )
            locators.append(pack)
        return locators


################# PICK PLACE #################


class PickPlaceProgram(Program):
    @staticmethod
    def generate_program_name(brush_id):
        raise NotImplementedError

    def __init__(self, brush, pack, name):

        super(PickPlaceProgram, self).__init__(name)

        self.brush = brush
        self.pack = pack
        self.targets = {}

    def write_brush(self, studio):

        existing_brush = studio.RL.Item(self.brush.name)
        if existing_brush.Valid():
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

        self.tool = studio.RL.Item(self.brush.name)
        if not self.tool.Valid():
            raise ProgramError("No Gripper Brush. Risk of damage. Can't continue.")

        self.program.setRounding(self.pack["rounding"])
        self.program.setPoseTool(self.tool)

        self.pin_target = self._create_target_from_pack("pin", studio.RL, studio.robot)
        self.pin_ap_target = self._create_target_from_pack(
            "pin_ap", studio.RL, studio.robot
        )
        self.clear_target = self._create_target_from_pack(
            "clear", studio.RL, studio.robot
        )
        self.clear_ap_target = self._create_target_from_pack(
            "clear_ap", studio.RL, studio.robot
        )

    def _create_target_from_pack(self, key, RL, robot):
        return self._create_a_target(
            self.pack[key].attr("worldMatrix[0]").get(),
            "pick_place_{}_{:02d}".format(key, self.pack["brush_id"]),
            k.FACING_RACK_JOINTS,
            RL,
            robot,
        )

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

        pause_ms = int(studio.pause)
        with uutl.minimize_robodk():
            super(PickProgram, self).write(studio)

            self.program.RunInstruction("Starting pick", INSTRUCTION_COMMENT)
            self.program.addMoveJ(self.pin_ap_target)

            self.program.setSpeed(
                self.pack["precision_lin_speed"], self.pack["ang_speed"]
            )
            self.program.addMoveL(self.pin_target)

            self.program.RunInstruction("Gripper closes here", INSTRUCTION_SHOW_MESSAGE)
            self.program.Pause(pause_ms)
            self.program.RunInstruction("$OUT[1]=FALSE", INSTRUCTION_INSERT_CODE)
            self.program.RunInstruction("$OUT[2]=TRUE", INSTRUCTION_INSERT_CODE)
            self.program.RunInstruction("WAIT FOR ($IN[1])", INSTRUCTION_INSERT_CODE)
            self.program.RunInstruction(
                "Gripper open. Make sure it dropped the tool properly.",
                INSTRUCTION_SHOW_MESSAGE,
            )
            self.program.Pause(pause_ms)
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

    def write(self, studio):

        pause_ms = int(studio.pause)
        with uutl.minimize_robodk():
            super(PlaceProgram, self).write(studio)

            self.program.RunInstruction("Starting place", INSTRUCTION_COMMENT)
            self.program.addMoveJ(self.clear_ap_target)
            self.program.setSpeed(self.pack["lin_speed"], self.pack["ang_speed"])
            self.program.addMoveL(self.clear_target)
            self.program.setSpeed(
                self.pack["precision_lin_speed"], self.pack["ang_speed"]
            )
            self.program.addMoveL(self.pin_target)

            self.program.RunInstruction("Gripper opens here", INSTRUCTION_SHOW_MESSAGE)
            self.program.Pause(pause_ms)
            self.program.RunInstruction("$OUT[2]=FALSE", INSTRUCTION_INSERT_CODE)
            self.program.RunInstruction("$OUT[1]=TRUE", INSTRUCTION_INSERT_CODE)
            self.program.RunInstruction("WAIT FOR ($IN[2])", INSTRUCTION_INSERT_CODE)
            self.program.Pause(pause_ms)
            self.program.addMoveL(self.pin_ap_target)


class PickAtHomeProgram(PickPlaceProgram):
    @staticmethod
    def generate_program_name(brush_id):
        return "pick_{:02d}".format(brush_id)

    def __init__(self, brush, pack):
        name = PickAtHomeProgram.generate_program_name(pack["brush_id"])
        super(PickAtHomeProgram, self).__init__(brush, pack, name)

    def write(self, studio):

        with uutl.minimize_robodk():
            super(PickAtHomeProgram, self).write(studio)

            self.program.RunInstruction("Starting pick", INSTRUCTION_COMMENT)

            self.program.addMoveJ(studio.home_approach)
            self.program.RunInstruction(
                "Continue when you are ready for the robot to grip the probe.",
                INSTRUCTION_SHOW_MESSAGE,
            )
            self.program.Pause(-1)
            self.program.RunInstruction("$OUT[1]=FALSE", INSTRUCTION_INSERT_CODE)
            self.program.RunInstruction("$OUT[2]=TRUE", INSTRUCTION_INSERT_CODE)
            self.program.RunInstruction("WAIT FOR ($IN[1])", INSTRUCTION_INSERT_CODE)

            self.program.RunInstruction(
                "Did it grip properly? If so hit continue.", INSTRUCTION_SHOW_MESSAGE
            )
            self.program.Pause(-1)


class PlaceAtHomeProgram(PickPlaceProgram):
    @staticmethod
    def generate_program_name(brush_id):
        return "place_{:02d}".format(brush_id)

    def __init__(self, brush, pack):
        name = PlaceAtHomeProgram.generate_program_name(pack["brush_id"])
        super(PlaceAtHomeProgram, self).__init__(brush, pack, name)

    def write(self, studio):

        with uutl.minimize_robodk():
            super(PlaceAtHomeProgram, self).write(studio)

            self.program.RunInstruction("Starting place", INSTRUCTION_COMMENT)
            self.program.addMoveJ(studio.home_approach)

            self.program.RunInstruction(
                "Continue when you are ready for the robot to release the probe.",
                INSTRUCTION_SHOW_MESSAGE,
            )
            self.program.Pause(-1)
            self.program.RunInstruction("$OUT[2]=FALSE", INSTRUCTION_INSERT_CODE)
            self.program.RunInstruction("$OUT[1]=TRUE", INSTRUCTION_INSERT_CODE)
            self.program.RunInstruction("WAIT FOR ($IN[2])", INSTRUCTION_INSERT_CODE)
            self.program.RunInstruction(
                "Did you catch it? Hit continue to end the program.",
                INSTRUCTION_SHOW_MESSAGE,
            )

