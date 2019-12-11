import pymel.core as pm

from paint import Paint
from brush import Brush
import palette_utils as putl

import const as k

import logging

logger = logging.getLogger("uprising")


def doit(**kw):

    node = pm.PyNode("mainPaintingShape")
    brushes = Brush.brushes(node)
    paints = Paint.paints(node)

    _delete_setup_nodes("RACK1_CONTEXT|j1|rack")

    for brush_rank in k.BRUSH_RANKS:
        brush_rank["brushes"] = []
        for _id in brush_rank["ids"]:
            brush_rank["brushes"].append(brushes[_id])
        create_paintings(brush_rank, paints)


def _delete_setup_nodes(parent):
    paintings = pm.ls(parent, dag=True, leaf=True, type="painting")
    paintings = pm.listRelatives(paintings, parent=True)
    if paintings:
        pm.delete(paintings)


def _get_curve_strokes(parent):
    curves = pm.ls(parent, dag=True, ni=True, shapes=True, type="nurbsCurve")
    result = []
    for c in curves:
        result += pm.listConnections(
            c.attr("worldSpace[0]"), d=True, s=False, type="curveStroke"
        )


def _find_nodes_by_short_name(nodes, name):
    return [n for n in nodes if n.split("|")[-1] == name]


def create_paintings(brush_rank, paints):

    collector = pm.PyNode("collectStrokesDW_dip_{}".format(brush_rank["name"]))
    dip_grp = _create_paintings("dip", brush_rank, paints)

    collector = pm.PyNode("collectStrokesDW_wipe_{}".format(brush_rank["name"]))
    wipe_grp = _create_paintings("wipe", brush_rank, paints)


def _create_paintings(which, brush_rank, paints):
    painting_template = (
        pm.PyNode("BRUSHES|curves|{}|{}PaintingControl".format(which, which)),
    )
    collector = pm.PyNode("collectStrokesDW_{}_{}".format(which, brush_rank["name"]))

    for p in paints:
        for brush in brush_rank["brushes"]:
            painting_xf = create_painting_node(
                which, brush, paints[p], collector, painting_template
            )


def create_painting_node(which, brush, paint, collector, template):

    ptg_xf = pm.duplicate(template)[0]
    ptg_node = ptg_xf.getShape()
    ptg_xf.rename("b{}".format(brush.id))

    collector.attr("output") >> ptg_node.attr("strokes[0]")

    brush_node = pm.PyNode(brush.node_name)
    att = "out{}Brush".format(which.capitalize())

    brush_node.attr(att) >> ptg_node.attr("brushes[0]")

    pot_node = pm.PyNode(paint.name)
    putl.connect_paint_to_node(pot_node, ptg_node, 0)

    loc_name = "{}_loc".format(which)
    locator = _find_nodes_by_short_name(
        pot_node.getParent().getParent().getChildren(), loc_name
    )[0]

    pm.parent(ptg_xf, locator, relative=True)
    ptg_xf.rename("b{:02d}".format(brush.id))

    # template_shape = template.getShape()
    # connect everything up
    # atts = [
    #     "linearSpeed",
    #     "angularSpeed",
    #     "approximationDistance",
    #     "maxPointToPointDistance",
    #     "approachDistanceStart",
    #     "approachDistanceMid",
    #     "approachDistanceEnd",
    #     "pointSize",
    #     "lineLength",
    #     "lineThickness",
    #     "displayPivots",
    #     "displayClusterPath",
    #     "displayIds",
    #     "displayParentIds",
    #     "displayLayerIds",
    #     "displayBrushIds",
    #     "displayPaintIds",
    #     "displayRepeatIds",
    #     "arrowheadSize",
    #     "displayTargets",
    #     "displayContactWidth",
    #     "stackGap",
    #     "idDisplayOffset",
    # ]
    # for att in atts:
    #     template_shape.attr(att) >> ptg_node.attr(att)

    return ptg_xf

