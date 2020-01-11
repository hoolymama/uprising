import logging

from robolink import ITEM_TYPE_ROBOT, Robolink

import brush_utils as butl
import const as k
import palette_utils as putl
import props
import pymel.core as pm
import sheets
import uprising_util as uutl
from brush import Brush
from paint import Paint


from program import (
    BoardCalibration,
    BrushHangProgram,
    DipProgram,
    HolderCalibration,
    MainProgram,
    ManualTriangulation,
    PapExerciseProgram,
    PerspexCalibration,
    PickAtHomeProgram,
    PickProgram,
    PlaceAtHomeProgram,
    PlaceProgram,
    PotCalibration,
    PotHandleExerciseProgram,
    PotHolderCalibration,
    RetardantProgram,
    WaterProgram,
)

logger = logging.getLogger("uprising")


DIP_TARGET = "dipTarget"
TOOL_TARGET = "toolChangeTarget"
HOME_TARGET = "homeTarget"


class StudioError(Exception):
    pass


class Studio(object):
    """Glue together the entire studio."""

    def __init__(self, **kw):

        uutl.clean_rdk()
        self.RL = Robolink()
        self.robot = self.RL.Item("", ITEM_TYPE_ROBOT)
        self.robot.setParam("PostProcessor", "KUKA KRC4_RN")
        self.approaches_frame = None
        self.dip_approach = None
        self.tool_approach = None
        self.home_approach = None
        self.painting_program = None
        self.rack_cal_program = None

        self.pot_cal_program = None
        self.holder_cal_program = None

        self.board_cal_program = None
        self.perspex_cal_program = None
        self.manual_tri_program = None

        self.dip_programs = []
        self.water_programs = []
        self.retardant_programs = []

        self.pick_place_programs = []
        self.exercise_program = None
        self.pot_handle_exercise_program = None
        self.brush_hang_program = None

        self.pause = kw.get("pause", -1)
        self.do_rack_and_holder_geo = kw.get("do_rack_and_holder_geo")

        self.dips_frame = None
        self.wash_frame = None

        self.do_subprograms = kw.get("do_subprograms")
        self.partial_size = kw.get("partial_size", 0)

        water_wipe_repeats = kw.get("water_wipe_repeats")

        # Must explicitly ask for pick and place to be generated, even
        # if gripper on. Otherwise we can't do partials, like validation.

        pick_and_place_slots = kw.get("pick_and_place_slots")
        pot_handle_exercise_data = kw.get("pot_handle_exercise_data", [])
        brush_hang_data = kw.get("brush_hang_data", [])

        if kw.get("do_painting"):
            logger.debug("Studio: main_painting")
            with uutl.final_position(pm.PyNode("mainPaintingShape")):
                pm.PyNode("mainPaintingShape").attr("applyBrushBiases").set(True)
                self.painting_program = MainProgram("px", **kw)
                pm.PyNode("mainPaintingShape").attr("applyBrushBiases").set(False)

        if kw.get("do_dips"):
            logger.debug("Studio: dips")
            with uutl.final_position(pm.PyNode("RACK1_CONTEXT")):
                self.dip_programs = self._build_dip_programs()

            logger.debug("Studio: water dips")
            with uutl.final_position(pm.PyNode("RACK1_CONTEXT")):
                self.water_programs = self._build_water_programs(water_wipe_repeats)

            logger.debug("Studio: retardant dips")
            with uutl.final_position(pm.PyNode("RACK1_CONTEXT")):
                self.retardant_programs = self._build_retardant_programs()

        if pick_and_place_slots:
            logger.debug("Studio: pick_place_programs")
            with uutl.final_position(pm.PyNode("RACK1_CONTEXT")):
                self.pick_place_programs = self._build_pick_place_programs(
                    pick_and_place_slots
                )

        if kw.get("do_pot_calibration"):
            logger.debug("Studio:  pot_holder_calibration")
            self.pot_cal_program = PotCalibration(k.POT_CALIBRATION_PROGRAM_NAME)

        if kw.get("do_holder_calibration"):
            logger.debug("Studio:  holder_calibration")
            self.holder_cal_program = HolderCalibration(
                k.HOLDER_CALIBRATION_PROGRAM_NAME
            )

        if kw.get("do_perspex_calibration"):
            logger.debug("Studio:  perspex_calibration")
            self.perspex_cal_program = PerspexCalibration(
                k.PERSPEX_CALIBRATION_PROGRAM_NAME
            )

        if kw.get("do_perspex_triangulation"):
            logger.debug("Studio:  manual_triangulation")
            self.manual_tri_program = ManualTriangulation(
                k.TRI_CALIBRATION_PROGRAM_NAME
            )

        if kw.get("do_board_calibration"):
            logger.debug("Studio: board_calibration")
            self.board_cal_program = BoardCalibration(k.BOARD_CALIBRATION_PROGRAM_NAME)

        if kw.get("do_pap_exercise"):
            self.exercise_program = PapExerciseProgram(k.PAP_EXERCISE_PROGRAM_NAME)

        if pot_handle_exercise_data:
            logger.debug("Studio:  pot_handle_exercise")
            brush_ids = list(set([b["brush"] for b in pot_handle_exercise_data]))
            self.pick_place_programs = self._build_pick_place_programs(brush_ids)

            with uutl.final_position(pm.PyNode("RACK1_CONTEXT")):
                self.dip_programs = self._build_dip_programs(
                    dip_combinations=pot_handle_exercise_data
                )

            self.pot_handle_exercise_program = PotHandleExerciseProgram(
                k.POT_HANDLE_EXERCISE_PROGRAM_NAME, pot_handle_exercise_data
            )

        if brush_hang_data:
            logger.debug("Studio:  brush_hang_data")
            brush_ids = [b["id"] for b in brush_hang_data]
            self.pick_place_programs = self._build_pick_place_programs(brush_ids)

            self.brush_hang_program = BrushHangProgram(
                k.BRUSH_HANG_PROGRAM_NAME, brush_hang_data
            )

    def _build_dip_programs(self, **kw):
        packs = putl.get_dip_wipe_packs(**kw)
        result = []
        if packs:
            for pid in packs:
                paint_pack = packs[pid]
                for bid in paint_pack:
                    pack = paint_pack[bid]
                    result.append(DipProgram(pack))
        return result

    def _build_water_programs(self, repeats):
        packs = putl.get_dip_wipe_packs(paint_id=k.WATER_POT_ID)
        result = []
        if packs:
            for pid in packs:
                paint_pack = packs[pid]
                for bid in paint_pack:
                    pack = paint_pack[bid]
                    result.append(WaterProgram(pack, repeats))
        return result

    def _build_retardant_programs(self):
        packs = putl.get_dip_wipe_packs(paint_id=k.RETARDANT_POT_ID)
        result = []
        if packs:
            for pid in packs:
                paint_pack = packs[pid]
                for bid in paint_pack:
                    pack = paint_pack[bid]
                    result.append(RetardantProgram(pack))
        return result

    def _build_pick_place_programs(self, brush_ids):

        gripper_geo = butl.setup_gripper_from_sheet()
        gripper = Brush(0, gripper_geo.attr("outPaintBrush"))
        if not gripper:
            raise StudioError("No Gripper. Risk of damage. Can't continue.")

        packs = putl.get_pick_place_packs(brush_ids)

        result = []
        for p in packs:
            pack = packs[p]

            if brush_ids == "calibration":
                pick_prg = PickAtHomeProgram(gripper, pack)
                place_prg = PlaceAtHomeProgram(gripper, pack)
            else:
                pick_prg = PickProgram(gripper, pack)
                place_prg = PlaceProgram(gripper, pack)
            result += [pick_prg, place_prg]
        return result

    ####################################

    def write_approaches(self):
        self.approaches_frame = uutl.create_frame("ax_frame")
        self.tool_approach = uutl._create_joint_target(
            pm.PyNode(TOOL_TARGET), "tool_approach", self.approaches_frame
        )
        self.home_approach = uutl._create_joint_target(
            pm.PyNode(HOME_TARGET), "home_approach", self.approaches_frame
        )
        self.dip_approach = uutl._create_joint_target(
            pm.PyNode(DIP_TARGET), "dip_approach", self.approaches_frame
        )

    def _write_rack_and_holder_geo(self):
        if self.do_rack_and_holder_geo:
            rack_context = pm.PyNode("RACK1_CONTEXT")
            with uutl.final_position(rack_context):
                ref_geo = [pm.PyNode("rackTop")]
                ref_geo += pm.ls("holders|*|holderTrans|lowResGeo")
                props.send(ref_geo)

    def write_painting_program(self, chunk_id, chunk_length):
        prg = self.painting_program
        if not prg:
            return
        prg.write(self, chunk_id=chunk_id, chunk_length=chunk_length)
        return prg.program_name

    def write_pick_place_program(self, index):
        rack_context = pm.PyNode("RACK1_CONTEXT")
        if self.pick_place_programs and index < len(self.pick_place_programs):
            self.pick_place_frame = uutl.create_frame("pick_place_frame")
            with uutl.final_position(rack_context):
                prg = self.pick_place_programs[index]
                prg.write(self)
            return prg.program_name

    def write_dip_program(self, index):
        if self.dip_programs and index < len(self.dip_programs):
            self.dips_frame = uutl.create_frame("dips_frame")
            prg = self.dip_programs[index]
            prg.write(self)
            return prg.program_name

    def write_water_program(self, index):
        if self.water_programs and index < len(self.water_programs):
            self.wash_frame = uutl.create_frame("wash_frame")
            prg = self.water_programs[index]
            prg.write(self)
            return prg.program_name

    def write_retardant_program(self, index):
        if self.retardant_programs and index < len(self.retardant_programs):
            self.wash_frame = uutl.create_frame("wash_frame")
            prg = self.retardant_programs[index]
            prg.write(self)
            return prg.program_name

    def write(self):

        self.write_approaches()

        rack_context = pm.PyNode("RACK1_CONTEXT")
        painting_context = pm.PyNode("mainPaintingGroup")

        if self.painting_program:
            self.painting_program.write(self)

        if self.pick_place_programs:
            self.pick_place_frame = uutl.create_frame("pick_place_frame")
            with uutl.final_position(rack_context):
                for prog in self.pick_place_programs:
                    prog.write(self)

        if self.dip_programs:
            self.dips_frame = uutl.create_frame("dips_frame")
            for dip in self.dip_programs:
                dip.write(self)
            if self.do_rack_and_holder_geo:
                with uutl.final_position(rack_context):
                    Paint.write_geos()

        if self.water_programs:
            self.wash_frame = uutl.create_frame("wash_frame")
            for water in self.water_programs:
                water.write(self)

        if self.retardant_programs:
            self.wash_frame = uutl.create_frame("wash_frame")
            for retardant in self.retardant_programs:
                retardant.write(self)

        if self.exercise_program:
            self.exercise_program.write(self)
            with uutl.final_position(rack_context):
                self._write_rack_and_holder_geo()

        if self.pot_handle_exercise_program:
            self.pot_handle_exercise_program.write(self)
            with uutl.final_position(rack_context):
                self._write_rack_and_holder_geo()

        if self.brush_hang_program:
            self.hang_frame = uutl.create_frame("hang_frame")
            with uutl.final_position(rack_context):
                self.brush_hang_program.write(self)
                props.send([pm.PyNode("rackTop")])

        if self.pot_cal_program:
            with uutl.final_position(rack_context):
                self.pot_cal_program.write(self.tool_approach, self.home_approach)

        if self.holder_cal_program:
            with uutl.final_position(rack_context):
                self.holder_cal_program.write(self.tool_approach, self.home_approach)

        if self.perspex_cal_program:
            with uutl.final_position(rack_context):
                self.perspex_cal_program.write(self.tool_approach, self.home_approach)

        if self.manual_tri_program:
            self.manual_tri_program.write(self.tool_approach, self.home_approach)

        if self.board_cal_program:
            with uutl.final_position(painting_context):
                self.board_cal_program.write(self.tool_approach, self.home_approach)
