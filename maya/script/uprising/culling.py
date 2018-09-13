
import pymel.core as pm


def cull(node, brush_id, paint_id, after):
    node.attr("strokeGate0").set(0)
    node.attr("strokeGate1").set(999999999)
    found = False
    num_clusters = pm.paintingQuery(node, cc=True)
    stroke_count = 0
    if after:
        for i in range(num_clusters):
            stroke_count += pm.paintingQuery(node, ci=i, sc=True)
            b_id = pm.paintingQuery(node, ci=i, cbi=True)
            p_id = pm.paintingQuery(node, ci=i, cpi=True)
            if b_id == brush_id and p_id == paint_id:
                found = True
                next_stroke_id = stroke_count + 1
        if found:
            node.attr("strokeGate1").set(next_stroke_id)
    else:
        for i in range(num_clusters):
            b_id = pm.paintingQuery(node, ci=i, cbi=True)
            p_id = pm.paintingQuery(node, ci=i, cpi=True)
            if b_id == brush_id and p_id == paint_id:
                found = True
                break
            stroke_count += pm.paintingQuery(node, ci=i, sc=True)
        if found:
            node.attr("strokeGate0").set(stroke_count)
    if not found:
        pm.warning("No cluster with given brush and paint IDs %d / %d" % (brush_id, paint_id))