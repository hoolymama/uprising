from robolink import INSTRUCTION_COMMENT, INSTRUCTION_SHOW_MESSAGE, INSTRUCTION_INSERT_CODE

from uprising.bot.session.bot_target import BotTarget, DepartureTarget, ArrivalTarget
import pymel.core as pm
from uprising import robo
from uprising import utils

from uprising.common.session.stroke import Stroke

from uprising import const as k


class BotStroke(Stroke):
    def __init__(self, cluster_id, stroke_index, node):
        self.cluster_id = cluster_id
        
        super(BotStroke, self).__init__(stroke_index, node)

        self.global_stroke_id = self.query_stroke_id()
        
    def _build_targets(self):

        positions = self.query_positions()
        rotations = self.query_rotations()
        self.targets = self._target_factory(positions, rotations, self.id, BotTarget)

        positions = self.query_arrival_positions()
        rotations = self.query_arrival_rotations()
        self.arrivals = self._target_factory(positions, rotations, self.id, ArrivalTarget)

        positions = self.query_departure_positions()
        rotations = self.query_departure_rotations()
        self.departure = self._target_factory(positions, rotations, self.id, DepartureTarget)[0]

    def name(self, prefix):
        return "{}_g{}_p{}_s{}".format(prefix, self.global_stroke_id, self.parent_id, self.id)

    @classmethod
    def _target_factory(cls, positions, rotations, stroke_id, target_type=BotTarget):
        result = []
        num_targets = len(positions)
        if not num_targets == len(rotations):
            raise utils.StrokeError("Length mismatch: positions, rotations")

        for i, (p, r) in enumerate(zip(positions, rotations)):
            try:
                target = target_type(i, (p * 10), r)
            except utils.StrokeError:
                print "Target Position:", p
                print "StrokeId:", stroke_id
                raise

            result.append(target)
        return result

    def send(self, prefix, program, frame, motion):
        stroke_name = self.name(prefix)
        program.RunInstruction("Stroke %s" % stroke_name, INSTRUCTION_COMMENT)
        
        for t in self.arrivals:
            t.send(stroke_name, program, frame)
        
        if self.override_path_parameters:
            program.setRounding(self.approximation_distance)
            program.setSpeed(self.linear_speed)
            program.setSpeedJoints(self.angular_speed)
        
        if self.ignore:
            self.departure.linear = False
        else:
            for t in self.targets:
                t.send(stroke_name, program, frame)
                
        if self.override_path_parameters:
            program.setSpeed(motion["linear_speed"])
            program.setRounding(motion["rounding"])
            program.setSpeedJoints(motion["angular_speed"])


        self.departure.send(stroke_name, program, frame)


    # NOTE: NAMING HERE
    # strokeIndex refers to the cluster relative index
    # globalStrokeId refers to the ID of the stroke within the painting.
    def query_stroke_id(self):
        return pm.paintingQuery(
            self.node,
            clusterIndex=self.cluster_id,
            strokeIndex=self.id,
            globalStrokeId=True
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
        ) * 10

    def query_angular_speed(self):
        return pm.paintingQuery(
            self.node,
            clusterIndex=self.cluster_id,
            strokeIndex=self.id,
            strokeSpeedAngular=True,
            rotateUnit="deg"
        )

    def query_approximation_distance(self):
        return pm.paintingQuery(
            self.node,
            clusterIndex=self.cluster_id,
            strokeIndex=self.id,
            strokeApoproximationDistance=True,
        ) * 10


    def query_positions(self):
        return utils.to_point_array(
            pm.paintingQuery(
                self.node, clusterIndex=self.cluster_id, strokeIndex=self.id, strokePositions=True
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
                strokeArrivalPositions=True,
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
                strokeDeparturePosition=True,
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


    def query_layer_id(self):
        return pm.paintingQuery(
            self.node,
            clusterIndex=self.cluster_id,
            strokeIndex=self.id,
            strokeLayerId=True
        )
  

