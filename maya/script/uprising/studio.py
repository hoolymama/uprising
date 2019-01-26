
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


# {
#     'p00':
#     {
#         'b11':
#         {
#             'dip': nt.Painting(u'holeRot_00|holeTrans|dip_loc|b11|bShape11'),
#             'wipe': nt.Painting(u'holeRot_00|holeTrans|wipe_loc|b11|bShape11')
#         },
#         'b14':
#         {
#             'dip': nt.Painting(u'holeRot_00|holeTrans|dip_loc|b14|bShape14'),
#             'wipe': nt.Painting(u'holeRot_00|holeTrans|wipe_loc|b14|bShape14')
#         },
#          'b16': {
#             'dip': nt.Painting(u'holeRot_00|holeTrans|dip_loc|b16|bShape16'),
#             'wipe': nt.Painting(u'holeRot_00|holeTrans|wipe_loc|b16|bShape16')
#         }
#     },
#     'p01': {
#         'b11': {
#             'dip': nt.Painting(u'holeRot_00|holeTrans|dip_loc|b11|bShape11'),
#             'wipe': nt.Painting(u'holeRot_00|holeTrans|wipe_loc|b11|bShape11')
#         }
#     }
# }


class Studio(object):
    """Glue together the entire studio."""

    def __init__(self, **kw):
        self.RL = Robolink()

        # self.rack_node = kw.get("rack_node")

        self.robot = self.RL.Item('', ITEM_TYPE_ROBOT)
        
        # self.painting_frame = None

        self.approaches_frame = None
        self.dip_approach = None
        self.tool_approach = None
        self.home_approach = None

        # self.dip = None
        # self.painting = None

        self.calibration = None
        self.verification = None

        self.dip_programs = self._build_dip_programs(
            kw.get("dip_wipe_packs", []))

        ptg_node = kw.get("painting_node")
        if ptg_node:
            logger.debug("Studio ptg_node %s" % ptg_node)
            with uutl.final_position(ptg_node):
                self.painting_program = prg.MainProgram("px", ptg_node, self.robot)
    

        # cal_node = kw.get("calibration_node")
        # ver_node = kw.get("verification_node")

        # if cal_node:
        #     logger.debug("Studio cal_node %s" % cal_node)
        #     with uutl.final_position(cal_node):
        #         self.calibration = ptg.Calibration(cal_node)

        # if ver_node:
        #     logger.debug("Studio ver_node %s" % ver_node)
        #     with uutl.final_position(ver_node):
        #         self.verification = ptg.Verification(ver_node)

    def _build_dip_programs(self, packs):
        print packs

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
                            pack["wipe"],
                            self.robot))
        return result


 

    def _write_canvas(self):

        canvas_frame = uutl.create_frame("cx_frame")
        painting_node = pm.PyNode("mainPaintingShape")
        disp_tx = None
        with uutl.zero_position(painting_node):
            disp_meshes = pm.listConnections(
                painting_node.attr("displacementMesh"), s=True, d=False)
            if disp_meshes:
                # disp_tf = disp_meshes[0].getParent()
                dups = pm.duplicate(disp_meshes[0])
                jpos = pm.PyNode("mainPaintingGroup|jpos")
                pm.parent(dups[0], jpos)
                with uutl.final_position(painting_node):
                    props.send(dups[0], canvas_frame)
                    pm.delete(dups)

        # with uutl.final_position(painting_node):
        #     canvas = pm.PyNode("mainPaintingGroup|jpos|canvas")
        #     if canvas:
        #         props.send(canvas, canvas_frame)



    # def write_dips(self):



    def write(self):
        """Clean up and make parent objects etc."""
        # should_clean = True
        # if (self.painting and self.dip) or self.calibration:
        uutl.delete_programs()
        uutl.delete_tools()

        logger.debug("_write_canvas")

        self._write_canvas()
        logger.debug("DONE _write_canvas")

        logger.debug("approaches")
        self.approaches_frame = uutl.create_frame("ax_frame")

        self.tool_approach = uutl._create_joint_target(
            pm.PyNode(TOOL_TARGET), "tool_approach", self.approaches_frame)
        self.home_approach = uutl._create_joint_target(
            pm.PyNode(HOME_TARGET), "home_approach", self.approaches_frame)
        self.dip_approach = uutl._create_joint_target(
            pm.PyNode(DIP_TARGET), "dip_approach", self.approaches_frame)
        logger.debug("DONE approaches")



        logger.debug("making frames")
        # self.dip_frame = uutl.create_frame("dx_frame")
        self.trays_frame = uutl.create_frame("tx_frame")
        # self.painting_frame = uutl.create_frame("px_frame")
        logger.debug("DONE making frames")

        # if self.painting:
        #     self.painting_program = uutl.create_program("px")
        #     self.painting_program.ShowInstructions(False)

        if self.dip_programs:
            logger.debug("write dip_programs")
            for dip in self.dip_programs:
                dip.write(self)
 
            Paint.write_geos()
            logger.debug("DONE write dips")

        if self.painting_program:
            self.painting_program.write(self)
            # logger.debug("write painting")
            # self.painting_program = uutl.create_program("px")
            # self.painting_program.ShowInstructions(False)
            # self.painting_program.addMoveJ(self.home_approach)
            # logger.debug("DONE write painting")

        # if self.calibration:
        #     logger.debug("write calibration")
        #     with uutl.final_position(self.calibration.node):
        #         self.calibration.write(self)
        #     logger.debug("DONE write calibration")

        # if self.verification:
        #     logger.debug("write verification")
        #     with uutl.final_position(self.verification.node):
        #         self.verification.write(self)
        #     logger.debug("DONE write verification")

    # def _create_joint_target(self, obj, name):

    #     mat = obj.attr("worldMatrix[0]").get()
    #     mat = uutl.maya_to_robodk_mat(mat)
    #     joint_poses = uutl.config_000_poses(mat)
    #     if not joint_poses:
    #         raise StudioError(
    #             "No configs for approach mat. Try repositioning.")
    #     joints = joint_poses[0]

    #     old_approach = self.RL.Item(name)
    #     if old_approach.Valid():
    #         old_approach.Delete()
    #     target = self.RL.AddTarget(name, self.approaches_frame, self.robot)
    #     target.setAsJointTarget()
    #     target.setJoints(joints)
    #     return target

    # def write_calibration(self):
    #     cal_program = uutl.create_program("xx")
    #     calibration_frame = uutl.create_frame("xx_frame")
    #     # self.calibration.write(self)
    #     # use brush 0 because there should always be one and
    #     # it should be smallest.
    #     # we write it now, because the painting hasn't
    #     # run yet. (This might not be necessary)
    #     brush = Brush.brush_at_index(self.calibration.node, 0)
    #     brush.write(self)

    #     probes = {}
    #     for p in ["TL", "BL", "BR", "C", "TR"]:
    #         probe_name = "probe%s" % p
    #         probe_node = pm.PyNode(probe_name)
    #         probe_target = self._create_calibration_target(
    #             probe_node, probe_name, calibration_frame)

    #         approach_name = "probe%sApproach" % p
    #         approach_node = pm.PyNode(approach_name)
    #         approach_target = self._create_calibration_target(
    #             approach_node, approach_name, calibration_frame)

    #         probes[p] = {
    #             "probe": {
    #                 "name": probe_name,
    #                 "node": probe_node,
    #                 "target": probe_target
    #             },
    #             "approach": {
    #                 "name": approach_name,
    #                 "node": approach_node,
    #                 "target": approach_target
    #             }
    #         }
    #     probes["TL"]["nice"] = "Top Left"
    #     probes["TR"]["nice"] = "Top Right"
    #     probes["BL"]["nice"] = "Bottom Left"
    #     probes["BR"]["nice"] = "Bottom Right"
    #     probes["C"]["nice"] = "Center"

    #     motion = self.painting.motion
    #     cal_program.setSpeed(
    #         motion["linear_speed"],
    #         motion["angular_speed"])
    #     cal_program.setRounding(motion["rounding"])

    #     # set tool
    #     tool = self.RL.Item(brush.name)
    #     if not tool.Valid():
    #         raise StudioError(
    #             "SERIOUS RISK OF DAMAGE! Can't find valid tool!")
    #     cal_program.setPoseTool(tool)

    #     cal_program.RunInstruction("Starting calibration",
    #                                INSTRUCTION_COMMENT)
    #     cal_program.addMoveJ(self.tool_approach)
    #     cal_program.RunInstruction(
    #         "Attach calibration Tool: %s PID:(%d)" %
    #         (brush.name, brush.physical_id),
    #         INSTRUCTION_SHOW_MESSAGE)
    #     cal_program.Pause()

    #     self.do_one_probe(cal_program, probes["TL"])
    #     self.do_one_probe(cal_program, probes["TR"])
    #     self.do_one_probe(cal_program, probes["BR"])
    #     self.do_one_probe(cal_program, probes["BL"])
    #     self.do_one_probe(cal_program, probes["C"])

        # cal_program.addMoveJ(self.home_approach)

    # def do_one_probe(self, program, probe):
    #     program.RunInstruction("Moving to Top Left",
    #                            INSTRUCTION_COMMENT)
    #     program.addMoveJ(probe["approach"]["target"])
    #     program.addMoveL(probe["probe"]["target"])
    #     program.RunInstruction(
    #         "Brush TCP should now be touching the board",
    #         INSTRUCTION_SHOW_MESSAGE)
    #     program.Pause()
    #     program.addMoveL(probe["approach"]["target"])
