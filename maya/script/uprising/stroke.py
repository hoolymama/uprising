import robodk as rdk


class StrokeError(Exception):
    pass


def config_key(config):
    if config:
        return "%d%d%d%d" % tuple(config.list2()[0][0:4])


class Stroke(object):

    def __init__(self, positions, rotations, arc_length, approach, curve_name, curve_stroke_id):
        # self.positions = positions
        # self.rotations = rotations
        self.arc_length = arc_length
        self.valid_configs = []
        self.valid_config = None
        self.targets = []
        self.curve_name = curve_name
        self.curve_stroke_id = curve_stroke_id

        self.positions = [(positions[0] + approach)] + \
            positions + [(positions[-1] + approach)]
        self.rotations = [rotations[0]] + \
            rotations + [rotations[-1]]

    def identifier(self):
        return "%s_%d" % (self.curve_name,self.curve_stroke_id) 

    def configure(self, robot, brush, last_joint_pose):

        # self.add_approach_positions(approach)
        # strategy:
        # see if we can do this stroke by continuing from the 
        # closest pose of the last stroke in the cluster
        try:
            self.configure_with_continuation(robot, brush, last_joint_pose)
        except IndexError:
            self.configure_with_search(robot, brush)
        # self.configure_with_search(robot, brush)

    def configure_with_continuation(self,  robot, brush, last_joint_pose):
        last_pose = last_joint_pose
        last_mat = None
        for i in range(len(self.positions)):
            target = self.generate_continuation_target(
                self.positions[i], self.rotations[i], robot, brush, last_pose, last_mat)

            self.targets.append(target)
            last_pose = target["valid_joint_pose"]
            last_mat = target["tool_pose"]

            



    def generate_continuation_target(self, p, r, robot, brush, last_joint_pose, last_mat):
        target = {
            # "valid": False,
            "tool_pose": None,
            "flange_pose": None,
            "configs": {}
        }
        mat = rdk.TxyzRxyz_2_Pose(
            [p.x * 10, p.y * 10, p.z * 10, r.x, r.y, r.z])
        target["tool_pose"] = mat
        target["flange_pose"] = mat * rdk.transl(brush.tcp).invH()
        ik = robot.SolveIK(target["flange_pose"], last_joint_pose)
        if len(ik.list()) < 6:
            raise IndexError("Cant solve IK for continuation")
        joints = ik.list2()[0]
        print "last_mat %s" % last_mat
        print "last_joint_pose %s" % last_joint_pose
        
        if last_mat and (robot.MoveL_Test(joints, last_mat) != 0):
            raise IndexError("Cant solve IK for continuation")
        target["valid_joint_pose"] = joints
        return target
        # target["valid"] = True


    def configure_with_search(self,  robot, brush):

        for i in range(len(self.positions)):
            target = self.generate_target(
                self.positions[i], self.rotations[i], robot, brush)
            self.targets.append(target)

        # now we have all valid joint_poses for all configs
        # loop sort and loop through to find the configs that
        # exist for all targets
        configs = sorted(self.targets[0]["configs"].keys())
        # configs =  self.targets[0]["configs"].keys()
        for config in configs:
            valid = True
            for target in self.targets:
                if config not in target["configs"]:
                    valid = False
                    break
            if valid:
                self.valid_configs.append(config)

        if not self.valid_configs:
            raise StrokeError(
                "Cant find any common configs")

        valid_config, joint_poses = self.search_continuous_joint_poses(robot)
        if not valid_config:
            raise StrokeError(
                "Cant find a continuous stroke sequence")
        self.valid_config = valid_config
        for i, target in enumerate(self.targets):
            target["valid_joint_pose"] = joint_poses[i]


        

    def generate_target(self, p, r, robot, brush):
        target = {
            # "valid": True,
            "tool_pose": None,
            "flange_pose": None,
            "configs": {}
        }
        mat = rdk.TxyzRxyz_2_Pose(
            [p.x * 10, p.y * 10, p.z * 10, r.x, r.y, r.z])
        target["tool_pose"] = mat
        target["flange_pose"] = mat * rdk.transl(brush.tcp).invH()

        ik = robot.SolveIK_All(target["flange_pose"])
        if len(ik.list()) < 6:
            raise IndexError("Cant solve IK")

        jointPoses = [el[0:6] for el in ik.list2()]
        for jointPose in jointPoses:
            config = config_key(robot.JointsConfig(jointPose))
            target["configs"].setdefault(config, []).append(jointPose)
        return target


    def search_continuous_joint_poses(self, robot):
        for config in self.valid_configs:
            result = self.search_continuous_joint_poses_for(config, robot)
            if result:
                return (config, result)
        return (None, None)

    # try a version where we only test the first pose of each config
    def search_continuous_joint_poses_for(self, config, robot):
        result = []
        last_mat = None
        for target in self.targets:

            joint_pose = target["configs"][config][0]  
            if not last_mat:
                result.append(joint_pose)
            else:
                if robot.MoveL_Test(joint_pose, last_mat) == 0:
                    result.append(joint_pose)
                else:
                    # return None because config cant be continuous
                    return None
            last_mat = target["tool_pose"]
        return result



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

