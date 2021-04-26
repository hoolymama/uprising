
import robodk as rdk
from robolink import INSTRUCTION_INSERT_CODE

from uprising.common.session.target import Target
from uprising import robo


class BotTarget(Target):
    def __init__(self, target_id, position, rotation):
        super(BotTarget, self).__init__(target_id, position, rotation)


    def send(self, prefix, program, frame):
        link = robo.link()
        robot = robo.robot()
        target_name = self.name(prefix)
        rdk_target = link.AddTarget(target_name, frame, robot)
        rdk_target.setPose(self.tool_pose)
        rdk_target.setJoints(self.joint_pose)
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
