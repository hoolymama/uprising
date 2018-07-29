import robodk as rdk

from uprising_util import StrokeError
from robolink import (Robolink, COLLISION_ON, COLLISION_OFF, ITEM_TYPE_ROBOT)

CONFIG_MASK = "000"

class Target(object):

    @staticmethod
    def config_key(config):
        if config:
            return "%d%d%d" % tuple(config.list2()[0][0:3])

    def __init__(self, id_, position, rotation, tangent, robot, brush, planeNormal):
        self.id = id_
        self.linear = False
        self.position = position
        self.rotation = rotation
        self.tangent = tangent
        
        self.joint_pose = None
        self.configs = {}
        self.vertices = []

        self.tool_pose = rdk.TxyzRxyz_2_Pose(list(position) + list(rotation))
        self.flange_pose = self.tool_pose * brush.matrix.invH()
        ik = robot.SolveIK_All(self.flange_pose)
        siz = ik.size()
   
        if not (ik and  siz[0] and  siz[1] and (len(ik.list()) > 5)):
            raise StrokeError("Cant initialize target: %d, no ik solutions exist" % self.id)
        joint_poses = [el[0:6] for el in ik.list2()]
        for pose in joint_poses:
            key = Target.config_key(robot.JointsConfig(pose))
            if key.startswith(CONFIG_MASK):
                self.configs.setdefault(key, []).append(pose)
        if not self.configs:
            raise StrokeError("Cant initialize target: %d, no ik solutions in mask %s " % (self.id, CONFIG_MASK))

        self.setVertices(planeNormal, brush.width)

    def setVertices(self, normal, width):
        side = (self.tangent^normal).normal() * width * 0.5
        self.vertices.append(self.position + side)
        self.vertices.append(self.position - side)

    def configure(self, robot, last_mat, ref_joint_pose, config):
        self.joint_pose =  self.configs[config][0]
        self.linear = bool(last_mat)  

