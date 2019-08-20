
import pymel.core as pm

from paint import Paint
from brush import Brush
import palette_utils as putl


import logging
logger = logging.getLogger('uprising')


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


def _create_painting_node(which, brush, paint, strokes_node, src_painting):

    ptg_xf = pm.duplicate(src_painting)[0]
    ptg_node = ptg_xf.getShapes()[0]
    ptg_xf.rename("b{:02d}".format(brush.id))

    strokes_node.attr("output") >> ptg_node.attr("strokes[0]")

    brush_node = pm.PyNode(brush.node_name)
    att = "out%sBrush" % which.capitalize()
    if which == "slop":
        att = "outWipeBrush"
    brush_node.attr(att) >> ptg_node.attr("brushes[0]")

    pot_node = pm.PyNode(paint.name if paint else "slop_loc|pot")
    putl.connect_paint_to_node(pot_node, ptg_node, 0)

    loc_name = "%s_loc" % which
    locator = _find_nodes_by_short_name(
        pot_node.getParent().getParent().getChildren(), loc_name)[0]

    pm.parent(ptg_xf, locator, relative=True)

    # connect everything up
    atts = [
        "linearSpeed",
        "angularSpeed",
        "approximationDistance",
        "maxPointToPointDistance",
        "approachDistanceStart",
        "approachDistanceMid",
        "approachDistanceEnd",
        "pointSize",
        "lineLength",
        "lineThickness",
        "displayPivots",
        "displayClusterPath",
        "displayIds",
        "displayParentIds",
        "displayLayerIds",
        "displayBrushIds",
        "displayPaintIds",
        "displayRepeatIds",
        "arrowheadSize",
        "displayTargets",
        "displayContactWidth",
        "stackGap",
        "idDisplayOffset"
    ]
    for att in atts:
        src_painting.attr(att) >> ptg_node.attr(att)

    return ptg_node


def doit():
    node = pm.PyNode("mainPaintingShape")
    brushes = Brush.brushes(node)
    paints = Paint.paints(node)

    dip_stroke_node = pm.PyNode("collectStrokesDip")
    wipe_stroke_node = pm.PyNode("collectStrokesWipe")
    # slop_stroke_node = pm.PyNode("collectStrokesSlop")

    dip_ctrl_painting = pm.PyNode("BRUSHES|curves|dip|dipPaintingControl")
    wipe_ctrl_painting = pm.PyNode("BRUSHES|curves|wipe|wipePaintingControl")
    # slop_ctrl_painting = pm.PyNode("BRUSHES|curves|slop|slopPaintingControl")

    _delete_paintings_under("RACK1_CONTEXT|j1|rack")

    for pkey in paints:
        for bkey in brushes:
            _create_painting_node(
                "dip",
                brushes[bkey],
                paints[pkey],
                dip_stroke_node,
                dip_ctrl_painting)
            _create_painting_node(
                "wipe",
                brushes[bkey],
                paints[pkey],
                wipe_stroke_node,
                wipe_ctrl_painting)

    # for bkey in brushes:
    #     _create_painting_node(
    #         "slop",
    #         brushes[bkey],
    #         None,
    #         slop_stroke_node,
    #         slop_ctrl_painting)
