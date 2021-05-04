import robodk as rdk
from robolink import INSTRUCTION_INSERT_CODE

from uprising.utils import StrokeError
from uprising import robo


class Target(object):
    def __init__(self, target_id, position, rotation):
        self.id = target_id
        self.tool_pose = rdk.TxyzRxyz_2_Pose(list(position) + list(rotation))
        self.joint_pose = None
        self.linear = None


        
    def solve_joint_poses(self, brush):
        """
        # joint_poses : {
        # "010": [[90,78,34,123,76,86], [90,78,34,123,76,86], ...],
        # "011": [[-124,78,34,123,76,86], [-124,78,34,123,76,86], ...],
        # ...
        # }
        """
        flange_pose = self.tool_pose * brush.matrix.invH()
        self.joint_poses = robo.solve_joint_poses(flange_pose)


    def valid_configs(self):
        """
        Return a list of configs.

        e.g. ["001", "010", "110"]
        """
        result = []
        for key in self.joint_poses:
            if self.joint_poses[key]:
                result.append(key)
        return result

    def configure(self, config):
        # print "Target configure self.joint_pose = self.joint_poses[config][0]", config
        self.joint_pose = self.joint_poses[config][0]
        # print "SET", self.joint_pose, "TO" , self.joint_poses[config][0]
        self.linear = True

    def name(self, prefix):
        return "{}_t{}".format(prefix, self.id)

    def send(self, prefix, program, frame):
        return NotImplementedError


  
        
        # self.flange_pose = self.tool_pose * brush.matrix.invH()
        # print "target_id:", target_id

        # possible joint poses
