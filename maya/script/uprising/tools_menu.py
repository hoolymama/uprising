
import csv
import json
import sys
from random import random

from uprising import brush_utils as butl
from uprising import curve_utils as cutl
import pymel.core as pm
from uprising import stats
from uprising import utils
from uprising.brush import Brush
# from uprising.pov.session.pov_session import PovTestSession
from uprising.paint import Paint 
from uprising import const as k
from uprising import robo


def create():
    menu = pm.menu(label="Tools", tearOff=True)

    pm.menuItem(label="Bake mapped paintIds",
                command=pm.Callback(on_bake_paint_ids))

    pm.menuItem(
        label="Randomize dips",
        command=pm.Callback(
            randomize_dips))

    pm.menuItem(
        label="Zero displacement mesh",
        command=pm.Callback(zero_disp_mesh))

    pm.menuItem(label="Print stats", command=pm.Callback(on_print_stats))
    
    pm.menuItem(
        label="Test Brush/Stroke Angle",
        command=pm.Callback(on_test_brush_stroke_angle))
 


    pm.menuItem(
        label="Print paint and brush csv",
        command=pm.Callback(
            on_print_paint_pot_and_brush_stats,
            "csv"))


    pm.menuItem(
        label="Print Painting Flow SS",
        command=pm.Callback(on_print_painting_flow_ss))

    pm.menuItem(
        label="Show RoboDK",
        command=pm.Callback(robo.show))


    return menu


def on_connect_brushids_to_skeleton():
    painting = pm.PyNode(k.PAINTING_NAME)
    skels = pm.ls(selection=True, type="skeletonStroke")
    if not skels:
        skels = pm.ls(type="skeletonStroke")
    butl.connect_skels(painting, skels)


def _bake_first_paint_id(painting, curve):
    print("Baking curve")
    cc = pm.paintingQuery(painting, cc=True)
    if cc:
        sc = pm.paintingQuery(painting, ci=0, sc=True)
        if sc:
            paint_id = pm.paintingQuery(
                painting, ci=0, si=0, clusterPaintId=True)
            print("paint id is %d" % paint_id)
            stroke_curve = cutl.get_stroke_node(curve)
            stroke_curve.attr("paintId").set(paint_id)


def on_bake_paint_ids():
    painting = pm.PyNode(k.PAINTING_NAME)
    curves = cutl.get_curves_from_painting(painting)
    visible_curves = [
        curve for curve in curves if curve.getParent().attr("visibility").get()]
    print("on_bake_paint_ids")

    for curve in curves:
        cutl.hide_objects(visible_curves)
        cutl.show_objects([curve])

        _bake_first_paint_id(painting, curve)
    cutl.show_objects(visible_curves)


def zero_disp_mesh():
    painting_node = pm.PyNode(k.PAINTING_NAME)
    displace = painting_node.listHistory(type="displaceStrokes")[0]
    
    mesh = pm.listConnections(displace.attr("mesh"), s=True, d=False)[0]
    for v in mesh.vtx:
        pos = v.getPosition(space="object")
        pos.z = 0
        v.setPosition(pos, space="object")


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
    utils.show_in_window(stats.stats(), title="Painting stats")


 


def on_print_paint_pot_and_brush_stats(fmt="json"):
    painting_node = pm.PyNode(k.PAINTING_NAME)
    brushes, paints, pots = list(zip(*(stats.used_pots_paints_and_brushes(painting_node))))

    result = {"brushes": [], "paints": [], "pots": pots}
    
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
    
    utils.show_in_window(result, title="Stats")

 
def randomize_dips():
    dip_paintings = pm.ls("rack|holes|holeRot*|holeTrans|dip_loc|*",
                          dag=True, leaf=True, type="painting"),
    dip_paintings = pm.listRelatives(dip_paintings, parent=True)
    for p in dip_paintings:
        ty = random()+0.5
        rz = (random()*60)-30
        p.attr("ty").set(ty)
        p.attr("rz").set(rz)

    wipe_paintings = pm.ls(
        "rack|holes|holeRot*|holeTrans|wipe_loc|*", dag=True, leaf=True, type="painting"),
    wipe_paintings = pm.listRelatives(wipe_paintings, parent=True)

    main_painting_node = pm.PyNode(k.PAINTING_NAME)

    for p in wipe_paintings:
        brush_id = int(p.split("|")[-1][1:])
        brush = Brush.brush_at_index(main_painting_node, brush_id)
        x_pos = brush.wipe_bar_position
        p.attr("tx").set(x_pos)
        p.attr("ty").set(0.6)





def on_print_painting_flow_ss():

    ptg = pm.PyNode(k.PAINTING_NAME)
    num_clusters = pm.paintingQuery(ptg, cc=True)

    num_continuous_strokes = -1
    header = ["","Red","Green","Blue","","Brush id","Brush Model", "Paint id", "Pot id", "Stroke count"]
    tab = "\t"
    print((tab.join(header)))

    palette = Paint.paints()
    data=[]
    for ci in range(num_clusters):
        num_strokes = pm.paintingQuery(ptg, ci=ci, sc=True)
        #print "num_strokes",num_strokes
        resason =  pm.paintingQuery(ptg, ci=ci, clusterReason=True)
    
        if resason == "tool":
            brush_id = pm.paintingQuery(ptg, ci=ci, clusterBrushId=True)
            paint_id = pm.paintingQuery(ptg, ci=ci, clusterPaintId=True)
            pot_id = pm.paintingQuery(ptg,   ci=ci, clusterPotId=True)

            brush_model = Brush.brush_at_index(ptg, brush_id).model

            col = palette[paint_id].color
        
            if num_continuous_strokes > -1:
                data.append(str(num_continuous_strokes))
                print((tab.join(data)))
            data = [str(s) for s in ["",int(col[0]*255),int(col[1]*255),int(col[2]*255),"",brush_id, brush_model, paint_id, pot_id]]
            num_continuous_strokes = num_strokes
        else:
            num_continuous_strokes += num_strokes
    data.append(str(num_continuous_strokes))
    print(tab.join(data))
            


def on_test_brush_stroke_angle():
    bad_strokes = set()
    ptg = pm.PyNode(k.PAINTING_NAME)
    num_clusters = pm.paintingQuery(ptg, cc=True)
    globalStrokeIndex = 0
    for ci in range(num_clusters):
        num_strokes = pm.paintingQuery(ptg, ci=ci, sc=True)
        for si in range(num_strokes):
            zAxes = utils.to_vector_array(pm.paintingQuery(ptg,ci=ci, si=si, strokeZAxis=True))
            positions = utils.to_point_array(pm.paintingQuery(ptg,ci=ci, si=si, strokePositions=True))
            num_targets = len(positions)
            for ti in range(num_targets):
                if ti < num_targets-1:
                    tan = positions[ti+1]  - positions[ti]
                else:
                    tan = positions[ti] - positions[ti-1]

                tan.normalize()

                zAxes[ti].normalize()
                dot = tan.dot(zAxes[ti])
                if dot > 0:
                    print("Bad angle on stroke %d, target %d" % (globalStrokeIndex, ti))
                    bad_strokes.add(globalStrokeIndex)
            globalStrokeIndex +=1

    print(sorted(list(bad_strokes)))