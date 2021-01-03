
from robolink import (
    INSTRUCTION_COMMENT,
    INSTRUCTION_SHOW_MESSAGE,
    INSTRUCTION_INSERT_CODE
)


from target import Target, PovTarget, DepartureTarget, ArrivalTarget
import pymel.core as pm

import uprising_util as uutl
from uprising_util import StrokeError

 
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

    def best_config(self):
        if not self.targets:
            return

        targets = self.targets + self.arrivals + [self.departure]

        common_configs = set(targets[0].valid_configs())
        for target in targets[1:]:
            common_configs = common_configs.intersection(
                target.valid_configs())
            if not common_configs:
                raise StrokeError(
                    "Can't find best config for stroke. No common configs")

        return sorted(list(common_configs))[0]

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

        num_targets = len(positions)
        if not num_targets == len(rotations):
            raise StrokeError(
                "Length mismatch: positions, rotations")

        for i, (p, r) in enumerate(zip(positions, rotations)):
            try:
                tg = Target(i, (p * 10), r, self.brush)
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
                tg = ArrivalTarget(i, (p * 10), r, self.brush)
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
                0, (position * 10), rotation, self.brush)
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
        config = self.best_config()
        if not config:  # no targets
            return

        for target in self.targets:
            target.configure(config)
        self.targets[0].linear = False

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

        colors = uutl.to_rgba_array(
            pm.paintingQuery(
                self.node,
                clusterIndex=self.cluster_id,
                strokeIndex=self.id,
                strokeColors=True
            )
        )

        num_targets = len(positions)
        if not num_targets == len(rotations) and num_targets == len(colors):
            raise StrokeError(
                "Length mismatch: positions, rotations, colors")

        for i, (p, r, c) in enumerate(zip(positions, rotations, colors)):
            try:
                tg = PovTarget(i, (p * 10), r, c, self.brush)
            except StrokeError:
                raise

            self.targets.append(tg)

    def best_config(self):
        if not self.targets:
            return

        common_configs = set(self.targets[0].valid_configs())
        for target in self.targets[1:]:
            common_configs = common_configs.intersection(
                target.valid_configs())
            if not common_configs:
                print "Impossible Stroke:"
                for target in self.targets:
                    print "Cluster:{} - Target:{} - configs:{}".format(
                        self.cluster_id, target.id, target.valid_configs())
                raise StrokeError(
                    "Can't find best config for stroke. No common configs")

        return sorted(list(common_configs))[0]

    def send(self, prefix, program, frame, motion):
        stroke_name = self.name(prefix)
        program.RunInstruction("Stroke %s" % stroke_name, INSTRUCTION_COMMENT)
        lin = motion["linear_speed"] * self.linear_speed
        ang = motion["angular_speed"] * self.angular_speed
        program.setSpeed(lin, ang)

        last_color = None
        for i, t in enumerate(self.targets):
            t.send(stroke_name, program, frame, last_color)
            last_color = t.color

        program.RunInstruction("End stroke {}. Set to black".format(stroke_name), INSTRUCTION_COMMENT)
        program.RunInstruction("$ANOUT[1]=0.0", INSTRUCTION_INSERT_CODE)
        program.RunInstruction("$ANOUT[2]=0.0", INSTRUCTION_INSERT_CODE)
        program.RunInstruction("$ANOUT[3]=0.0", INSTRUCTION_INSERT_CODE)
        program.RunInstruction("$ANOUT[4]=0.0", INSTRUCTION_INSERT_CODE)





    def configure(self):
        config = self.best_config()
        print "Best Config for Cluster ID:{} Stroke ID:{} = {}".format(
            self.cluster_id, self.id, config)
        if not config:  # no targets
            return

        for target in self.targets:
            target.configure(config)
        self.targets[0].linear = False
