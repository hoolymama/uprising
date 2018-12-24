
import pymel.core as pm
 
from paint import Paint
from brush import Brush
import curve_utils as cutl
import uprising.uprising_util as uutl
import logging
logger = logging.getLogger('uprising')


# DIP_CURVE_DEFAULTS = [
#     ("followStroke", 0),
#     ("strokeLength", 1000),
#     ("randomOverlapFactor", 0),
#     ("randomLengthFactor", 0),
#     ("repeats", 0),
#     ("overlap", 0)
# ]


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
    grp = cutl.duplicate_grp_with_stroke_curves(src_grp, grp_name, True, ["brushId", "paintId"])
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
        stroke_curve.attr("brushId").set(brush_id)
        stroke_curve.attr("paintId").set(paint_id)

    return grp



def doit():
    print "!!!!!!!!!!!!!!!!! SETUP DIP !!!!!!!!!!!!!!!!!!!!!!!!"
    painting_node = pm.PyNode("mainPaintingShape")
    dip_node = pm.PyNode("dipPaintingShape")

    dip_assembly = uutl.assembly(dip_node)
    logger.debug("dip_assembly: %s" % dip_assembly)

    zpos = dip_assembly.attr("zeroPosition").get()
    dip_assembly.attr("zeroPosition").set(True)

    pm.delete(dip_node.attr("strokes").connections(destination=False, source=True, type="curveStroke"))

    curves_grp = pm.PyNode("%s|curves" % dip_assembly)
    
    logger.debug("curves_grp: %s" % curves_grp)

    pm.delete(curves_grp.getChildren())

    combinations = dip_combinations(painting_node)

    logger.debug("combinations: %s" % combinations)

    # before making new dip curves, delete the old ones
    pm.delete(pm.PyNode("%s|curves|" % dip_assembly ).getChildren() )

 

    for dip in combinations:
        paint_id = combinations[dip]["paint"].id
        brush_id = combinations[dip]["brush"].id
        brush_dip_curve_grp = "brushes|dipCurves|%s" % combinations[dip]["brush"].name.replace("bpx", "bdcx")

        dip_curve_grp = duplicate_and_connect(brush_dip_curve_grp, dip_node, paint_id, brush_id)
        tray = dip_node.attr("paints[%d].paintTravel" % paint_id).connections(source=1,destination=0 )[0]


        pm.parent(dip_curve_grp, tray, relative=True)
        pm.parent(dip_curve_grp, curves_grp , absolute=True)


    dip_assembly.attr("zeroPosition").set(zpos)

 