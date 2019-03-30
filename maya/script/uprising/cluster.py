
from robolink import (
    INSTRUCTION_SHOW_MESSAGE,
    INSTRUCTION_COMMENT,
    INSTRUCTION_CALL_PROGRAM)


from uprising_util import ClusterError
from stroke import Stroke
import uprising_util as uutl
import pymel.core as pm


import logging
logger = logging.getLogger('uprising')


class Cluster(object):

    def __init__(self, _id, node, robot, brush, paint):
        self.id = _id
        self.brush = brush
        self.paint = paint
        self.node = node
        # self.motion = motion
        self.travel = pm.paintingQuery(
            self.node, clusterIndex=_id, clusterTravel=True)
        self.robot = robot
        self.reason = pm.paintingQuery(
            self.node, clusterIndex=_id, clusterReason=True)
        self.build_strokes()

    def build_strokes(self):
        logger.debug("Cluster %d build strokes" % self.id)
        self.strokes = []
        num_strokes = pm.paintingQuery(
            self.node, clusterIndex=self.id, strokeCount=True)
        for i in range(num_strokes):
            logger.debug("Stroke %d" % i)
            stroke = Stroke(
                self.id,
                i,
                self.node,
                self.robot,
                self.brush
            )
            self.strokes.append(stroke)

    def change_tool_message(self):
        return "Change to: Brs({:d}) - Pnt({:d}) {}".format(
          self.brush.physical_id, self.paint.id, self.brush.name)

    def name(self, program):
        return "{}_c{}".format(program.Name(), self.id)

    def write(self, program, frame, motion, RL, robot):
        tool = RL.Item(self.brush.name)
        if not tool.Valid():
            raise ClusterError(
                "SERIOUS RISK OF DAMAGE! Can't find valid tool!")

        program.setPoseTool(tool)

        cluster_name = self.name(program)

        program.RunInstruction(
            "Cluster %s" %
            cluster_name,
            INSTRUCTION_COMMENT)

        program.setSpeed(
            motion["linear_speed"],
            motion["angular_speed"])
        program.setRounding(motion["rounding"])

        for i, stroke in enumerate(self.strokes):
            stroke.write(cluster_name, program, frame, RL, robot)
 