import sys
import os
import errno
import robodk as rdk
import pymel.core as pm
from contextlib import contextmanager
# from paint import Paint
# from brush import Brush

PI = 3.14159265359


CLEAN_FILE = os.path.join(
    os.environ["UPRISING_PROJECT_PATH"],
    "robodk",
    "clean.rdk")

from robolink import (
    Robolink,
    ITEM_TYPE_ROBOT,
    ITEM_TYPE_TOOL,
    ITEM_TYPE_PROGRAM)

# RL = Robolink()


def conform_activatable_checkbox(ctl):
    val = ctl.getEnable()
    form = ctl.parent()
    cb = form.getChildArray()[-1]
    pm.checkBox(cb, e=True, value=val)


def assembly(node):
    top = node
    p = node.getParent()
    while p:
        top = p
        p = p.getParent()
    return top


@contextmanager
def minimize_robodk():
    RL = Robolink()
    RL.HideRoboDK()
    try:
        yield
    except Exception:
        t, v, tb = sys.exc_info()
        raise t, v, tb
    finally:
        RL.ShowRoboDK()
 

# @contextmanager
# def minimize_robodk():
#     RL = Robolink()
#     # RL.HideRoboDK()
#     try:
#         yield
#     except Exception:
#         t, v, tb = sys.exc_info()
#         # RL.ShowRoboDK()
#         raise t, v, tb
#     # RL.ShowRoboDK()




@contextmanager
def at_height(node, h):
    old =  node.attr("tz").get() 
    node.attr("tz").set(h)
    yield
    node.attr("tz").set(old)

@contextmanager
def at_position(node, x, y, z):
    old =  node.attr("t").get() 
    node.attr("t").set(x, y, z)
    yield
    node.attr("t").set(old)


@contextmanager
def final_position(*nodes):
    remember = []
    for node in nodes:
        asy = assembly(node)
        remember.append([asy, asy.attr("zeroPosition").get()]) 
        asy.attr("zeroPosition").set(False)
    yield
    for asy , rem in remember:
        asy.attr("zeroPosition").set(rem)


@contextmanager
def zero_position(node):
    asy = assembly(node)
    zpos = asy.attr("zeroPosition").get()
    asy.attr("zeroPosition").set(True)
    yield
    asy.attr("zeroPosition").set(zpos)


@contextmanager
def filters_off(*nodes):
    curr_vals = [node.attr("applyFilters").get() for node in nodes]
    for node in nodes:
        node.attr("applyFilters").set(False)
    yield
    for node, val in zip(nodes, curr_vals):
        node.attr("applyFilters").set(val)


def _on_active_cb_change(ctrl, cb_ctrl):
    state = pm.checkBox(cb_ctrl, q=True, v=True)
    pm.control(ctrl, edit=True, en=state)


@contextmanager
def activatable(**kw):
    label = kw.get("label", "Active")
    state = kw.get("state", True)
    form = pm.formLayout(nd=100)
    yield
    cb = pm.checkBox(label=label, value=state)

    children = pm.formLayout(form, query=True, childArray=True)

    ctrl = children[0]
    pm.checkBox(
        cb,
        edit=True,
        changeCommand=pm.Callback(
            _on_active_cb_change,
            ctrl,
            cb))

    form.attachForm(cb, 'top', 2)
    form.attachForm(cb, 'bottom', 2)
    form.attachForm(cb, 'right', 2)
    form.attachNone(cb, 'left')

    form.attachForm(ctrl, 'left', 2)
    form.attachForm(ctrl, 'top', 2)
    form.attachForm(ctrl, 'bottom', 2)
    form.attachControl(ctrl, 'right', 2, cb)

    _on_active_cb_change(ctrl, cb)

    pm.setParent('..')


def mkdir_p(path):
    try:
        os.makedirs(path)
    except OSError as exc:
        if exc.errno == errno.EEXIST and os.path.isdir(path):
            pass
        else:
            raise


def to_vector_array(arr):
    if not arr:
        return []
    in_len = len(arr)
    out_len = in_len / 3
    if out_len * 3 != in_len:
        raise ValueError("Input array must be multiple of 3")
    result = []

    for i in range(out_len):
        j = i * 3
        result.append(pm.dt.Vector(arr[j], arr[j + 1], arr[j + 2]))
    return result


def to_point_array(arr):
    if not arr:
        return []
    in_len = len(arr)
    out_len = in_len / 3
    if out_len * 3 != in_len:
        raise ValueError("Input array must be multiple of 3")
    result = []

    for i in range(out_len):
        j = i * 3
        result.append(pm.dt.Point(arr[j], arr[j + 1], arr[j + 2]))
    return result


def rad2deg(rad):
    return rad * (180 / PI)


def deg2rad(deg):
    return deg / (180 / PI)


class PaintingError(Exception):
    pass


class ClusterError(Exception):
    pass


class StrokeError(Exception):
    pass


def maya_to_robodk_mat(rhs):
    """Get transposed mat with translate in mm."""
    mat = rhs.transpose()
    mat = [list(row) for row in mat]
    mat[0][3] = mat[0][3] * 10.0
    mat[1][3] = mat[1][3] * 10.0
    mat[2][3] = mat[2][3] * 10.0
    return rdk.Mat(mat)


# def mat_from_connected(attribute, space):
#     conns = attribute.connections(
#         source=True, destination=False)
#     if not conns:
#         return None
#     mat = None
#     if space == "world":
#         mat = pm.PyNode(conns[0]).attr("worldMatrix[0]").get()
#     else:
#         mat = pm.PyNode(conns[0]).attr("matrix").get()
#     return maya_to_robodk_mat(mat)


def create_program(name):
    RL = Robolink()
    program = RL.Item(name)
    if program.Valid():
        program.Delete()
    return RL.AddProgram(name)


def create_frame(name, force=True):
    RL = Robolink()
    frame = RL.Item(name)
    if frame.Valid():
        if force:
            frame.Delete()
        else:
            return frame
    frame = RL.AddFrame(name)
    frame.setPose(rdk.eye())
    return frame

def delete_tools():
    RL = Robolink()
    for t in RL.ItemList(filter=ITEM_TYPE_TOOL):
        t.Delete()


def delete_programs():
    RL = Robolink()
    for t in RL.ItemList(filter=ITEM_TYPE_PROGRAM):
        t.Delete()


def numeric(s):
    try:
        return float(s)
    except ValueError:
        return s


def config_key(config):
    if config:
        return "%d%d%d" % tuple(config.list2()[0][0:3])


def config_000_poses(pose):
    RL = Robolink()
    configs = {}
    result = []
    robot = RL.Item('', ITEM_TYPE_ROBOT)
    ik = robot.SolveIK_All(pose)
    siz = ik.size()
    if not (ik and siz[0] and siz[1] and (len(ik.list()) > 5)):
        return result
    joint_poses = [el[0:6] for el in ik.list2()]
    for joint_pose in joint_poses:
        key = config_key(robot.JointsConfig(joint_pose))
        if key == "000":
            result.append(joint_pose)
    return result


def _create_joint_target(obj, name, frame):
    RL = Robolink()
    robot = RL.Item('', ITEM_TYPE_ROBOT)
    mat = obj.attr("worldMatrix[0]").get()

 
    mat = maya_to_robodk_mat(mat)
 
    joint_poses = config_000_poses(mat)
    if not joint_poses:
        raise Exception(
            "No configs for approach mat. Try repositioning.")
    joints = joint_poses[0]

    old_approach = RL.Item(name)
    if old_approach.Valid():
        old_approach.Delete()
    target = RL.AddTarget(name, frame, robot)
    target.setAsJointTarget()
    target.setJoints(joints)
    return target


def clean_rdk():
    RL = Robolink()
    for station in RL.getOpenStations():
        station.Delete()
    RL.AddFile(CLEAN_FILE)

# def config_first_pose(pose):
#     RL = Robolink()
#     configs = {}
#     result = []
#     robot = RL.Item('', ITEM_TYPE_ROBOT)
#     ik = robot.SolveIK_All(pose)
#     siz = ik.size()
#     if not (ik and  siz[0] and  siz[1] and (len(ik.list()) > 5)):
#         return result
#     joint_poses = [el[0:6] for el in ik.list2()]
#     for joint_pose in joint_poses:
#         key = config_key(robot.JointsConfig(joint_pose))
#         if key == "000":
#             result.append(joint_pose)
#     return result




