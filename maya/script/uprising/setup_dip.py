# import pymel.core as pm

# from paint import Paint
# from brush import Brush
# import palette_utils as putl


# import logging

# logger = logging.getLogger("uprising")


# def get_brush_ranks():
#     node = pm.PyNode("mainPaintingShape")
#     ranks = {}
#     for brush_id in node.attr("brushes").getArrayIndices():
#         brush = Brush.brush_at_index(node, brush_id)
#         m = brush.model
#         if not m in ranks:
#             ranks[m] = []
#         ranks[m].append(brush)
#     return ranks


# def doit(**kw):

#     node = pm.PyNode("mainPaintingShape")
#     paints = Paint.paints(node)

#     _delete_setup_nodes("RACK1_CONTEXT|j1|rack")

#     ranks = get_brush_ranks()
#     for rank in ranks:
#         create_paintings(rank, ranks[rank], paints)


# def _delete_setup_nodes(parent):
#     paintings = pm.ls(parent, dag=True, leaf=True, type="painting")
#     paintings = pm.listRelatives(paintings, parent=True)
#     if paintings:
#         pm.delete(paintings)


# def _get_curve_strokes(parent):
#     curves = pm.ls(parent, dag=True, ni=True, shapes=True, type="nurbsCurve")
#     result = []
#     for c in curves:
#         result += pm.listConnections(
#             c.attr("worldSpace[0]"), d=True, s=False, type="curveStroke"
#         )


# def _find_nodes_by_short_name(nodes, name):
#     return [n for n in nodes if n.split("|")[-1] == name]


# def create_paintings(model, brushes, paints):

#     _create_paintings("dip", model, brushes, paints)

#     _create_paintings("wipe", model, brushes, paints)


# def _create_paintings(which, model, brushes, paints):
#     painting_template = (
#         pm.PyNode("BRUSHES|curves|{}|{}PaintingControl".format(which, which)),
#     )
#     collector = pm.PyNode("collectStrokesDW_{}_bk_{}".format(which, model))

#     for p in paints:
#         for brush in brushes:
#             create_painting_node(
#                 which, brush, paints[p], collector, painting_template)


# def create_painting_node(which, brush, paint, collector, template):

#     ptg_xf = pm.duplicate(template)[0]
#     ptg_node = ptg_xf.getShape()
#     ptg_xf.rename("b{}".format(brush.id))

#     collector.attr("output") >> ptg_node.attr("strokes[0]")

#     brush_node = pm.PyNode(brush.node_name)
#     att = "out{}Brush".format(which.capitalize())

#     brush_node.attr(att) >> ptg_node.attr("brushes[0]")

#     pot_node = pm.PyNode(paint.name)
#     putl.connect_paint_to_node(pot_node, ptg_node, 0)

#     loc_name = "{}_loc".format(which)
#     locator = _find_nodes_by_short_name(
#         pot_node.getParent().getParent().getChildren(), loc_name
#     )[0]

#     pm.parent(ptg_xf, locator, relative=True)
#     ptg_xf.rename("b{:02d}".format(brush.id))

#     return ptg_xf
