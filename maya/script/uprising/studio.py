

import pymel.core as pm
import uprising_util as uutl
import palette_utils as putl  

from paint import Paint
import props

from robolink import (
    Robolink,
    ITEM_TYPE_ROBOT
)

# import program as prg

from program import (
    MainProgram,
    DipProgram,
    PotCalibration,
    HolderCalibration,
    BoardCalibration,
    PickProgram,
    PlaceProgram)

 
import brush_utils as butl
from brush import Brush
import logging
logger = logging.getLogger('uprising')


# from painting import Calibration

# from contextlib import contextmanager
# import stroke_factory_utils as sfu


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

        self.approaches_frame = None
        self.dip_approach = None
        self.tool_approach = None
        self.home_approach = None
        self.painting_program = None
        self.rack_cal_program = None
        self.pot_cal_program = None
        self.holder_cal_program = None
        self.board_cal_program = None
        self.dip_programs = []
        self.pick_place_programs = []

        print "STUDIO KW"
        print kw


        do_painting = kw.get("do_painting")
        do_dips = kw.get("do_dips")
        use_gripper = kw.get("use_gripper")

        do_board_calibration = kw.get("do_board_calibration")
        do_pot_calibration = kw.get("do_pot_calibration")
        do_holder_calibration = kw.get("do_holder_calibration")

        if do_painting:
            logger.debug("Studio: main_painting")
            with uutl.final_position(pm.PyNode("mainPaintingShape")):
                self.painting_program = MainProgram("px", use_gripper)

        if do_dips:
            logger.debug("Studio: dips")
            with uutl.final_position(pm.PyNode("RACK1_CONTEXT")):
                self.dip_programs = self._build_dip_programs()

        if use_gripper:
            logger.debug("Studio: pick_place_programs")
            with uutl.final_position(pm.PyNode("RACK1_CONTEXT")):
                self.pick_place_programs = self._build_pick_place_programs("used")

        if do_pot_calibration:
            logger.debug("Studio:  pot_calibration")
            self.pot_cal_program = PotCalibration("pot",  use_gripper)
            if use_gripper:
                with uutl.final_position(pm.PyNode("RACK1_CONTEXT")):
                    self.pick_place_programs = self._build_pick_place_programs([0])

        if do_holder_calibration:
            logger.debug("Studio:  holder_calibration")
            self.holder_cal_program = HolderCalibration("holder", use_gripper)
            if use_gripper:
                with uutl.final_position(pm.PyNode("RACK1_CONTEXT")):
                    self.pick_place_programs = self._build_pick_place_programs([0])

        if do_board_calibration:
            logger.debug("Studio: board_calibration")
            self.board_cal_program = BoardCalibration("bx", use_gripper)
            if use_gripper:
                with uutl.final_position(pm.PyNode("RACK1_CONTEXT")):
                    self.pick_place_programs = self._build_pick_place_programs([0])



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
        return result

    def _build_pick_place_programs(self, brush_ids):
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

    def write(self):

        self._write_approaches()

        rack_context = pm.PyNode("RACK1_CONTEXT")
        painting_context = pm.PyNode("mainPaintingGroup")

        if self.painting_program:
            self.painting_program.write(self)

        if self.dip_programs:
            self.dips_frame = uutl.create_frame("dips_frame")
            for dip in self.dip_programs:
                dip.write(self)
            with uutl.final_position(rack_context):
                Paint.write_geos()


        if self.pick_place_programs:
            self.pick_place_frame = uutl.create_frame("pick_place_frame")
            with uutl.final_position(rack_context):
                for prog in self.pick_place_programs:
                    prog.write(self)

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

        if self.board_cal_program:
            with uutl.final_position(painting_context):
                self.board_cal_program.write(
                    self.tool_approach,
                    self.home_approach)
