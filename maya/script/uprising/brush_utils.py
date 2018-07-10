import pymel.core as pm
import stroke_factory_utils as sfu
from brush import Brush
from robolink import (Robolink, ITEM_TYPE_ROBOT)
import uprising.maya_util as uut
import robodk as rdk


RL = Robolink()




def add_brush_to_sf():
    node = pm.ls(selection=True, dag=True, leaf=True, type="strokeFactory")[0]
    brushes = pm.ls(selection=True, dag=True, leaf=True, type="mesh")
    for brush in brushes:
        brush_tf = brush.getParent()
        connect_brush_to_node(brush_tf, node)

def connect_brush_to_node(brush_tf, node, connect_to="next_available"):
    index = sfu.get_index(node, "brushes.brushMatrix", connect_to)
    brush_tf.attr("matrix") >> node.attr("brushes[%d].brushMatrix" % index)
    whitelist = ["double", "short", "bool", "doubleAngle"]
    atts = node.attr("brushes[%d]" % index).getChildren()
    for att in atts:
        att_type = att.type()
        if att_type in whitelist:
            sfu.create_and_connect_driver(brush_tf, att)
        elif att_type == "double3":
            child_atts = att.getChildren()
            for c_att in child_atts:
                sfu.create_and_connect_driver(brush_tf, c_att)


def send_brushes(factory):
    robot = RL.Item('', ITEM_TYPE_ROBOT)
    base =  pm.PyNode("brushes|base")
    brushes = Brush.used_brushes(factory)
    for _id in brushes:
        send_brush(robot, brushes[_id], base )

def send_brush(robot, brush, base):
    """Send a node to robodk.

    If the node is a locator, it becomes a target in robodk
    and exists in world space. If it is a transform, it is
    added as a reference frame to the hierarchy. If it is a
    mesh, we rebuild triangles in robodk and add it to the
    hierarchy.
    """

    geo = pm.PyNode(brush.name).getShapes() + base.getShapes()
    # print brush.name
    triangles = []
    for g in geo:
        points = g.getPoints(space='world')
        _, vids = g.getTriangles()
        for vid in vids:
            triangles.append(
                [points[vid].x * 10, points[vid].y * 10, points[vid].z * 10])
    
 
    tool_item = RL.Item(brush.name)
    if tool_item.Valid():
        tool_item.Delete()

    color = uut.shape_color(g)
    tool_item = robot.AddTool(brush.matrix, brush.name)
    shape = RL.AddShape(triangles)
    shape.setColor(list(color))
    tool_item.AddGeometry(shape, rdk.eye())
    robot.setPoseTool(tool_item)
    shape.Delete()
 