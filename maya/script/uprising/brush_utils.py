import pymel.core as pm
import stroke_factory_utils as sfu
import curve_utils as cutl
from brush import Brush
import uprising.uprising_util as uutl
import sheets
import copy


def connect_skels(painting, skels):
    # remove existing
    for skel in skels:
        indices = skel.attr("brushWidths").getArrayIndices()
        for i in indices:
            pm.removeMultiInstance(skel.attr("brushWidths[%d]" % i), b=True)
 
    indices = painting.attr("brushes").getArrayIndices()
    for skel in skels:
        for i in indices:
            painting.attr("brushes[%d].brushWidth"%i) >> skel.attr("brushWidths[%d]" % i)
 

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


def brush_name(**kw):
    return "_".join([
        kw["prefix"],
        str(int(kw["id"])),
        "%dmm" % int(kw["unsplay_width"] * 10),
        "p%d" % int(kw["physical_id"]),
        kw["profile"].lower(),
        kw["desc"].lower()
    ])


def create_brush_geo(name, **kw):

    height = kw["height"]
    bristle_height = kw["bristle_height"]
    tip = kw["tip"]
    width = kw["unsplay_width"]
    profile = kw["profile"]
    x_offset = kw["x_offset"]
    y_offset = kw["y_offset"]


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
    geo.attr("tx").set(x_offset)
    geo.attr("ty").set(y_offset)

    return geo


def create_and_connect_single_brush_geo(node, **kw):

    if not node:
        pm.warning("No destination node. Skipping brush gen")
        return

    name = brush_name(**kw)

    tf = create_brush_geo(name, **kw)

    profile_shape = 0
    if kw["profile"].lower() == "round":
        profile_shape = 1


    # print "create_and_connect_single_brush_geo: %s" % kw["retention"]
    connect_brush_to_node(tf, node)
    tf.attr("sfBrushWidth").set(kw["splay_width"])
    tf.attr("sfBrushTip").set(kw["tip"])
    tf.attr("sfBrushShape").set(profile_shape)
    tf.attr("sfBrushPhysicalId").set(kw["physical_id"])
    tf.attr("sfBrushRetention").set(kw["retention"])
    tf.attr("sfBrushTransitionHeight").set(kw["trans_height"])
    tf.attr("sfBrushTransitionPower").set(kw["trans_power"])
    
    pm.parent(tf, kw["parent"])
    return tf


# def create_and_connect_both_brushes_geo(painting_node, dip_node, **kw):

#     painting_kwargs = copy(kw)
#     painting_kwargs["prefix"] = "bpx"
#     painting_kwargs["parent"] = '|brushes|paintingBrushes'
#     painting_brush_tf = create_and_connect_single_brush_geo(
#         painting_node, **painting_kwargs)

#     dip_kwargs = copy(kw)
#     dip_kwargs["prefix"] = "bdx"
#     dip_kwargs["tip"] = kw["dip_tip"]
#     dip_kwargs["parent"] = '|brushes|dipBrushes'
#     dip_brush_tf = create_and_connect_single_brush_geo(dip_node, **dip_kwargs)

#     return (painting_brush_tf, dip_brush_tf)


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
    old_curves = pm.ls("brushes|dipCurves|bdcx*")
    if old_curves:
        pm.delete(old_curves)

    for row in pouch:
        row = [uutl.numeric(s) for s in row]

        common_args = {
            "id": row[0],
            "height": row[2],
            "bristle_height": row[3],
            # "tip": row[4],
            "unsplay_width": row[6],
            "splay_width": row[7],
            "desc": row[8],
            "profile": row[9],
            # "dip_tip": row[10],
            "wipe_tip": row[12],
            "physical_id": row[16],
            "x_offset": row[17],
            "y_offset": row[18],
            "y_offset": row[18],
            "trans_height": row[20],
            "trans_power": row[21]          
        }
        # print "RETENTION: %s" % row[19]
        painting_kwargs = copy.copy(common_args)
        painting_kwargs["prefix"] = "bpx"
        painting_kwargs["tip"] = row[4]
        painting_kwargs["retention"] =row[19]
        painting_kwargs["parent"] = '|brushes|paintingBrushes'
        painting_brush_tf = create_and_connect_single_brush_geo(
            painting_node, **painting_kwargs)

        dip_kwargs = copy.copy(common_args)
        dip_kwargs["prefix"] = "bdx"
        dip_kwargs["tip"] = row[10]
        dip_kwargs["retention"] = 1000
        dip_kwargs["parent"] = '|brushes|dipBrushes'
        dip_brush_tf = create_and_connect_single_brush_geo(
            dip_node, **dip_kwargs)

        wipe_offset = dip_kwargs["tip"] - common_args["wipe_tip"]
        
        generate_brush_dip_curves(painting_brush_tf, dip_brush_tf, wipe_offset)


def generate_brush_dip_curves(
        painting_brush_tf,
        dip_brush_tf,
        wipe_offset):

    src = "brushes|dipCurves|defaultSource"
    pm.PyNode("brushes").attr("visibility").set(True)

    name = "_".join(["bdcx"] + dip_brush_tf.name().split("|")
                    [-1].split("_")[1:])

    full_name = "brushes|dipCurves|%s" % name

    grp = cutl.duplicate_grp_with_stroke_curves(src, full_name, True, ["brushId", "paintId"])

    for wipe_curve in grp.getChildren()[1:]:
        wipe_curve.attr("tz").set(wipe_offset)
    # pm.PyNode("brushes").attr("visibility").set(False)


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
