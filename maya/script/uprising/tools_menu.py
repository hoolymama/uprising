import os
import pymel.core as pm
import setup_dip
import curve_utils as cutl
import images
from paint import Paint





def create():
    menu = pm.menu(label="Tools", tearOff=True)
    pm.menuItem( label="Bake mapped paintIds", command=pm.Callback(on_bake_paint_ids) )
    pm.menuItem( label="Make dip combinations", command=pm.Callback(setup_dip.doit) )
    pm.menuItem( label="Zero displacement mesh", command=pm.Callback(zero_disp_mesh) )
    pm.menuItem( label="Print stats", command=pm.Callback(on_print_stats) )
    
    return menu

def _bake_first_paint_id(painting, curve):
    print "Baking curve"
    cc = pm.paintingQuery(painting, cc=True)
    if cc:
        sc =  pm.paintingQuery(painting, ci=0, sc=True)
        if sc:
            paint_id =  pm.paintingQuery(painting, ci=0, si=0, clusterPaintId=True)
            print "paint id is %d" % paint_id
            stroke_curve = cutl.get_stroke_curve(curve)
            stroke_curve.attr("paintId").set(paint_id)

def on_bake_paint_ids():
    painting = pm.PyNode("mainPaintingShape")
    curves = cutl.get_curves_from_painting(painting)
    visible_curves = [curve for curve in curves if curve.getParent().attr("visibility").get()]
    print "on_bake_paint_ids"

    for curve in curves:
        cutl.hide_objects( visible_curves)
        cutl.show_objects([curve])

        _bake_first_paint_id(painting, curve)
    cutl.show_objects(visible_curves)


def zero_disp_mesh():
    painting_node = pm.PyNode("mainPaintingShape")
    disp_mesh = pm.listConnections(painting_node.attr("displacementMesh"), s=True, d=False)[0]
    for v in disp_mesh.vtx:
        pos = v.getPosition(space="world")
        pos.z = 0
        v.setPosition(pos, space="world")

def _used_paints(painting_node):
    ids = pm.paintingQuery(painting_node, dc=True)[1::2]
    ids = sorted(set(ids))
    paints = Paint.paints(painting_node)
    return [paints[_id] for _id in ids]

def _painting_stats(node):
    cluster_count = pm.paintingQuery(node, cc=True)
    stroke_count = 0
    reason_result = {"dip": 0, "tool": 0, "tcp": 0}
    for i in range(cluster_count):
        stroke_count += pm.paintingQuery(node, ci=i, sc=True)
        reason = pm.paintingQuery(node, ci=i, clusterReason=True)
        reason_result[reason] += 1
    strokes_per_cluster = stroke_count / float(cluster_count)

    result = {
        "Cluster count": cluster_count,
        "Stroke count": stroke_count,
        "Strokes per cluster": strokes_per_cluster,
        "Tool changes": reason_result["tool"],
        "Dip only changes": reason_result["dip"],
        "Tcp only changes": reason_result["tcp"]
    }
    return result



def on_print_stats():
    painting_node = pm.PyNode("mainPaintingShape")
    print "-" * 50
    print "Painting node stats:"
    painting_stats = _painting_stats(painting_node)
    for k in painting_stats:
        print "%s: %s" % (k, painting_stats[k])
    print "\n"
    
    # dip_node = pm.PyNode("dipPaintingShape")
    print "Dip combinations:"
    for k in setup_dip.dip_combinations(painting_node):
        print  "%s" % k
    print "\n"

    paints_in_use = _used_paints(painting_node)
    print "Paints in use:"
    for paint in paints_in_use:
        print "%s\t:%s" % (paint.id, paint.name)
    print "\n"

