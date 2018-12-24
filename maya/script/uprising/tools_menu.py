import os
import sys
import pymel.core as pm
import setup_dip
import write
import curve_utils as cutl

import uprising_util as uutl
import json
import csv


def create():
    menu = pm.menu(label="Tools", tearOff=True)
    pm.menuItem( label="Bake mapped paintIds", command=pm.Callback(on_bake_paint_ids) )
    pm.menuItem( label="Make dip combinations", command=pm.Callback(setup_dip.doit) )
    pm.menuItem( label="Zero displacement mesh", command=pm.Callback(zero_disp_mesh) )
    
    pm.menuItem( label="Print stats", command=pm.Callback(on_print_stats) )
    pm.menuItem( label="Print stats range", command=pm.Callback(on_print_stats_range) )

    pm.menuItem( label="Print paint and brush json", command=pm.Callback(on_print_paint_and_brush_stats, "json") )
    pm.menuItem( label="Print paint and brush csv", command=pm.Callback(on_print_paint_and_brush_stats, "csv") )

    
    pm.menuItem( label="Connect texture to brushId", command=pm.Callback(on_connect_texture, "brushIdTexture") )
    pm.menuItem( label="Connect texture to paintId", command=pm.Callback(on_connect_texture, "paintIdTexture") )
    pm.menuItem( label="Connect texture to strokeSort", command=pm.Callback(on_connect_texture, "strokeSortTexture") )
    pm.menuItem( label="Connect texture to strokeFilter", command=pm.Callback(on_connect_texture, "strokeFilterTexture") )
    pm.menuItem( label="Connect texture to rotation", command=pm.Callback(on_connect_texture, "rotationTexture") )
    pm.menuItem( label="Connect texture to translation", command=pm.Callback(on_connect_texture, "translationTexture") )

    # pm.menuItem( label="Conditional select", command=pm.Callback(on_conditional_select) )

    # pm.menuItem( label="Key filters for portrait", command=pm.Callback(on_key_filters) )
    
    return menu

def _bake_first_paint_id(painting, curve):
    print "Baking curve"
    cc = pm.paintingQuery(painting, cc=True)
    if cc:
        sc =  pm.paintingQuery(painting, ci=0, sc=True)
        if sc:
            paint_id =  pm.paintingQuery(painting, ci=0, si=0, clusterPaintId=True)
            print "paint id is %d" % paint_id
            stroke_curve = cutl.get_stroke_node(curve)
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
 
def on_connect_texture(attribute):
    curves = pm.ls(
        selection=True,
        dag=True,
        leaf=True,
        type=("nurbsCurve"),
        ni=True)
    tex =  pm.ls(
        selection=True,
        textures=True)[0]
    for curve in curves:
        strk = cutl.get_stroke_node(curve)
        tex.attr("outColor") >> strk.attr(attribute)

def on_print_stats():
    painting_node = pm.PyNode("mainPaintingShape")
    print "-" * 50
    
    
    # dip_node = pm.PyNode("dipPaintingShape")
    print "Dip combinations:"
    for k in setup_dip.dip_combinations(painting_node):
        print  "%s" % k
    print "\n"

    paints, brushes = write.used_paints_and_brushes(painting_node)
    print "Paints in use:"
    for paint in paints:
        print "%s\t:%s" % (paint.id, paint.name)
    print "\n"

    print "Brushes in use:"
    for brush in brushes:
        print "%s\t:%s" % (brush.id, brush.name)
    print "\n"

    print "Painting node stats:"
    p_stats = write.painting_stats(painting_node)
    for k in p_stats:
        print "%s: %s" % (k, p_stats[k])
    print "\n"

def on_print_stats_range():
    min_frame = int(pm.playbackOptions(min=True, query=True))
    max_frame = int(pm.playbackOptions(max=True, query=True))
    painting_node = pm.PyNode("mainPaintingShape")
   
    for f in range(int(min_frame), int(max_frame+1)):
        pm.currentTime(f)
        p_stats = write.painting_stats(painting_node)
        print "%d, %d, %f, %d, %d, %f, %d, %d, %f" % tuple([f] + p_stats.values())




def on_print_paint_and_brush_stats(fmt="json"):
    painting_node = pm.PyNode("mainPaintingShape")
    paints, brushes = write.used_paints_and_brushes(painting_node)
    result = {"brushes": [], "paints": []}
    for brush in brushes:
        result["brushes"].append({
            "id": brush.id,
            "physical_id": brush.physical_id,
            "width(mm)":  brush.width,
            "shape":  brush.shape,
            "retention":  brush.retention,
            "tip":  brush.tip,
            "name":  brush.name
            })

    for paint in paints:
        result["paints"].append({
            "id": paint.id,
            "color_r": paint.color[0],
            "color_g": paint.color[1],
            "color_b": paint.color[2],
            "travel(cm)":  paint.travel,
            "name":  paint.name
            })

    j = json.dumps(result)
    if fmt=="json":
        print j
    else:
        for key in result:
            data = result[key]
            output = csv.writer(sys.stdout)
            output.writerow(data[0].keys())
            for row in data:
                output.writerow(row.values())
     
     

# def _deactivate_all_nodes(nodes):
#     for node in nodes:
#         node.attr("active").set(0)

# def _activate_all_nodes(nodes):
#     for node in nodes:
#         node.attr("active").set(1)
