import pymel.core as pm
import uprising.maya_util as mutil


from robolink import Robolink, ITEM_TYPE_ROBOT, ITEM_TYPE_TARGET
import robodk as rdk


def get_targets_frame():
    RL = Robolink()
    targets_parent = RL.Item('Targets', ITEM_TYPE_TARGET)
    if not targets_parent.Valid():
        targets_parent = RL.AddFrame("Targets", itemparent=0)
        targets_parent.setPose(rdk.eye())
    return targets_parent


def send_object(node, parent):
    """Send a node to robodk.

    If the node is a locator, it becomes a target in robodk
    and exists in world space. If it is a transform, it is
    added as a reference frame to the hierarchy. If it is a
    mesh, we rebuild triangles in robodk and add it to the
    hierarchy.
    """
    RL = Robolink()
    robot = RL.Item('', ITEM_TYPE_ROBOT)
    name = node.name()

    if isinstance(node, pm.nodetypes.Transform):
        shapes = node.getShapes()
        if shapes and isinstance(shapes[0], pm.nodetypes.Locator):
            targets_parent = get_targets_frame()
            wm = mutil.get_robodk_mat(node, "world")
            item = RL.AddTarget(name, targets_parent, robot)
            item.setAsCartesianTarget()
            item.setPose(rdk.Mat(wm))
            item.setVisible(True, visible_frame=True)
        else:
            mat = mutil.get_robodk_mat(node, "object")
            frame = RL.AddFrame(name, parent)
            frame.setPose(rdk.Mat(mat))
            frame.setVisible(False, visible_frame=False)
            for child in node.getChildren():
                send_object(child, frame)
    elif isinstance(node, pm.nodetypes.Mesh):
        points = node.getPoints(space='object')
        color = mutil.shape_color(node)
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


def send(objects, frame=None):
    transforms = pm.ls(objects, transforms=True)
    dups = pm.duplicate(transforms)
    for dup in dups:
        for att in ["tx", "ty", "tz", "rx", "ry", "rz", "sx", "sy", "sz"]:
            dup.attr(att).setLocked(False)
        if dup.getParent():
            pm.parent(dup, world=True)

    pm.makeIdentity(dups, apply=True, t=False, r=False, s=True, pn=True)
    for dup in dups:
        send_object(dup, frame)
    pm.delete(dups)


def send_selected():
    transforms = pm.ls(selected=True, transforms=True)
    send(transforms)




# def brush_triangles(brushName):

#     data = pm.brushQuery -tri -w  "bpx_2_portrait_set_B9_6mm_new9_roundShape";

    
#     for t in triangles:
#     for vid in vids:
#         triangles.append(
#             [points[vid].x * 10, points[vid].y * 10, points[vid].z * 10])
#     shape = RL.AddShape(triangles)
#     shape.setParent(parent)
#     shape.setName(name)
#     shape.setVisible(True, visible_frame=False)



