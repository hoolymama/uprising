
import pymel.core as pm

from paint import Paint
from brush import Brush
import palette_utils as putl

 
import logging
logger = logging.getLogger('uprising')


 


def dip_combination_ids():
    painting_node = pm.PyNode("mainPaintingShape")
    result = []
    combos = pm.paintingQuery(painting_node, dc=True)
    for i in range(0, len(combos), 2):
        result.append(
            {
                "brush": int(combos[i]),
                "paint": int(combos[i + 1])
            }
        )
    return result


def dip_combinations():

    painting_node = pm.PyNode("mainPaintingShape")
    result = {}

    brushes = Brush.brushes(painting_node)
    paints = Paint.paints(painting_node)

    combos = pm.paintingQuery(painting_node, dc=True)


    for i in range(0, len(combos), 2):
        brush_id = int(combos[i])
        paint_id = int(combos[i + 1])
        key = "p%02d_b%02d" % (paint_id, brush_id)

        try:
            b = brushes[brush_id]
            p = paints[paint_id]
        except KeyError:
            raise KeyError("Bad Brush or Paint ID")

        result[key] = {
            "brush": b,
            "paint": p
        }
    return result


def delete_if_exist(name):
    try:
        pm.delete(name)
    except BaseException:
        pass
 


def _get_curve_strokes(parent):
    curves = pm.ls(parent, dag=True, ni=True, shapes=True, type="nurbsCurve")
    result = []
    for c in curves:
        result += pm.listConnections(
            c.attr("worldSpace[0]"),
            d=True,
            s=False,
            type="curveStroke")


def _delete_paintings_under(parent):
    paintings = pm.ls(
        parent,
        dag=True,
        leaf=True,
        type="painting")

    paintings = pm.listRelatives(paintings, parent=True)
    if paintings:
        pm.delete(paintings)


def _find_nodes_by_short_name(nodes, name):
    return [n for n in nodes if n.split("|")[-1] == name]


def _create_painting_node(which, brush, paint, strokes_node):
    ptg_node = pm.createNode("painting")
    ptg_node.attr("linearSpeed").set(100)
    ptg_node.attr("angularSpeed").set(70.000)
    ptg_node.attr("approximationDistance").set(2)
    ptg_node.attr("maxPointToPointDistance").set(15)
    ptg_node.attr("approachDistanceStart").set(20)
    ptg_node.attr("approachDistanceMid").set(2.5)
    ptg_node.attr("approachDistanceEnd").set(5)

    ptg_node.attr("pointSize").set(1)
    ptg_node.attr("lineLength").set(1)
    ptg_node.attr("lineThickness").set(1)
    ptg_node.attr("displayPivots").set(0)
    ptg_node.attr("displayClusterPath").set(0)
    ptg_node.attr("displayIds").set(0)
    ptg_node.attr("displayParentIds").set(0)
    ptg_node.attr("displayLayerIds").set(0)
    ptg_node.attr("displayBrushIds").set(0)
    ptg_node.attr("displayPaintIds").set(0)
    ptg_node.attr("displayRepeatIds").set(0)
    ptg_node.attr("arrowheadSize").set(0)

    # ptg_name = "p{:02d}_b{:02d}_{}".format(paint.id, brush.id, which)
    ptg_name = "b{:02d}".format(brush.id)
    ptg_xf = ptg_node.getParent()
    ptg_xf.rename(ptg_name)

    strokes_node.attr("output") >> ptg_node.attr("strokes[0]")

    brush_node = pm.PyNode(brush.node_name)
    att = "out%sBrush" % which.capitalize()
    brush_node.attr(att) >> ptg_node.attr("brushes[0]")

    pot_node = pm.PyNode(paint.name)
    putl.connect_paint_to_node(pot_node, ptg_node, 0)

    loc_name = "%s_loc" % which
    locator = _find_nodes_by_short_name(
        pot_node.getParent().getParent().getChildren(), loc_name)[0]

    pm.parent(ptg_xf, locator, relative=True)

    return ptg_node


def doit():
    node = pm.PyNode("mainPaintingShape")
    brushes = Brush.brushes (node)
    paints = Paint.paints(node)

    dip_stroke_node = pm.PyNode("collectStrokesDip")
    wipe_stroke_node = pm.PyNode("collectStrokesWipe")
    
    _delete_paintings_under("rack1")



    for pkey in paints:
        for bkey in brushes:
            dip_ptg_node = _create_painting_node("dip", brushes[bkey], paints[pkey], dip_stroke_node)
            wipe_ptg_node = _create_painting_node("wipe", brushes[bkey], paints[pkey], wipe_stroke_node)
 




 

# def doit():
#     print "!!!!!!!!!!!!!!!!! SETUP DIP & WIPE !!!!!!!!!!!!!!!!!!!!!!!!"
#     # painting_node =
#     combinations = dip_combinations(pm.PyNode("mainPaintingShape"))

#     dip_stroke_node = pm.PyNode("collectStrokesDip")
#     wipe_stroke_node = pm.PyNode("collectStrokesWipe")

#     _delete_paintings_under("rack1")

#     for combo in combinations:
#         b = combinations[combo]["brush"]
#         p = combinations[combo]["paint"]

#         dip_ptg_node = _create_painting_node("dip", b, p, dip_stroke_node)
#         wipe_ptg_node = _create_painting_node("wipe", b, p, wipe_stroke_node)

