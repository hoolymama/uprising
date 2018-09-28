import robodk as rdk
import pymel.core as pm

PI = 3.14159265359


from robolink import (Robolink, ITEM_TYPE_ROBOT, ITEM_TYPE_TOOL, ITEM_TYPE_PROGRAM)

# RL = Robolink()

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


def create_frame(name):
    RL = Robolink()
    frame = RL.Item(name)
    if frame.Valid():
        frame.Delete()
    frame = RL.AddFrame(name)
    frame.setPose(rdk.eye())
    return frame

def delete_tools():
    RL = Robolink()
    for t in RL.ItemList(filter = ITEM_TYPE_TOOL):
        t.Delete()

def delete_programs():
    RL = Robolink()
    for t in RL.ItemList(filter = ITEM_TYPE_PROGRAM):
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
    if not (ik and  siz[0] and  siz[1] and (len(ik.list()) > 5)):
        return result
    joint_poses = [el[0:6] for el in ik.list2()]
    for joint_pose in joint_poses:
        key = config_key(robot.JointsConfig(joint_pose))
        if key == "000":
            result.append(joint_pose)
    return result





