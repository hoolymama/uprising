import robodk as rdk
from robolink import INSTRUCTION_INSERT_CODE

from utils import StrokeError
import robo


class Target(object):
    def __init__(self, target_id, position, rotation, brush):
        self.id = target_id
        self.tool_pose = rdk.TxyzRxyz_2_Pose(list(position) + list(rotation))

        self.joint_pose = None
        flange_pose = self.tool_pose * brush.matrix.invH()
        print "target_id:" , target_id
        self.joint_poses = robo.config_poses(flange_pose)
        self.linear = True

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
        self.joint_pose = self.joint_poses[config][0]

    def name(self, prefix):
        return "{}_t{}".format(prefix, self.id)

    def send(self, prefix, program, frame):
        return NotImplementedError


class BotTarget(object):
    def __init__(self, target_id, position, rotation, brush):
        super(BotTarget, self).__init__(target_id, position, rotation, brush)

    def send(self, prefix, program, frame):
        link = robo.link()
        robot = robo.robot()
        target_name = self.name(prefix)
        rdk_target = link.AddTarget(target_name, frame, robot)
        rdk_target.setPose(self.tool_pose)
        rdk_target.setJoints(self.joint_pose)
        if program:
            if self.linear:
                program.addMoveL(rdk_target)
            else:
                program.addMoveJ(rdk_target)


class ArrivalTarget(BotTarget):
    def name(self, prefix):
        return "{}_ar{}".format(prefix, self.id)

    def configure(self, config):
        self.joint_pose = self.joint_poses[config][0]
        self.linear = False


class DepartureTarget(BotTarget):
    def name(self, prefix):
        return "{}_dp{}".format(prefix, self.id)
