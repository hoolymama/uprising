
import pymel.core as pm
import stroke_factory_utils as sfu
from brush import Brush
import re
import math
import random
import logging

from uprising.sequence import Sequence
logger = logging.getLogger('uprising')


def get_painting(curve):
    if curve.type() != "nurbsCurve":
        curve = pm.ls(
            curve, dag=True,
            ni=True,
            shapes=True,
            type="nurbsCurve")[0]

    paintings = pm.listHistory(curve, future=True, type="painting")
    return paintings[0]


def get_curve(stroke_curve):
    conns = pm.listConnections(
        stroke_curve.attr("curve"),
        s=True,
        d=False,
        shapes=True)
    if conns:
        return conns[0]


def get_stroke_curve(curve):
    if curve.type() != "nurbsCurve":
        curve = pm.ls(
            curve, dag=True,
            ni=True,
            shapes=True,
            type="nurbsCurve")[0]

    conns = pm.listConnections(
        curve.attr("worldSpace[0]"),
        d=True,
        s=False,
        type="strokeCurve")
    if conns:
        return conns[0]


def connect_curve_to_painting(curve, painting_node, **kw):

    connect_to = kw.get("connect_to", None)

    crv = pm.PyNode(curve)
    if crv.type() == "strokeCurve":
        stroke_curve = crv
    else:
        stroke_curve = get_stroke_curve(curve)
        if not stroke_curve:
            c_names = curve.name().split("|")
            sc_name = "%s_SC" % curve.name()
            stroke_curve = pm.createNode("strokeCurve", name=sc_name)
            curve.attr("worldSpace[0]") >> stroke_curve.attr("curve")

            stroke_curve.attr(
                "strokeProfileRamp[0].strokeProfileRamp_FloatValue").set(0.5)
            stroke_curve.attr(
                "brushTiltRamp[0].brushTiltRamp_FloatValue").set(0.5)
            stroke_curve.attr(
                "brushBankRamp[0].brushBankRamp_FloatValue").set(0.5)
            stroke_curve.attr(
                "brushTwistRamp[0].brushTwistRamp_FloatValue").set(0.5)

    # plane.attr("worldMatrix[0]") >> stroke_curve.attr("planeMatrix")
    # print "connect_to: %s" % connect_to
    if connect_to:
        index = sfu.get_index(painting_node, "strokeCurves.", connect_to)
        stroke_curve.attr("output") >> painting_node.attr(
            "strokeCurves[%d]" % index)

    return stroke_curve


def duplicate_curve_to_painting(curve):
    node = get_painting(curve)
    xf = curve.getParent()
    stroke_curve = get_stroke_curve(curve)
    new_xf = pm.duplicate(xf)[0]
    new_shape = new_xf.getChildren()[0]
    new_sc = pm.duplicate(stroke_curve)[0]
    new_shape.attr("worldSpace[0]") >> new_sc.attr("curve")
    connect_curve_to_painting(new_sc, node, connect_to="next_available")
    return (new_xf, new_shape, new_sc)


def duplicate_grp_with_stroke_curves(src, full_name):
    logger.debug("src: %s -- full_name: %s " % (src, full_name))
    plane = pm.PyNode("dipProjectionShape")
    shapes = pm.ls(src, dag=True, ni=True, shapes=True)
    logger.debug("shapes: %s " % shapes)

    stroke_curves = pm.listConnections(
        shapes,
        destination=True,
        source=False,
        type="strokeCurve")
    # print stroke_curves
    new_stroke_curves = pm.duplicate(stroke_curves)

    grp = pm.duplicate(src)[0]
    grp.rename(full_name)
    new_shapes = pm.ls(grp, dag=True, ni=True, shapes=True)
    for i, shape in enumerate(new_shapes):
        stroke_curve = new_stroke_curves[i]
        shape.attr("worldSpace[0]") >> stroke_curve.attr("curve")
        # plane.attr("worldMatrix[0]") >> stroke_curve.attr("planeMatrix")
    return grp


def ensure_grp_has_stroke_curves(src, existing_curves_grp):
    plane = pm.PyNode("dipProjectionShape")
    src_shapes = pm.ls(
        src,
        dag=True,
        ni=True,
        shapes=True,
        type="nurbsCurve")
    shapes = pm.ls(
        existing_curves_grp,
        dag=True,
        ni=True,
        shapes=True,
        type="nurbsCurve")
    # should be a 1:1 relationship
    num = len(src_shapes)
    if not num == len(shapes):
        raise ValueError(
            "Different number of dip curves to source curves for this brush. Equalize or setup manually")
    for src_shape, shape in zip(src_shapes, shapes):
        if not get_stroke_curve(shape):
            new_stroke_curve = pm.duplicate(get_stroke_curve(src_shape))[0]
            shape.attr("worldSpace[0]") >> new_stroke_curve.attr("curve")
            # plane.attr("worldMatrix[0]") >> new_stroke_curve.attr(
            #     "planeMatrix")
    return existing_curves_grp


def generate_brush_dip_curves(lift, force):

    src = "brushes|dipCurves|defaultSource"

    # tmp - ensure freeze_transforms on src so we can reliably offset them
    pm.makeIdentity(src, t=True, r=True, s=True)

    pm.PyNode("brushes").attr("visibility").set(True)
    node = pm.PyNode("dipPaintingShape")
    if not node:
        raise IndexError("Can't find dipPaintingShape")
    # brushes = pm.ls(
    #     "brushes|dipBrushes|bdx*",
    #     selection=True,
    #     dag=True,
    #     leaf=True)
    # if not brushes:
    dip_brushes = pm.ls("brushes|dipBrushes|bdx*", dag=True, leaf=True)
    painting_brushes = pm.ls(
        "brushes|paintingBrushes|bpx*",
        dag=True,
        leaf=True)

    brushes = zip(dip_brushes, painting_brushes)

    for dip_brush, painting_brush in brushes:
        brush_tf = dip_brush.getParent()
        name = "_".join(["bdcx"] + brush_tf.name().split("|")
                        [-1].split("_")[1:])

        logger.debug("name: %s" % name)
        full_name = "brushes|dipCurves|%s" % name
        logger.debug("full_name: %s" % full_name)

        if force:
            if pm.objExists(full_name):
                pm.delete(full_name)
        if not pm.objExists(full_name):
            grp = duplicate_grp_with_stroke_curves(src, full_name)
        else:
            grp = ensure_grp_has_stroke_curves(src, pm.PyNode(full_name))

        # lift higher if weight is low so wipes off less paint.
        wipe_offset = painting_brush.getParent().attr(
            "tz").get() - dip_brush.getParent().attr("tz").get()
        wipe_offset = wipe_offset * lift

        for wipe_curve in grp.getChildren()[1:]:
            wipe_curve.attr("tz").set(wipe_offset)

    # for curve in curves:
    #     cutl.connect_curve_to_painting(curve, node, connect_to="next_available")

    # cutl.generate_brush_dip_curves(dip_painting_node,dip_curves_grp, dip_brushes )


def remove_unconnected_curve_plugs(painting):

    stroke_curves = pm.listConnections(
        painting, s=True, d=False, type="strokeCurve")
    for sc in stroke_curves:
        conns = pm.listConnections(
            sc, d=True,
            s=False,
            type="painting",
            c=True, p=True)
       # disconnect all conns after first
        for conn in conns[1:]:
            conn[0] // conn[1]

    for i in painting.attr("strokeCurves").getArrayIndices():
        plug = painting.attr("strokeCurves[%d]" % i)
        conns = pm.listConnections(plug, s=True, d=False)
        if not conns:
            pm.removeMultiInstance(plug, b=True)
        else:
            sc = conns[0]
            curves = pm.listConnections(sc.attr("curve"), s=True, d=False)
            if not curves:
                pm.delete(sc)
                pm.removeMultiInstance(plug, b=True)


def delete_curve_instances(curves):

    paintings = pm.listHistory(curves, future=True, type="painting")

    for curve in curves:
        stroke_curve = get_stroke_curve(curve)
        if stroke_curve:
            pm.delete(stroke_curve)
        parents = curve.getParent()
        if parents:
            pm.delete(parents)

    for painting in paintings:
        remove_unconnected_curve_plugs(painting)


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



def _assign_random_resource(curves, resource_attr, id_attr, spec, set_key):

    seq = Sequence.create(spec)
    # print list(seq)
    painting = pm.listHistory(curves[0], future=True, type="painting")[0]
    r_indices = painting.attr(resource_attr).getArrayIndices()
    r_indices = list(seq.intersection(r_indices))
    num = len(r_indices)
    if num:
        last = num - 1
        for curve in curves:
            stroke_curve = get_stroke_curve(curve)
            rand_int = random.randint(0, last)
            index = r_indices[rand_int]
            attr = stroke_curve.attr(id_attr)
            try:
                attr.set(index)
                if set_key:
                    attr.setKey(value=index)
            except RuntimeError:
                pm.warning("Skipping locked attribute %s" % attr)


def assign_random_paints(curves, spec, set_key=False):
    _assign_random_resource(curves, "paints", "paintId", spec, set_key)


def assign_random_brushes(curves, spec, set_key=False):
    _assign_random_resource(curves, "brushes", "brushId", spec, set_key)


def get_extent(node, stroke_curve, curve, side="outer"):
    brushId = stroke_curve.attr("brushId").get()
    brush_width = Brush.brush_at_index(node, brushId).width / 10.0  # mm to cm
    curveRadius = get_curve(stroke_curve).getParent().attr("sx").get()
    if side == "outer":
        return curveRadius + (brush_width * 0.5)
    return curveRadius - (brush_width * 0.5)


def arrange_rings_gap(curves, gap, set_key=False):
    node = pm.listHistory(curves[0], future=True, levels=0, type="painting")[0]
    stroke_curves = pm.listConnections(
        curves, d=True, s=False, type="strokeCurve")

    extent = get_extent(node, stroke_curves[0], curves[0])

    for stroke_curve in stroke_curves[1:]:
        # print "progress: %s" % stroke_curve.name()
        # print "progress: %s" % stroke_curve.name()
        brushId = stroke_curve.attr("brushId").get()
        brush_width = Brush.brush_at_index(
            node, brushId).width / 10.0  # mm to cm
        next_radius = (brush_width * 0.5) + gap + extent
        curve = get_curve(stroke_curve)
        curve_tf = curve.getParent()
        curve_tf.attr("sx").set(next_radius)
        curve_tf.attr("sy").set(next_radius)
        if set_key:
            curve_tf.attr("sx").setKey(value=next_radius)
            curve_tf.attr("sy").setKey(value=next_radius)
        extent = next_radius + (brush_width * 0.5)


def arrange_rings_spine(curves, dist, set_key=False):
    node = pm.listHistory(curves[0], future=True, levels=0, type="painting")[0]
    stroke_curves = pm.listConnections(
        curves, d=True, s=False, type="strokeCurve")
    curveRadius = curves[0].getParent().attr("sx").get()

    for stroke_curve in stroke_curves[1:]:
        curveRadius += dist
        curve = get_curve(stroke_curve)
        curve_tf = curve.getParent()
        curve_tf.attr("sx").set(curveRadius)
        curve_tf.attr("sy").set(curveRadius)
        if set_key:
            curve_tf.attr("sx").setKey(value=curveRadius)
            curve_tf.attr("sy").setKey(value=curveRadius)

def set_max_extent_visibility(curves, max_extent, set_key=False):
    node = pm.listHistory(curves[0], future=True, levels=0, type="painting")[0]
    for curve in curves:
        tf = curve.getParent()
        stroke_curve =  get_stroke_curve(curve)
        extent = get_extent(node, stroke_curve, curve, "outer")
        # print "extent %s - max %s" % (extent, max_extent) 
        vis = not (extent > max_extent)
        tf.attr("visibility").set(vis )
        if set_key:
            tf.attr("visibility").setKey(value=vis)
 






def _randomize(
        curves,
        paint_spec,
        brush_spec,
        max_extent,
        spacing,
        spacing_type,
        do_keys):
    if paint_spec:
        _assign_random_resource(curves, "paints", "paintId", paint_spec, do_keys)
    if brush_spec:
        _assign_random_resource(curves, "brushes", "brushId", brush_spec, do_keys)
    if spacing_type == "spine":
        arrange_rings_spine(curves, spacing, do_keys)
    else: # gap
        arrange_rings_gap(curves, spacing, do_keys)
    
    set_max_extent_visibility(curves, max_extent, do_keys)




def duplicate_into_gaps(curves):

    painting = pm.listHistory(curves[0], future=True, type="painting")[0]

    for curve_pair in zip(curves, curves[1:]):
        curve_tf, curve_shape, stroke_curve = duplicate_curve_to_painting(
            curve_pair[0], painting)
        inner_stroke_curve = get_stroke_curve(curve_pair[0])
        outer_stroke_curve = get_stroke_curve(curve_pair[1])

        first_edge = get_extent(
            painting,
            inner_stroke_curve,
            curve_pair[0],
            side="outer")
        second_edge = get_extent(
            painting,
            outer_stroke_curve,
            curve_pair[1],
            side="inner")
        new_radius = (first_edge + second_edge) * 0.5
        curve_tf.attr("sx").set(new_radius)
        curve_tf.attr("sy").set(new_radius)
        stroke_curve.attr("brushId").set(0)
        stroke_curve.attr("paintId").set(7)


# setAttr "cShape0_SC1.strokeProfileRamp[0].strokeProfileRamp_Position" 0;
# setAttr "cShape0_SC1.strokeProfileRamp[0].strokeProfileRamp_FloatValue" 0.48;
# setAttr "cShape0_SC1.strokeProfileRamp[0].strokeProfileRamp_FloatValue" 0.5;
# setAttr "cShape0_SC1.brushTiltRamp[0].brushTiltRamp_Position" 0;
# setAttr "cShape0_SC1.brushTiltRamp[0].brushTiltRamp_FloatValue" 0.46;
# setAttr "cShape0_SC1.brushBankRamp[0].brushBankRamp_Position" 0;
# setAttr "cShape0_SC1.brushBankRamp[0].brushBankRamp_FloatValue" 0.44;
# setAttr "cShape0_SC1.brushTwistRamp[0].brushTwistRamp_Position" 0;
# setAttr "cShape0_SC1.brushTwistRamp[0].brushTwistRamp_FloatValue" 0.44;


def propagate_ramp_attribute(att, rangeAtt, flat_only):
    curves = pm.ls(
        selection=True,
        dag=True,
        leaf=True,
        type="nurbsCurve",
        ni=True)
    painting = pm.listHistory(curves[0], future=True, type="painting")[0]

    last_curve = curves[-1]
    last_stroke_curve = get_stroke_curve(last_curve)
    if last_stroke_curve:
        rangeVals = last_stroke_curve.attr(rangeAtt).get()
        indices = last_stroke_curve.attr(att).getArrayIndices()
        for curve in curves[:-1]:
            stroke_curve = get_stroke_curve(curve)

            brushId = stroke_curve.attr("brushId").get()
            is_flat = Brush.brush_at_index(painting, brushId).is_flat()
            # print is_flat
            if is_flat or (not flat_only):
                # print curve
                indices_to_clear = stroke_curve.attr(att).getArrayIndices()
                for itc in indices_to_clear:
                    pm.removeMultiInstance(
                        stroke_curve.attr(
                            "%s[%d]" %
                            (att, itc)), b=True)
                stroke_curve.attr(rangeAtt).set(rangeVals)
                for index in indices:
                    for suffix in ["Position", "FloatValue", "Interp"]:
                        att_str = "%s[%d].%s_%s" % (att, index, att, suffix)
                        val = last_stroke_curve.attr(att_str).get()
                        stroke_curve.attr(att_str).set(val)


def set_ramp(attribute, values, interp=1):
    indices = attribute.getArrayIndices()
    for i in attribute.getArrayIndices():
        pm.removeMultiInstance("%s[%d]" % (attribute, i))

    for index, (pos, val) in enumerate(values):
        attr_name = attribute.attrName(longName=True)

        pos_att = "%s[%d].%s_Position" % (attribute, index, attr_name)
        val_att = "%s[%d].%s_FloatValue" % (attribute, index, attr_name)
        int_att = "%s[%d].%s_Interp" % (attribute, index, attr_name)
        pm.Attribute(pos_att).set(pos)
        pm.Attribute(val_att).set(val)
        pm.Attribute(int_att).set(interp)


def _clamp(minval, val, maxval):
    """Clamp value to min and max."""
    return sorted([minval, val, maxval])[1]

 


def auto_set_rings(
        curves,
        distribution,
        lift,
        twist,
        profile,
        subcurve_factor,
        rand_rotate,
        density):

    
    pi = 3.14159265359

    subcurvemin = 0


    for curve in curves:
        painting = get_painting(curve)
        stroke_curve = get_stroke_curve(curve)

        if subcurve_factor is not None:
            subcurvemin = stroke_curve.attr("liftLength").get() * subcurve_factor
        xf = curve.getParent()
        radius = xf.attr("sx").get()

        strokelength = stroke_curve.attr("strokeLength").get()
        overlap = stroke_curve.attr("overlap").get()
         
        if distribution:
            overlap = distribution["overlap"]
            max_length = distribution["max_length"]
            min_strokes = distribution["min_strokes"]
            full_circle = radius * 2.0 * pi
            strokes = math.ceil((full_circle + overlap) / max_length)
            if strokes < min_strokes:
                strokes = min_strokes

            full_arc = full_circle + overlap
            strokelength = (full_arc / strokes) + overlap
            subcurvemax = subcurvemin + full_arc

            stroke_curve.attr("strokeLength").set(strokelength)
            stroke_curve.attr("overlap").set(overlap)
            try:
                stroke_curve.attr("subcurveMax").set(subcurvemax)
            except BaseException:
                pass

        try:
            stroke_curve.attr("subcurveMin").set(subcurvemin)
        except BaseException:
            pass

        brushId = stroke_curve.attr("brushId").get()
        brush=Brush.brush_at_index(painting, brushId)
        tip_height = brush.tip 

        if lift:
            stroke_curve.attr("liftLength").set(tip_height*lift[0])
            stroke_curve.attr("liftHeight").set(tip_height*lift[1])
            stroke_curve.attr("liftBias").set(tip_height*lift[2])


        is_flat = Brush.brush_at_index(painting, brushId).is_flat()
        # now set twist, only on flat brushes
        if twist and is_flat:
            twist_range_atts = ("brushTwistRangeMin", "brushTwistRangeMax")
            stroke_curve.attr(twist_range_atts[0]).set(-90)
            stroke_curve.attr(twist_range_atts[1]).set(90)

            # print "twist_distance: %s - tist_angle: %s" % (
            #     twist_distance, twist_angle)

            twist_stop_pos = _clamp(0.0, (twist[1] / strokelength), 1.0)
            twist_stop_val = _clamp(0.0, (twist[0] / 180.0) + 0.5, 1.0)

            attribute = stroke_curve.attr("brushTwistRamp")
            values = [(0.0, twist_stop_val), (twist_stop_pos, 0.5)]
            set_ramp(attribute, values, 2)

        # now set profile
        if profile:
            
            # print "brush: %d tip is %f" % (brush.id,brush.tip)
            profile_range_atts = ("strokeProfileScaleMin", "strokeProfileScaleMax")
            stroke_curve.attr(profile_range_atts[0]).set(0)
            stroke_curve.attr(profile_range_atts[1]).set(1)

            profile_stop_pos = _clamp(0.0, (profile[1] / strokelength), 1.0)
            profile_stop_val = _clamp(0.0, (profile[0]  * tip_height )  , 1.0)

            attribute = stroke_curve.attr("strokeProfileRamp")
            values = [(0.0, profile_stop_val), (profile_stop_pos, 0.0)]
            set_ramp(attribute, values, 1)

        if rand_rotate:
            rand_int = random.randint(rand_rotate[0], rand_rotate[1])
            xf.attr("rz").set(rand_int)

        if density:
            radmin = density[0][0]
            denmin = density[0][1]
            radmax = density[1][0]
            denmax = density[1][1]
            power  = density[2]
            val = (radius - radmin) / (radmax - radmin)
            val = _clamp(0, val, 1)
            val = math.pow(val, power) 
            val = denmin + (val*(denmax - denmin))
            stroke_curve.attr("pointDensity").set(val)
            





def connect_to_containment(curve, containment):
    strokeCurve = get_stroke_curve(curve)
    index = sfu.get_index(containment, "curve.", connect_to="next_available")
    curve.attr("worldSpace[0]") >> containment.attr("curve[%d]" % index)
    containment.attr("outLength[%d]" % index) >> strokeCurve.attr("subcurve")


def _is_connected(curve, containment):
    indices = containment.attr("curve").getArrayIndices()
    for index in indices:
        if pm.isConnected(
            curve.attr("worldSpace[0]"),
            containment.attr(
                "curve[%d]" %
                index)):
            return True
    return False


def contain_strokes_in_mesh(curves, mesh):
    # if curves are already contained by this mesh, ignore
    # if curves are already contained by another mesh, switch
    try:
        ctn = pm.listConnections(
            mesh, destination=True, source=False, type="curveContainment")[0]
    except IndexError:
        ctn = pm.createNode("curveContainment")
        ctn.attr("maxIterations").set(10)
        ctn.attr("tolerance").set(0.1)
        ctn.attr("sampleDistance").set(1)
        ctn.attr("containmentType").set(0)
        mesh.attr("worldMesh[0]") >> ctn.attr("mesh")

    for curve in curves:
        if not _is_connected(curve, ctn):
            connect_to_containment(curve, ctn)


def curve_vis_active_connection(curves, connect):
    for curve in curves:
        try:
            stroke_curve = get_stroke_curve(curve)
            if connect:
                curve.attr("visibility") >> stroke_curve.attr("active")
            else:
                curve.attr("visibility") // stroke_curve.attr("active")
        except BaseException as ex:
            print ex


# def get_index(node, att, connect_to):

#     array_att, connection_att  = att.split(".")
#     indices = node.attr(array_att).getArrayIndices()
#     index = 0
#     if connect_to == "next_available":
#         if not indices:
#             return 0
#         while(True):
#             if index not in indices:
#                 break
#             full_att =  "%s[%d]" % (array_att, index)
#             if connection_att:
#                 full_att = "%s.%s" % (full_att, connection_att);
#             elif not pm.listConnections(node.attr(full_att), source=True, destination=False):
#                 break
#             index += 1
#     elif connect_to == "at_end":
#         if not indices:
#             return 0
#         index = indices[-1] + 1
#     else:
#         index = connect_to
#     return index


# def propagate_ramp_attribute(brushTiltRam):
#     curves = pm.ls(selection=True, dag=True, leaf=True, type="nurbsCurve", ni=True, ut=True, v=True)

# def propagate_ramp_attribute(brushBankRamp):
#     curves = pm.ls(selection=True, dag=True, leaf=True, type="nurbsCurve", ni=True, ut=True, v=True)

# def propagate_ramp_attribute(brushTwistRamp):
#     curves = pm.ls(selection=True, dag=True, leaf=True, type="nurbsCurve", ni=True, ut=True, v=True)
