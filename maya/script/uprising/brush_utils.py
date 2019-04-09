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
        indices = skel.attr("brushes").getArrayIndices()
        for i in indices:
            pm.removeMultiInstance(skel.attr("brushes[%d]" % i), b=True)

    indices = painting.attr("brushes").getArrayIndices()
    for skel in skels:
        for i in indices:
            brush = pm.listConnections(
                painting.attr(
                    "brushes[%d]" %
                    i), s=True, d=False)[0]
            brush.attr("outPaintBrush") >> skel.attr("brushes[%d]" % i)


def brush_name(**kw):
    return "_".join([
        kw["prefix"],
        str(int(kw["id"])),
        kw["pouch"],
        "B%d" % int(kw["physical_id"]),
        "%dmm" % int(kw["width"] * 10),
        kw["name"].lower(),
        kw["shape"].lower()
    ])


def create_brush_geo(**kw):

    name = brush_name(**kw)
    try:
        pm.PyNode(name)
        pm.delete(name)
    except:
        pass
    geo = pm.createNode("brushNode")
    geo.getParent().rename(name)

    shape_strings = pm.attributeQuery("shape", n=geo, le=True)[0].split(":")
    shape_index = shape_strings.index(kw["shape"].lower())

    geo.attr("physicalId").set(kw["physical_id"])
    geo.attr("tip").set(kw["x"], kw["y"], kw["z"])

    geo.attr("bristleHeight").set(kw["bristle_height"])
    geo.attr("paintingParam").set(kw["painting_param"])
    geo.attr("dipParam").set(kw["dip_param"])
    geo.attr("wipeParam").set(kw["wipe_param"])

    geo.attr("width").set(kw["width"])
    geo.attr("retention").set(kw["retention"])
    geo.attr("shape").set(shape_index)
    geo.attr("transHeightParam").set(kw["trans_param"])

    return geo


def remove_brush_multi_atts(*nodes):
    for node in nodes:
        indices = node.attr("brushes").getArrayIndices()
        for i in indices:
            pm.removeMultiInstance(node.attr("brushes[%d]" % i), b=True)


def delete_brushes(painting_node):
    try:
        pm.delete("brushes|brushNodes")
    except pm.MayaNodeError:
        pass
    remove_brush_multi_atts(painting_node)


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

def setup_custom_brush_from_sheet(cell):
    service = sheets._get_service()
    result = service.spreadsheets().values().get(
        spreadsheetId=sheets.SHEETS["Measurements"],
        range='Brushes!{}'.format(cell)).execute()

    row = result.get('values', [])[0]
    row = [uutl.numeric(s) for s in row]
    kw = {
            "id":0,
            "physical_id":0,
            "x": row[1],
            "y": row[2],
            "z": row[13],
            "bristle_height": row[4],
            "width": row[5],
            "name": row[6],
            "shape": row[7],
            "painting_param": row[8],
            "dip_param": row[9],
            "wipe_param": row[10],
            "retention": row[11],
            "trans_param": row[12],
            "pouch": "utility",
            "prefix": "bpx"
        }
    return create_brush_geo(**kw)

def setup_probe_from_sheet():
    return setup_custom_brush_from_sheet("A3:N3")

def setup_gripper_from_sheet():
    return setup_custom_brush_from_sheet("A4:N4")


def setup_brushes_from_sheet(pouch_name):
    painting_node = pm.PyNode("mainPaintingShape")
    pouch = sheets.get_resource_by_name(pouch_name, "Brushes")
    data = validate_brush_data(pouch["data"])
    delete_brushes(painting_node)

    try:
        brush_nodes_grp = pm.PyNode("brushes|brushNodes")
    except BaseException:
        g = pm.group(empty=True)
        pm.parent(g, pm.PyNode("brushes"))
        g.rename("brushNodes")
        brush_nodes_grp = pm.PyNode("brushes|brushNodes")

    for i, row in enumerate(data):
        row = [uutl.numeric(s) for s in row]

        kw = {
            "id": i,
            "physical_id": row[0],
            "x": row[1],
            "y": row[2],
            "z": row[13],
            "bristle_height": row[4],
            "width": row[5],
            "name": row[6],
            "shape": row[7],
            "painting_param": row[8],
            "dip_param": row[9],
            "wipe_param": row[10],
            "retention": row[11],
            "trans_param": row[12],
            "pouch": pouch["name"],
            "prefix": "bpx"
        }

        geo = create_brush_geo(**kw)
        pm.parent(geo.getParent(), brush_nodes_grp)
        geo.attr("outPaintBrush") >> painting_node.attr("brushes[%d]" % i)

    connect_skels(painting_node, pm.ls(type="skeletonStroke"))
    


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


# def add_brush_to_painting():
#     node = pm.ls(selection=True, dag=True, leaf=True, type="strokeFactory")[0]
#     brushes = pm.ls(selection=True, dag=True, leaf=True, type="mesh")
#     for brush in brushes:
#         brush_tf = brush.getParent()
#         connect_brush_to_node(brush_tf, node)


# def connect_brush_to_node(brush, node, connect_to="next_available"):
#     index = sfu.get_index(node, "brushes.brushMatrix", connect_to)
#     brush_tf.attr("matrix") >> node.attr("brushes[%d].brushMatrix" % index)
#     whitelist = ["double", "short", "bool", "doubleAngle", "enum"]
#     atts = node.attr("brushes[%d]" % index).getChildren()
#     for att in atts:
#         att_type = att.type()
#         if att_type in whitelist:
#             sfu.create_and_connect_driver(brush_tf, att)
#         elif att_type == "double3":
#             child_atts = att.getChildren()
#             for c_att in child_atts:
#                 sfu.create_and_connect_driver(brush_tf, c_att)
#     return index


# def connect_brush_to_node(brush_tf, node, connect_to="next_available"):
#     index = sfu.get_index(node, "brushes.brushMatrix", connect_to)
#     brush_tf.attr("matrix") >> node.attr("brushes[%d].brushMatrix" % index)
#     whitelist = ["double", "short", "bool", "doubleAngle", "enum"]
#     atts = node.attr("brushes[%d]" % index).getChildren()
#     for att in atts:
#         att_type = att.type()
#         if att_type in whitelist:
#             sfu.create_and_connect_driver(brush_tf, att)
#         elif att_type == "double3":
#             child_atts = att.getChildren()
#             for c_att in child_atts:
#                 sfu.create_and_connect_driver(brush_tf, c_att)
#     return index

    # # prepare for dip curves
    # dip_curves = "brushes|curves|dip"
    # wipe_curves = "brushes|curves|wipe"

    # # pm.makeIdentity(dip_curves_src, t=True, r=True, s=True)
    # # old_curves = pm.ls("brushes|dipCurves|bdcx*")
    # # if old_curves:
    # #     pm.delete(old_curves)

    #     geo = create_brush_geo( **kw)
    #     pm.parent(geo.getParent(), brush_nodes_grp)
    #     geo.attr("outPaintBrush") >> painting_node.attr("brushes[%d]" % i)
    #     geo.attr("outDipBrush") >> dip_node.attr("brushes[%d]" % i)
    #     geo.attr("outWipeBrush") >> wipe_node.attr("brushes[%d]" % i)
    #     ####################
    #     # wipe_offset = dip_kwargs["tip"] - common_args["wipe_tip"]

    #     generate_brush_dip_and_wipe_curves(geo)

# def setup_brushes_from_sheet(painting_node, dip_node, wipe_node, pouch_name):
#     print "%s %s %s %s" % (painting_node, dip_node, wipe_node, pouch_name)
#     # (name, desc, pouch) = sheets.get_resource_by_name(pouch_name, "Brushes")
#     pouch = sheets.get_resource_by_name(pouch_name, "Brushes")

#     data = validate_brush_data(pouch["data"])
#     delete_brushes(painting_node, dip_node, wipe_node)

#     try:
#         brush_nodes_grp = pm.PyNode("brushes|brushNodes")
#     except:
#         g = pm.group(empty=True)
#         pm.parent(g, pm.PyNode("brushes"))
#         g.rename("brushNodes")
#         brush_nodes_grp = pm.PyNode("brushes|brushNodes")

#     # prepare for dip curves
#     dip_curves_src = "brushes|dipCurves|defaultSource"
#     wipe_curves_src = "brushes|wipeCurves|defaultSource"

#     pm.makeIdentity(dip_curves_src, t=True, r=True, s=True)
#     old_curves = pm.ls("brushes|dipCurves|bdcx*")
#     if old_curves:
#         pm.delete(old_curves)

#     for i, row in enumerate(data):
#         row = [uutl.numeric(s) for s in row]

#         kw = {
#             "id": i,
#             "physical_id": row[0],
#             "x": row[1],
#             "y": row[2],
#             "z": row[13],
#             "bristle_height": row[4],
#             "width": row[5],
#             "name": row[6],
#             "shape": row[7],
#             "painting_param": row[8],
#             "dip_param": row[9],
#             "wipe_param": row[10],
#              "retention" : row[11],
#             "trans_param": row[12],
#             "pouch": pouch["name"],
#             "prefix":  "bpx"
#         }

#         geo = create_brush_geo( **kw)
#         pm.parent(geo.getParent(), brush_nodes_grp)
#         geo.attr("outPaintBrush") >> painting_node.attr("brushes[%d]" % i)
#         geo.attr("outDipBrush") >> dip_node.attr("brushes[%d]" % i)
#         geo.attr("outWipeBrush") >> wipe_node.attr("brushes[%d]" % i)
#         ####################
#         # wipe_offset = dip_kwargs["tip"] - common_args["wipe_tip"]

#         generate_brush_dip_and_wipe_curves(geo)

# def generate_brush_dip_and_wipe_curves(brushGeo):


#     brushGeo = pm.PyNode(brushGeo)
#     tf = brushGeo.getParent()
#     pm.PyNode("brushes").attr("visibility").set(True)
#     dup = pm.duplicate("brushes|sourceCurves", rr=True, instanceLeaf=True)[0]
#     name = "_".join(["bdx"] + tf.name().split("|")[-1].split("_")[1:])
#     dup.rename(name)
#     pm.parent(dup, "brushes|curveInstances")
#     # generate_curves_from_source(brushGeo, "brushes|curves", "bdcx")

#     # generate_curves_from_source(brushGeo, "brushes|curves|wipe", "bwcx")


#     # src = "brushes|dipCurves|defaultSource"


#     # name = "_".join(["bdcx"] + dip_brush_tf.name().split("|")
#     #                 [-1].split("_")[1:])

#     # full_name = "brushes|dipCurves|%s" % name

#     # grp = cutl.duplicate_grp_with_stroke_curves(src, full_name, True, ["brushId", "paintId"])

#     # for wipe_curve in grp.getChildren()[1:]:
#     #     wipe_curve.attr("tz").set(wipe_offset)
#     # # pm.PyNode("brushes").attr("visibility").set(False)

# def generate_curves_from_source(brushGeo, grp, prefix ):
#     # grp:  e.g. "brushes|dipCurves"
#     # prefix: e.g. "bdcx"

#     src = "%s|defaultSource|" % grp
#     tf = brushGeo.getParent()
#     name = "_".join([prefix] + tf.name().split("|")[-1].split("_")[1:])
#     full_name = "%s|%s" % (grp, name)
