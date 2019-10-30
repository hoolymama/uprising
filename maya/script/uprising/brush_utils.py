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
                painting.attr("brushes[%d]" % i), s=True, d=False
            )[0]
            brush.attr("outPaintBrush") >> skel.attr("brushes[%d]" % i)


def brush_name(**kw):
    return "_".join(
        [
            kw["prefix"],
            str(int(kw["id"])),
            kw["pouch"],
            "B%d" % int(kw["physical_id"]),
            "%dmm" % int(kw["width"] * 10),
            kw["name"].lower(),
            kw["shape"].lower(),
        ]
    )


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
    geo.attr("contactPower").set(kw["contact_power"])
    geo.attr("customId").set(kw["physical_id"])

    return geo


def reset_brush(brush_xf, **kw):
    name = brush_name(**kw)
    brush_xf.rename(name)
    geo = brush_xf.getShape()

    shape_strings = pm.attributeQuery("shape", n=geo, le=True)[0].split(":")
    shape_index = shape_strings.index(kw["shape"].lower())

    geo.attr("physicalId").set(kw["physical_id"])
    geo.attr("tip").set(kw["x"], kw["y"], kw["z"])

    geo.attr("bristleHeight").set(kw["bristle_height"])

    geo.attr("paintingParam").set(kw["painting_param"])
    geo.attr("dipParam").set(kw["dip_param"])
    geo.attr("wipeParam").set(kw["wipe_param"])

    geo.attr("width").set(kw["width"])
    geo.attr("shape").set(shape_index)

    # geo.attr("retention").set(kw["retention"])
    # geo.attr("transHeightParam").set(kw["trans_param"])
    # geo.attr("contactPower").set(kw["contact_power"])
    # geo.attr("customId").set(kw["physical_id"])


def remove_brush_multi_atts(*nodes):
    for node in nodes:
        indices = node.attr("brushes").getArrayIndices()
        for i in indices:
            pm.removeMultiInstance(node.attr("brushes[%d]" % i), b=True)


def delete_brushes(painting_node):
    try:
        brushNodes = pm.ls("BRUSHES|brushNodes|*")
        if brushNodes:
            pm.delete(brushNodes)
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
    result = (
        service.spreadsheets()
        .values()
        .get(
            spreadsheetId=sheets.SHEETS["Measurements"], range="Brushes!{}".format(cell)
        )
        .execute()
    )

    row = result.get("values", [])[0]
    row = [uutl.numeric(s) for s in row]
    kw = {
        "id": 0,
        "physical_id": 0,
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
        "contact_power": 1,
        "pouch": "utility",
        "prefix": "bpx",
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
        brush_nodes_grp = pm.PyNode("BRUSHES|brushNodes")
    except BaseException:
        g = pm.group(empty=True)
        pm.parent(g, pm.PyNode("brushes"))
        g.rename("brushNodes")
        brush_nodes_grp = pm.PyNode("BRUSHES|brushNodes")

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
            "contact_power": row[14],
            "pouch": pouch["name"],
            "prefix": "bpx",
        }

        geo = create_brush_geo(**kw)
        pm.parent(geo.getParent(), brush_nodes_grp)
        geo.attr("outPaintBrush") >> painting_node.attr("brushes[%d]" % i)

    # connect_skels(painting_node, pm.ls(type="skeletonStroke"))


def apply_brush_values_from_sheet(pouch_name):
    painting_node = pm.PyNode("mainPaintingShape")
    pouch = sheets.get_resource_by_name(pouch_name, "Brushes")
    data = validate_brush_data(pouch["data"])

    brush_nodes_grp = pm.PyNode("BRUSHES|brushNodes")

    num_brushes_in_sheet = len(data)
    brushes_in_scene = brush_nodes_grp.getChildren()

    num_brushes_in_scene = len(brushes_in_scene)
    if num_brushes_in_scene != num_brushes_in_sheet:
        raise ValueError("Different number of brushes in sheet to scene")

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
            "contact_power": row[14],
            "pouch": pouch["name"],
            "prefix": "bpx",
        }

        geo = reset_brush(brushes_in_scene[i], **kw)
        # pm.parent(geo.getParent(), brush_nodes_grp)
        # geo.attr("outPaintBrush") >> painting_node.attr("brushes[%d]" % i)

    # connect_skels(painting_node, pm.ls(type="skeletonStroke"))


def set_stroke_curve_att_from_brush_tip(attribute, mult=1, offset=0):
    sc = attribute.node()
    painting = pm.listConnections(
        sc, destination=True, source=False, shapes=True, type="painting"
    )[0]
    index = sc.attr("brushId").get()
    brush = Brush.brush_at_index(painting, index)
    val = (brush.tip * mult) + offset
    attribute.set(val)
