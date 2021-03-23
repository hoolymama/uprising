
"""
Module as a singleton that stores the robot and the link.
"""

import os

from robolink import (ITEM_TYPE_PROGRAM, ITEM_TYPE_ROBOT,
                      ITEM_TYPE_STATION, Robolink)

import pymel.core as pm
import robodk as rdk


ROBODK_PATH = os.path.expanduser("~/RoboDK/RoboDK.app/Contents/MacOS/RoboDK")
DIP_TARGET = "dipTarget"
TOOL_TARGET = "toolChangeTarget"
HOME_TARGET = "homeTarget"

ALL_CONFIGS = {
        "000": [],
        "001": [],
        "010": [],
        "011": [],
        "100": [],
        "101": [],
        "110": [],
        "111": []
    }

_model = None
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


def get_uprising_project_path():
    return os.path.dirname(pm.getModulePath(moduleName="uprising"))


def get_clean_file(model):
    project_path = get_uprising_project_path()
    return os.path.join(project_path, "robodk", "{}_clean.rdk".format(model))


def get_license_file():
    project_path = get_uprising_project_path()
    return os.path.join(project_path, "robodk", "RobAndNickCarterRoboDKLicense.rdklic")


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
    global _debug
    _link = Robolink(robodk_path=ROBODK_PATH)
    _link.AddFile(get_license_file())


def link():
    global _link
    if not _link:
        new()
    return _link


def robot():
    global _robot
    return _robot


def show():
    link().ShowRoboDK()


def hide():
    link().HideRoboDK()


def clean(model="kr30"):
    global _model
    global _link
    global _robot
    deleteAllStations()
    cleanfile = get_clean_file(model)

    _link.AddFile(cleanfile)
    _robot = _link.Item("", ITEM_TYPE_ROBOT)
    _model = model
    # _robot.setParam("PostProcessor", "KUKA KRC4 RN")
    _create_infrastructure()


def create_program(name):
    global _link
    global _robot

    program = _link.Item(name)
    if program.Valid():
        program.Delete()
    return _link.AddProgram(name, _robot)


def create_frame(name, force=True):
    print "Create Frame for ", name
    global _link
    frame = _link.Item(name)
    if frame.Valid():
        if force:
            frame.Delete()
        else:
            return frame
    print "ADDING FRAME ", name
    frame = _link.AddFrame(name)
    print "ADDED FRAME ", name

    print "VALID:", _link.Item(name).Valid()

    frame.setPose(rdk.eye())
    return frame


def _config_key(config):
    if config:
        return "%d%d%d" % tuple(config.list2()[0][0:3])


def config_poses(pose):
    global _model
    global _robot

    result = ALL_CONFIGS.copy()
    ik = _robot.SolveIK_All(pose)
    siz = ik.size()
    if not (ik and siz[0] and siz[1] and (len(ik.list()) > 5)):
        return result
    joint_poses = [el[0:6] for el in ik.list2()]
    for joint_pose in joint_poses:
        jcfg = _robot.JointsConfig(joint_pose)
        key = _config_key(jcfg)
        result[key].append(joint_pose)
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

    wm = obj.attr("worldMatrix[0]").get()
    mat = maya_to_robodk_mat(wm)

    joint_poses = config_poses(mat)
    joints = find_best_pose(joint_poses, ["000", "001"])

    old_approach = _link.Item(name)
    if old_approach.Valid():
        old_approach.Delete()
    target = _link.AddTarget(name, frame, _robot)
    target.setAsJointTarget()
    target.setJoints(joints)
    return target


def find_best_pose(poses, keys):
    """For now, take the first"""
    for key in keys:
        if poses[key]:
            return poses[key][0]
    raise ValueError("[find_best_pose]: No configs match.")


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

        # text_file = os.path.join(directory, "{}.txt".format(name))
        # if os.path.isfile(text_file):
        #     os.rename(text_file, src_file)
        src_file = os.path.join(directory, "{}.src".format(name))
        return src_file


def _create_infrastructure():
    global dip_approach
    global home_approach
    global tool_approach
    global dips_frame
    global wash_frame
    global pick_place_frame
    global calibration_frame

    try:
        pm.PyNode(TOOL_TARGET)
    except:
        pass

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
