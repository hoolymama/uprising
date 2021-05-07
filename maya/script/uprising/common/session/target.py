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

    def name(self, prefix):
        return "{}_t{}".format(prefix, self.id)

    def send(self, prefix, program, frame):
        return NotImplementedError

    def  flange_pose(self, brush): 
        return self.tool_pose * brush.matrix.invH()