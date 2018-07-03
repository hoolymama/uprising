import robolink
import pymel.core as pm
from robolink import Robolink, ITEM_TYPE_ROBOT, ITEM_TYPE_TARGET
import robodk as rdk
import uprising.maya_util as uut

RL = Robolink()

def get_robot():
    """Get the robot item and make sure its in the right place.

    Load from disk if necessary.
    """
    robot = RL.Item('', ITEM_TYPE_ROBOT)
    if not robot.Valid():
        robot = RL.AddFile(
            '/Users/julian/projects/robot/mechanisms/KR_30_3.robot')
        frame = robot.Parent()
        frame.setPose(rdk.transl(0, 0, 30))
    if not robot.Valid():
        raise Exception("Robot not valid")
    return robot


def status(text):
    RL.ShowMessage(text, False)


def send_object(node, targets_parent, parent, robot):
    """Send a node to robodk.

    If the node is a locator, it becomes a target in robodk
    and exists in world space. If it is a transform, it is
    added as a reference frame to the hierarchy. If it is a
    mesh, we rebuild triangles in robodk and add it to the
    hierarchy.
    """
    name = node.name()

    if isinstance(node, pm.nodetypes.Transform):
        shapes = node.getShapes()
        if shapes and isinstance(shapes[0], pm.nodetypes.Locator):
            wm = uut.get_robodk_mat(node, "world")
            item = RL.AddTarget(name, targets_parent, robot)
            item.setAsCartesianTarget()
            item.setPose(rdk.Mat(wm))
            item.setVisible(True, visible_frame=True)
        else:
            mat = uut.get_robodk_mat(node, "object")
            frame = RL.AddFrame(name, parent)
            frame.setPose(rdk.Mat(mat))
            frame.setVisible(False, visible_frame=False)
            for child in node.getChildren():
                send_object(child, targets_parent, frame, robot)
    elif isinstance(node, pm.nodetypes.Mesh):
        points = node.getPoints(space='object')
        color = uut.shape_color(node)
        _, vids = node.getTriangles()
        triangles = []
        for vid in vids:
            triangles.append(
                [points[vid].x * 10, points[vid].y * 10, points[vid].z * 10])
        shape = RL.AddShape(triangles)
        shape.setParent(parent)
        shape.setName(name)
        shape.setColor(list(color))
        shape.setVisible(True, visible_frame=False)


def send_objects():
    """Send all recognized nodes to robodk."""
    robot = get_robot()

    targets_parent = RL.Item('Targets')
    if targets_parent.Valid():
        status("frame: Targets already exists")
    else:
        targets_parent = RL.AddFrame("Targets", itemparent=0)
        targets_parent.setPose(rdk.eye())
        status("Created frame: Targets")

    transforms = pm.ls(sl=True, transforms=True)
    for transform in transforms:
        pm.makeIdentity(
            transform,
            apply=True,
            t=False,
            r=False,
            s=True,
            pn=True)
        send_object(transform, targets_parent, 0, robot)
 