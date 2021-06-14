import pymel.core as pm
from brush import Brush


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
            kw["name"].lower(),
            kw["shape"].lower(),
        ]
    )


def create_brush_geo(**kw):

    name = brush_name(**kw)
    try:
        pm.PyNode(name)
        pm.delete(name)
    except BaseException:
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
    geo = brush_xf.getShape()

    brush_xf.rename(name)

    shape_strings = pm.attributeQuery("shape", n=geo, le=True)[0].split(":")
    shape_index = shape_strings.index(kw["shape"].lower())

    geo.attr("physicalId").set(kw["physical_id"])

    if geo.attr("tip2").get() < 0.0001:
        geo.attr("tip2").set(kw["z"])

    geo.attr("bristleHeight").set(kw["bristle_height"])

    geo.attr("shape").set(shape_index)


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


def set_stroke_curve_att_from_brush_tip(attribute, mult=1, offset=0):
    sc = attribute.node()
    painting = pm.listConnections(
        sc, destination=True, source=False, shapes=True, type="painting"
    )[0]
    index = sc.attr("brushId").get()
    brush = Brush.brush_at_index(painting, index)
    val = (brush.tip * mult) + offset
    attribute.set(val)
