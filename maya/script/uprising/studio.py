

import pymel.core as pm
import uprising_util as uutl
import palette_utils as putl
import sheets

from paint import Paint
import props
import const as k


from robolink import (
    Robolink,
    ITEM_TYPE_ROBOT
)

# import program as prg

from program import (
    MainProgram,
    DipProgram,
    SlopProgram,
    PotHolderCalibration,
    PotCalibration,
    HolderCalibration,
    BoardCalibration,
    PerspexCalibration,
    PickProgram,
    PlaceProgram,
    PickAtHomeProgram,
    PlaceAtHomeProgram,
    PapExerciseProgram,
    ManualTriangulation)


import brush_utils as butl
from brush import Brush
import logging
logger = logging.getLogger('uprising')


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
        self.robot = self.RL.Item('', ITEM_TYPE_ROBOT)
        self.robot.setParam("PostProcessor", "KUKA KRC4_RN")
        self.approaches_frame = None
        self.dip_approach = None
        self.tool_approach = None
        self.home_approach = None
        self.painting_program = None
        self.rack_cal_program = None

        self.pot_holder_cal_program = None
        self.pot_cal_program = None
        self.holder_cal_program = None
        
        self.board_cal_program = None
        self.perspex_cal_program = None
        self.manual_tri_program = None

        self.dip_programs = []
        self.slop_programs = []
        self.pick_place_programs = []
        self.exercise_program = None

        self.pause = kw.get("pause", -1)

        self.do_rack_and_holder_geo = kw.get("do_rack_and_holder_geo")

        self.first_dip_repeats = kw.get("first_dip_repeats", 1)

        do_painting = kw.get("do_painting")
        do_dips = kw.get("do_dips")
        do_slop = kw.get("do_slop")
        do_pap_exercise = kw.get("do_pap_exercise")
        do_board_calibration = kw.get("do_board_calibration")

        do_pot_holder_calibration = kw.get("do_pot_holder_calibration")
        do_pot_calibration = kw.get("do_pot_calibration")  or kw.get("do_pot_holder_calibration")
        do_holder_calibration = kw.get("do_holder_calibration") or kw.get("do_pot_holder_calibration")

        do_perspex_calibration = kw.get("do_perspex_calibration")
        do_perspex_triangulation = kw.get("do_perspex_triangulation")

        first_dip_repeats = kw.get("first_dip_repeats", 1)
        # Must explicitly ask for pick and place to be generated, even
        # if gripper on. Otherwise we can't do partials, like validation.

        pick_and_place_slots = kw.get("pick_and_place_slots")

        if do_painting:
            logger.debug("Studio: main_painting")
            with uutl.final_position(pm.PyNode("mainPaintingShape")):
                pm.PyNode("mainPaintingShape").attr("applyBrushBiases").set(True)
                self.painting_program = MainProgram("px")
                pm.PyNode("mainPaintingShape").attr("applyBrushBiases").set(False)

        if do_dips:
            logger.debug("Studio: dips")
            with uutl.final_position(pm.PyNode("RACK1_CONTEXT")):
                self.dip_programs = self._build_dip_programs()

        if pick_and_place_slots:
            logger.debug("Studio: pick_place_programs")
            with uutl.final_position(pm.PyNode("RACK1_CONTEXT")):
                self.pick_place_programs = self._build_pick_place_programs(
                    pick_and_place_slots)
        if do_slop:
            logger.debug("Studio: do_slop")
            with uutl.final_position(pm.PyNode("RACK1_CONTEXT")):
                self.slop_programs = self._build_slop_programs()


        if do_pot_calibration:
            logger.debug("Studio:  pot_holder_calibration")
            self.pot_cal_program = PotCalibration(
                k.POT_CALIBRATION_PROGRAM_NAME)

        if do_holder_calibration:
            logger.debug("Studio:  holder_calibration")
            self.holder_cal_program = HolderCalibration(
                k.HOLDER_CALIBRATION_PROGRAM_NAME)

        if do_pot_holder_calibration:
            logger.debug("Studio:  pot_holder_calibration")
            self.pot_holder_cal_program = PotHolderCalibration(
                k.POT_HOLDER_CALIBRATION_PROGRAM_NAME)


        if do_perspex_calibration:
            logger.debug("Studio:  perspex_calibration")
            self.perspex_cal_program = PerspexCalibration(
                k.PERSPEX_CALIBRATION_PROGRAM_NAME)

        if do_perspex_triangulation:
            logger.debug("Studio:  manual_triangulation")
            self.manual_tri_program = ManualTriangulation(
                k.TRI_CALIBRATION_PROGRAM_NAME)

        if do_board_calibration:
            logger.debug("Studio: board_calibration")
            self.board_cal_program = BoardCalibration(
                k.BOARD_CALIBRATION_PROGRAM_NAME)

        if do_pap_exercise:
            self.exercise_program = PapExerciseProgram(
                k.PAP_EXERCISE_PROGRAM_NAME)

    def _build_dip_programs(self):
        packs = putl.get_dip_wipe_packs()
        result = []
        if packs:
            for pid in packs:
                paint_pack = packs[pid]
                for bid in paint_pack:
                    pack = paint_pack[bid]
                    result.append(
                        DipProgram(
                            pack["name"],
                            pack["dip"],
                            pack["wipe"]))
        print packs
        return result

    def _build_slop_programs(self):
        packs = putl.get_slop_packs()
        result = []
        if packs:
            for bid in packs:
                pack = packs[bid]
                result.append(
                    SlopProgram(
                        pack["name"],
                        pack["painting"]))
        print packs
        return result

    def _build_pick_place_programs(self, brush_ids):
        # print("_build_pick_place_programs")
        # print("brush_ids")
        # print(brush_ids)

        gripper_geo = butl.setup_gripper_from_sheet()
        gripper = Brush.brush_at_plug(
            0, gripper_geo.attr("outPaintBrush"))
        if not gripper:
            raise StudioError(
                "No Gripper. Risk of damage. Can't continue.")

        packs = putl.get_pick_place_packs(brush_ids)

        result = []
        for p in packs:
            pack = packs[p]
            # print "Pack"
            # print pack
            if brush_ids == "calibration":
                pick_prg = PickAtHomeProgram(gripper, pack)
                place_prg = PlaceAtHomeProgram(gripper, pack)
            else:
                pick_prg = PickProgram(gripper, pack)
                place_prg = PlaceProgram(gripper, pack)
            result += [pick_prg, place_prg]
        return result

    def _write_approaches(self):
        self.approaches_frame = uutl.create_frame("ax_frame")
        self.tool_approach = uutl._create_joint_target(
            pm.PyNode(TOOL_TARGET), "tool_approach", self.approaches_frame)
        self.home_approach = uutl._create_joint_target(
            pm.PyNode(HOME_TARGET), "home_approach", self.approaches_frame)
        self.dip_approach = uutl._create_joint_target(
            pm.PyNode(DIP_TARGET), "dip_approach", self.approaches_frame)

    def _write_rack_and_holder_geo(self):
        if self.do_rack_and_holder_geo:
            rack_context = pm.PyNode("RACK1_CONTEXT")
            with uutl.final_position(rack_context):
                ref_geo = [pm.PyNode("rackTop")]
                ref_geo += pm.ls("holders|*|holderTrans|lowResGeo")
                props.send(ref_geo)

 
    def write(self):

        self._write_approaches()

        rack_context = pm.PyNode("RACK1_CONTEXT")
        painting_context = pm.PyNode("mainPaintingGroup")

        if self.painting_program:
            self.painting_program.write(self)
            # with final_position(pm.PyNode("canvas")):
            #     props.send([pm.PyNode("canvas")])

        if self.dip_programs:
            self.dips_frame = uutl.create_frame("dips_frame")
            for dip in self.dip_programs:
                dip.write(self)
            if self.do_rack_and_holder_geo:
                with uutl.final_position(rack_context):
                    Paint.write_geos()

        if self.slop_programs:
            self.dips_frame = uutl.create_frame("dips_frame")
            for slop in self.slop_programs:
                slop.write(self)

        if self.exercise_program:
            self.exercise_program.write(self)
            with uutl.final_position(rack_context):
                self._write_rack_and_holder_geo()

        if self.pick_place_programs:
            self.pick_place_frame = uutl.create_frame("pick_place_frame")
            with uutl.final_position(rack_context):
                for prog in self.pick_place_programs:
                    prog.write(self)
                # self._write_rack_and_holder_geo()



        if self.pot_cal_program:
            with uutl.final_position(rack_context):
                self.pot_cal_program.write(
                    self.tool_approach,
                    self.home_approach)

        if self.holder_cal_program:
            with uutl.final_position(rack_context):
                self.holder_cal_program.write(
                    self.tool_approach,
                    self.home_approach)

        if self.pot_holder_cal_program:
            self.pot_holder_cal_program.write()

        if self.perspex_cal_program:
            with uutl.final_position(rack_context):
                self.perspex_cal_program.write(
                    self.tool_approach,
                    self.home_approach)
                # self._write_rack_and_holder_geo()

        if self.manual_tri_program:
            self.manual_tri_program.write(
                self.tool_approach,
                self.home_approach)

        if self.board_cal_program:
            with uutl.final_position(painting_context):
                self.board_cal_program.write(
                    self.tool_approach,
                    self.home_approach)
