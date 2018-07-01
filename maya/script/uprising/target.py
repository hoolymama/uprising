import robodk as rdk

from uprising_util import StrokeError
from robolink import (Robolink, COLLISION_ON, COLLISION_OFF, ITEM_TYPE_ROBOT)


class Target(object):

    @staticmethod
    def config_key(config):
        if config:
            return "%d%d%d" % tuple(config.list2()[0][0:3])

    def __init__(self, id_, position, rotation, robot, brush, mask="00"):
        self.id = id_
        self.linear = False
        self.position = position
        self.rotation = rotation
        self.joint_pose = None
        self.configs = {}

        self.tool_pose = rdk.TxyzRxyz_2_Pose(list(position) + list(rotation))
        self.flange_pose = self.tool_pose * brush.matrix.invH()
        ik = robot.SolveIK_All(self.flange_pose)
        siz = ik.size()
   
        if not (ik and  siz[0] and  siz[1] and (len(ik.list()) > 5)):
            raise StrokeError("Cant initialize target: %d, no ik solutions exist" % self.id)
        joint_poses = [el[0:6] for el in ik.list2()]
        for pose in joint_poses:
            key = Target.config_key(robot.JointsConfig(pose))
            if key.startswith(mask):
                self.configs.setdefault(key, []).append(pose)
        if not self.configs:
            raise StrokeError("Cant initialize target: %d, no ik solutions in mask %s " % (self.id, mask))


    def configure(self, robot, last_mat, ref_joint_pose, allow_flip):

        self.linear = False
        ik = robot.SolveIK(self.flange_pose, ref_joint_pose)
        if len(ik.list()) < 6:
            raise StrokeError("Can't configure. No IK solution")
        self.joint_pose = ik.list2()[0]

        # if stroke started, test to see if we can easily get to this pose
        if self.id > 0:
            self.linear = True
            # check the two joint poses to see if it flips
            if not allow_flip:
                this_cfg = Target.config_key(
                    robot.JointsConfig(self.joint_pose))
                ref_cfg = Target.config_key(robot.JointsConfig(ref_joint_pose))
                if this_cfg != ref_cfg:
                    raise StrokeError(
                        "Can't create stroke %d when flips not allowed" %
                        self.id)

            can_move_lin = robot.MoveL_Test(self.joint_pose, last_mat)
            if can_move_lin == 0:
                return
 
            if robot.MoveJ_Test(self.joint_pose, ref_joint_pose) == 0:
                return
 
            self.linear = False


    # def configure(self, robot, last_mat, ref_joint_pose, allow_flip):

    #     # self.joint_pose = None
    #     self.linear = False
    #     ik = robot.SolveIK(self.flange_pose, ref_joint_pose)
    #     if len(ik.list()) < 6:
    #         raise StrokeError("Can't configure. No IK solution")
    #     self.joint_pose = ik.list2()[0]

    #     # if stroke started, test to see if we can easily get to this pose
    #     if self.id > 0:
    #         self.linear = True
    #         # check the two joint poses to see if it flips
            
    #         flips = self.does_flip(robot, ref_joint_pose)
    #         if flips and not allow_flip:
    #             raise StrokeError(
    #                     "Can't create stroke %d when flips not allowed" %
    #                     self.id)

    #         if robot.MoveJ_Test(self.joint_pose, ref_joint_pose) != 0:
    #             raise StrokeError("Can't perform move %d" % self.id)

    #         if flips:
    #             self.linear = False

    # def does_flip(self, robot, ref_pose):
    #     this_cfg = Target.config_key(robot.JointsConfig(self.joint_pose))
    #     ref_cfg = Target.config_key(robot.JointsConfig(ref_pose))
    #     return this_cfg != ref_cfg





        # print "---------------- init %d" % self.id
        # print "self.tool_pose isHomogeneous %s" % self.tool_pose.isHomogeneous()
        # print "self.flange_pose isHomogeneous %s" % self.flange_pose.isHomogeneous()


# configs = {}

# if configs:
#     print "hi"


# RL = Robolink()

# robot = RL.Item('', ITEM_TYPE_ROBOT)


# move_L test failed for 2
# [-39.30594875706214, -38.66247694597341, 108.18758516558246, 179.32396227118153, -20.356207178616316, -116.39837351112735]
# [[ 0.213, -0.977, -0.001, 1143.922 ],
#  [ -0.977, -0.213, 0.005, 888.584 ],
#  [ -0.005, 0.000, -1.000, 283.414 ],
#  [ 0.000, 0.000, 0.000, 1.000 ]]

# [1143.9217535233195, 888.5835382407311, 283.4138530502215, -3.137090304364405, -0.000979527245828814, 1.3565768194958825]

# joints = [-39.30594875706214, -38.66247694597341, 108.18758516558246,
#           179.32396227118153, -20.356207178616316, -116.39837351112735]
# mat = rdk.TxyzRxyz_2_Pose([1143.9217535233195, 888.5835382407311, 283.4138530502215, -
#                            3.137090304364405, -0.000979527245828814, 1.3565768194958825])



# can_move_lin = robot.MoveL_Test(joints, mat)
# print "can %d" % can_move_lin

# target = RL.AddTarget(tname, parent_frame, robot)

# # move_L test failed
# joints = [-40.49560526769006, -38.72402277013895, 108.32979275382006,
#           179.3285671856602, -20.270666795959063, -117.59200664937714]


# rows = [[0.213, -0.977, -0.001, 1099.258],
#         [-0.977, -0.213, 0.005, 921.081],
#         [-0.005, 0.000, -1.000, 253.604],
#         [0.000, 0.000, 0.000, 1.000]]

# mat = rdk.Mat(rows)
# print mat
# print mat.isHomogeneous()


# j = robot.Joints()

# print robot.JointsConfig(j)


# try:
#     raise StrokeError("foo")
#     print "hello"
# except StrokeError:
#     raise StrokeError("bar")
#     print "E1"
# except StrokeError:
#     print "E2"
# except StrokeError:
#     print "E3"
