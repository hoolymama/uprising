"""
Solve the configuration and then the joint poses for a stroke. 

The targets, the last arrival and the departure must all have the same configuration.

"""

from uprising.utils import StrokeError
from uprising import robo
from uprising import const


PAINTING_START_JOINTS = [0, -90, 120, 0, -30, 0]

EPSILON = 0.0001


def solve(stroke, brush):

    for config in ["000", "001"]:
        solved_config = config
        last_joint_pose = const.PAINTING_START_JOINTS
        for target in [stroke.arrivals[-1]] + stroke.targets + [stroke.departure]:

            flange_pose = curr_target.flange_pose(brush)
            pose = solve_pose(flange_pose, config, last_joint_pose)

            if not pose:
                solved_config = False
                break
            last_joint_pose = pose
            target.joint_pose = pose

        if solved_config:
            break

    if not solved_config:
        raise StrokeError("Stroke not solvable: {}".format(stroke.name("-")))

    configs = ["000", "001"] if solved_config == "000" else ["001", "000"]

    last_joint_pose = stroke.arrivals[-1].joint_pose
    for target in reversed(stroke.arrivals[:-1]):
        solved_config = False
        for config in configs:
            flange_pose = target.flange_pose(brush)
            pose = solve_pose(flange_pose, config, last_joint_pose)
            if pose:
                target.joint_pose = pose
                last_joint_pose = pose
                solved_config = config
                break
        if not solved_config:
            raise StrokeError("Stroke arrival not solvable: {}".format(stroke.name("-")))

    for target in stroke.arrivals:
        target.linear = False
    for target in stroke.targets + [stroke.departure]:
        target.linear = True


def solve_pose(flange_pose, config, last_joint_pose):
    joint_pose = solve_single_joint_pose(flange_pose, last_joint_pose, config)
    if joint_pose:
        return joint_pose

    joint_poses = solve_all_joint_poses(flange_pose, config)
    if not joint_poses:
        return

    # Now we find which is closest to the last pose
    joint_pose = get_closest_pose(joint_poses, last_joint_pose)
    return joint_pose


def get_closest_pose(joint_poses, to_pose):
    differences = []
    tp = to_pose
    for jp in joint_poses:
        differences.append([abs(jp[i] - tp[i]) for i in range(6)])

    combined = zip(joint_poses, differences)
    # print combined

    combined.sort(cmp=compare)
    return combined[0][0]


def compare(a, b):

    """
    Most probably, J1, J2, J3 will be the same. Only J4,J5,J6 differ.
    """
    for i in range(6):
        d = a[1][i] - b[1][i]
        if abs(d) < EPSILON:
            continue
        if d < 0:
            return -1
        elif d > 0:
            return 1
    return 0


def config_key(config):
    if config:
        return "{}{}{}".format(tuple(config.list2()[0][0:3]))


def solve_all_joint_poses(pose, config):
    robot = robo.robot()
    ik = robot.SolveIK_All(pose)
    siz = ik.size()

    if not (ik and siz[0] and siz[1] and (len(ik.list()) > 5)):
        return []

    result = []
    joint_poses = [el[0:6] for el in ik.list2()]
    for joint_pose in joint_poses:
        key = config_key(robot.JointsConfig(joint_pose))
        if key == config:
            result.append(joint_pose)
    return result


def solve_single_joint_pose(flange_pose, last_joints_pose, config):
    robot = robo.robot()
    # result = copy.deepcopy(CONFIGS)

    joint_pose = robot.SolveIK(flange_pose, last_joints_pose)
    joint_pose = joint_pose.list()
    if len(joint_pose) < 6:
        return

    key = config_key(robot.JointsConfig(joints_pose))
    if key == config:
        return joint_pose


# to_pose = [22.188563, -57.217051, 117.247278, -334.966991, -62.145943, 240.042379]

# joint_poses = [
# [20.423403, -51.990616, 107.482965, 24.137242, -57.627862, -121.149523],
# [20.423403, -51.990616, 107.482965, -335.862758, -57.627862, -121.149523],
# [20.423403, -51.990616, 107.482965, 24.137242, -57.627862, 238.850477],
# [20.423403, -51.990616, 107.482965, -335.862758, -57.627862, 238.850477]
# ]


#     curr_target = stroke.arrivals[-1]
#     solution = solve_single_joint_pose(flange_pose, const.PAINTING_START_JOINTS)
#     if solution:
#         current_config, joint_pose = solution
#         curr_target.joint_pose = joint_pose
#         for t in stroke.targets + [stroke.departure]:
#             flange_pose = t.flange_pose(self.brush)
#             solution = solve_single_joint_pose(flange_pose, joint_pose)
#             if solution:

# prev_target = None

# # start with the last arrival
# last_target

# for target in stroke.arrivals + stroke.targets + [stroke.departure]:
#     target.solve_single_joint_poses(self.brush, prev_target)
#     prev_target = target

# try:
#     self.config = self.best_config()
#     solved = True


# def initialize_solution(self):
#     solution = {
#         "arrivals": [],
#         "targets": [],
#         "departures": []
#     }
#     for t in stroke.arrivals:
#         self.solution["arrivals"].append({"target": t, "configs":None})
#     for t in stroke.targets:
#         self.solution["targets"].append({"target": t, "configs":None})
#     self.solution["departure"] = {"target": stroke.departure, "configs":None}
#     return solution
