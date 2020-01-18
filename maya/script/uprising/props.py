import pymel.core as pm
import uprising.maya_util as mutil
import robodk as rdk
import robo


def get_targets_frame():
    link = robo.link()

    targets_parent = link.Item("Targets", ITEM_TYPE_TARGET)
    if not targets_parent.Valid():
        targets_parent = link.AddFrame("Targets", itemparent=0)
        targets_parent.setPose(rdk.eye())
    return targets_parent


def send_object(node, parent):
    """Send a node to robodk.

    If it is a transform, it is added as a reference frame 
    to the hierarchy. If it is a
    mesh, we rebuild triangles in robodk and add it to the
    hierarchy.
    """
    link = robo.link()

    name = node.name()

    if isinstance(node, pm.nodetypes.Transform):
        mat = mutil.get_robodk_mat(node, "object")
        frame = link.AddFrame(name, parent)
        frame.setPose(rdk.Mat(mat))
        frame.setVisible(False, visible_frame=False)
        for child in node.getChildren():
            send_object(child, frame)

    elif isinstance(node, pm.nodetypes.Mesh):
        points = node.getPoints(space="object")
        color = mutil.shape_color(node)
        _, vids = node.getTriangles()
        triangles = []
        for vid in vids:
            triangles.append(
                [points[vid].x * 10, points[vid].y * 10, points[vid].z * 10]
            )
        shape = link.AddShape(triangles)
        shape.setParent(parent)
        shape.setName(name)
        shape.setColor(list(color))
        shape.setVisible(True, visible_frame=False)


def mesh_triangles(node):
    meshes = pm.PyNode(node).getChildren(allDescendents=True, type="mesh")
    triangles = []
    for mesh in meshes:
        points = mesh.getPoints(space="world")
        _, vids = mesh.getTriangles()
        for vid in vids:
            triangles.append(
                [points[vid].x * 10, points[vid].y * 10, points[vid].z * 10]
            )
    return triangles


def send(objects, parent=None):
    transforms = pm.ls(objects, transforms=True)
    for xf in transforms:
        xf = pm.duplicate(xf, rr=True)[0]
        hi = pm.ls(xf, dag=True, transforms=True)
        for node in hi:
            for att in ["tx", "ty", "tz", "rx", "ry", "rz", "sx", "sy", "sz"]:
                node.attr(att).setLocked(False)
        if xf.getParent():
            pm.parent(xf, world=True)
        pm.makeIdentity(xf, apply=True, t=0, r=0, s=1, n=0, pn=1)
        send_object(xf, parent)
        pm.delete(xf)


def send_selected():
    transforms = pm.ls(sl=True, transforms=True)
    send(transforms)

