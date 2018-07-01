import robodk as rdk

from target import Target

from uprising_util import StrokeError


def config_key(config):
    if config:
        return "%d%d%d%d" % tuple(config.list2()[0][0:4])


class Stroke(object):

    def __init__(
            self,
            robot,
            positions,
            rotations,
            brush,
            paint,
            arc_length,
            approach,
            curve_name,
            curve_stroke_id):
        self.arc_length = arc_length
        self.common_configs = []
        self.valid_config = None
        self.targets = []
        self.curve_name = curve_name
        self.curve_stroke_id = curve_stroke_id

        ps = [(positions[0] + approach)] + \
            positions + [(positions[-1] + approach)]
        rs = [rotations[0]] + \
            rotations + [rotations[-1]]

        for i in range(len(ps)):
            # will throw if no solutions
            t = Target(i, (ps[i] * 10), rs[i], robot, brush)
            self.targets.append(t)

    def identifier(self):
        return "%s_%d" % (self.curve_name, self.curve_stroke_id)

    def configure(self, robot, brush, ref_joint_pose):

        # print "Stroke.configure() ------------------------------- %s" % self.identifier()
        self.common_configs = self.get_common_configs()
        if not self.common_configs:
            raise StrokeError("Cant find any common configs")

        found = False
        # try the basic ref pose
        # print "1. try the basic ref pose - no flip"
        found = self.configure_targets(
            robot, ref_joint_pose, allow_flip=False)

        # try start by looping through configurations

        print self.common_configs
        if not found:
            # print "2. try all configs - no flip"
            for config in self.common_configs:
                print "Try %s " % config
                joint_pose = self.targets[0].configs[config][0]
                found = self.configure_targets(
                    robot, joint_pose, allow_flip=False)
                if found:
                    break

        # try global ref pose and allow flips
        # print "3. try the basic ref pose - with flip"

        if not found:
            found = self.configure_targets(
                robot, ref_joint_pose, allow_flip=True)

        # try start by looping through configurations

        if not found:
            # print "4. try all configs - with flip"
            for config in self.common_configs:
                joint_pose = self.targets[0].configs[config][0]
                found = self.configure_targets(
                    robot, joint_pose, allow_flip=True)
                if found:
                    break

        if not found:
            raise StrokeError("Cannot find any stroke solution %s" % self.identifier())


        
        # print "------------FINAL STROKE DETAILS--------------"
        # for t in self.targets:
        #     print "t.id -----------------------------------%s" % t.id
        #     print "t.linear %s " % t.linear
        #     print "t.position %s" % t.position
        #     print "t.rotation %s" % t.rotation
        #     print "t.tool_pose %s" % t.tool_pose
        #     print "t.flange_pose %s" % t.flange_pose
        #     print "t.joint_pose %s" % t.joint_pose
 

        # try:
        #     self.configure_all_linear(robot, last_joint_pose)
        # except StrokeError:
        #     print "CANT configure_with_single_config"

        # self.configure_by_common_configs(robot, brush)

    def configure_targets(self, robot, ref_pose, **kw):
        allow_flip = kw["allow_flip"]
        last_pose = ref_pose
        last_mat = None
        for target in self.targets:
            try:
                target.configure(robot, last_mat, last_pose, allow_flip)
            except StrokeError:
                return False
            last_pose = target.joint_pose
            last_mat = target.tool_pose
        return True

    # def configure_with_last_pose(self, robot, last_joint_pose):
    #     last_pose = last_joint_pose
    #     last_mat = None
    #     for target in self.targets:
    #         target.configure(robot, last_pose, last_mat)
    #         last_pose = target.joint_pose
    #         last_mat = target.tool_pose

    def get_common_configs(self):
        result = []
        keys = self.targets[0].configs.keys()
        for key in keys:
            valid = True
            for target in self.targets:
                if key not in target.configs:
                    valid = False
                    break
            if valid:
                result.append(key)
        return result

    # def configure_by_common_configs(self, robot):
    #     for target in self.targets:
    #         target.prepare_for_search(robot)
    #     mask = "00"
    #     self.valid_configs = self.get_common_configs(mask)

    #     if not self.valid_configs:
    #         raise StrokeError("Cant find any common configs with mask" % mask)

    #     for config in self.valid_configs:

    #         result = self.search_continuous_joint_poses_for(config, robot)

    #     #     if result:
    #     #         return (config, result)
    #     # return (None, None)

    #     valid_config, joint_poses = self.search_continuous_joint_poses(robot)
    #     if not valid_config:
    #         raise StrokeError(
    #             "Cant find a continuous stroke sequence")
    #     self.valid_config = valid_config
    #     for i, target in enumerate(self.targets):
    #         target["valid_joint_pose"] = joint_poses[i]

    # def search_continuous_joint_poses_for(self, config, robot):
    #     first_target = self.targets[0]

    #     last_pose = first_target.configs[config]
    #     last_mat = None
    #     for target in self.targets:
    #         last_joint_pose = targets

    # try a version where we test the first pose of each config
    # def search_continuous_joint_poses_for(self, config, robot):
    #     result = []
    #     last_mat = None
    #     last_joint_pose = None
    #     for target in self.targets:

    #         joint_pose = target["configs"][config][0]
    #         if not last_mat:
    #             result.append(joint_pose)
    #         else:
    #             if robot.MoveL_Test(joint_pose, last_mat) == 0:
    #                 print "searching: Can do linear"
    #                 target["linear"] = True
    #                 result.append(joint_pose)
    #             else:

    #                 # solve IK to get the best next joints
    #                 ik = robot.SolveIK(target["flange_pose"], last_joint_pose)
    #                 if len(ik.list()) > 5:
    #                     # overwrite joint pose
    #                     joint_pose = ik.list2()[0]
    #                     if robot.MoveJ_Test(joint_pose, last_joint_pose) == 0:
    #                         print "searching: Cant do linear but can do joint"
    #                         result.append(joint_pose)
    #                         target["linear"] = False
    #                     else:
    #                         print "searching: Cant do linear or valid joints - try next config"
    #                         # return None because config cant be continuous
    #                         return None
    #                 else:  # no solution
    #                     print "searching: Cant do linear or find any joints - try next config"
    #                     # return None because config cant be continuous
    #                     return None

    #         last_joint_pose = joint_pose
    #         last_mat = target["tool_pose"]
    #     return result

    # def search_continuous_joint_poses(self, robot):
    #     for config in self.valid_configs:
    #         result = self.search_continuous_joint_poses_for(config, robot)
    #         if result:
    #             return (config, result)
    #     return (None, None)

        # # self.targets = []
        # for i in range(len(self.positions)):
        #     target = self.generate_search_target(
        #         self.positions[i], self.rotations[i], robot, i)
        #     self.targets.append(target)

        # now we have all valid joint_poses for all configs
        # loop sort and loop through to find the configs that
        # exist for all targets
        # configs = sorted(self.targets[0]["configs"].keys())

        #configs = [k for k in self.targets[0]["configs"].keys() if k.startswith("00")]

        # configs = [k for k in self.targets[0]["configs"].keys()]
        # print configs
        # print "xxxxxxxxxxxxxxxxx"
        # for target in self.targets:
        #     print sorted(target["configs"].keys())
        #     if not len(target["configs"]):
        #         print target

        #configs = self.targets[0]["configs"].keys()
        # configs =  self.targets[0]["configs"].keys()

    # try a version where we test the first pose of each config
    # def search_continuous_joint_poses_for(self, config, robot):
    #     result = []
    #     last_mat = None
    #     last_joint_pose = None
    #     for target in self.targets:

    #         joint_pose = target["configs"][config][0]
    #         if not last_mat:
    #             result.append(joint_pose)
    #         else:
    #             if robot.MoveL_Test(joint_pose, last_mat) == 0:
    #                 print "searching: Can do linear"
    #                 target["linear"] = True
    #                 result.append(joint_pose)
    #             else:

    #                 # solve IK to get the best next joints
    #                 ik = robot.SolveIK(target["flange_pose"], last_joint_pose)
    #                 if len(ik.list()) > 5:
    #                     # overwrite joint pose
    #                     joint_pose = ik.list2()[0]
    #                     if robot.MoveJ_Test(joint_pose, last_joint_pose) == 0:
    #                         print "searching: Cant do linear but can do joint"
    #                         result.append(joint_pose)
    #                         target["linear"] = False
    #                     else:
    #                         print "searching: Cant do linear or valid joints - try next config"
    #                         # return None because config cant be continuous
    #                         return None
    #                 else:  # no solution
    #                     print "searching: Cant do linear or find any joints - try next config"
    #                     # return None because config cant be continuous
    #                     return None

    #         last_joint_pose = joint_pose
    #         last_mat = target["tool_pose"]
    #     return result

    # try a version where we test the first pose of each config
    # def search_continuous_joint_poses_for(self, config, robot):
    #     result = []
    #     last_mat = None
    #     last_joint_pose = None
    #     for target in self.targets:

    #         joint_pose = target["configs"][config][0]
    #         if not last_mat:
    #             result.append(joint_pose)
    #         else:
    #             if robot.MoveL_Test(joint_pose, last_mat) == 0:
    #                 print "searching: Can do linear"
    #                 result.append(joint_pose)
    #             elif robot.MoveJ_Test(joint_pose, last_joint_pose) == 0:
    #                 print "searching: Cant do linear but can do joint"
    #                 result.append(joint_pose)
    #                 target["linear"] = False
    #             else:
    #                 print "searching: Cant do linear or joint - try next config"
    #                 # return None because config cant be continuous
    #                 return None

    #         last_joint_pose = joint_pose
    #         last_mat = target["tool_pose"]
    #     return result

    # def generate_search_target(self, p, r, robot, brush, index):
    #     target = {
    #         "linear": (index > 0),
    #         # "valid": True,
    #         "tool_pose": None,
    #         "flange_pose": None,
    #         "configs": {}
    #     }
    #     mat = rdk.TxyzRxyz_2_Pose(
    #         [p.x * 10, p.y * 10, p.z * 10, r.x, r.y, r.z])
    #     target["tool_pose"] = mat
    #     target["flange_pose"] = mat * rdk.transl(brush.tcp).invH()

    #     ik = robot.SolveIK_All(target["flange_pose"])
    #     if not ik or (len(ik.list()) < 6):
    #         print "THERE ARE NO CONFIGS"
    #         raise IndexError("Cant solve IK")

    #     jointPoses = [el[0:6] for el in ik.list2()]
    #     print "LEN JOINT POSES: %s" % len(jointPoses)
    #     for jointPose in jointPoses:
    #         cfg_arr = robot.JointsConfig(jointPose)
    #         # print "cfg_arr: %s" % cfg_arr
    #         config_k = config_key(cfg_arr)
    #         # print "config_k %s" % config_k
    #         target["configs"].setdefault(config_k, []).append(jointPose)

    #     # print target
    #     print target["configs"]
    #     # print "KKK: %s " % target["configs"].keys()
    #     # print "-------"
    #     return target

    # def generate_continuation_target(
    #         self,
    #         p,
    #         r,
    #         robot,
    #         brush,
    #         last_joint_pose,
    #         last_mat):

    #     target = {
    #         "linear": bool(last_mat),
    #         "tool_pose": None,
    #         "flange_pose": None,
    #         "p": p * 10,
    #         "r": r,
    #         "configs": {}
    #     }
    #     mat = rdk.TxyzRxyz_2_Pose([target["p"].x,
    #                                target["p"].y,
    #                                target["p"].z,
    #                                target["r"].x,
    #                                target["r"].y,
    #                                target["r"].z])
    #     target["tool_pose"] = mat
    #     target["flange_pose"] = mat * rdk.transl(brush.tcp).invH()

    #     ik = robot.SolveIK(target["flange_pose"], last_joint_pose)

    #     print "HERE"

    #     if len(ik.list()) < 6:
    #         raise IndexError("Cant solve IK for continuation")

    #     print "HERE 2"
    #     joint_pose = ik.list2()[0]
    #     # print "last_mat %s" % last_mat
    #     # print "last_joint_pose %s" % last_joint_pose
    #     # print "last_mat: %s " % last_mat
    #     print joint_pose
    #     print "Testing"
    #     if not last_mat:  # first go round
    #         target["linear"] = False
    #         print "not last_mat so okay"
    #     elif robot.MoveL_Test(joint_pose, last_mat) == 0:
    #         print "MoveL_Test okay"
    #         target["linear"] = True
    #     elif robot.MoveJ_Test(joint_pose, last_joint_pose) == 0:
    #         print "MoveJ_Test okay"
    #         target["linear"] = False
    #     else:
    #         target["linear"] = True
    #         print "Cant do anything"
    #         # raise IndexError("Cant solve IK for continuation")

    #     target["valid_joint_pose"] = joint_pose
    #     # target["valid"] = True
    #     # print "target[linear] %s" %  target["linear"]
    #     return target

    # try a version where we  testall poses
    # def search_continuous_joint_poses_for(self, config, robot):
    #     result = []
    #     last_mat = None
    #     for target in self.targets:

    #         joint_pose = target["configs"][config][0]
    #         joint_poses = target["configs"][config]
    #         if not last_mat:
    #             result.append(joint_pose)
    #         else:
    #             found = None
    #             for p in joint_poses:
    #                 if robot.MoveL_Test(p, last_mat) == 0:
    #                     found = p

    #             if found:
    #                 result.append(found)
    #             else:
    #                 return None

    #             # if robot.MoveL_Test(joint_pose, last_mat) == 0:
    #             #     result.append(joint_pose)
    #             # else:
    #             #     # return None because config cant be continuous
    #             #     return None

    #         last_mat = target["tool_pose"]
    #     return result

    # def generate_dumb_target(self, p, r, robot, brush):
    #     target = {
    #         "valid": True,
    #         "tool_pose": None,
    #         "flange_pose": None,
    #         "configs": {}
    #     }
    #     mat = rdk.TxyzRxyz_2_Pose(
    #         [p.x * 10, p.y * 10, p.z * 10, r.x, r.y, r.z])
    #     target["tool_pose"] = mat
    #     # target["flange_pose"] = mat * rdk.transl(brush.tcp).invH()

    #     # ik = robot.SolveIK_All(target["flange_pose"])
    #     # if len(ik.list()) < 6:
    #     #     raise IndexError("Cant solve IK")

    #     # jointPoses = [el[0:6] for el in ik.list2()]
    #     # for jointPose in jointPoses:
    #     #     config = config_key(robot.JointsConfig(jointPose))
    #     #     target["configs"].setdefault(config, []).append(jointPose)
    #     return target

    # def search_continuous_joint_poses_for(self, config, robot):
    #     result = []
    #     last_mat = None
    #     for target in self.targets:
    #         joint_poses = target["configs"][config]  # array
    #         if not last_mat:
    #             result.append(joint_poses[0])
    #         else:
    #             found = None
    #             for joint_pose in joint_poses:
    #                 if robot.MoveL_Test(joint_pose, last_mat) == 0:
    #                     found = joint_pose
    #                     break
    #             if found:
    #                 result.append(found)
    #             else:
    #                 return
    #         last_mat = target["tool_pose"]
    #     return result
