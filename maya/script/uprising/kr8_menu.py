
import pymel.core as pm

from uprising import props
import pymel.core as pm
from uprising import robo
 

def create():
    menu = pm.menu(label="KR8 Menu", tearOff=True)

    pm.menuItem(label="Add curve group(s)",
                command=pm.Callback(add_curve_groups))

    pm.menuItem(label="Disconnect curve group(s)",
                command=pm.Callback(disconnect_curve_groups))

    pm.menuItem(label="Create Image Gradient Field",
                command=pm.Callback(create_gradient_image_field))

    return menu

    
def _remove_unconnected_curve_plugs(node):
    for plug in node.attr("curves"):
        conns = pm.listConnections(plug, s=True, d=False)
        if not len(conns):
            pm.removeMultiInstance(plug, b=True)


def add_curve_groups():
    subject_group = pm.PyNode("SUBJECT")
    group_nodes = pm.ls(sl=True)
    for gn in group_nodes:
        if not pm.PyNode(gn).getParent() == subject_group: 
            pm.parent(gn, subject_group, relative=True)

    curve_stroke = pm.PyNode("curveStrokeSubject_01")
    _remove_unconnected_curve_plugs(curve_stroke)

    curves = pm.ls(group_nodes, dag=True, shapes=True, v=True, ni=True)
    existing = [p for p in curve_stroke.attr("curves")]
    next_index = existing[-1].index()+1 if existing else 0
    for curve in curves:
        curve.attr("worldSpace[0]") >> curve_stroke.attr("curves")[next_index]
        next_index += 1

def disconnect_curve_groups():
    curves = pm.ls(sl=True, dag=True, shapes=True, v=True, ni=True)
    curve_stroke = pm.PyNode("curveStrokeSubject_01")
    skipped = []

    conns = pm.listConnections(curves, d=True, s=False, p=True, c=True )
    for conn in conns:
        if conn[1].node() == curve_stroke:
            try:
                conn[0] // conn[1] 
            except:
                skipped.append(conn[0])
    if skipped:
        pm.displayInfo("Skipped: {} attributes".format(len(skipped)))


    _remove_unconnected_curve_plugs(curve_stroke)


def create_gradient_image_field():

    # The thing we'll add the field to
    target = pm.ls(sl=True)[0]

    field=pm.createNode("cImgGradField")

    pm.connectDynamic(target, f=field)

    grad=pm.createNode("cImgGradient") 
    grad.attr("output") >> field.attr("inputGradientImage")

    for val in [0,1]:
        for att in ["Position", "FloatValue"]:
            field.attr("magnitudeRemapRamp[{}].magnitudeRemapRamp_{}".format(val,att)).set(val)

    pm.select(field)
 