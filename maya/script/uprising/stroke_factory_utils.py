import pymel.core as pm
import json
from paint import Paint
from brush import Brush


def add_curve_to_sf():
    node = pm.ls(selection=True, dag=True, leaf=True, type="strokeFactory")[0]
    curves = pm.ls(selection=True, dag=True, leaf=True, type="nurbsCurve")
    print curves
    for curve in curves:
        print curves
        connect_curve_to_node(curve, node)


def get_index(node, connect_to):
    indices = node.attr("curves").getArrayIndices()
    index = 0
    if not indices:
        return index
    if connect_to is "next_available":
        while(True):
            if index not in indices:
                break
            elif not pm.listConnections(node.attr("curves[%d].curve" % index), source=True, destination=False):
                break
            index += 1
    elif connect_to is "at_end":
        index = indices[-1] + 1
    else:
        index = connect_to
    return index


def curve_att_name(att):
    name = att.longName()
    name = name.split(".")[-1]
    return "sf%s%s" % (name[0].upper(), name[1:])


def create_and_connect(curve, att):
    att_type = att.type()
    name = curve_att_name(att)
    val = att.get()
    if not pm.attributeQuery(name, node=curve, exists=True):
        pm.addAttr(curve, longName=name, at=att_type)
        curve.attr(name).setKeyable(True)
        curve.attr(name).set(val)
    curve.attr(name) >> att


def connect_curve_to_node(curve, node, connect_to="next_available"):
    index = get_index(node, connect_to)
    curve.attr("worldSpace[0]") >> node.attr("curves[%d].curve" % index)
    whitelist = ["double", "short", "bool", "doubleAngle"]
    atts = node.attr("curves[%d]" % index).getChildren()
    for att in atts:
        att_type = att.type()
        if att_type in whitelist:
            create_and_connect(curve, att)
        elif att_type == "double3":
            child_atts = att.getChildren()
            for c_att in child_atts:
                create_and_connect(curve, c_att)


def create_stroke_factory():
    node = pm.createNode("strokeFactory")
    plane = pm.createNode("sketchPlane")
    plane_transform = plane.getParent()
    plane_transform.rename("projection")
    plane.attr("worldMatrix[0]") >> node.attr("planeMatrix")
    pos_grp = pm.group(node.getParent(), name="positioner")
    crvs_grp = pm.group(empty=True, name="curves")
    pm.group(plane_transform, pos_grp, crvs_grp, name="strokeFactoryGroup")


def delete_curve_instances(node):
    indices = node.attr("curves").getArrayIndices()
    for i in indices:
        pm.removeMultiInstance(node.attr("curves[%d]" % i), b=True)


def get_dip_curve(dip_factory):
    # first see if one is selected
    curves = pm.ls(selection=True, dag=True, leaf=True, type="nurbsCurve")
    if not curves:
        curves = pm.listConnections(dip_factory.attr("curves"), source=True,
                                    destination=False,
                                    shapes=True)
    if not curves:
        raise TypeError("Need a dip curve")
    
    curves[0].getParent().rename("source_dip_curve")
 
    pm.delete(pm.listRelatives(pm.ls("p??_b*Shape", type="nurbsCurve"), parent=True))

    # make sure this this curve creates a separate cluster,
    # although it should do anyway because either the paint
    # or the brush will have changed. Also set some other sensible
    defaults = [
        ("sfForceDip", 1),
        ("sfFollowStroke", 0),
        ("sfActive", 1),
        ("sfStrokeLength", 1000),
        ("sfRandomOverlapFactor", 0),
        ("sfRandomLengthFactor", 0),
        ("sfRepeatOscillate", 0),
        ("sfRepeatMirror", 0),
        ("sfOverlap", 0)
    ]
    for d in defaults:
        try:
            curves[0].attr(d[0]).set(d[1])
        except:
            pass

    return curves[0]


def setup_dip_factory(painting_factory, dip_factory):

    source_dip_curve = get_dip_curve(dip_factory)



    if not source_dip_curve:
        raise AttributeError(
            "No curves specified. Select or attach a dip curve to the dip factory")

    brushes = Brush.brushes(painting_factory)
    paints = Paint.paints(painting_factory)
    dips = {}  # the combinations that are actually used

    indices = painting_factory.attr("curves").getArrayIndices()
    for index in indices:
        brush_id = painting_factory.attr("curves[%d].brushId" % index).get()
        paint_id = painting_factory.attr("curves[%d].paintId" % index).get()
        key = "p%02d_b%02d" % (paint_id, brush_id)
        dips[key] = {
            "brush": brushes[brush_id],
            "paint": paints[paint_id]
        }

    delete_curve_instances(dip_factory)

    for dip in dips:
        paint_id = dips[dip]["paint"].id
        brush_id = dips[dip]["brush"].id
        curve = pm.duplicate(source_dip_curve)[0]
        curve.rename("p%02d_b%02d" % (paint_id, brush_id))
        curve.attr("sfPaintId").set(paint_id)
        curve.attr("sfBrushId").set(brush_id)
        curve = pm.listRelatives(curve, children=True)[0]
        connect_curve_to_node(curve, dip_factory)

    pm.delete(source_dip_curve.getParent())
    connect_brushes(painting_factory, dip_factory)
    connect_paints(painting_factory, dip_factory)


def connect_brushes(src, dest):
    for index in dest.attr("brushes").getArrayIndices():
        pm.removeMultiInstance(dest.attr("brushes[%d]" % index), b=True)

    for index in src.attr("brushes").getArrayIndices():
        atts = [
            "brushWidth",
            "brushName",
            "brushLift",
            "brushTcp",
            "brushRetention"]
        for att in atts:
            # print "connecting brush att %s %s" % (index, att)
            src.attr(
                "brushes[%d].%s" %
                (index, att)) >> dest.attr(
                "brushes[%d].%s" %
                (index, att))


def connect_paints(src, dest):
    for index in dest.attr("paints").getArrayIndices():
        pm.removeMultiInstance(dest.attr("paints[%d]" % index), b=True)

    for index in src.attr("paints").getArrayIndices():
        for att in [
            "paintColor",
            "paintName",
            "paintOpacity",
                "paintMaxArcLength"]:
           
            src.attr(
                "paints[%d].%s" %
                (index, att)) >> dest.attr(
                "paints[%d].%s" %
                (index, att))

        # pm.removeMultiInstance(dip.attr("curves[%d]" % index), b=True)

    # for dip in sorted(dips.keys()):
    #     paint_id =

    # nodes = pm.listConnections(
    #     factoryNode.attr(
    #         "curves[%d].curve" %
    #         index),
    #     source=True,
    #     destination=False)
    # if nodes:
    #     self.curve_names[index] = str(nodes[0])

 # global proc int attr.findNextNonExistentIndex ( string $plug ) {
#     int $mi[] = `getAttr -mi $plug`;
#     int $siz = `size  $mi`;
#     if (! $siz) return 0;
#     $mi = `sort $mi`; // just in case
#     for ($i=0;$i< $siz;$i++) {
#         if ( $mi[$i] > $i) return $i;
#     }
#     return $siz;
# }
