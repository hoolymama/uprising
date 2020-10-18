
"""
Module as a singleton that stores the robot and the link.
"""

import os
import time
from uprising import uprising_util as uutl


from robolink import (ITEM_TYPE_PROGRAM, ITEM_TYPE_ROBOT, ITEM_TYPE_STATION,
                      ITEM_TYPE_TARGET, Robolink)

import pymel.core as pm
import robodk as rdk

UPRISING_PROJECT_PATH = os.path.dirname(
    pm.getModulePath(moduleName="uprising"))
CLEAN_FILE = os.path.join(UPRISING_PROJECT_PATH, "robodk", "clean.rdk")
LICENSE_FILE = os.path.join(
    UPRISING_PROJECT_PATH, "robodk", "RobAndNickCarterRoboDKLicense.rdklic")


ROBODK_PATH = os.path.expanduser("~/RoboDK/RoboDK.app/Contents/MacOS/RoboDK")
DIP_TARGET = "dipTarget"
TOOL_TARGET = "toolChangeTarget"
HOME_TARGET = "homeTarget"


_robot = None
_link = None
dip_approach = None
home_approach = None
tool_approach = None
dips_frame = None
wash_frame = None
pick_place_frame = None
calibration_frame = None

_debug = None


def deleteAllStations():
    global _link
    try:
        for station in _link.getOpenStations():
            station.Delete()
    except BaseException:
        print "No stations to empty"


def close():
    global _link
    global _robot
    deleteAllStations()
    try:
        _link.Disconnect()
    except BaseException:
        print "Not connected"
    _link = None
    _robot = None


def new():
    global _link
    global _robot
    global _debug

    # _debug = debug

    # args = ["-SKIPINI"]
    # if _debug:
    #     args.append("-DEBUG")
    # if newinst:
    #     close()
    #     args.append("-NEWINSTANCE")

    # args = ["-SKIPINI", "-NOUI"]

    _link = Robolink(robodk_path=ROBODK_PATH)
    _link.AddFile(LICENSE_FILE)


def link():
    global _link
    if not _link:
        new()
    return _link


def robot():
    global _robot
    if not _robot:
        new()
    return _robot


def show():
    global _link
    _link.ShowRoboDK()


def hide():
    global _link
    _link.HideRoboDK()


def clean():
    global _link
    global _robot
    deleteAllStations()
    _link.AddFile(CLEAN_FILE)
    _robot = _link.Item("", ITEM_TYPE_ROBOT)
    _robot.setParam("PostProcessor", "KUKA KRC4")
    _create_infrastructure()
 
 
def create_program(name):
    global _link
    program = _link.Item(name)
    if program.Valid():
        program.Delete()
    return _link.AddProgram(name)


def create_frame(name, force=True):
    global _link
    frame = _link.Item(name)
    if frame.Valid():
        if force:
            frame.Delete()
        else:
            return frame
    frame = _link.AddFrame(name)
    frame.setPose(rdk.eye())
    return frame


def _config_key(config):
    if config:
        return "%d%d%d" % tuple(config.list2()[0][0:3])


def config_000_poses(pose):
    global _robot
    result = []
    ik = _robot.SolveIK_All(pose)
    siz = ik.size()
    if not (ik and siz[0] and siz[1] and (len(ik.list()) > 5)):
        return result
    joint_poses = [el[0:6] for el in ik.list2()]
    for joint_pose in joint_poses:
        key = _config_key(_robot.JointsConfig(joint_pose))
        if key == "000":
            result.append(joint_pose)
    return result


def maya_to_robodk_mat(rhs):
    """Get transposed mat with translate in mm."""
    mat = rhs.transpose()
    mat = [list(row) for row in mat]
    mat[0][3] = mat[0][3] * 10.0
    mat[1][3] = mat[1][3] * 10.0
    mat[2][3] = mat[2][3] * 10.0
    return rdk.Mat(mat)


def create_joint_target(obj, name, frame):
    global _link
    global _robot

    mat = maya_to_robodk_mat(obj.attr("worldMatrix[0]").get())

    joint_poses = config_000_poses(mat)
    if not joint_poses:
        raise Exception("No configs for approach mat. Try repositioning.")
    joints = joint_poses[0]

    old_approach = _link.Item(name)
    if old_approach.Valid():
        old_approach.Delete()
    target = _link.AddTarget(name, frame, _robot)
    target.setAsJointTarget()
    target.setJoints(joints)
    return target


def write_station(directory, name):
    global _link
    if _link:
        station = _link.Item("", ITEM_TYPE_STATION)
        station.setName(name)
        filename = os.path.join(directory, "{}.rdk".format(name))
        _link.Save(filename)
        return filename


def write_program(directory, name):
    global _link
    if _link:
        program = _link.Item(name, ITEM_TYPE_PROGRAM)
        program.MakeProgram(directory)
        return os.path.join(directory, "{}.src".format(name))


def _create_infrastructure():
    global dip_approach
    global home_approach
    global tool_approach
    global dips_frame
    global wash_frame
    global pick_place_frame
    global calibration_frame

    calibration_frame = create_frame("calibration_frame")
    dips_frame = create_frame("dips_frame")
    wash_frame = create_frame("wash_frame")
    pick_place_frame = create_frame("pick_place_frame")

    _approaches_frame = create_frame("ax_frame")

    tool_approach = create_joint_target(
        pm.PyNode(TOOL_TARGET), "tool_approach", _approaches_frame
    )
    home_approach = create_joint_target(
        pm.PyNode(HOME_TARGET), "home_approach", _approaches_frame
    )
    dip_approach = create_joint_target(
        pm.PyNode(DIP_TARGET), "dip_approach", _approaches_frame
    )
