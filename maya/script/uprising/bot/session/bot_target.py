
import robodk as rdk
from robolink import INSTRUCTION_INSERT_CODE

from uprising.common.session.target import Target
from uprising import robo
from uprising import const

import logging
logger = logging.getLogger(__name__)
logger.setLevel(logging.ERROR)

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
        if not self.joint_pose:
            logger.error("No joint pose for target {}".format(target_name))
            raise ValueError("No joint pose for target {}".format(target_name))
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


#   File "/Volumes/xhf/tools/darwin64/uprising/999.0.0/maya/scripts/uprising/bot/session/bot_stroke.py", line 72, in send
#     self.departure.send(stroke_name, program, frame)
#   File "/Volumes/xhf/tools/darwin64/uprising/999.0.0/maya/scripts/uprising/bot/session/bot_target.py", line 20, in send
#     rdk_target.setJoints(self.joint_pose)