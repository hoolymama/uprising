import pymel.core as pm
import os
from uprising import props, online
import pymel.core as pm
from uprising import robo
from uprising import const as k


def create():
    menu = pm.menu(label="KR8 Menu", tearOff=True)

    pm.menuItem(label="Add curve group(s)", command=pm.Callback(add_curve_groups))

    pm.menuItem(label="Disconnect curve group(s)", command=pm.Callback(disconnect_curve_groups))

    pm.menuItem(
        label="Create Image Gradient Field", command=pm.Callback(create_gradient_image_field)
    )

    pm.menuItem(label="Test online", command=pm.Callback(test_online))

    pm.menuItem(label="Batch Illustrator", command=pm.Callback(batch_illustrator))

    return menu


def test_online():
    online.test()


def _remove_unconnected_curve_plugs(node):
    for plug in node.attr("curves"):
        conns = pm.listConnections(plug, s=True, d=False)
        if not len(conns):
            pm.removeMultiInstance(plug, b=True)


def add_curve_groups():
    subject_group = pm.PyNode(k.SUBJECT_GROUP)
    group_nodes = pm.ls(sl=True)
    for gn in group_nodes:
        if not pm.PyNode(gn).getParent() == subject_group:
            pm.parent(gn, subject_group, relative=True)
            gn.attr("t").set(0.0, 0.0, -154.0)

    curve_stroke = pm.PyNode(k.SUBJECT_STROKES_NODE)
    _remove_unconnected_curve_plugs(curve_stroke)

    curves = pm.ls(group_nodes, dag=True, shapes=True, v=True, ni=True)
    existing = [p for p in curve_stroke.attr("curves")]
    next_index = existing[-1].index() + 1 if existing else 0
    for curve in curves:
        curve.attr("worldSpace[0]") >> curve_stroke.attr("curves")[next_index]
        next_index += 1


def disconnect_curve_groups():
    curves = pm.ls(sl=True, dag=True, shapes=True, v=True, ni=True)
    curve_stroke = pm.PyNode(k.SUBJECT_STROKES_NODE)
    skipped = []

    conns = pm.listConnections(curves, d=True, s=False, p=True, c=True)
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

    field = pm.createNode("cImgGradField")

    pm.connectDynamic(target, f=field)

    grad = pm.createNode("cImgGradient")
    grad.attr("output") >> field.attr("inputGradientImage")

    for val in [0, 1]:
        for att in ["Position", "FloatValue"]:
            field.attr("magnitudeRemapRamp[{}].magnitudeRemapRamp_{}".format(val, att)).set(val)

    pm.select(field)


def batch_illustrator():

    entries = pm.fileDialog2(
        caption="Choose Adobe Illustrator files",
        okCaption="Import",
        dialogStyle=2,
        fileMode=4,
        fileFilter="*.ai",
        dir=pm.workspace.getPath(),
    )
    if not entries:
        pm.displayWarning("Nothing Selected")
        return

    # prepare choice node
    choice = pm.PyNode(k.SUBJECT_CHOICE)
    conns = pm.listConnections(choice.attr("input"), s=True, d=False)
    for conn in conns:
        pm.delete(conn)

    # delete inputs and plugs
    for plug in choice.attr("input"):
        pm.removeMultiInstance(plug, b=True)

    directives = []
    for i, entry in enumerate(entries):
        frame=i+1
        name = os.path.splitext(os.path.basename(entry))[0]
        node = pm.PyNode(pm.illustratorCurves(ch=False, ifn=entry)[0])
        node.rename("f_{}_{}".format(frame, name))
        directives.append("{}.visibility=frame=={};".format(node, frame))
        # pm.expression(s="{}.visibility=frame=={};".format(frame, node), ae=1, uc="all")
        curve_stroke = hookup_illustrator_curves(node)
        curve_stroke.attr("output") >> choice.attr("input")[i]

    expression = pm.createNode("expression")
    pm.expression(expression, edit=True, s="\n".join(directives))
    pm.playbackOptions(e=True, min=1, max=len(entries))


def hookup_illustrator_curves(illustrator_node):

    max_height = 80
    max_width = 120
    

    # Reposition scale and set pivots
    pm.xform(illustrator_node, cpc=True)
    offset = pm.dt.Vector(pm.xform(illustrator_node, q=True, ws=True, rp=True)) * -1
    pm.xform(illustrator_node, ws=True, r=True, t=offset)
    pm.makeIdentity(illustrator_node, apply=True, t=1, r=1, s=1, n=0, pn=1)
    height = pm.PyNode(illustrator_node).attr("bbsy").get()
    width = pm.PyNode(illustrator_node).attr("bbsx").get()
    scale = min(max_height / height, max_width / width) 
    pm.xform(illustrator_node, ws=True, s=(scale, scale, scale))

    # parent to subject group (-154.0 is offset in Z from camera eye)
    subject_group = pm.PyNode(k.SUBJECT_GROUP)
    pm.parent(illustrator_node, subject_group, relative=True)
    illustrator_node.attr("t").set(0.0, 0.0, -154.0)

    # create stroke node
    curve_stroke = pm.createNode("curveStroke", name="cvstk_{}".format(illustrator_node.name()))
    curve_stroke.attr("strokeLength").set(100000)
    curve_stroke.attr("minimumStrokeAdvance").set(0.3)
    curve_stroke.attr("pivot").set(1)

    # connect illustrator curves to stroke node
    pm.PyNode(k.TARGET_ROTATION_PLANE).attr("worldMatrix[0]") >> curve_stroke.attr("canvasMatrix")
    curves = pm.ls(illustrator_node, dag=True, shapes=True, v=True, ni=True)
    next_index = 0
    for curve in curves:
        curve.attr("worldSpace[0]") >> curve_stroke.attr("curves")[next_index]
        next_index += 1

    return curve_stroke
