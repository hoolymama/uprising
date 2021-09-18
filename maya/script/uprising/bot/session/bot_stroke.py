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
            program.setSpeed(self.linear_speed, self.angular_speed)
            program.setRounding(self.approximation_distance)
   
        if self.ignore:
            self.departure.linear = False
        else:
            for t in self.targets:
                t.send(stroke_name, program, frame)
                
        if self.override_path_parameters:
            program.setSpeed(motion["linear_speed"], motion["angular_speed"])
            program.setRounding(motion["rounding"])
   


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

    # def send(self, with_geo=False):
    #     link = robo.link()
    #     robot = robo.robot()

    #     old_brush = link.Item(self.name)
    #     if old_brush.Valid():
    #         return
    #     tool_item = robot.AddTool(self.matrix, self.name)
    #     if with_geo:
    #         triangles = uutl.to_vector_array(
    #             pm.brushQuery(self.plug, tri=True))
    #         triangles = [[t.x * 10, t.y * 10, t.z * 10] for t in triangles]
    #         shape = link.AddShape(triangles)
    #         tool_item.AddGeometry(shape, rdk.eye())
    #         shape.Delete()
    #     robot.setPoseTool(tool_item)

    # def best_config(self):
    #     if not self.targets:
    #         return

    #     targets = self.targets + self.arrivals[-1] + [self.departure]

    #     common_configs = set(targets[0].valid_configs())
    #     for target in targets[1:]:
    #         common_configs = common_configs.intersection(
    #             target.valid_configs())
    #         if not common_configs:
    #             raise utils.StrokeError(
    #                 "Can't find best config for stroke. No common configs")

    #     return sorted(list(common_configs))[0]

    # def configure(self, brush):

    #     # self.brush.send()
    #     solved = False
    #     prev_target = None
    #     for target in self.arrivals + self.targets + [self.departure]:
    #         target.solve_single_joint_poses(brush, prev_target)
    #         prev_target = target
    #     try:
    #         self.config = self.best_config()
    #         solved = True

    #     except utils.StrokeError:
    #         pass
    #     if not solved:
    #         for target in self.arrivals + self.targets + [self.departure]:
    #             target.solve_all_joint_poses(brush)
    #         try:
    #             self.config = self.best_config()

    #         except utils.StrokeError:
    #             configs = self.all_configs()
    #             print ("FAILED:!! CONFIGS FOR STROKE : {}".format(self.id))
    #             print (configs)
    #             for target in self.targets + [self.departure]:
    #                 print target.name("T"), target.valid_configs()
    #             raise

    #     for target in self.arrivals:
    #         target.configure(self.config)
    #         # print "STK Configured", target.name("-"), self.config, target.joint_pose

    #     for target in self.targets:
    #         target.configure(self.config)
    #         # print "STK Configured", target.name("-"), self.config,self.joint_pose

    #     self.departure.configure(self.config)
    #     # print "STK Configured", self.departure.name("-"), self.config,self.joint_pose

    # def best_config(self):
    #     if not self.targets:
    #         print "Stroke has no targets {}".format(self.name("-"))
    #         return

    #     # Fined best configs for linear targets (targets + departure target).
    #     # NOTE: May have to set the config explicitly for joint targets.
    #     targets = self.targets + [self.departure]

    #     common_configs = set([k for k in robo.ALL_KR30_CONFIGS])
    #     for target in targets:
    #         common_configs = common_configs.intersection(target.valid_configs())
    #         if not common_configs:
    #             raise utils.StrokeError(
    #                 "Common Config failure, can't find best config for stroke. No common configs {}".format(
    #                     self.name("-")
    #                 )
    #             )

    #     return sorted(list(common_configs))[0]
    #     # common_configs = sorted(list(common_configs))

    #     # name = self.name("-")
    #     # print "Stroke {} has common_configs {} -- Testing linear moves".format(
    #     #     name, common_configs)
    #     # config = self.test_linear_moves(common_configs)
    #     # if not config:
    #     #     raise utils.StrokeError(
    #     #         "Linear move failure, can't find config where all linear moves are possible {}".format(self.name("-")))
    #     # return config

    # def all_configs(self):
    #     result = []
    #     for target in self.targets + [self.departure]:
    #         result.append(target.valid_configs())
    #     return result
