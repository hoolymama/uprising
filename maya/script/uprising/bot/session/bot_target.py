
import robodk as rdk
from robolink import INSTRUCTION_INSERT_CODE

from uprising.common.session.target import Target
from uprising import robo
from uprising import const


class BotTarget(Target):
    def __init__(self, target_id, position, rotation):
        super(BotTarget, self).__init__(target_id, position, rotation)


    def solve_single_joint_poses(self, brush, prev_target=None):
        """
    # joint_poses : {
        # "010": [[90,78,34,123,76,86], [90,78,34,123,76,86], ...],
        # "011": [[-124,78,34,123,76,86], [-124,78,34,123,76,86], ...],
        # ...
        # }
        """
        prev_pose = const.PAINTING_START_JOINTS
        if prev_target:

            # print "prev:", prev_target.name("<-") , "self:", self.name("-"), "Prev JP:",  prev_target.joint_poses
            key = prev_target._find_first_config()
            prev_pose = prev_target.joint_poses[key][0]

        flange_pose = self.tool_pose * brush.matrix.invH()

        self.joint_poses = robo.solve_single_joint_pose(flange_pose, prev_pose)

    def flange_pose(self, brush):
        return self.tool_pose * brush.matrix.invH()

    def solve_all_joint_poses(self, brush):
        """
        # joint_poses : {
        # "010": [[90,78,34,123,76,86], [90,78,34,123,76,86], ...],
        # "011": [[-124,78,34,123,76,86], [-124,78,34,123,76,86], ...],
        # ...
        # }
        """
        flange_pose = self.tool_pose * brush.matrix.invH()
        self.joint_poses = robo.solve_joint_poses(flange_pose)

    def _find_first_config(self):
        return [k for k in self.joint_poses.keys() if self.joint_poses[k]][0]

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
        self.linear = False
        try:
            self.joint_pose = self.joint_poses[config][0]
        except IndexError:
            config = self._find_first_config()
            self.joint_pose = self.joint_poses[config][0]

class DepartureTarget(BotTarget):
    def name(self, prefix):
        return "{}_dp{}".format(prefix, self.id)
