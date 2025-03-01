"""
Solve the configuration and then the joint poses for a stroke. 

The targets must all have the same configuration.
"""

from uprising.utils import StrokeError
from uprising import robo
from uprising import const
import functools


PAINTING_START_JOINTS = [0, -90, 120, 0, -30, 0]

EPSILON = 0.0001

def solve(stroke, brush):

    for config in ["001", "000"]:
        solved_config = config
        last_joint_pose = PAINTING_START_JOINTS

        for target in stroke.targets :
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
    combined.sort(key=functools.cmp_to_key(compare))
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
    key = config_key(robot.JointsConfig(joint_pose))
    if key == config:
        return joint_pose


 

