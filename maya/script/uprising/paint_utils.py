import pymel.core as pm
import stroke_factory_utils as sfu
from brush import Brush
from robolink import (Robolink, ITEM_TYPE_ROBOT)
import uprising.maya_util as uut
import robodk as rdk


# RL = Robolink()


def add_all_trays_to_sf():
    node = pm.ls(selection=True, dag=True, leaf=True, type="strokeFactory")[0]
    paints = pm.ls(selection=True, dag=True, leaf=True, type="mesh")
    for i, paint in enumerate(paints):
        paint_tf = paint.getParent()
        connect_paint_to_node(paint_tf, node, i)


def connect_paint_to_node(paint_tf, node, connect_to="next_available"):
    index = sfu.get_index(node, "paints.name", connect_to)
    whitelist = ["double", "float", "short", "bool", "string"]
    atts = node.attr("paints[%d]" % index).getChildren()
    for att in atts:
        att_type = att.type()
        if att_type in whitelist:
            sfu.create_and_connect_driver(paint_tf, att)
        elif att_type in ["float3", "double3"]:
            child_atts = att.getChildren()
            for c_att in child_atts:
                sfu.create_and_connect_driver(paint_tf, c_att)


def send_paints(factory):
    RL = Robolink()
    robot = RL.Item('', ITEM_TYPE_ROBOT)
    p_indices = factory.attr("paints").getArrayIndices()
    for i in p_indices:
        att = sfu.input_connection(factory.attr("paints[%d].paintOpacity" % i))
        if att:
            tray = att.node()
            send_paint(robot, tray)


def send_paint(robot, tray):
    RL = Robolink()
    geo = tray.getShapes()

    triangles = []
    for g in geo:
        points = g.getPoints(space='world')
        _, vids = g.getTriangles()
        for vid in vids:
            triangles.append(
                [points[vid].x * 10, points[vid].y * 10, points[vid].z * 10])

    name = "tx_%s" % str(tray)

    tray_item = RL.Item(name)
    if tray_item.Valid():
        tray_item.Delete()

    cR = tray.attr("sfPaintColorR").get()
    cG = tray.attr("sfPaintColorG").get()
    cB = tray.attr("sfPaintColorB").get()
    shape = RL.AddShape(triangles)
    shape.setName(name)
    shape.setColor([cR, cG, cB])
