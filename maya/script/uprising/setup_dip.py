
import pymel.core as pm
import stroke_factory_utils as sfu
from paint import Paint
from brush import Brush
import curve_utils as cutl
import logging
logger = logging.getLogger('uprising')


DIP_CURVE_DEFAULTS = [
    ("forceDip", 1),
    ("followStroke", 0),
    ("strokeLength", 1000),
    ("randomOverlapFactor", 0),
    ("randomLengthFactor", 0),
    ("repeats", 0),
    ("overlap", 0)
]


def dip_combinations(painting_node):

    result = {}
    brushes = Brush.brushes(painting_node)
    paints = Paint.paints(painting_node)
 
    combos = pm.paintingQuery(painting_node, dc=True)

    for i in range(0, len(combos),2):
        brush_id = int(combos[i])
        paint_id = int(combos[i+1])

        key = "p%02d_b%02d" % (paint_id, brush_id)
        result[key] = {
            "brush": brushes[brush_id],
            "paint": paints[paint_id]
        }
    return result


def delete_if_exist(name):
    try:
        pm.delete(name)
    except BaseException:
        pass







def duplicate_and_connect(src_grp, dip_node, paint_id, brush_id):

    grp_name = "dcx_p%02d_b%02d" % (paint_id, brush_id)
    delete_if_exist(grp_name)
    grp = cutl.duplicate_grp_with_stroke_curves(src_grp, grp_name)
    grp.rename(grp_name)
    curves = grp.getChildren()

    for i, curve in enumerate(curves):
        name = "dcx_p%02d_b%02d_c%02d" % (paint_id, brush_id, i)
        curve.rename(name)
        shape = pm.listRelatives(curve, children=True)[0]
        stroke_curve = shape.attr("worldSpace[0]").connections(destination=True, source=False)[0]

        first = (i == 0)
        cutl.connect_curve_to_painting(
            stroke_curve, dip_node, connect_to="next_available")
        
        set_dip_curve_defaults(stroke_curve, first, brush_id, paint_id)
    return grp


def set_dip_curve_defaults(stroke_curve, first, brush_id, paint_id):
    for d in DIP_CURVE_DEFAULTS:
        stroke_curve.attr(d[0]).set(d[1])
    if not first:
        stroke_curve.attr("forceDip").set(0)
    stroke_curve.attr("brushId").set(brush_id)
    stroke_curve.attr("paintId").set(paint_id)




def doit():
    painting_node = pm.PyNode("mainPaintingShape")
    dip_node = pm.PyNode("dipPaintingShape")

    dip_assembly = sfu.assembly(dip_node)
    logger.debug("dip_assembly: %s" % dip_assembly)

    zpos = dip_assembly.attr("zeroPosition").get()
    dip_assembly.attr("zeroPosition").set(True)

    pm.delete(dip_node.attr("strokeCurves").connections(destination=False, source=True, type="strokeCurve"))

    curves_grp = pm.PyNode("%s|curves" % dip_assembly)
    
    logger.debug("curves_grp: %s" % curves_grp)

    pm.delete(curves_grp.getChildren())

    combinations = dip_combinations(painting_node)

    logger.debug("combinations: %s" % combinations)


    for dip in combinations:
        paint_id = combinations[dip]["paint"].id
        brush_id = combinations[dip]["brush"].id
        brush_dip_curve_grp = "brushes|dipCurves|%s" % combinations[dip]["brush"].name.replace("bpx", "bdcx")

        dip_curve_grp = duplicate_and_connect(brush_dip_curve_grp, dip_node, paint_id, brush_id)
        tray = dip_node.attr("paints[%d].paintTravel" % paint_id).connections(source=1,destination=0 )[0]


        pm.parent(dip_curve_grp, tray, relative=True)
        pm.parent(dip_curve_grp, curves_grp , absolute=True)


    dip_assembly.attr("zeroPosition").set(zpos)


# def connect_brushes(src, dest):
#     for index in dest.attr("brushes").getArrayIndices():
#         pm.removeMultiInstance(dest.attr("brushes[%d]" % index), b=True)

#     for index in src.attr("brushes").getArrayIndices():
#         atts = [
#             "brushWidth",
#             "brushMatrix",
#             "brushRetention"]
#         for att in atts:
#             src_attribute = src.attr("brushes[%d].%s" % (index, att))
#             dest_attribute = dest.attr("brushes[%d].%s" % (index, att))
#             src_conn = sfu.input_connection(src_attribute)
#             if src_conn:
#                 src_conn >> dest_attribute
#             else:
#                 src_attribute >> dest_attribute


# def connect_paints(src, dest):
#     for index in dest.attr("paints").getArrayIndices():
#         pm.removeMultiInstance(dest.attr("paints[%d]" % index), b=True)

#     for index in src.attr("paints").getArrayIndices():
#         dest.attr("paints[%d].paintMaxArcLength" % index).set(1000)
#         for att in [
#             "paintColor",
#             "paintName",
#                 "paintOpacity"]:

#             src.attr(
#                 "paints[%d].%s" %
#                 (index, att)) >> dest.attr(
#                 "paints[%d].%s" %
#                 (index, att))
