import pymel.core as pm
import stroke_factory_utils as sfu
from brush import Brush
from robolink import (Robolink, ITEM_TYPE_ROBOT)
import uprising.maya_util as uut
import uprising.uprising_util as uput

import robodk as rdk
import sheets

# RL = Robolink()


def add_brush_to_painting():
    node = pm.ls(selection=True, dag=True, leaf=True, type="strokeFactory")[0]
    brushes = pm.ls(selection=True, dag=True, leaf=True, type="mesh")
    for brush in brushes:
        brush_tf = brush.getParent()
        connect_brush_to_node(brush_tf, node)


def connect_brush_to_node(brush_tf, node, connect_to="next_available"):
    index = sfu.get_index(node, "brushes.brushMatrix", connect_to)
    brush_tf.attr("matrix") >> node.attr("brushes[%d].brushMatrix" % index)
    whitelist = ["double", "short", "bool", "doubleAngle", "enum"]
    atts = node.attr("brushes[%d]" % index).getChildren()
    for att in atts:
        att_type = att.type()
        if att_type in whitelist:
            sfu.create_and_connect_driver(brush_tf, att)
        elif att_type == "double3":
            child_atts = att.getChildren()
            for c_att in child_atts:
                sfu.create_and_connect_driver(brush_tf, c_att)
    return index


def send_brushes(factory):
    RL = Robolink()
    robot = RL.Item('', ITEM_TYPE_ROBOT)
    base = pm.PyNode("brushes|base")
    brushes = Brush.used_brushes(factory)
    for _id in brushes:
        send_brush(robot, brushes[_id], base)


def send_brush(robot, brush, base):
    """Send a node to robodk.

    If the node is a locator, it becomes a target in robodk
    and exists in world space. If it is a transform, it is
    added as a reference frame to the hierarchy. If it is a
    mesh, we rebuild triangles in robodk and add it to the
    hierarchy.
    """
    RL = Robolink()
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

######################################
######################################
######################################
######################################


def create_brush_geo(height, bristle_height, tip, width, name, profile):

    half_bristle_height = bristle_height / 2.0
    tcp = height - tip
    handle_height = height - bristle_height
    half_handle_height = handle_height / 2.0

    if profile == "FLAT":
        head = pm.polyCylinder(
            axis=(0, 0, 1), sx=6, sy=1, sz=1,
            radius=0.5, height=1)[0]
        head.attr("sy").set(0.4)
    elif profile == "FILBERT":
        head = pm.polySphere(axis=(0, 0, 1), sx=6, sy=6, radius=0.5)[0]
        head.attr("sy").set(0.4)
    else:  # ROUND
        head = pm.polySphere(axis=(0, 0, 1), sx=6, sy=6, radius=0.5)[0]
        head.attr("sy").set(width)

    head.attr("sx").set(width)
    head.attr("sz").set(bristle_height)
    head.attr("tz").set(tip - half_bristle_height)

    handle = pm.polyCylinder(
        axis=(0, 0, 1), sx=8, sy=1, sz=1,
        h=handle_height, radius=0.5)[0]
    handle.attr("tz").set(-(half_handle_height + bristle_height) + tip)

    geo = pm.polyUnite(head, handle, ch=0, mergeUVSets=1, name=name)[0]
    geo.attr("tz").set(tcp)
    return geo


def brush_name(idx, unsplay_width, xname, profile, type):
    parts = [
        "bx%s" % str(int(idx)),
        "%smm" % str(int(unsplay_width * 10)),
        profile.lower()
    ]
    if xname:
        parts.append(xname.lower())

    if type:
        parts.append(type)


    return "_".join(parts)

def create_and_connect_brush_geo(
        painting_node,
        dip_node,
        idx,
        _1,
        height,
        bristle_height,
        tip,
        _2,
        unsplay_width,
        width,
        xname,
        profile,
        dip_tip,
        _4
        ):

    name = brush_name(idx, unsplay_width, xname, profile, "P")
    painting_tf = create_brush_geo(height, bristle_height, tip, width, name, profile)
    profile_shape = 0
    if profile.lower() == "round":
        profile_shape = 1



    if painting_node:
        connect_brush_to_node(painting_tf, painting_node)
        painting_tf.attr("sfBrushWidth").set(width)
        painting_tf.attr("sfBrushRetention").set(1)
        painting_tf.attr("sfBrushShape").set(profile_shape)
        
    else:
        pm.warning("No painting node. Skipping")

    name = brush_name(idx, unsplay_width, xname, profile, "D")
    dip_tf = create_brush_geo(height, bristle_height, dip_tip, width, name, profile)
    if dip_node:
        connect_brush_to_node(dip_tf, dip_node)
        dip_tf.attr("sfBrushWidth").set(width)
        dip_tf.attr("sfBrushRetention").set(1000)
        dip_tf.attr("sfBrushShape").set(profile_shape)
    else:
        pm.warning("No dip node. Skipping")

    pm.parent( painting_tf, '|brushes' )
    pm.parent( dip_tf, '|brushes' )



def delete_brushes(node):
    brushes = pm.listRelatives("|brushes", children=True)
    for brush in brushes:
        if brush.name().startswith("bx"):
            pm.delete(brush)
    indices = node.attr("brushes").getArrayIndices()
    for i in indices:
        pm.removeMultiInstance(node.attr("brushes[%d]" % i), b=True)


def create_brush_geo_from_sheet(painting_node, dip_node):
    data =  get_raw_brushes_data()
    validate_brush_data(data)

    delete_brushes(painting_node)
    delete_brushes(dip_node)

    for row in data:
        row = [uput.numeric(s) for s in row]
        create_and_connect_brush_geo(painting_node, dip_node, *row)


# def numeric(s):
#     try:
#         return float(s)
#     except ValueError:
#         return s


def validate_brush_data(data):
    if not len(data):
        raise ValueError("No brush data from Google sheets")
    for row in data:
        if not len(row) > 9:
            raise ValueError("Invalid brush data from Google sheets")



def get_raw_brushes_data():
    service = sheets._get_service()
    result = service.spreadsheets().values().get(
        spreadsheetId=sheets.SHEETS["Measurements"],
        range='Brushes!A2:L18').execute()
    values = result.get('values', [])
    return values



    # delete existing brushes


# (self, the_id, name, matrix, width, lift, retention):
