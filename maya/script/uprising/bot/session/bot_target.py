
import robodk as rdk
from robolink import INSTRUCTION_INSERT_CODE

from uprising.common.session.target import Target
from uprising import robo
from uprising import const

class BotTarget(Target):
    def __init__(self, target_id, position, rotation):
        super(BotTarget, self).__init__(target_id, position, rotation)
        self.linear = True

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
    def __init__(self, target_id, position, rotation):
        super(ArrivalTarget, self).__init__(target_id, position, rotation)
        self.linear = False

    def name(self, prefix):
        return "{}_ar{}".format(prefix, self.id)

class DepartureTarget(BotTarget):

    def name(self, prefix):
        return "{}_dp{}".format(prefix, self.id)


