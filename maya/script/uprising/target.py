import robodk as rdk

from uprising_util import StrokeError
import robo


class Target(object):
    def __init__(self, id_, position, rotation, brush):
        self.id = id_
        self.linear = True

        self.joint_pose = None
        self.tool_pose = rdk.TxyzRxyz_2_Pose(list(position) + list(rotation))
        self.flange_pose = self.tool_pose * brush.matrix.invH()

        self.joint_poses = robo.config_poses(self.flange_pose)

    def valid_configs(self):
        result = []
        for k in self.joint_poses:
            if self.joint_poses[k]:
                result.append(k)
        return result

    def configure(self, config):
        self.joint_pose = self.joint_poses[config][0]

    def name(self, prefix):
        return "{}_t{}".format(prefix, self.id)

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


class ArrivalTarget(Target):
    def name(self, prefix):
        return "{}_ar{}".format(prefix, self.id)

    def configure(self, config):
        self.joint_pose = self.joint_poses[config][0]
        self.linear = False


class DepartureTarget(Target):
    def name(self, prefix):
        return "{}_dp{}".format(prefix, self.id)
