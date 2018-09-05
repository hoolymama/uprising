import robodk as rdk

import uprising_util as uutl
from uprising_util import StrokeError
# from robolink import (COLLISION_ON, COLLISION_OFF, ITEM_TYPE_ROBOT)

CONFIG_MASK = "000"

class Target(object):
 
    def __init__(self, id_, position, rotation, tangent, robot, brush, planeNormal):
        self.id = id_
        self.linear = False

        self.tangent = tangent
        self.joint_pose = None
        self.tool_pose = rdk.TxyzRxyz_2_Pose(list(position) + list(rotation))
        self.flange_pose = self.tool_pose * brush.matrix.invH()
        
        self.joint_poses =  uutl.config_000_poses(self.flange_pose)

        if not self.joint_poses:
            raise StrokeError("Cant initialize target: %d, no ik solutions" % self.id)
 
    def configure(self, robot, last_mat, ref_joint_pose):
        self.joint_pose =  self.joint_poses[0]
        self.linear = bool(last_mat)  

    def name(self, prefix):
        return "%s_t%d" % (prefix, self.id)

    def write(self, prefix, program, frame, studio):
        target_name = self.name(prefix)
        rdk_target = studio.RL.AddTarget(target_name, frame, studio.robot)
        rdk_target.setPose(self.tool_pose)
        rdk_target.setJoints(self.joint_pose)
        if self.linear:
            program.addMoveL(rdk_target)
        else:
            program.addMoveJ(rdk_target)