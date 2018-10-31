import pymel.core as pm
import stroke_factory_utils as sfu
import curve_utils as cutl
from brush import Brush
from robolink import (Robolink, ITEM_TYPE_ROBOT)
import uprising.maya_util as uut
import uprising.uprising_util as uput

import robodk as rdk
import sheets


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


# def brush_name(idx, unsplay_width, desc, profile, prefix):
#     return "_".join([
#         prefix,
#         str(int(idx)),
#         "%smm" % str(int(unsplay_width * 10)),
#         profile.lower(),
#         desc.lower()
#     ])

def brush_name(prefix, idx, width, physical_id, profile,desc):
    return "_".join([
        prefix,
        str(int(idx)),
        "%dmm" % int(width * 10),
        "p%d"%int(physical_id),
        profile.lower(),
        desc.lower()
    ])


def create_and_connect_single_brush_geo(
        node,
        idx,
        unsplay_width,
        desc,
        profile,
        prefix,
        height,
        bristle_height,
        tip,
        width,
        physical_id,
        parent):
    if not node:
        pm.warning("No destination node. Skipping brush gen")
        return

    # name = brush_name(idx, unsplay_width, desc, profile, prefix)
    name = brush_name(prefix, idx, width, physical_id, profile,desc)
    
    tf = create_brush_geo(height, bristle_height, tip, width, name, profile)
    profile_shape = 0
    if profile.lower() == "round":
        profile_shape = 1

    connect_brush_to_node(tf, node)
    tf.attr("sfBrushWidth").set(width)
    tf.attr("sfBrushTip").set(tip)
    tf.attr("sfBrushShape").set(profile_shape)
    tf.attr("sfBrushPhysicalId").set(physical_id)


    retention = 1 if prefix == "bpx" else 1000
    tf.attr("sfBrushRetention").set(retention)

    pm.parent(tf, parent)
    return tf

 
def create_and_connect_both_brushes_geo( painting_node, dip_node, **kw):

    painting_brush_tf = create_and_connect_single_brush_geo(
        painting_node,
        kw["id"],
        kw["unsplay_width"],
        kw["desc"],
        kw["profile"],
        "bpx",
        kw["height"],
        kw["bristle_height"],
        kw["tip"],
        kw["splay_width"],
        kw["physical_id"],
        '|brushes|paintingBrushes')

    dip_brush_tf = create_and_connect_single_brush_geo(
        dip_node,
        kw["id"],
        kw["unsplay_width"],
        kw["desc"],
        kw["profile"],
        "bdx",
        kw["height"],
        kw["bristle_height"],
        kw["dip_tip"],
        kw["splay_width"],
        kw["physical_id"],
        '|brushes|dipBrushes')
    return (painting_brush_tf, dip_brush_tf)

def remove_brush_multi_atts(*nodes):
    for node in nodes:
        indices = node.attr("brushes").getArrayIndices()
        for i in indices:
            pm.removeMultiInstance(node.attr("brushes[%d]" % i), b=True)


def delete_brushes(painting_node, dip_node):
    try:
        pm.delete("|brushes|dipBrushes|*")
    except pm.MayaNodeError:
        pass
    try:
        pm.delete("|brushes|paintingBrushes|*")
    except pm.MayaNodeError:
        pass
    remove_brush_multi_atts(painting_node, dip_node)


def validate_brush_data(data):
    result = []
    if not len(data):
        raise ValueError("No brush data from Google sheets")

    for row in data:
        if not len(row) > 9:
            continue
        try:
            int(row[0])
        except ValueError:
            continue
        result.append(row)
    return result


def setup_brushes_from_sheet(painting_node, dip_node, pouch_name):
    print "%s %s %s" % (painting_node, dip_node, pouch_name)
    (name, desc, pouch) = sheets.get_resource_by_name(pouch_name, "Brushes")

    pouch = validate_brush_data(pouch)
    delete_brushes(painting_node, dip_node)


    # prepare for dip curves
    dip_curves_src = "brushes|dipCurves|defaultSource"
    pm.makeIdentity(dip_curves_src, t=True, r=True, s=True)
    pm.delete("brushes|dipCurves|bdcx*")


    for row in pouch:
        row = [uput.numeric(s) for s in row]

        kwargs = {
            "id": row[0],
            "height": row[2],
            "bristle_height": row[3],
            "tip": row[4],
            "unsplay_width": row[6],
            "splay_width": row[7],
            "desc": row[8],
            "profile": row[9],
            "dip_tip": row[10],
            "wipe_tip":  row[12],
            "physical_id": row[16]
        }

        p_brush_tf, d_brush_tf = create_and_connect_both_brushes_geo(painting_node, dip_node, **kwargs)
        wipe_offset = kwargs["dip_tip"] - kwargs["wipe_tip"] 

        generate_brush_dip_curves(p_brush_tf, d_brush_tf, wipe_offset)


# def get_raw_brushes_data():
#     service = sheets._get_service()
#     result = service.spreadsheets().values().get(
#         spreadsheetId=sheets.SHEETS["Measurements"],
#         range='Brushes!A2:L18').execute()
#     values = result.get('values', [])
#     return values

def generate_brush_dip_curves(
    painting_brush_tf, 
    dip_brush_tf, 
    wipe_offset):



    src = "brushes|dipCurves|defaultSource"
    pm.PyNode("brushes").attr("visibility").set(True)
    # node = pm.PyNode("dipPaintingShape")
    

    painting_brush = painting_brush_tf.getChildren()[0]
    dip_brush = dip_brush_tf.getChildren()[0]
    
    # dip_brushes = pm.ls("brushes|dipBrushes|bdx*", dag=True, leaf=True)
    # painting_brushes = pm.ls(
    #     "brushes|paintingBrushes|bpx*",
    #     dag=True,
    #     leaf=True)

    # brushes = zip(dip_brushes, painting_brushes)

    

    # for dip_brush, painting_brush in brushes:
        # brush_tf = dip_brush.getParent()
    name = "_".join(["bdcx"] + dip_brush_tf.name().split("|")
                    [-1].split("_")[1:])

    # logger.debug("name: %s" % name)
    full_name = "brushes|dipCurves|%s" % name
    # logger.debug("full_name: %s" % full_name)

        # if force:
        #     if pm.objExists(full_name):
        #         pm.delete(full_name)
        # if not pm.objExists(full_name):
    grp = cutl.duplicate_grp_with_stroke_curves(src, full_name)
        # else:
        # grp = cutl.ensure_grp_has_stroke_curves(src, pm.PyNode(full_name))

        # lift higher if weight is low so wipes off less paint.
        # wipe_offset = painting_brush.getParent().attr(
        #     "tz").get() - dip_brush.getParent().attr("tz").get()
        # wipe_offset = wipe_offset * lift

    for wipe_curve in grp.getChildren()[1:]:
        wipe_curve.attr("tz").set(wipe_offset)
    pm.PyNode("brushes").attr("visibility").set(False)




def set_stroke_curve_att_from_brush_tip(attribute, mult=1, offset=0):
    sc = attribute.node()
    painting = pm.listConnections(
        sc,
        destination=True,
        source=False,
        shapes=True,
        type="painting")[0]
    index = sc.attr("brushId").get()
    brush = Brush.brush_at_index(painting, index)
    val = (brush.tip * mult) + offset
    attribute.set(val)


# def send_brushes(factory):
#     RL = Robolink()
#     robot = RL.Item('', ITEM_TYPE_ROBOT)
#     base = pm.PyNode("brushes|base")
#     brushes = Brush.used_brushes(factory)
#     for _id in brushes:
#         send_brush(robot, brushes[_id], base)


# def send_brush(robot, brush, base):
#     """Send a node to robodk.

#     If the node is a locator, it becomes a target in robodk
#     and exists in world space. If it is a transform, it is
#     added as a reference frame to the hierarchy. If it is a
#     mesh, we rebuild triangles in robodk and add it to the
#     hierarchy.
#     """
#     RL = Robolink()
#     geo = pm.PyNode(brush.name).getShapes() + base.getShapes()
#     # print brush.name
#     triangles = []
#     for g in geo:
#         points = g.getPoints(space='world')
#         _, vids = g.getTriangles()
#         for vid in vids:
#             triangles.append(
#                 [points[vid].x * 10, points[vid].y * 10, points[vid].z * 10])

#     tool_item = RL.Item(brush.name)
#     if tool_item.Valid():
#         tool_item.Delete()

#     color = uut.shape_color(g)
#     tool_item = robot.AddTool(brush.matrix, brush.name)
#     shape = RL.AddShape(triangles)
#     shape.setColor(list(color))
#     tool_item.AddGeometry(shape, rdk.eye())
#     robot.setPoseTool(tool_item)
#     shape.Delete()

######################################
######################################
######################################
######################################
