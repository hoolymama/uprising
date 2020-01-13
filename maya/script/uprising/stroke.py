from robolink import INSTRUCTION_COMMENT
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
        # self.last_valid_joints = last_valid_joints

        self.set_stroke_params()
        self.build_targets()
        self.build_arrivals()
        self.build_departure()

        # self.build_stop_targets()

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

        self.targets = []

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
            raise StrokeError("Length mismatch: positions, rotations, tangents")

        for i, (p, r, t) in enumerate(zip(positions, rotations, tangents)):
            tg = Target(i, (p * 10), r, t, self.brush)
            self.targets.append(tg)

    def build_arrivals(self):

        self.arrivals = []

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
            tg = ArrivalTarget(i, (p * 10), r, None, self.brush)
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

        self.departure = DepartureTarget(0, (position * 10), rotation, None, self.brush)

    def name(self, prefix):
        return "%s_p%d_s%d" % (prefix, self.parent_id, self.id)

    def write(self, prefix, program, frame, motion):
        stroke_name = self.name(prefix)
        program.RunInstruction("Stroke %s" % stroke_name, INSTRUCTION_COMMENT)
        lin = motion["linear_speed"] * self.linear_speed
        ang = motion["angular_speed"] * self.angular_speed
        program.setSpeed(lin, ang)
        for t in self.arrivals:
            t.write(stroke_name, program, frame)

        for t in self.targets:
            t.write(stroke_name, program, frame)

        self.departure.write(stroke_name, program, frame)

    def configure(self):

        for target in self.arrivals:
            target.configure()

        for target in self.targets:
            target.configure()

        self.departure.configure()

