
import pymel.core as pm
import stroke_factory_utils as sfu
from brush import Brush
import re
import random
import logging
logger = logging.getLogger('uprising')

 
def get_curve(stroke_curve):
    conns = pm.listConnections(stroke_curve.attr("curve"), s=True, d=False, shapes=True)
    if conns:
        return conns[0]

def get_stroke_curve(curve):
    conns = pm.listConnections(curve.attr("worldSpace[0]"), d=True, s=False, type="strokeCurve")
    if conns:
        return conns[0]

def connect_curve_to_painting(curve, painting_node, **kw):

    connect_to = kw.get("connect_to", None)

    try:
        if painting_node == "mainPaintingShape":
            plane = pm.PyNode("mainProjectionShape")
        elif painting_node == "dipPaintingShape":
            plane = pm.PyNode("dipProjectionShape")
    except pm.MayaNodeError:
        plane = pm.createNode("sketchPlane")

    crv = pm.PyNode(curve)
    if crv.type() == "strokeCurve":
        stroke_curve = crv
    else:
        c_names = curve.name().split("|")
        sc_name = "%s_SC" % curve.name()
        stroke_curve = pm.createNode("strokeCurve", name=sc_name)
        curve.attr("worldSpace[0]") >> stroke_curve.attr("curve")
        plane.attr("worldMatrix[0]") >> stroke_curve.attr("planeMatrix")

        stroke_curve.attr(
            "strokeProfileRamp[0].strokeProfileRamp_FloatValue").set(0.5)
        stroke_curve.attr("brushTiltRamp[0].brushTiltRamp_FloatValue").set(0.5)
        stroke_curve.attr("brushBankRamp[0].brushBankRamp_FloatValue").set(0.5)
        stroke_curve.attr("brushTwistRamp[0].brushTwistRamp_FloatValue").set(0.5)

    if connect_to:
        index = sfu.get_index(painting_node, "strokeCurves.", connect_to)
        stroke_curve.attr("output") >> painting_node.attr(
            "strokeCurves[%d]" % index)

    return stroke_curve


def duplicate_grp_with_stroke_curves(src, full_name):
    logger.debug("src: %s -- full_name: %s " % (src, full_name))
    plane = pm.PyNode("dipProjectionShape")
    shapes =  pm.ls(src,  dag=True,ni=True, shapes=True )
    logger.debug("shapes: %s " % shapes)

    stroke_curves =   pm.listConnections(shapes, destination=True, source=False, type="strokeCurve")
    print stroke_curves
    new_stroke_curves = pm.duplicate(stroke_curves)

    grp = pm.duplicate(src)[0]
    grp.rename(full_name)
    new_shapes =  pm.ls(grp,  dag=True, ni=True, shapes=True )
    for i, shape in enumerate(new_shapes):
        stroke_curve = new_stroke_curves[i]
        shape.attr("worldSpace[0]") >> stroke_curve.attr("curve")
        plane.attr("worldMatrix[0]") >> stroke_curve.attr("planeMatrix")
    return grp

def ensure_grp_has_stroke_curves(src, existing_curves_grp) :
    plane = pm.PyNode("dipProjectionShape")
    src_shapes =  pm.ls(src,  dag=True,ni=True, shapes=True, type="nurbsCurve", v=True)
    shapes =  pm.ls(existing_curves_grp,  dag=True,ni=True, shapes=True, type="nurbsCurve", v=True)
    # should be a 1:1 relationship
    num = len(src_shapes) 
    if not num == len(shapes):
        raise ValueError("Different number of dip curves to source curves for this brush. Equalize or setup manually")
    for src_shape, shape in zip(src_shapes, shapes):
        if not get_stroke_curve(shape):
            new_stroke_curve = pm.duplicate(get_stroke_curve(src_shape))[0]
            shape.attr("worldSpace[0]") >> new_stroke_curve.attr("curve")
            plane.attr("worldMatrix[0]") >> new_stroke_curve.attr("planeMatrix")
    return existing_curves_grp



def generate_brush_dip_curves(force):
    pm.PyNode("brushes").attr("visibility").set(True)
    node = pm.PyNode("dipPaintingShape")
    if not node:
        raise IndexError("Can't find dipPaintingShape")
    brushes = pm.ls(
        "brushes|dipBrushes|bdx*",
        selection=True,
        dag=True,
        leaf=True)
    if not brushes:
        brushes = pm.ls("brushes|dipBrushes|bdx*", dag=True, leaf=True)

    for brush in brushes:
        brush_tf = brush.getParent()
        name = "_".join(["bdcx"] + brush_tf.name().split("|")
                        [-1].split("_")[1:])

        logger.debug("name: %s" % name)
        full_name = "brushes|dipCurves|%s" % name
        logger.debug("full_name: %s" % full_name)
        src = "brushes|dipCurves|defaultSource"

        if force:
            if pm.objExists(full_name):
                pm.delete(full_name)
        if not pm.objExists(full_name):
            grp = duplicate_grp_with_stroke_curves(src, full_name)
        else:
            grp = ensure_grp_has_stroke_curves(src, pm.PyNode(full_name)) 

    # for curve in curves:
    #     cutl.connect_curve_to_painting(curve, node, connect_to="next_available")

    # cutl.generate_brush_dip_curves(dip_painting_node,dip_curves_grp, dip_brushes )


# def update_curve_sf():
#     node = pm.ls(selection=True, dag=True, leaf=True, type="strokeFactory")
#     if not node:
#         raise IndexError("No strokeFactory node selected")
#     curves = pm.ls(selection=True, dag=True, leaf=True, type="nurbsCurve")
#     for curve in curves:
#         update_curve_connections(curve, node[0])


# def update_curve_connections(curve, node):
#     conns = curve.attr("worldSpace[0]").connections(
#         type="strokeFactory", plugs=True)
#     for conn in conns:
#         found = re.compile(r'curves\[([0-9]+)].curve').search(str(conn))
#         if found:
#             index = int(found.group(1))
#             # print "WANT TO CONNECT: %s to index %d" % (curve, index)
#             connect_curve_to_painting(curve, node, connect_to=index)
#         else:
#             print "%s not connected - "

def randomize_ring_rotation(curves):
    for curve in curves:
         rand_int = random.randint(0, 360)
         tf = curve.getParent()
         tf.attr("rz").set(rand_int)


def _assign_random_resource(node, resource_attr, id_attr, set_key):
    r_indices = node.attr(resource_attr).getArrayIndices()
    last = len(r_indices) - 1
    c_indices = node.attr("strokeCurves").getArrayIndices()
    for c in c_indices:
        rand_int = random.randint(0, last)
        index = r_indices[rand_int]
        src_conn = sfu.input_connection( node.attr("strokeCurves[%d]" % c ) )
        if src_conn:
            attr = src_conn.node().attr(id_attr)
            attr.set(index)
        if set_key:
            attr.setKey(value=index)


def assign_random_paints(node, set_key=False):
    _assign_random_resource(node, "paints", "paintId", set_key)


def assign_random_brushes(node, set_key=False):
    _assign_random_resource(node, "brushes", "brushId", set_key)



def get_extent(node, stroke_curve, curve):
    brushId = stroke_curve.attr("brushId").get()
    brush_width = Brush.brush_at_index(node, brushId).width / 10.0 #mm to cm
    curveRadius = get_curve(stroke_curve).getParent().attr("sx").get()
    return (brush_width*0.5) + curveRadius
 

def arrange_rings_gap(node, gap, set_key=False):
    stroke_curves = pm.listConnections(node.attr("strokeCurves"), s=True, d=False)
    curve = get_curve(stroke_curves[0])
    extent = get_extent(node, stroke_curves[0], curve)


    for stroke_curve in stroke_curves[1:]:
        # print "progress: %s" % stroke_curve.name()
        brushId = stroke_curve.attr("brushId").get()
        brush_width = Brush.brush_at_index(node, brushId).width / 10.0 #mm to cm
        next_radius =  (brush_width*0.5) + gap + extent
        curve = get_curve(stroke_curve)
        curve_tf = curve.getParent()
        curve_tf.attr("sx").set(next_radius)
        curve_tf.attr("sy").set(next_radius)
        if set_key:
            curve_tf.attr("sx").setKey(value=next_radius)
            curve_tf.attr("sy").setKey(value=next_radius)
        extent =  next_radius +   (brush_width*0.5)
 


def arrange_rings_spine(node, dist, set_key=False):
    stroke_curves = pm.listConnections(node.attr("strokeCurves"), s=True, d=False)
    curve = get_curve(stroke_curves[0])
    curveRadius = curve.getParent().attr("sx").get()

    for stroke_curve in stroke_curves[1:]:
        curveRadius += dist
        curve = get_curve(stroke_curve)
        curve_tf = curve.getParent()
        curve_tf.attr("sx").set(curveRadius)
        curve_tf.attr("sy").set(curveRadius)
        if set_key:
            curve_tf.attr("sx").setKey(value=curveRadius)
            curve_tf.attr("sy").setKey(value=curveRadius)
 


# setAttr "cShape0_SC1.strokeProfileRamp[0].strokeProfileRamp_Position" 0;
# setAttr "cShape0_SC1.strokeProfileRamp[0].strokeProfileRamp_FloatValue" 0.48;
# setAttr "cShape0_SC1.strokeProfileRamp[0].strokeProfileRamp_FloatValue" 0.5;
# setAttr "cShape0_SC1.brushTiltRamp[0].brushTiltRamp_Position" 0;
# setAttr "cShape0_SC1.brushTiltRamp[0].brushTiltRamp_FloatValue" 0.46;
# setAttr "cShape0_SC1.brushBankRamp[0].brushBankRamp_Position" 0;
# setAttr "cShape0_SC1.brushBankRamp[0].brushBankRamp_FloatValue" 0.44;
# setAttr "cShape0_SC1.brushTwistRamp[0].brushTwistRamp_Position" 0;
# setAttr "cShape0_SC1.brushTwistRamp[0].brushTwistRamp_FloatValue" 0.44;


def propagate_ramp_attribute(att, rangeAtt):
    curves = pm.ls(selection=True, dag=True, leaf=True, type="nurbsCurve", ni=True)
    last_curve = curves[-1]
    last_sc = get_stroke_curve(last_curve)
    if last_sc:
        rangeVals = last_sc.attr(rangeAtt).get()
        indices = last_sc.attr(att).getArrayIndices()
        for curve in curves[:-1]:
            sc = get_stroke_curve(curve)
            sc.attr(rangeAtt).set(rangeVals)
            for index in indices:
                pos_att_str = "%s[%d].%s_Position" % (att, index, att)
                val_att_str = "%s[%d].%s_FloatValue" % (att, index, att)
                pos = last_sc.attr(pos_att_str).get()
                val = last_sc.attr(val_att_str).get()
                sc.attr(pos_att_str).set(pos)
                sc.attr(val_att_str).set(val)









# def propagate_ramp_attribute(brushTiltRam):
#     curves = pm.ls(selection=True, dag=True, leaf=True, type="nurbsCurve", ni=True, ut=True, v=True)

# def propagate_ramp_attribute(brushBankRamp):
#     curves = pm.ls(selection=True, dag=True, leaf=True, type="nurbsCurve", ni=True, ut=True, v=True)

# def propagate_ramp_attribute(brushTwistRamp):
#     curves = pm.ls(selection=True, dag=True, leaf=True, type="nurbsCurve", ni=True, ut=True, v=True)

