
from robolink import (
    INSTRUCTION_COMMENT,
    INSTRUCTION_SHOW_MESSAGE
)


from target import Target, DepartureTarget, ArrivalTarget
import pymel.core as pm

import uprising_util as uutl
from uprising_util import StrokeError


def config_key(config):
    if config:
        return "%d%d%d%d" % tuple(config.list2()[0][0:4])


class Stroke(object):
    def __init__(self, cluster_id, _id, brush, node):
        self.cluster_id = cluster_id
        self.id = _id
        self.node = node

        self.brush = brush

        self.targets = []
        self.arrivals = []
        self.departure = None

        self.set_stroke_params()
        self.build_targets()
        self.build_arrivals()
        self.build_departure()

        self.configure()

    def set_stroke_params(self):

        self.backstroke = pm.paintingQuery(
            self.node,
            clusterIndex=self.cluster_id,
            strokeIndex=self.id,
            strokeBackstroke=True,
        )

        self.arc_length = pm.paintingQuery(
            self.node,
            clusterIndex=self.cluster_id,
            strokeIndex=self.id,
            strokeArcLength=True,
        )

        self.parent_id = pm.paintingQuery(
            self.node,
            clusterIndex=self.cluster_id,
            strokeIndex=self.id,
            strokeParentIndex=True,
        )

        self.linear_speed = pm.paintingQuery(
            self.node,
            clusterIndex=self.cluster_id,
            strokeIndex=self.id,
            strokeSpeedLinear=True,
        )

        self.angular_speed = pm.paintingQuery(
            self.node,
            clusterIndex=self.cluster_id,
            strokeIndex=self.id,
            strokeSpeedAngular=True,
        )

    def build_targets(self):

        positions = uutl.to_point_array(
            pm.paintingQuery(
                self.node,
                clusterIndex=self.cluster_id,
                strokeIndex=self.id,
                strokePositions=True,
            )
        )

        rotations = uutl.to_vector_array(
            pm.paintingQuery(
                self.node,
                clusterIndex=self.cluster_id,
                strokeIndex=self.id,
                strokeRotations=True,
                rotateOrder="zyx",
                rotateUnit="rad",
            )
        )

        tangents = uutl.to_vector_array(
            pm.paintingQuery(
                self.node,
                clusterIndex=self.cluster_id,
                strokeIndex=self.id,
                strokeTangents=True,
            )
        )

        num_targets = len(positions)
        if not (num_targets == len(rotations) and num_targets == len(tangents)):
            raise StrokeError(
                "Length mismatch: positions, rotations, tangents")

        for i, (p, r, t) in enumerate(zip(positions, rotations, tangents)):
            try:
                tg = Target(i, (p * 10), r, t, self.brush)
            except StrokeError:
                print "Target Position:", p
                print "StrokeId:", self.id
                raise

            self.targets.append(tg)

    def build_arrivals(self):

        positions = uutl.to_point_array(
            pm.paintingQuery(
                self.node,
                clusterIndex=self.cluster_id,
                strokeIndex=self.id,
                strokeArrivalPositions=True,
            )
        )

        rotations = uutl.to_vector_array(
            pm.paintingQuery(
                self.node,
                clusterIndex=self.cluster_id,
                strokeIndex=self.id,
                strokeArrivalRotations=True,
                rotateOrder="zyx",
                rotateUnit="rad",
            )
        )

        num = len(positions)
        if not (num == len(rotations)):
            raise StrokeError("Arrivals length mismatch: positions, rotations")

        for i, (p, r) in enumerate(zip(positions, rotations)):

            try:
                tg = ArrivalTarget(i, (p * 10), r, None, self.brush)
            except StrokeError:
                print "ArrivalTarget Position:", p
                print "StrokeId:", self.id
                raise
            self.arrivals.append(tg)

    def build_departure(self):

        position = uutl.to_point_array(
            pm.paintingQuery(
                self.node,
                clusterIndex=self.cluster_id,
                strokeIndex=self.id,
                strokeDeparturePosition=True,
            )
        )[0]

        rotation = uutl.to_vector_array(
            pm.paintingQuery(
                self.node,
                clusterIndex=self.cluster_id,
                strokeIndex=self.id,
                strokeDepartureRotation=True,
                rotateOrder="zyx",
                rotateUnit="rad",
            )
        )[0]

        try:
            self.departure = DepartureTarget(
                0, (position * 10), rotation, None, self.brush)
        except StrokeError:
            print "DepartureTarget Position:", position
            print "StrokeId:", self.id
            raise

    def name(self, prefix):
        return "%s_p%d_s%d" % (prefix, self.parent_id, self.id)

    def send(self, prefix, program, frame, motion):
        stroke_name = self.name(prefix)
        program.RunInstruction("Stroke %s" % stroke_name, INSTRUCTION_COMMENT)
        lin = motion["linear_speed"] * self.linear_speed
        ang = motion["angular_speed"] * self.angular_speed
        program.setSpeed(lin, ang)
        for t in self.arrivals:
            t.send(stroke_name, program, frame)

        for t in self.targets:
            t.send(stroke_name, program, frame)

        self.departure.send(stroke_name, program, frame)

    def configure(self):

        for target in self.arrivals:
            target.configure()

        for target in self.targets:
            target.configure()

        self.departure.configure()


class PovStroke(Stroke):
    def __init__(self, cluster_id, _id, brush, node):
        self.cluster_id = cluster_id
        self.id = _id
        self.node = node
        self.brush = brush
        self.targets = []

        self.set_stroke_params()
        self.build_targets()
        self.configure()

    def send(self, prefix, program, frame, motion):
        stroke_name = self.name(prefix)
        program.RunInstruction("Stroke %s" % stroke_name, INSTRUCTION_COMMENT)
        lin = motion["linear_speed"] * self.linear_speed
        ang = motion["angular_speed"] * self.angular_speed
        program.setSpeed(lin, ang)

        for i, t in enumerate(self.targets):
            t.send(stroke_name, program, frame)

            if i == 0:
                program.RunInstruction(
                    "Turn on the light and continue",
                    INSTRUCTION_SHOW_MESSAGE,
                )
                program.Pause()

            if (i+1) == len(self.targets):
                program.RunInstruction(
                    "Turn off the light and continue",
                    INSTRUCTION_SHOW_MESSAGE,
                )
                program.Pause()

    def configure(self):
        if self.targets:
            for target in self.targets:
                target.configure()
            self.targets[0].linear = False
