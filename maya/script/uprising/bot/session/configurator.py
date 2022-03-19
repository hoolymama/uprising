"""
Solve the configuration and then the joint poses for a stroke. 

The targets, the last arrival and the departure must all have the same configuration.
"""

from uprising.utils import StrokeError
from uprising import robo
from uprising import const

PAINTING_START_JOINTS = [0, -90, 120, 0, -30, 0]

PAINTING_START_JOINTS = [10.000000, -70.000000, 120.000000, 0.000000, -30.000000, 0.000000]

EPSILON = 0.0001

def solve(stroke, brush):

    for config in ["000", "001"]:
        solved_config = config
        last_joint_pose = PAINTING_START_JOINTS

        for target in [stroke.arrivals[-1]] + stroke.targets + [stroke.departure]:
            flange_pose = target.flange_pose(brush)
            pose = _solve_pose(flange_pose, config, last_joint_pose)
            if not pose:
                solved_config = False
                break
            last_joint_pose = pose
            target.joint_pose = pose

        if solved_config:
            break
    if not solved_config:
        raise StrokeError("Stroke not solvable: {}".format(stroke.name("-")))

    main_solved_config = solved_config
    configs = ["000", "001"] if solved_config == "000" else ["001", "000"]

    arrival_configs = set()
    last_joint_pose = stroke.arrivals[-1].joint_pose
    for target in reversed(stroke.arrivals[:-1]):
        solved_config = False
        for config in configs:
            flange_pose = target.flange_pose(brush)
            pose = _solve_pose(flange_pose, config, last_joint_pose)
            if pose:
                target.joint_pose = pose
                last_joint_pose = pose
                solved_config = config
                break
        arrival_configs.add(solved_config)
        if not solved_config:
            raise StrokeError("Stroke arrival not solvable: {}".format(stroke.name("-")))

    # print "main_solved_config",stroke.name("-"), main_solved_config, "arr:", arrival_configs

def _solve_pose(flange_pose, config, last_joint_pose):  
    joint_pose = _solve_single_joint_pose(flange_pose, last_joint_pose, config)
    if joint_pose:
        return joint_pose

    joint_poses = _solve_all_joint_poses(flange_pose, config)
    if not joint_poses:
        return

    # Now we find which is closest to the last pose
    joint_pose = _get_closest_pose(joint_poses, last_joint_pose)
    return joint_pose


def _get_closest_pose(joint_poses, to_pose):
    differences = []
    tp = to_pose
    for jp in joint_poses:
        differences.append([abs(jp[i] - tp[i]) for i in range(6)])

    combined = list(zip(joint_poses, differences))
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
        return "%d%d%d" % tuple(config.list2()[0][0:3])


def _solve_all_joint_poses(pose, config):
    robot = robo.robot()
    ik = robot.SolveIK_All(pose)
    siz = ik.size()

    if not (ik and siz[0] and siz[1] and (len(ik.list()) > 5)):
        return []

    result = []
    joint_poses = [el[0:6] for el in ik.list2()]
    for joint_pose in joint_poses:
        jcfg = robot.JointsConfig(joint_pose)
        key = config_key(jcfg)
        if key == config:
            result.append(joint_pose)
    return result


def _solve_single_joint_pose(flange_pose, last_joint_pose, config):
    robot = robo.robot()

    joint_pose = robot.SolveIK(flange_pose, last_joint_pose)
    joint_pose = joint_pose.list()
    if len(joint_pose) < 6:
        return
    jcfg = robot.JointsConfig(joint_pose)
    # print joint_pose, jcfg
    key = config_key(jcfg)
    if key == config:
        return joint_pose



