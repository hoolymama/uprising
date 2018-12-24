from robolink import (
    INSTRUCTION_COMMENT
)
from target import (Target, DepartureTarget, ArrivalTarget)
import pymel.core as pm

import uprising_util as uutl
from uprising_util import StrokeError


def config_key(config):
    if config:
        return "%d%d%d%d" % tuple(config.list2()[0][0:4])


class Stroke(object):

    def __init__(self, cluster_id, _id, node, robot, brush):
        self.cluster_id = cluster_id
        self.id = _id
        self.node = node
        self.robot = robot
        self.brush = brush
        # self.last_valid_joints = last_valid_joints
        
        self.set_stroke_params()
        self.build_targets()
        self.build_arrivals()
        self.build_departure()

        # self.build_stop_targets()
        
        self.configure()

    def set_stroke_params(self):
        # self.normal = pm.paintingQuery(
        #     self.node,
        #     clusterIndex=self.cluster_id,
        #     strokeIndex=self.id,
        #     strokeNormal=True)

        self.backstroke = pm.paintingQuery(
            self.node,
            clusterIndex=self.cluster_id,
            strokeIndex=self.id,
            strokeBackstroke=True)

        self.arc_length = pm.paintingQuery(
            self.node,
            clusterIndex=self.cluster_id,
            strokeIndex=self.id,
            strokeArcLength=True)

        self.parent_id = pm.paintingQuery(
            self.node,
            clusterIndex=self.cluster_id,
            strokeIndex=self.id,
            strokeParentIndex=True)

 
    def build_targets(self):

        self.targets = []
 
        positions = uutl.to_point_array(
            pm.paintingQuery(
                self.node,
                clusterIndex=self.cluster_id,
                strokeIndex=self.id,
                strokePositions=True))

        rotations = uutl.to_vector_array(
            pm.paintingQuery(
                self.node,
                clusterIndex=self.cluster_id,
                strokeIndex=self.id,
                strokeRotations=True,
                rotateOrder="zyx",
                rotateUnit="rad"))

        tangents = uutl.to_vector_array(
            pm.paintingQuery(
                self.node,
                clusterIndex=self.cluster_id,
                strokeIndex=self.id,
                strokeTangents=True))

        num_targets = len(positions)
        if not (num_targets == len(rotations) and
                num_targets == len(tangents)):
            raise StrokeError(
                "Length mismatch: positions, rotations, tangents")

        for i, (p, r, t) in enumerate(zip(positions, rotations, tangents)):
            tg = Target(i, (p * 10), r, t, self.robot, self.brush)
            self.targets.append(tg)


    def build_arrivals(self):

        self.arrivals = []

        positions = uutl.to_point_array(
            pm.paintingQuery(
                self.node,
                clusterIndex=self.cluster_id,
                strokeIndex=self.id,
                strokeArrivalPositions=True))

        rotations = uutl.to_vector_array(
            pm.paintingQuery(
                self.node,
                clusterIndex=self.cluster_id,
                strokeIndex=self.id,
                strokeArrivalRotations=True,
                rotateOrder="zyx",
                rotateUnit="rad"))

        num = len(positions)
        if not (num == len(rotations)  ):
            raise StrokeError(
                "Arrivals length mismatch: positions, rotations")

        for i, (p, r) in enumerate(zip(positions, rotations)):
            tg = ArrivalTarget(i, (p * 10), r, None , self.robot, self.brush)
            self.arrivals.append(tg)


    def build_departure(self):

        position = uutl.to_point_array(
            pm.paintingQuery(
                self.node,
                clusterIndex=self.cluster_id,
                strokeIndex=self.id,
                strokeDeparturePosition=True))[0]

        rotation = uutl.to_vector_array(
            pm.paintingQuery(
                self.node,
                clusterIndex=self.cluster_id,
                strokeIndex=self.id,
                strokeDepartureRotation=True,
                rotateOrder="zyx",
                rotateUnit="rad"))[0]

        self.departure =   DepartureTarget(0, (position * 10), rotation, None , self.robot, self.brush)

 

    def name(self, prefix):
        return "%s_p%d_s%d" % (prefix, self.parent_id, self.id)

    def write(self, prefix, program, frame, studio):
        stroke_name = self.name(prefix)
        program.RunInstruction("Stroke %s" % stroke_name, INSTRUCTION_COMMENT)

        for t in self.arrivals:
            t.write(stroke_name, program,frame, studio)

        for t in self.targets:
            t.write(stroke_name, program,frame, studio)

        self.departure.write(stroke_name, program,frame, studio)

    # Well there should only be the 000 config key anyway.
    # def get_common_configs(self):
    #     result = []
    #     keys = self.targets[0].configs.keys()
    #     for key in keys:
    #         valid = True
    #         for target in self.targets:
    #             if key not in target.configs:
    #                 valid = False
    #                 break
    #         if valid:
    #             result.append(key)
    #     return result

    def configure(self):

        # self.common_configs = self.get_common_configs()
        # if not self.common_configs:
        #     raise StrokeError("Cant find any common configs")

        # for config in self.common_configs:
        # ref_pose = self.targets[0].joint_poses[0]
        # self.configure_targets(self.robot, ref_pose)

        # last_mat = None
        for target in self.arrivals:
            target.configure()

        for target in self.targets:
            target.configure()

        self.departure.configure()
            # target.configure(self.robot, last_mat, ref_pose)
            # ref_pose = target.joint_pose
            # last_mat = target.tool_pose


        # for stop_target in self.stop_targets:
        #     stop_target.configure(self.robot)



            # if found:
            #     break

        # if not found:
        #     raise StrokeError(
        #         "Cannot find any stroke solution %s" %
        #         self.identifier())

    # def configure_targets(self, robot, ref_pose):
    #     last_mat = None
    #     for target in self.targets:
    #         # try:
    #         target.configure(robot, last_mat, ref_pose)
    #         # except StrokeError:
    #             # return False
    #         ref_pose = target.joint_pose
    #         last_mat = target.tool_pose
    #     # return True


    # def __init__(
    #         self,
    #         robot,
    #         positions,
    #         rotations,
    #         tangents,
    #         brush,
    #         paint,
    #         arc_length,
    #         planeNormal,
    #         start_approach_dist,
    #         end_approach_dist,
    #         curve_name,
    #         curve_stroke_id):
    #     self.arc_length = arc_length
    #     self.common_configs = []
    #     self.valid_config = None
    #     self.targets = []
    #     self.curve_name = curve_name
    #     self.curve_stroke_id = curve_stroke_id

    #     start_approach = planeNormal * start_approach_dist
    #     end_approach = planeNormal * end_approach_dist

    #     ps = [(positions[0] + start_approach)] + \
    #         positions + [(positions[-1] + end_approach)]
    #     rs = [rotations[0]] + \
    #         rotations + [rotations[-1]]
    #     ts = [tangents[0]] + \
    #         tangents + [tangents[-1]]

    #     for i in range(len(ps)):
    #         # will throw if no solutions
    #         t = Target(
    #             i,
    #             (ps[i] * 10),
    #             rs[i],
    #             ts[i],
    #             robot,
    #             brush,
    #             planeNormal)
    #         self.targets.append(t)


    # def configure(self, robot, brush):

    #     # print "Stroke.configure() ------------------------------- %s" % self.identifier()
    #     self.common_configs = self.get_common_configs()
    #     if not self.common_configs:
    #         raise StrokeError("Cant find any common configs")

    #     found = False
    #     # try the basic ref pose
    #     # print "1. try the basic ref pose - no flip"
    #     # found = self.configure_targets(
    #     #     robot, allow_flip=False)

    #     # try start by looping through configurations

    #     # print self.common_configs
    #     # if not found:
    #     #     # print "2. try all configs - no flip"
    #     #     for config in self.common_configs:
    #     #         # print "Try %s " % config
    #     #         joint_pose = self.targets[0].configs[config][0]
    #     #         found = self.configure_targets(
    #     #             robot, joint_pose, allow_flip=False)
    #     #         if found:
    #     #             break

    #     # try global ref pose and allow flips
    #     # print "3. try the basic ref pose - with flip"

    #     # if not found:
    #     #     found = self.configure_targets(
    #     #         robot, allow_flip=True)

    #     # try start by looping through configurations

    #     # if not found:
    #     #     # print "4. try all configs - with flip"
    #     #     for config in self.common_configs:
    #     #         joint_pose = self.targets[0].configs[config][0]
    #     #         found = self.configure_targets(
    #     #             robot, joint_pose, allow_flip=True)
    #     #         if found:
    #     #             break

    #     # ALTERNATIVE
    #     if not found:
    #         # print "4. try all configs - with flip"
    #         for config in self.common_configs:
    #             ref_pose = self.targets[0].configs[config][0]
    #             found = self.configure_targets(robot, ref_pose, config)
    #             if found:
    #                 break

    #     print "------------------- %s" % self.identifier()
    #     for t in self.targets:
    #         print t.configs

    #     if not found:
    #         raise StrokeError(
    #             "Cannot find any stroke solution %s" %
    #             self.identifier())

    # # def configure_targets(self, robot, ref_pose, **kw):
    # #     allow_flip = kw["allow_flip"]
    # #     last_pose = ref_pose
    # #     last_mat = None
    # #     for target in self.targets:
    # #         try:
    # #             target.configure(robot, last_mat, last_pose, allow_flip)
    # #         except StrokeError:
    # #             return False
    # #         last_pose = target.joint_pose
    # #         last_mat = target.tool_pose
    # #     return True

    # # ALTERNATIVE
    
   
