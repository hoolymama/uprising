import pymel.core as pm
from uprising import robo
from uprising.common.session.program import Program
from uprising.common.session.target import Target
from robolink import (
    COLLISION_OFF
)

import copy



class ClusterTestProgram(Program):
    def __init__(self, name, cluster, motion):

        super(ClusterTestProgram, self).__init__(name)
        self.cluster = cluster
        self.motion = motion
        self.cache = {"index":None,"arrivals": None}
        

    def configure_internal(self, i):
        if i < 1 or i >= len(self.cluster.strokes):
            raise ValueError("Bad Stroke Index")
        self.current_index = i

        self.cache["index"] = i
        self.cache["arrivals"] = copy.deepcopy(self.cluster.strokes[i].arrivals)
        
        self.cluster.strokes[i].arrivals = self.cluster.strokes[i].arrivals[-1:]
        self.cluster.strokes[i].arrivals[-1].linear=True

    def undo(self):
        self.cluster.strokes[self.cache["index"]].arrivals = self.cache["arrivals"]


    def send(self, **kw):
        super(ClusterTestProgram, self).send()
        frame = robo.create_frame("from_to_test_frame")
        self.cluster.brush.send()
        self.cluster.send(self.program, frame, self.motion)


    def validate_path(self):
        return super(ClusterTestProgram, self).validate_path()


# class LinkProgram(Program):
#     def __init__(self, name, from_target, to_target, brush):
#         super(LinkProgram, self).__init__(name)
#         self.from_target = from_target
#         self.to_target = to_target
#         self.brush = brush


#         self.rdk_from_target=None
#         self.rdk_to_target=None



#     def send(self, **kw):
#         super(LinkProgram, self).send()
#         frame = robo.create_frame("from_to_test_frame")

#         self.brush.send()

#         FROM_NAME = "from_target"
#         TO_NAME = "to_target"
#         if isinstance(self.from_target, Target):
#             rdk_from_target = robo.link().AddTarget(FROM_NAME, frame, robo.robot())
#             rdk_from_target.setPose(self.from_target.tool_pose)
#             rdk_from_target.setJoints(self.from_target.joint_pose)
#         else:
#             rdk_from_target = robo.create_joint_target(
#                 pm.PyNode(self.from_target), FROM_NAME, frame
#             )



#         self.program.addMoveJ(rdk_from_target)

#         if isinstance(self.to_target, Target):
#             rdk_to_target = robo.link().AddTarget(TO_NAME, frame, robo.robot())
#             rdk_to_target.setPose(self.to_target.tool_pose)
#             rdk_to_target.setJoints(self.to_target.joint_pose)
#         else:
#             rdk_to_target = robo.create_joint_target(pm.PyNode(self.to_target), TO_NAME, frame)

#         self.program.addMoveL(rdk_to_target)

#         self.rdk_from_target = rdk_from_target
#         self.rdk_to_target = rdk_to_target



#     def validate_path(self):
#         return super(LinkProgram, self).validate_path()

#         # robo.link().setCollisionActive(False)
#         # result = { "status": "FAILURE"}
#         # if (not path_result) or( path_result["status"] != "SUCCESS"):
#         #     return result
        
#         # # path_result was success so try linear move
#         # robot = robo.robot()

#         # robot.MoveJ( self.rdk_from_target )
#         # joints = robot.Joints()
#         # robot.MoveJ( self.rdk_to_target )
#         # pose = robot.Pose()

#         # if robot.MoveL_Test(joints, pose, 10) == 0:
#         #     return { "status": "SUCCESS"}
#         # return { "status": "FAILURE"}
 

