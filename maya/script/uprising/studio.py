

import pymel.core as pm
import uprising_util as uutl

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


import setup_dip
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


def get_dip_wipe_packs():

    result = {}
    # racks = ["rack1"]

    dip_combinations = setup_dip.dip_combination_ids()

    for combo in dip_combinations:

        dip_ptg_path = "rack*|holes|holeRot_{:02d}|holeTrans|dip_loc|b{:02d}|*".format(
            combo["paint"], combo["brush"])
        wipe_ptg_path = "rack*|holes|holeRot_{:02d}|holeTrans|wipe_loc|b{:02d}|*".format(
            combo["paint"], combo["brush"])

        paint_key = "p{:02d}".format(combo["paint"])
        brush_key = "b{:02d}".format(combo["brush"])

        # print "{} -- {}".format(dip_ptg_path, wipe_ptg_path)
        try:
            dip_ptg = pm.ls(dip_ptg_path, type="painting")[0]
            wipe_ptg = pm.ls(wipe_ptg_path, type="painting")[0]
        except IndexError:
            raise IndexError(
                "Either dip or wipe node is missing: for {} {}".format(
                    paint_key, brush_key))

        if paint_key not in result:
            result[paint_key] = {}
        result[paint_key][brush_key] = {
            "dip": dip_ptg,
            "wipe": wipe_ptg,
            "name": "{}_{}".format(paint_key, brush_key)
        }

    return result


def get_pick_place_packs(all_holders=False):
    result = {}
    painting = pm.PyNode("mainPaintingShape")
    if all_holders:
        bids = [int(n[-2:])
                for n in pm.ls("RACK1_CONTEXT|j1|rack1|holders|holderRot*")]
    else:
        dc = pm.paintingQuery(painting, dc=True)
        bids = set(dc[::2])

    holders_node = pm.PyNode("RACK1_CONTEXT|j1|rack1|holders")
    path_attributes = {
        "lin_speed": holders_node.attr("linearSpeed").get() * 10,
        "ang_speed": holders_node.attr("angularSpeed").get(),
        "rounding": holders_node.attr("approximationDistance").get() * 10,
    }
    for bid in bids:
        key = "b{:02d}".format(bid)
        trans = "holderRot{:02d}|holderTrans".format(bid)
        result[key] = {
            "trans_node": pm.PyNode(trans),
            "brush_id": bid,
            "probe": pm.PyNode("{}|probe_loc".format(trans)),
            "pin": pm.PyNode("{}|pin_loc".format(trans)),
            "pin_ap": pm.PyNode("{}|pin_approach_loc".format(trans)),
            "clear": pm.PyNode("{}|clear_loc".format(trans)),
            "clear_ap": pm.PyNode("{}|clear_approach_loc".format(trans))
        }
        result[key].update(path_attributes)
    return result


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

        self.do_approaches = kw.get("do_approaches", True)
        do_painting = kw.get("do_painting")
        do_dips = kw.get("do_dips")
        do_auto_change = kw.get("do_auto_change")

        do_board_calibration = kw.get("do_board_calibration")
        do_pot_calibration = kw.get("do_pot_calibration")
        do_holder_calibration = kw.get("do_holder_calibration")

        if do_dips:
            logger.debug("Studio: dips")
            with uutl.final_position(pm.PyNode("RACK1_CONTEXT")):
                self.dip_programs = self._build_dip_programs()

        if do_auto_change:
            logger.debug("Studio: pick_place_programs")
            with uutl.final_position(pm.PyNode("RACK1_CONTEXT")):
                self.pick_place_programs = self._build_pick_place_programs()

        if do_painting:
            logger.debug("Studio: main_painting")
            with uutl.final_position(pm.PyNode("mainPaintingShape")):
                self.painting_program = MainProgram("px")

        if do_pot_calibration:
            logger.debug("Studio:  pot_calibration")
            self.pot_cal_program = PotCalibration("pot")

        if do_holder_calibration:
            packs = get_pick_place_packs(True)
            logger.debug("Studio:  holder_calibration")
            self.holder_cal_program = HolderCalibration("holder", packs)

        if do_board_calibration:
            logger.debug("Studio: board_calibration")
            self.board_cal_program = BoardCalibration("bx")

    def _build_dip_programs(self):
        packs = get_dip_wipe_packs()
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

    def _build_pick_place_programs(self):
        gripper_geo = butl.setup_gripper_from_sheet()
        gripper = Brush.brush_at_plug(
            0, gripper_geo.attr("outPaintBrush"))
        if not gripper:
            raise StudioError(
                "No Gripper. Risk of damage. Can't continue.")

        packs = get_pick_place_packs()
        # print packs
        result = []
        for p in packs:
            pack = packs[p]
            pick_prg = PickProgram(gripper, pack)
            place_prg = PlaceProgram(gripper, pack)
            result += [pick_prg, place_prg]
        return result

    def _write_canvas(self):
        canvas_frame = uutl.create_frame("cx_frame")
        painting_node = pm.PyNode("mainPaintingShape")
        with uutl.zero_position(painting_node):
            disp_meshes = pm.listConnections(
                painting_node.attr("displacementMesh"), s=True, d=False)
            if disp_meshes:
                dups = pm.duplicate(disp_meshes[0])
                jpos = pm.PyNode("mainPaintingGroup|jpos")
                pm.parent(dups[0], jpos)
                with uutl.final_position(painting_node):
                    props.send(dups[0], canvas_frame)
                    pm.delete(dups)

    def _write_approaches(self):
        self.approaches_frame = uutl.create_frame("ax_frame")
        self.tool_approach = uutl._create_joint_target(
            pm.PyNode(TOOL_TARGET), "tool_approach", self.approaches_frame)
        self.home_approach = uutl._create_joint_target(
            pm.PyNode(HOME_TARGET), "home_approach", self.approaches_frame)
        self.dip_approach = uutl._create_joint_target(
            pm.PyNode(DIP_TARGET), "dip_approach", self.approaches_frame)

    def write(self):

        if self.do_approaches:
            self._write_approaches()

        rack_context = pm.PyNode("RACK1_CONTEXT")
        painting_context = pm.PyNode("mainPaintingGroup")

        if self.dip_programs:
            self.dips_frame = uutl.create_frame("dips_frame")
            for dip in self.dip_programs:
                dip.write(self)
            with uutl.final_position(rack_context):
                Paint.write_geos()

        if self.painting_program:
            self.painting_program.write(self)
            self._write_canvas()

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
