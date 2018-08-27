
import pymel.core as pm
import stroke_factory_utils as sfu
import re
import random


# def add_curve_to_painting():
#     node = pm.ls(selection=True, dag=True, leaf=True, type="strokeFactory")
#     if not node:
#         raise IndexError("No strokeFactory node selected")
#     curves = pm.ls(selection=True, dag=True, leaf=True, type="nurbsCurve")
#     for curve in curves:
#         connect_curve_to_painting(curve, node[0])


def connect_curve_to_painting(curve, painting_node, **kw):

    connect_to = kw.get("connect_to", None)

    try:
        if painting_node == "mainPaintingShape":
            plane = pm.PyNode("mainProjectionShape")
        elif painting_node == "dipPaintingShape":
            plane = pm.PyNode("dipProjectionShape")
    except pm.MayaNodeError:
        plane = pm.createNode("sketchPlane")

    crv = pm.PyNode(curve)
    if crv.type() == "strokeCurve":
        stroke_curve = crv
    else:
        c_names = curve.name().split("|")
        sc_name = "%s_SC" % curve.name()
        stroke_curve = pm.createNode("strokeCurve", name=sc_name)
        curve.attr("worldSpace[0]") >> stroke_curve.attr("curve")
        plane.attr("worldMatrix[0]") >> stroke_curve.attr("planeMatrix")

        stroke_curve.attr(
            "strokeProfileRamp[0].strokeProfileRamp_FloatValue").set(0.5)
        stroke_curve.attr("brushTiltRamp[0].brushTiltRamp_FloatValue").set(0.5)
        stroke_curve.attr("brushBankRamp[0].brushBankRamp_FloatValue").set(0.5)
        stroke_curve.attr("brushTwistRamp[0].brushTwistRamp_FloatValue").set(0.5)

    if connect_to:
        index = sfu.get_index(painting_node, "strokeCurves.", connect_to)
        stroke_curve.attr("output") >> painting_node.attr(
            "strokeCurves[%d]" % index)

    return stroke_curve


def duplicate_grp_with_stroke_curves(src, full_name):
    plane = pm.PyNode("dipProjectionShape")
    shapes =  pm.ls(src,  dag=True, v=True, ni=True, shapes=True )
    stroke_curves =   pm.listConnections(shapes, destination=True, source=False, type="strokeCurve")
    print stroke_curves
    new_stroke_curves = pm.duplicate(stroke_curves)

    grp = pm.duplicate(src)[0]
    grp.rename(full_name)
    new_shapes =  pm.ls(grp,  dag=True, v=True, ni=True, shapes=True )
    for i, shape in enumerate(new_shapes):
        stroke_curve = new_stroke_curves[i]
        shape.attr("worldSpace[0]") >> stroke_curve.attr("curve")
        plane.attr("worldMatrix[0]") >> stroke_curve.attr("planeMatrix")
    return grp


def generate_brush_dip_curves(force):

    node = pm.PyNode("dipPaintingShape")
    if not node:
        raise IndexError("Can't find dipPaintingShape")
    brushes = pm.ls(
        "brushes|dipBrushes|bdx*",
        selection=True,
        dag=True,
        leaf=True)
    if not brushes:
        brushes = pm.ls("brushes|dipBrushes|bdx*", dag=True, leaf=True)

    for brush in brushes:
        brush_tf = brush.getParent()
        name = "_".join(["bdcx"] + brush_tf.name().split("|")
                        [-1].split("_")[1:])

        full_name = "brushes|dipCurves|%s" % name
        src = "brushes|dipCurves|defaultSource"

        if force:
            if pm.objExists(full_name):
                pm.delete(full_name)
        if not pm.objExists(full_name):
            grp = duplicate_grp_with_stroke_curves(src, full_name)
        else:
            grp = pm.PyNode(full_name)

    # for curve in curves:
    #     cutl.connect_curve_to_painting(curve, node, connect_to="next_available")

    # cutl.generate_brush_dip_curves(dip_painting_node,dip_curves_grp, dip_brushes )


# def update_curve_sf():
#     node = pm.ls(selection=True, dag=True, leaf=True, type="strokeFactory")
#     if not node:
#         raise IndexError("No strokeFactory node selected")
#     curves = pm.ls(selection=True, dag=True, leaf=True, type="nurbsCurve")
#     for curve in curves:
#         update_curve_connections(curve, node[0])


# def update_curve_connections(curve, node):
#     conns = curve.attr("worldSpace[0]").connections(
#         type="strokeFactory", plugs=True)
#     for conn in conns:
#         found = re.compile(r'curves\[([0-9]+)].curve').search(str(conn))
#         if found:
#             index = int(found.group(1))
#             # print "WANT TO CONNECT: %s to index %d" % (curve, index)
#             connect_curve_to_painting(curve, node, connect_to=index)
#         else:
#             print "%s not connected - "


# def _assign_random_resource(node, resource_attr, id_attr, set_key):
#     r_indices = node.attr(resource_attr).getArrayIndices()
#     last = len(r_indices) - 1
#     c_indices = node.attr("curves").getArrayIndices()
#     for c in c_indices:
#         rand_int = random.randint(0, last)
#         index = r_indices[rand_int]
#         attribute = node.attr("curves[%d].%s" % (c, id_attr))
#         src_conn = sfu.input_connection(attribute)
#         if src_conn:
#             attribute = src_conn
#         attribute.set(index)
#         if set_key:
#             attribute.setKey(value=index)


# def assign_random_paints(node, set_key=False):
#     _assign_random_resource(node, "paints", "paintId", set_key)


# def assign_random_brushes(node, set_key=False):
#     _assign_random_resource(node, "brushes", "brushId", set_key)
