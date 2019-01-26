
import math
import pymel.core as pm
# import const as k

from robolink import (
    INSTRUCTION_COMMENT,
    INSTRUCTION_SHOW_MESSAGE
)
# import robodk as rdk

from paint import Paint
from brush import Brush
# from stroke import Stroke
from cluster import (Cluster)

from uprising_util import PaintingError
import uprising_util as uutl
import const as k


import logging
logger = logging.getLogger('uprising')


def paint_and_brush_name(paint, brush):
    return "%s_%d_%s_%d" % (paint.name, paint.id, brush.name, brush.id)

# should be a singleton


class Painting(object):

    def __init__(self, node, robot):
        logger.debug("Initialize Painting")
        self.node = node
        self.brushes = Brush.brushes(node)
        self.paints = Paint.paints(node)
        self.clusters = []
        self.motion = {
            "linear_speed": self.node.attr("linearSpeed").get() * 10,
            "angular_speed": self.node.attr("angularSpeed").get(),
            "rounding": self.node.attr("approximationDistance").get() * 10
        }
        self._create_clusters(robot)

    def _create_clusters(self, robot):

        num_clusters = pm.paintingQuery(self.node, clusterCount=True)

        for i in range(num_clusters):
            brush_id = pm.paintingQuery(
                self.node, clusterIndex=i, clusterBrushId=True)
            paint_id = pm.paintingQuery(
                self.node, clusterIndex=i, clusterPaintId=True)

            brush = self.brushes.get(brush_id)
            paint = self.paints.get(paint_id)
  
            cluster = Cluster(i, self.node, robot, brush, paint)
            self.clusters.append(cluster)


    def write_brushes(self):
        for brush in self.brushes:
            self.brushes[brush].write()

    # def write(self, studio):

    #     logger.debug("Write program")
    #     for brush in self.brushes:
    #         self.brushes[brush].write(studio)

    #     for cluster in self.clusters:
    #         cluster.write(studio, self.motion)

 

class Calibration(Painting):

    PROGRAM_NAME = "xx"
    FRAME_NAME = "xx_frame"
    PAUSE_MESSAGE = "Please enter distance from tip to board:"
    PROBE_SUFFIX = "base"

    def __init__(self, node):
        super(Calibration, self).__init__(node)

        self.brush = Brush.find_by_regex(node, r'^.*_probe$')
        if not self.brush:
            raise PaintingError("No Probe Brush")
        self.max_p2p_span = node.attr("maxPointToPointDistance").get()

        assembly = uutl.assembly(self.node)
        probe_group = pn = pm.PyNode("%s|jpos|probes" % assembly)
        self.probes = probe_group.getChildren()
        if not self.probes:
            pm.warning("No probes")
            raise PaintingError("No probes for calibration")

    def _create_clusters(self, robot):
        pass

    def write(self, studio):
        self.brush.write()
        self.tool = studio.RL.Item(self.brush.name)
        if not self.tool.Valid():
            raise PaintingError(
                "SERIOUS RISK OF DAMAGE! Can't find valid tool!")

        self.program = uutl.create_program(self.PROGRAM_NAME)
        self.frame = uutl.create_frame(self.FRAME_NAME)

        self._write_setup(studio)

        last = None
        for probe in self.probes:
            self._write_probe_stops(last, probe, studio)
            self._write_one_probe(probe, studio)
            last = probe
        self.program.addMoveJ(studio.home_approach)

    def _write_setup(self, studio):
        self.program.setSpeed(
            self.motion["linear_speed"],
            self.motion["angular_speed"])
        self.program.setRounding(self.motion["rounding"])
        self.program.setPoseTool(self.tool)
        self.program.RunInstruction(
            ("Starting %s" %
             self.__class__.__name__.lower()),
            INSTRUCTION_COMMENT)
        self.program.addMoveJ(studio.tool_approach)
        self.program.RunInstruction(
            "Attach calibration Tool: %s PID:(%d)" %
            (self.brush.name, self.brush.physical_id),
            INSTRUCTION_SHOW_MESSAGE)
        self.program.Pause()

    def _write_probe_stops(self, a, b, studio):
        if a and b:
            approach_a = pm.PyNode("%s|approach" % a)
            approach_b = pm.PyNode("%s|approach" % b)

            tmat = pm.dt.TransformationMatrix(
                approach_a.attr("worldMatrix[0]").get())
            tx_a = approach_a.attr("worldMatrix[0]").get().translate
            tx_b = approach_b.attr("worldMatrix[0]").get().translate

            dist = (tx_b - tx_a).length()

            if dist > self.max_p2p_span:
                num_spans = math.ceil(dist / self.max_p2p_span)
                gap = dist / num_spans
                vec = (tx_b - tx_a).normal() * gap
                for i in range(int(num_spans) - 1):
                    tx_a += vec
                    tmat.setTranslation(tx_a, "world")
                    name = "%s_stop_%d" % (b, i)
                    target = self._create_a_target(
                        tmat.asMatrix(), name, studio)
                    self.program.addMoveJ(target)

    def _create_a_target(self, mat, name, studio):
        tool_pose = uutl.maya_to_robodk_mat(mat)
        flange_pose = tool_pose * self.brush.matrix.invH()
        target = studio.RL.AddTarget(name, self.frame, studio.robot)
        joints = studio.robot.SolveIK(flange_pose, k.FACING_BOARD_JOINTS)
        target.setPose(tool_pose)
        target.setJoints(joints)
        return target

    def _write_one_probe(self, probe_node, studio):
        base_name = "%s_%s" % (probe_node, self.PROBE_SUFFIX)
        base_node = pm.PyNode("%s|%s" % (probe_node, self.PROBE_SUFFIX))
        base_mat = base_node.attr("worldMatrix[0]").get()

        base_target = self._create_a_target(
            base_mat, base_name, studio)

        approach_name = "%s_approach" % probe_node
        approach_node = pm.PyNode("%s|approach" % probe_node)
        approach_mat = approach_node.attr("worldMatrix[0]").get()

        approach_target = self._create_a_target(
            approach_mat, approach_name, studio)

        self.program.RunInstruction(("Moving to %s" % probe_node),
                                    INSTRUCTION_COMMENT)
        self.program.addMoveJ(approach_target)
        self.program.addMoveL(base_target)
        self.program.RunInstruction(
            ("%s %s" % (self.PAUSE_MESSAGE, probe_node)),
            INSTRUCTION_SHOW_MESSAGE)
        self.program.Pause()
        self.program.addMoveL(approach_target)


class Verification(Calibration):
    PROGRAM_NAME = "vx"
    FRAME_NAME = "vx_frame"
    PAUSE_MESSAGE = "Please check the tip is touching the board:"
    PROBE_SUFFIX = "actual"
