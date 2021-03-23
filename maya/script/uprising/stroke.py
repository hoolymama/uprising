
from robolink import (
    INSTRUCTION_COMMENT,
    INSTRUCTION_SHOW_MESSAGE,
    INSTRUCTION_INSERT_CODE
)

from target import Target, DepartureTarget, ArrivalTarget
import pymel.core as pm
from uprising import robo


from uprising import utils


class Stroke(object):

    def __init__(self, stroke_id, brush, node):
        self.id = stroke_id
        self.brush = brush
        self.node = node
        self.targets = []
        self.backstroke = self.query_backstroke()
        self.arc_length = self.query_arc_length()
        self.parent_id = self.query_parent_id()
        self.linear_speed = self.query_linear_speed()
        self.angular_speed = self.query_angular_speed()

        self.build_targets()
        self.configure_targets()

    def query_backstroke(self):
        raise NotImplementedError

    def query_arc_length(self):
        raise NotImplementedError

    def query_parent_id(self):
        raise NotImplementedError

    def query_linear_speed(self):
        raise NotImplementedError

    def query_angular_speed(self):
        raise NotImplementedError

    def configure_targets(self):
        raise NotImplementedError

    def send(self):
        raise NotImplementedError

    def name(self, prefix):
        return "{}_p{}_s{}".format(prefix, self.parent_id, self.id)

    def best_config(self):
        if not self.targets:
            return
        common_configs = set([k for k in robo.ALL_CONFIGS])

        for target in self.targets:
            common_configs = common_configs.intersection(
                target.valid_configs())
            if not common_configs:
                raise utils.StrokeError(
                    "Can't find best config for stroke. No common configs")

        return sorted(list(common_configs))[0]

    def all_configs(self):
        result = []
        for target in self.targets:
            result.append(target.valid_configs())
        return result


class BotStroke(Stroke):

    def __init__(self, cluster_id, stroke_id, brush, node):

        self.cluster_id = cluster_id

        super(BotStroke, self).__init__(stroke_id, brush, node)

    def best_config(self):
        if not self.targets:
            return

        targets = self.targets + self.arrivals + [self.departure]

        common_configs = set(targets[0].valid_configs())
        for target in targets[1:]:
            common_configs = common_configs.intersection(
                target.valid_configs())
            if not common_configs:
                raise utils.StrokeError(
                    "Can't find best config for stroke. No common configs")

        return sorted(list(common_configs))[0]

    def build_targets(self):
        positions = self.query_positions()
        rotations = self.query_rotations()
        self.targets = self._build_target_array(
            positions, rotations, self.brush, self.id)

        positions = self.query_arrival_positions()
        rotations = self.query_arrival_rotations()
        self.arrivals = self._build_target_array(
            positions, rotations, self.brush, self.id, ArrivalTarget)

        positions = self.query_departure_positions()
        rotations = self.query_departure_rotations()
        self.departure = self._build_target_array(
            positions, rotations, self.brush, self.id, DepartureTarget)[0]

    def configure_targets(self):
        try:
            config = self.best_config()
        except utils.StrokeError:
            configs = self.all_configs()
            print("CONFIGS FOR STROKE : {}".format(self.id))
            print(configs)
            raise

        for target in self.targets:
            target.configure(config)
        self.targets[0].linear = False

        for target in self.arrivals:
            target.configure(config)

        self.departure.configure(config)

    @classmethod
    def _build_target_array(
            cls,
            positions,
            rotations,
            brush,
            stroke_id,
            target_type=Target):
        result = []
        num_targets = len(positions)
        if not num_targets == len(rotations):
            raise utils.StrokeError(
                "Length mismatch: positions, rotations")

        for i, (p, r) in enumerate(zip(positions, rotations)):
            try:
                tg = target_type(i, (p * 10), r, brush)
            except utils.StrokeError:
                print "Target Position:", p
                print "StrokeId:", stroke_id
                raise

            result.append(tg)
        return result

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

    def query_backstroke(self):
        return pm.paintingQuery(
            self.node,
            clusterIndex=self.cluster_id,
            strokeIndex=self.id,
            strokeBackstroke=True,
        )

    def query_arc_length(self):
        return pm.paintingQuery(
            self.node,
            clusterIndex=self.cluster_id,
            strokeIndex=self.id,
            strokeArcLength=True,
        )

    def query_parent_id(self):
        return pm.paintingQuery(
            self.node,
            clusterIndex=self.cluster_id,
            strokeIndex=self.id,
            strokeParentIndex=True,
        )

    def query_linear_speed(self):
        return pm.paintingQuery(
            self.node,
            clusterIndex=self.cluster_id,
            strokeIndex=self.id,
            strokeSpeedLinear=True,
        )

    def query_angular_speed(self):
        return pm.paintingQuery(
            self.node,
            clusterIndex=self.cluster_id,
            strokeIndex=self.id,
            strokeSpeedAngular=True,
        )

    def query_positions(self):
        return utils.to_point_array(
            pm.paintingQuery(
                self.node,
                clusterIndex=self.cluster_id,
                strokeIndex=self.id,
                strokePositions=True
            )
        )

    def query_rotations(self):
        return utils.to_vector_array(
            pm.paintingQuery(
                self.node,
                clusterIndex=self.cluster_id,
                strokeIndex=self.id,
                strokeRotations=True,
                rotateOrder="zyx",
                rotateUnit="rad",
            )
        )

    def query_arrival_positions(self):
        return utils.to_point_array(
            pm.paintingQuery(
                self.node,
                clusterIndex=self.cluster_id,
                strokeIndex=self.id,
                strokeArrivalPositions=True
            )
        )

    def query_arrival_rotations(self):
        return utils.to_vector_array(
            pm.paintingQuery(
                self.node,
                clusterIndex=self.cluster_id,
                strokeIndex=self.id,
                strokeArrivalRotations=True,
                rotateOrder="zyx",
                rotateUnit="rad",
            )
        )

    def query_departure_positions(self):
        return utils.to_point_array(
            pm.paintingQuery(
                self.node,
                clusterIndex=self.cluster_id,
                strokeIndex=self.id,
                strokeDeparturePosition=True
            )
        )

    def query_departure_rotations(self):
        return utils.to_vector_array(
            pm.paintingQuery(
                self.node,
                clusterIndex=self.cluster_id,
                strokeIndex=self.id,
                strokeDepartureRotation=True,
                rotateOrder="zyx",
                rotateUnit="rad",
            )
        )
