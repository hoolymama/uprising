import pymel.core as pm
from uprising.paint import Paint
from uprising.brush import Brush
from uprising import const as k



def _used_paints(painting_node):
    ids = pm.paintingQuery(painting_node, toolCombinations=True)[1::3]
    ids = sorted(set(ids))
    paints = Paint.paints()
    return [paints[_id] for _id in ids]

def _used_pot_ids(painting_node):
    ids = pm.paintingQuery(painting_node, toolCombinations=True)[2::3]
    return sorted(set(ids))

def _used_brushes(painting_node):
    ids = pm.paintingQuery(painting_node, toolCombinations=True)[0::3]
    ids = sorted(set(ids))
    brushes = Brush.brushes(painting_node)
    return [brushes[_id] for _id in ids]


def used_pots_paints_and_brushes(painting_node):

    tc = pm.paintingQuery(painting_node, toolCombinations=True)
    
    bids = list(set(tc[::3]))
    paintids = list(set(tc[1::3]))
    _used_pots = list(set(tc[2::3]))
    
    paints = Paint.paints()
    brushes = Brush.brushes(painting_node)
    _used_paints = [paints[_id] for _id in paintids]
    _used_brushes = [brushes[_id] for _id in bids]

    return list(zip(_used_brushes, _used_paints, _used_pots))


def painting_stats(node):
    try:
        cluster_count = pm.paintingQuery(node, cc=True)
    except RuntimeError:
        return
    stroke_count = 0
    reason_result = {"dip": 0, "tool": 0, "tcp": 0}
    travel = []
    for i in range(cluster_count):
        stroke_count += pm.paintingQuery(node, ci=i, sc=True)
        travel.append(pm.paintingQuery(node, ci=i, clusterTravel=True))
        reason = pm.paintingQuery(node, ci=i, clusterReason=True)
        reason_result[reason] += 1
    avg_strokes_per_cluster = stroke_count / float(cluster_count)
    total_travel = sum(travel)
    avg_travel_per_cluster = total_travel / float(cluster_count)

    result = {
        "cluster_count": cluster_count,
        "stroke_count": stroke_count,
        "avg_strokes_per_cluster": avg_strokes_per_cluster,
        "tool_changes": reason_result["tool"],
        "dip_only changes": reason_result["dip"],
        "tcp_only_changes": reason_result["tcp"],
        "total_stroke_travel": total_travel,
        "avg_stroke_travel_per_cluster": avg_travel_per_cluster,
        "linear_speed": pm.PyNode(k.PAINTING_NAME).attr("linearSpeed").get(),
        "angular_speed": pm.PyNode(k.PAINTING_NAME).attr("angularSpeed").get(),
    }
    return result

def light_painting_stats(node):

    stroke_count = pm.lightPaintingQuery(node, sc=True)
    arclen = 0
    target_count = 0
    for i in  range(stroke_count):
        arclen += pm.lightPaintingQuery(node, si=i, strokeArcLength=True)
        target_count += pm.lightPaintingQuery(node, si=i, tc=True)
    
    avg_arclen =  arclen / float(stroke_count)  if stroke_count else 0
    avg_target_count =  target_count / float(stroke_count)   if stroke_count else 0
    result = {
        "stroke_count": stroke_count,
        "total_arc_length": arclen,
        "average_arc_length": avg_arclen,
        "total_target_count": target_count,
        "average_target_count": avg_target_count,
        "linear_speed": node.attr("linearSpeed").get(),
        "angular_speed": node.attr("angularSpeed").get(),
    }
    return result


def stats():
    result = {}

    if pm.objExists("lightPaintingShape"):
        painting_node = pm.PyNode("lightPaintingShape")
        result["light_painting_node"] = light_painting_stats(painting_node)
        return result

    painting_node = pm.PyNode(k.PAINTING_NAME)

    result["brush_paint_pairs"] = []
    for brush, paint, pot_id in used_pots_paints_and_brushes(painting_node):
        result["brush_paint_pairs"].append(
            "brush:{}({:02d})-P({}) & paint:{}({:02d}) in pot({:02d})".format(
                brush.node_name, 
                brush.id, 
                brush.physical_id, 
                paint.name, 
                paint.id,
                pot_id
            )
        )

    result["summary"] = {"brushes_in_use": [], "paints_in_use": [], "pots_in_use": []}

    result["brushes"] = []
    result["paints"] = []


    for brush in _used_brushes(painting_node):
        result["summary"]["brushes_in_use"].append(
            "brush:{}({:02d})-P({})".format(brush.node_name, brush.id, brush.physical_id)
        )


    for paint in _used_paints(painting_node):
        result["summary"]["paints_in_use"].append("paint:{}({:02d})".format(paint.name, paint.id))

    for pot_id in _used_pot_ids(painting_node):
        result["summary"]["pots_in_use"].append("{:02d}".format( pot_id))


    result["painting_node"] = painting_stats(painting_node)

    return result

    

# def stats_per_brush():
#     result = {}

#     painting_node = pm.PyNode(k.PAINTING_NAME)
#     collector_node = pm.PyNode("collectStrokesMain")

#     brush_operator = collector_node.attr("strokeFilterList[3].strokeFilterOperator").get()
#     brush_operand = collector_node.attr("strokeFilterList[3].strokeFilterOperand").get()

#     collector_node.attr("strokeFilterList[3].strokeFilterOperator").set(2)

#     brushes = painting_node.attr("brushes").connections(s=True, d=False)
#     for i, brush in enumerate(brushes):
#         collector_node.attr("strokeFilterList[3].strokeFilterOperand").set(i)
#         brush_stats = painting_stats(painting_node)
#         if brush_stats:
#             result[str(brush)] = brush_stats

#     collector_node.attr("strokeFilterList[3].strokeFilterOperator").set(brush_operator)
#     collector_node.attr("strokeFilterList[3].strokeFilterOperand").set(brush_operand)

#     return result
