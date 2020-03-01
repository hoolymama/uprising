import pymel.core as pm
from paint import Paint
from brush import Brush


def _used_paints(painting_node):
    ids = pm.paintingQuery(painting_node, dc=True)[1::2]
    ids = sorted(set(ids))
    paints = Paint.paints(painting_node)
    return [paints[_id] for _id in ids]


def _used_brushes(painting_node):
    ids = pm.paintingQuery(painting_node, dc=True)[0::2]
    ids = sorted(set(ids))
    brushes = Brush.brushes(painting_node)
    return [brushes[_id] for _id in ids]


def used_paints_and_brushes(painting_node):
    dc = pm.paintingQuery(painting_node, dc=True)
    bids = dc[::2]
    pids = dc[1::2]
    paints = Paint.paints(painting_node)
    brushes = Brush.brushes(painting_node)
    _used_paints = [paints[_id] for _id in pids]
    _used_brushes = [brushes[_id] for _id in bids]
    return zip(_used_brushes, _used_paints)


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
    }
    return result


def stats():
    result = {}

    painting_node = pm.PyNode("mainPaintingShape")
    brush_paint_pairs = used_paints_and_brushes(painting_node)
    result["brush_paint_pairs"] = []
    for brush, paint in brush_paint_pairs:
        result["brush_paint_pairs"].append(
            "brush:{}({:02d})-P({}) & paint:{}({:02d})".format(
                brush.node_name, brush.id, brush.physical_id, paint.name, paint.id
            )
        )

    result["brushes_in_use"] = []
    for brush in _used_brushes(painting_node):
        result["brushes_in_use"].append(
            "brush:{}({:02d})-P({})".format(
                brush.node_name, brush.id, brush.physical_id
            )
        )

    result["paints_in_use"] = []
    for paint in _used_paints(painting_node):
        result["paints_in_use"].append("paint:{}({:02d})".format(paint.name, paint.id))

    result["painting_node"] = painting_stats(painting_node)
    return result


def stats_per_brush():
    result = {}

    painting_node = pm.PyNode("mainPaintingShape")
    collector_node = pm.PyNode("collectStrokesMain")

    brush_operator = collector_node.attr("strokeFilterList[3].strokeFilterOperator").get()
    brush_operand = collector_node.attr("strokeFilterList[3].strokeFilterOperand").get()

    collector_node.attr("strokeFilterList[3].strokeFilterOperator").set(2)

    brushes = painting_node.attr("brushes").connections(s=True, d=False)
    for i, brush in enumerate(brushes):
        collector_node.attr("strokeFilterList[3].strokeFilterOperand").set(i)
        brush_stats = painting_stats(painting_node)
        if brush_stats:
            result[str(brush)] = brush_stats

    collector_node.attr("strokeFilterList[3].strokeFilterOperator").set(brush_operator)
    collector_node.attr("strokeFilterList[3].strokeFilterOperand").set(brush_operand)

    return result
