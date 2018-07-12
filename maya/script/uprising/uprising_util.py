import robodk as rdk
import pymel.core as pm

PI = 3.14159265359


from robolink import (Robolink, ITEM_TYPE_ROBOT)

RL = Robolink()


def rad2deg(rad):
    return rad * (180 / PI)


def deg2rad(deg):
    return deg / (180 / PI)


class StrokeError(Exception):
    pass


class ClusterError(Exception):
    pass


def maya_to_robodk_mat(rhs):
    """Get transposed mat with translate in mm."""
    mat = rhs.transpose()
    mat = [list(row) for row in mat]
    mat[0][3] = mat[0][3] * 10.0
    mat[1][3] = mat[1][3] * 10.0
    mat[2][3] = mat[2][3] * 10.0
    return rdk.Mat(mat)


def mat_from_connected(attribute, space):
    conns = attribute.connections(
        source=True, destination=False)
    if not conns:
        return None
    mat = None
    if space == "world":
        mat = pm.PyNode(conns[0]).attr("worldMatrix[0]").get()
    else:
        mat = pm.PyNode(conns[0]).attr("matrix").get()
    return maya_to_robodk_mat(mat)


def create_program(name):
    program = RL.Item(name)
    if program.Valid():
        program.Delete()
    return RL.AddProgram(name)


def create_frame(name):
    frame = RL.Item(name)
    if frame.Valid():
        frame.Delete()
    frame = RL.AddFrame(name)
    frame.setPose(rdk.eye())
    return frame
