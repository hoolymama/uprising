
from robolink import (
    INSTRUCTION_COMMENT,
    INSTRUCTION_SHOW_MESSAGE,
    INSTRUCTION_INSERT_CODE
)

import pymel.core as pm
from uprising import robo
from uprising import utils


class Stroke(object):

    def __init__(self, stroke_index, node):
        self.id = stroke_index
        self.node = node
        self.targets = []
        self.arc_length = self.query_arc_length()
        self.parent_id = self.query_parent_id()
        self.linear_speed = self.query_linear_speed()
        self.angular_speed = self.query_angular_speed()
        self.approximation_distance = self.query_approximation_distance()
        self.layerId = self.query_layer_id()

        self.override_path_parameters = self.linear_speed > 0 and self.angular_speed > 0
        
        self.config = None
        self.ignore = False
        self._build_targets()

    def name(self, prefix):
        return "{}_p{}_s{}".format(prefix, self.parent_id, self.id)

    def _build_targets(self):
        raise NotImplementedError

    def query_arc_length(self):
        raise NotImplementedError

    def query_parent_id(self):
        raise NotImplementedError

    def query_linear_speed(self):
        raise NotImplementedError

    def query_angular_speed(self):
        raise NotImplementedError

    def query_approximation_distance(self):
        raise NotImplementedError

    def query_layer_id(self):
        raise NotImplementedError

    def configure(self):
        raise NotImplementedError

    def send(self):
        raise NotImplementedError

    #############################
    #############################

    # def test_linear_moves(self,  brush, *names):

    #     robot = robo.robot()

    #     # setPoseTool to the brush if not already set
    #     brush.send()

    #     for name in names[1:]:
    #         _link.Item(name)


    #     for config in configs:
    #         print "Tesing linear move for Stroke {} config: {}".format(
    #             self.name("-"), config)
    #         joints = self.targets[0].joint_poses[config][0]
    #         config_okay = True
    #         for target in self.targets[1:]:
    #             fail = robot.MoveL_Test(joints, target.tool_pose, minstep_mm=2)
    #             if fail:
    #                 config_okay = False
    #                 break
    #             joints = target.joint_poses[config][0]

    #         if config_okay:
    #             print "Config {} is okay! Returning".format(config)
    #             return config
    #         else:
    #             print "Config {} NOT okay! Continuing".format(config)


    # def best_config(self):
    #     name = self.name("-")
    #     if not self.targets:
    #         print "Stroke has no targets {}".format(name)
    #         return
    #     common_configs = set([k for k in robo.ALL_CONFIGS])

    #     for target in self.targets:
    #         common_configs = common_configs.intersection(
    #             target.valid_configs())
    #         if not common_configs:
    #             raise utils.StrokeError(
    #                 "Common Config failure, can't find best config for stroke. No common configs {}".format(name))

    #     common_configs = sorted(list(common_configs))

    #     print "Stroke {} has common_configs {} -- Testing linear moves".format(name, common_configs)
    #     config = self.test_linear_moves(common_configs)
    #     if not config:
    #         raise utils.StrokeError(
    #                 "Linear move failure, can't find config where all linear moves are possible {}".format(name))

    #     return config


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
