
"""
Module as a singleton that stores the robot and the link.
"""

import os

from robolink import (ITEM_TYPE_PROGRAM, ITEM_TYPE_ROBOT,
                      ITEM_TYPE_STATION, Robolink)



import pymel.core as pm
import robodk as rdk
import copy

import logging
logger = logging.getLogger(__name__)
logger.setLevel(logging.DEBUG)

ROBODK_PATH = os.path.expanduser("~/RoboDK/RoboDK.app/Contents/MacOS/RoboDK")
DIP_TARGET = "dipTarget"
TOOL_TARGET = "toolChangeTarget"
HOME_TARGET = "homeTarget"
CANVAS_TARGET = "canvasTarget"


ALL_KR8_CONFIGS = {
    "000": [],
    "001": [],
    "010": [],
    "011": [],
    "100": [],
    "101": [],
    "110": [],
    "111": []
}

ALL_KR30_CONFIGS = {
    "000": [],
    "001": []
}

_model = None
_robot = None
_link = None
dip_approach = None
home_approach = None
tool_approach = None
canvas_approach = None

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
    return os.path.join(project_path, "robodk", "JulianMannRoboDKLicense.rdklic")




def deleteAllStations():
    try:
        for station in link().getOpenStations():
            station.Delete()
    except BaseException:
        print("No stations to empty")


def close():
    global _link
    global _robot
    deleteAllStations()
    try:
        _link.Disconnect()
    except BaseException:
        print("Not connected")
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
    if not _robot:
        _robot = _link.Item("", ITEM_TYPE_ROBOT)
    return _robot


def show():
    link().ShowRoboDK()


def hide():
    link().HideRoboDK()


def clean(model="kr30", infrastructure=True):
    global _model
    global _link
    global _robot
    deleteAllStations()
    cleanfile = get_clean_file(model)

    _link.AddFile(cleanfile)

    _robot = _link.Item("", ITEM_TYPE_ROBOT)
    _model = model
    # _robot.setParam("PostProcessor", "KUKA KRC4")

    if infrastructure:
        _create_infrastructure()


def create_program(name):
    global _link
    global _robot

    program = _link.Item(name)
    if program.Valid():
        program.Delete()
    return _link.AddProgram(name, _robot)


def create_frame(name, force=True):
    # print "Create Frame for ", name
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


def solve_joint_poses(pose):

    global _model
    global _robot

    if _model == "kr30":
        result = copy.deepcopy(ALL_KR30_CONFIGS)
    else:
        result = copy.deepcopy(ALL_KR8_CONFIGS)

    ik = _robot.SolveIK_All(pose)
    siz = ik.size()

    if not (ik and siz[0] and siz[1] and (len(ik.list()) > 5)):
        return result
    joint_poses = [el[0:6] for el in ik.list2()]
    for joint_pose in joint_poses:
        jcfg = _robot.JointsConfig(joint_pose)
        key = _config_key(jcfg)
        if key in result:
            result[key].append(joint_pose)
    return result


def solve_single_joint_pose(flange_pose, last_joints_pose):
    global _model
    global _robot
    if _model == "kr30":
        result = copy.deepcopy(ALL_KR30_CONFIGS)
    else:
        result = copy.deepcopy(ALL_KR8_CONFIGS)

    joints_pose = _robot.SolveIK(flange_pose, last_joints_pose)
    joints_pose = joints_pose.list()
    if len(joints_pose) < 6:
        return result
    jcfg = _robot.JointsConfig(joints_pose)
    key = _config_key(jcfg)
    if key in result:
        result[key].append(joints_pose)
    return result

    # self.joint_poses = robo.solve_joint_poses(flange_pose)


def maya_to_robodk_mat(rhs):
    """Get transposed mat with translate in mm."""
    mat = rhs.transpose()
    mat = [list(row) for row in mat]
    mat[0][3] = mat[0][3] * 10.0
    mat[1][3] = mat[1][3] * 10.0
    mat[2][3] = mat[2][3] * 10.0
    return rdk.Mat(mat)


def create_joint_target(obj, name, frame, configs=["000", "001"]):
    global _link
    global _robot
    logger.setLevel(logging.DEBUG)

    wm = obj.attr("worldMatrix[0]").get()
    logger.debug("Create joint target:".format(name, wm) )
    mat = maya_to_robodk_mat(wm)

    joint_poses = solve_joint_poses(mat)
    joints = find_best_pose(joint_poses, configs)

    old_item = _link.Item(name)
    if old_item.Valid():
        old_item.Delete()
    target = _link.AddTarget(name, frame, _robot)
    target.setAsJointTarget()
    target.setJoints(joints)
    return target

def create_cartesian_target(obj, name, frame, configs=["000", "001"]):

    global _link
    global _robot

    wm = obj.attr("worldMatrix[0]").get()
    tool_pose = maya_to_robodk_mat(wm)
    joint_poses = solve_joint_poses(tool_pose)

    old_item = _link.Item(name)
    if old_item.Valid():
        old_item.Delete()

    joint_poses = solve_joint_poses(tool_pose)
    joints = find_best_pose(joint_poses, configs)

    target = _link.AddTarget(name, frame, _robot)
    target.setPose(tool_pose)
    target.setJoints(joints)
    target.setAsCartesianTarget()
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
    global _model
    global dip_approach
    global home_approach
    global tool_approach
    global canvas_approach
    
    global dips_frame
    global wash_frame
    global pick_place_frame
    global calibration_frame

    try:
        pm.PyNode(TOOL_TARGET)
    except:
        pass

    _approaches_frame = create_frame("ax_frame")

    home_approach = create_joint_target(
        pm.PyNode(HOME_TARGET), "home_approach", _approaches_frame
    )

    if _model == "kr30":
        calibration_frame = create_frame("calibration_frame")
        dips_frame = create_frame("dips_frame")
        wash_frame = create_frame("wash_frame")
        pick_place_frame = create_frame("pick_place_frame")
    
        tool_approach = create_joint_target(
            pm.PyNode(TOOL_TARGET), "tool_approach", _approaches_frame
        )
        dip_approach = create_joint_target(
            pm.PyNode(DIP_TARGET), "dip_approach", _approaches_frame, ["001"]
        )

        canvas_approach = create_cartesian_target(
            pm.PyNode(CANVAS_TARGET), "canvas_approach", _approaches_frame , ["001"]
        )


 

def linear_test(brushname, *names):

    new()


    link().setCollisionActive(False)
    # plug = pm.PyNode(brushNode).attr("outPaintBrush")
    # Brush(0, plug).send( with_geo=True, force=True)
 

    # brush.send()
    brush = link().Item(brushname)
    if not brush.Valid():
         print("Invalid brush: {}. Abort".format(brushname))
         return

    robot().setPoseTool(brush)  

    for i in range(1, len(names)):
        t0 = link().Item(names[i-1])
        t1 = link().Item(names[i])
        if not t0.Valid() and t1.Valid():
            print("Skipping invalid pair: {} <> {}".format(names[i-1], names[i]))
            continue
        print("testing lin move between {} and {}".format(names[i-1], names[i]))

        robot().MoveJ(t0.Pose())
        joints = robot().Joints()
        fail = robot().MoveL_Test(joints, t1.Pose())

        print("FAIL" if fail else "SUCCESS")

