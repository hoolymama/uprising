import os
import sys
import pymel.core as pm
import setup_dip
import write
import curve_utils as cutl
import brush_utils as butl
import uprising_util as uutl
import json
import csv
from brush import Brush
import palette_utils as putl
import board_utils as bdutl
from random import seed
from random import random

def create():
    menu = pm.menu(label="Tools", tearOff=True)

    pm.menuItem(label="Bake mapped paintIds",
                command=pm.Callback(on_bake_paint_ids))
    pm.menuItem(
        label="Make dips and wipes",
        command=pm.Callback(
            setup_dip.doit))

    pm.menuItem(
        label="Randomize dips",
        command=pm.Callback(
            randomize_dips))

    pm.menuItem(
        label="Zero displacement mesh",
        command=pm.Callback(zero_disp_mesh))

    pm.menuItem(label="Print stats", command=pm.Callback(on_print_stats))
    # pm.menuItem(label="Print stats range",
    #             command=pm.Callback(on_print_stats_range))

    pm.menuItem(
        label="Print paint and brush json",
        command=pm.Callback(
            on_print_paint_and_brush_stats,
            "json"))
    pm.menuItem(
        label="Print paint and brush csv",
        command=pm.Callback(
            on_print_paint_and_brush_stats,
            "csv"))

    # pm.menuItem(label="Connect brushIds to skeletons",
    #             command=pm.Callback(on_connect_brushids_to_skeleton))

    # pm.menuItem(label="Send brushes from selected paintings",
    #             command=pm.Callback(on_send_brushes))

    pm.menuItem(label="Connect texture", subMenu=True)
    pm.menuItem(
        label="to brushId",
        command=pm.Callback(
            on_connect_texture,
            "brushIdTexture"))
    pm.menuItem(
        label="to paintId",
        command=pm.Callback(
            on_connect_texture,
            "paintIdTexture"))
    pm.menuItem(
        label="to strokeSort",
        command=pm.Callback(
            on_connect_texture,
            "strokeSortTexture"))
    pm.menuItem(
        label="to strokeFilter",
        command=pm.Callback(
            on_connect_texture,
            "strokeFilterTexture"))
    pm.menuItem(
        label="to rotation",
        command=pm.Callback(
            on_connect_texture,
            "rotationTexture"))
    pm.menuItem(
        label="to translation",
        command=pm.Callback(
            on_connect_texture,
            "translationTexture"))

    pm.setParent("..", menu=True)

    return menu


def on_connect_brushids_to_skeleton():
    painting = pm.PyNode("mainPaintingShape")
    skels = pm.ls(selection=True, type="skeletonStroke")
    if not skels:
        skels = pm.ls(type="skeletonStroke")
    butl.connect_skels(painting, skels)





def _bake_first_paint_id(painting, curve):
    print "Baking curve"
    cc = pm.paintingQuery(painting, cc=True)
    if cc:
        sc = pm.paintingQuery(painting, ci=0, sc=True)
        if sc:
            paint_id = pm.paintingQuery(
                painting, ci=0, si=0, clusterPaintId=True)
            print "paint id is %d" % paint_id
            stroke_curve = cutl.get_stroke_node(curve)
            stroke_curve.attr("paintId").set(paint_id)


def on_bake_paint_ids():
    painting = pm.PyNode("mainPaintingShape")
    curves = cutl.get_curves_from_painting(painting)
    visible_curves = [
        curve for curve in curves if curve.getParent().attr("visibility").get()]
    print "on_bake_paint_ids"

    for curve in curves:
        cutl.hide_objects(visible_curves)
        cutl.show_objects([curve])

        _bake_first_paint_id(painting, curve)
    cutl.show_objects(visible_curves)


def zero_disp_mesh():
    painting_node = pm.PyNode("mainPaintingShape")
    disp_mesh = pm.listConnections(painting_node.attr(
        "displacementMesh"), s=True, d=False)[0]
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
    tex = pm.ls(
        selection=True,
        textures=True)[0]
    for curve in curves:
        strk = cutl.get_stroke_node(curve)
        tex.attr("outColor") >> strk.attr(attribute)


def on_print_stats():
    painting_node = pm.PyNode("mainPaintingShape")

    result = {}

    # dip_node = pm.PyNode("dipPaintingShape")
    brush_paint_pairs = write.used_paints_and_brushes(painting_node)
    # print "Brush / Paint pairs:"
    result["brush_paint_pairs"] = []
    for brush, paint in brush_paint_pairs:
        result["brush_paint_pairs"].append(
            "brush:{}({:02d})-P({}) & paint:{}({:02d})".format(brush.node_name,brush.id,brush.physical_id,paint.name, paint.id)
        )

    result["brushes_in_use"] = []
    for brush in write.used_brushes(painting_node):
       result["brushes_in_use"].append(
           "brush:{}({:02d})-P({})".format(brush.node_name,brush.id,brush.physical_id)
           )

    result["paints_in_use"] = []
    for paint in write.used_paints(painting_node):
        result["paints_in_use"].append(
            "paint:{}({:02d})".format(paint.name,paint.id)
         )

    result["painting_node"] = write.painting_stats(painting_node)
    uutl.show_in_window(result, title="Painting stats")



def on_print_stats_range():
    min_frame = int(pm.playbackOptions(min=True, query=True))
    max_frame = int(pm.playbackOptions(max=True, query=True))
    painting_node = pm.PyNode("mainPaintingShape")

    for f in range(int(min_frame), int(max_frame + 1)):
        pm.currentTime(f)
        p_stats = write.painting_stats(painting_node)
        print "%d, %d, %f, %d, %d, %f, %d, %d, %f" % tuple(
            [f] + p_stats.values())


def on_print_paint_and_brush_stats(fmt="json"):
    painting_node = pm.PyNode("mainPaintingShape")
    paints, brushes = write.used_paints_and_brushes(painting_node)
    result = {"brushes": [], "paints": []}
    for brush in brushes:
        result["brushes"].append({
            "id": brush.id,
            "physical_id": brush.physical_id,
            "width(mm)": brush.width,
            "shape": brush.shape,
            "retention": brush.retention,
            "tip": brush.tip,
            "name": brush.name
        })

    for paint in paints:
        result["paints"].append({
            "id": paint.id,
            "color_r": paint.color[0],
            "color_g": paint.color[1],
            "color_b": paint.color[2],
            "travel(cm)": paint.travel,
            "name": paint.name
        })

    j = json.dumps(result)
    if fmt == "json":
        print j
    else:
        for key in result:
            data = result[key]
            output = csv.writer(sys.stdout)
            output.writerow(data[0].keys())
            for row in data:
                output.writerow(row.values())


def randomize_dips():
    dip_paintings = pm.ls("rack|holes|holeRot*|holeTrans|dip_loc|*", dag=True, leaf=True, type="painting"),
    dip_paintings = pm.listRelatives(dip_paintings, parent=True)
    for p in dip_paintings:
        ty = random()+0.5
        rz=(random()*60)-30
        p.attr("ty").set(ty)
        p.attr("rz").set(rz)

    wipe_paintings = pm.ls("rack|holes|holeRot*|holeTrans|wipe_loc|*", dag=True, leaf=True, type="painting"),
    wipe_paintings = pm.listRelatives(wipe_paintings, parent=True)

    main_painting_node = pm.PyNode("mainPaintingShape")

    for p in wipe_paintings:
        brush_id = int(p.split("|")[-1][1:])
 
        brush = Brush.brush_at_index(main_painting_node, brush_id)
        x_pos =brush.wipe_bar_position
        p.attr("tx").set(x_pos)



