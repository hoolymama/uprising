## TARGET

    def configure(self, robot, last_mat, ref_joint_pose, allow_flip):

        self.linear = False
        ik = robot.SolveIK(self.flange_pose, ref_joint_pose)
        if len(ik.list()) < 6:
            raise StrokeError("Can't configure. No IK solution")
        self.joint_pose = ik.list2()[0]

        cfg = Target.config_key(robot.JointsConfig(self.joint_pose))
        if not cfg.startswith(CONFIG_MASK):
            raise StrokeError("Can't create stroke with elbow down %d" %  self.id) 

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
 

    # ALTERNATIVE      
    def configure(self, robot, last_mat, ref_joint_pose, config):
 
        self.joint_pose =  self.configs[config][0]
 

        self.linear = bool(last_mat)  
 




    def configure(self, robot, last_mat, ref_joint_pose, allow_flip):

        # self.joint_pose = None
        self.linear = False
        ik = robot.SolveIK(self.flange_pose, ref_joint_pose)
        if len(ik.list()) < 6:
            raise StrokeError("Can't configure. No IK solution")
        self.joint_pose = ik.list2()[0]

        # if stroke started, test to see if we can easily get to this pose
        if self.id > 0:
            self.linear = True
            # check the two joint poses to see if it flips
            
            flips = self.does_flip(robot, ref_joint_pose)
            if flips and not allow_flip:
                raise StrokeError(
                        "Can't create stroke %d when flips not allowed" %
                        self.id)

            if robot.MoveJ_Test(self.joint_pose, ref_joint_pose) != 0:
                raise StrokeError("Can't perform move %d" % self.id)

            if flips:
                self.linear = False

    def does_flip(self, robot, ref_pose):
        this_cfg = Target.config_key(robot.JointsConfig(self.joint_pose))
        ref_cfg = Target.config_key(robot.JointsConfig(ref_pose))
        return this_cfg != ref_cfg





        print "---------------- init %d" % self.id
        print "self.tool_pose isHomogeneous %s" % self.tool_pose.isHomogeneous()
        print "self.flange_pose isHomogeneous %s" % self.flange_pose.isHomogeneous()


configs = {}

if configs:
    print "hi"


RL = Robolink()

robot = RL.Item('', ITEM_TYPE_ROBOT)

 

can_move_lin = robot.MoveL_Test(joints, mat)
print "can %d" % can_move_lin

target = RL.AddTarget(tname, parent_frame, robot)

# move_L test failed
joints = [-40.49560526769006, -38.72402277013895, 108.32979275382006,
          179.3285671856602, -20.270666795959063, -117.59200664937714]


rows = [[0.213, -0.977, -0.001, 1099.258],
        [-0.977, -0.213, 0.005, 921.081],
        [-0.005, 0.000, -1.000, 253.604],
        [0.000, 0.000, 0.000, 1.000]]

mat = rdk.Mat(rows)
print mat
print mat.isHomogeneous()


j = robot.Joints()

print robot.JointsConfig(j)


try:
    raise StrokeError("foo")
    print "hello"
except StrokeError:
    raise StrokeError("bar")
    print "E1"
except StrokeError:
    print "E2"
except StrokeError:
    print "E3"
