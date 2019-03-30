
import logging
logger = logging.getLogger('uprising')


import pymel.core as pm
import uprising_util as uutl

from robolink import (
    Robolink,
    ITEM_TYPE_ROBOT)
from paint import Paint
import props
# from brush import Brush
import painting as ptg
import program as prg

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
        # self.RL = Robolink()
        # self.robot = self.RL.Item('', ITEM_TYPE_ROBOT)
        
        self.approaches_frame = None
        self.dip_approach = None
        self.tool_approach = None
        self.home_approach = None
        self.painting_program = None
        self.rack_cal_program = None
        self.board_cal_program = None
        
        dip_wipe_packs =  kw.get("dip_wipe_packs", [])

        with uutl.final_position(pm.PyNode("RACK1_CONTEXT")):
            self.dip_programs = self._build_dip_programs(dip_wipe_packs)

        ptg_node = kw.get("painting_node")
        if ptg_node:
            logger.debug("Studio ptg_node %s" % ptg_node)
            with uutl.final_position(ptg_node):
                self.painting_program = prg.MainProgram("px", ptg_node)

        if  kw.get("rack_calibration"):
            logger.debug("Studio rack_calibration")
            # node = pm.PyNode("RACK1_CONTEXT")
            self.rack_cal_program = prg.RackCalibration("rx")
                 

        if  kw.get("board_calibration"):
            logger.debug("Studio board_calibration")
            # node = pm.PyNode("mainPaintingGroup")
            self.board_cal_program = prg.BoardCalibration("bx")
                 
        # if ver_node:
        #     logger.debug("Studio ver_node %s" % ver_node)
        #     with uutl.final_position(ver_node):
        #         self.verification = ptg.Verification(ver_node)

    def _build_dip_programs(self, packs):
        # print packs

        result = []
        if packs:
            for pid in packs:
                paint_pack = packs[pid]
                for bid in paint_pack:
                    pack = paint_pack[bid]
                    result.append(
                        prg.DipProgram(
                            pack["name"],
                            pack["dip"],
                            pack["wipe"]))
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
        logger.debug("approaches")
        self.approaches_frame = uutl.create_frame("ax_frame")
        self.tool_approach = uutl._create_joint_target(
            pm.PyNode(TOOL_TARGET), "tool_approach", self.approaches_frame)
        self.home_approach = uutl._create_joint_target(
            pm.PyNode(HOME_TARGET), "home_approach", self.approaches_frame)
        self.dip_approach = uutl._create_joint_target(
            pm.PyNode(DIP_TARGET), "dip_approach", self.approaches_frame)
        logger.debug("DONE approaches")


    def write(self):
        """Clean up and make parent objects etc."""
        # should_clean = True
        # if (self.painting and self.dip) or self.calibration:
        uutl.delete_programs()
        uutl.delete_tools()
        self._write_approaches()
        
        if self.dip_programs:
            logger.debug("write dip_programs")
            for dip in self.dip_programs:
                dip.write(self)
            with uutl.final_position(pm.PyNode("RACK1_CONTEXT")):
                Paint.write_geos()
            logger.debug("DONE write dips")

        if self.painting_program:
            self.painting_program.write(self)
            self._write_canvas()
 

        if self.rack_cal_program:
            rack_node = pm.PyNode("RACK1_CONTEXT")
            with uutl.final_position(rack_node):
                self.rack_cal_program.write(
                    self.tool_approach,
                    self.home_approach)

        if self.board_cal_program:
            top_node = pm.PyNode("mainPaintingGroup")
            with uutl.final_position(top_node):
                self.board_cal_program.write(
                    self.tool_approach,
                    self.home_approach)
 
 