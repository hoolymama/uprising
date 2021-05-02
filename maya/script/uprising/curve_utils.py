
import pymel.core as pm
import stroke_factory_utils as sfu
from brush import Brush
import re
import math
import random
import logging
from uprising.sequence import Sequence
from wrandom import StepRandomizer


logger = logging.getLogger('uprising')


def do_rename_inputs(crv_template, stroke_template, curves):
    # make sure templates are okay
    crv_template % 3
    stroke_template % 3

    for curve in curves:
        try:
            stroke_curve = pm.listHistory(
                curve, future=True, levels=1, type="curveStroke")[0]
        except IndexError:
            pm.warning(
                "Skipping: %s not connected to a curveStroke" %
                curve)
            continue

        conns = pm.listConnections(
            stroke_curve,
            d=True,
            s=False,
            c=True,
            p=True,
            type=["painting", "collectStrokes"])
        if not conns:
            pm.warning(
                "Skipping: %s and %s not connected to a painting or collectStrokes" %
                (curve, stroke_curve))
            continue

        index = int(re.compile(
            r".*\[(\d+)]$").match(conns[0][1].name()).groups()[0])
        curve.getParent().rename(crv_template % index)
        stroke_curve.rename(stroke_template % index)


def get_curves_from_painting(painting):
    return pm.listHistory(painting.attr("strokes"), type="nurbsCurve")


def get_painting(curve):
    if curve.type() != "nurbsCurve":
        curve = pm.ls(
            curve, dag=True,
            ni=True,
            shapes=True,
            type="nurbsCurve")[0]

    paintings = pm.listHistory(curve, future=True, type="painting")
    return paintings[0]


def get_strokes_destination(curve):
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
        type="curveStroke")
    print "curveStrokes"
    print conns
    conns = pm.listConnections(
        conns[0].attr("output"), d=True, s=False, type=[
            "painting", "collectStrokes"])
    print "destinations"
    print conns
    return conns[0]


def get_curve(stroke_curve):
    conns = pm.listConnections(
        stroke_curve.attr("curve"),
        s=True,
        d=False,
        shapes=True)
    if conns:
        return conns[0]


def get_stroke_node(curve):
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
        type="curveStroke")
    if conns:
        return conns[0]


def get_stroke_nodes(objs):
    result = []
    for c in objs:
        if "strokeNode" in pm.nodeType(c, i=True):
            result.append(c)
            continue

        c = get_stroke_node(c)
        if c:
            result.append(c)
    return result


def connect_curve_using_src(curve, src, dest):
    stroke = pm.duplicate(src, rr=True, ic=True)[0]

    index = sfu.get_index(dest, "strokes.", "next_available")
    stroke.attr("output") >> dest.attr("strokes[%d]" % index)
    curve.attr("worldSpace[0]") >> stroke.attr("curve")
    curve.getParent().attr("visibility") >> stroke.attr("active")
    top = pm.PyNode("mainPaintingGroup")
    if not curve.getParent().getParent():
        pm.parent(curve.getParent(), "%s|curves" % top)


def connect_curve_to_strokes_att(curve, strokes_dest_node, **kw):

    connect_to = kw.get("connect_to", None)

    crv = pm.PyNode(curve)

    stroke_curve = get_stroke_node(curve)
    if not stroke_curve:
        c_name = curve.name().split("|")[-1]
        sc_name = "%s_SC" % c_name
        stroke_curve = pm.createNode("curveStroke", name=sc_name)
        curve.attr("worldSpace[0]") >> stroke_curve.attr("curve")

        stroke_curve.attr(
            "brushTiltRamp[0].brushTiltRamp_FloatValue").set(0.5)
        stroke_curve.attr(
            "brushBankRamp[0].brushBankRamp_FloatValue").set(0.5)
        stroke_curve.attr(
            "brushTwistRamp[0].brushTwistRamp_FloatValue").set(0.5)

        stroke_curve.attr(
            "contactRamp[0].contactRamp_FloatValue").set(0)

    if connect_to:
        index = sfu.get_index(strokes_dest_node, "strokes.", connect_to)
        stroke_curve.attr("output") >> strokes_dest_node.attr(
            "strokes[%d]" % index)

    return stroke_curve


def duplicate_curve_to_painting(curve):
    node = get_strokes_destination(curve)
    xf = curve.getParent()
    stroke_curve = get_stroke_node(curve)
    new_xf = pm.duplicate(xf)[0]
    new_shape = new_xf.getChildren()[0]

    new_stroke = pm.duplicate(stroke_curve, rr=True, ic=True)[0]

    new_shape.attr("worldSpace[0]") >> new_stroke.attr("curve")
    new_xf.attr("visibility") >> new_stroke.attr("active")

    index = sfu.get_index(node, "strokes.", "next_available")
    new_stroke.attr("output") >> node.attr("strokes[%d]" % index)

    return (new_xf, new_shape, new_stroke)


STROKE_NODE_ATTS = [
    "strokeLength",
    "randomLengthFactor",
    "overlap",
    "randomOverlapFactor",
    "subcurve",
    "subcurveMethod",
    "brushRampScope",
    "projection",
    "strokeCountFactor",
    "pointDensity",
    "reverseDirection",
    "entryLength",
    "exitLength",
    "repeats",
    "repeatOffset",
    "repeatMirror",
    "repeatOscillate",
    "repeatFan",
    "pivotFraction",
    "repeatPivot",
    "brushId",
    "paintId",
    "layerId",
    "active",
    "followStroke",
    "brushTiltRangeMin",
    "brushTiltRangeMax",
    "brushBankRangeMin",
    "brushBankRangeMax",
    "brushTwistRangeMin",
    "brushTwistRangeMax"
]


def connect_stroke_curves(src, dest, omit_list=[]):
    atts_to_connect = list(set(STROKE_NODE_ATTS) - set(omit_list))
    for att in atts_to_connect:
        src.attr(att) >> dest.attr(att)


def duplicate_grp_with_stroke_curves(
        src, full_name, connect=False, omit_list=[]):
    logger.debug("src: %s -- full_name: %s " % (src, full_name))

    shapes = pm.ls(src, dag=True, ni=True, shapes=True)

    logger.debug("shapes: %s " % shapes)

    stroke_curves = pm.listConnections(
        shapes,
        destination=True,
        source=False,
        type="curveStroke")
    # print stroke_curves
    new_stroke_curves = pm.duplicate(stroke_curves)
    if connect:
        for sc_src, sc_dest in zip(stroke_curves, new_stroke_curves):
            connect_stroke_curves(sc_src, sc_dest, omit_list)

    grp = pm.duplicate(src)[0]
    grp.rename(full_name)
    new_shapes = pm.ls(grp, dag=True, ni=True, shapes=True)
    for i, shape in enumerate(new_shapes):
        stroke_curve = new_stroke_curves[i]
        shape.attr("worldSpace[0]") >> stroke_curve.attr("curve")

    # if connect:
    #     for shp_src, shp_dest in zip(shapes, new_shapes):
    #         shp_src.attr("local") >> shp_dest.attr("create")

    curves_xfs = pm.listRelatives(new_shapes, parent=True)
    curve_vis_active_connection(curves_xfs, True)

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
        if not get_stroke_node(shape):
            new_stroke_curve = pm.duplicate(get_stroke_node(src_shape))[0]
            shape.attr("worldSpace[0]") >> new_stroke_curve.attr("curve")
            # plane.attr("worldMatrix[0]") >> new_stroke_curve.attr(
            #     "planeMatrix")
    return existing_curves_grp


def delete_strokes_from(nodes):
    for node in nodes:
        conns = node.attr("strokes").connections(s=True, d=False)
        for conn in conns:
            if pm.nodeType(conn) == "collectStrokes":
                delete_strokes_from([conn])
            else:
                pm.delete(conn)


def remove_unconnected_curve_plugs(painting):
    print "remove_unconnected_curve_plugs %s" % painting
    stroke_curves = pm.listConnections(
        painting, s=True, d=False, type="curveStroke")
    for sc in stroke_curves:
        conns = pm.listConnections(
            sc, d=True,
            s=False,
            type="painting",
            c=True, p=True)
       # disconnect all conns after first
        for conn in conns[1:]:
            conn[0] // conn[1]

    for i in painting.attr("strokes").getArrayIndices():
        print "index %s" % i
        plug = painting.attr("strokes[%d]" % i)
        conns = pm.listConnections(plug, s=True, d=False)
        print "conns"
        print conns
        if not len(conns):
            print "removing %s" % plug
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
        stroke_curve = get_stroke_node(curve)
        if stroke_curve:
            pm.delete(stroke_curve)
        parents = curve.getParent()
        if parents:
            pm.delete(parents)

    for painting in paintings:
        remove_unconnected_curve_plugs(painting)


def do_random_all(curves, r_indices, id_attr, set_key):
    num = len(r_indices)
    if not num:
        return

    last = num - 1
    for curve in curves:
        stroke_curve = get_stroke_node(curve)
        attr = stroke_curve.attr(id_attr)
        rand_int = random.randint(0, last)
        index = r_indices[rand_int]
        try:
            attr.set(index)
            if set_key:
                attr.setKey(value=index)
        except RuntimeError:
            pm.warning("Skipping locked attribute %s" % attr)


def do_random_last(curves, r_indices, id_attr, steps, power, set_key):
    num = len(r_indices)
    if not num:
        return
    last = num - 1

    randomizer = StepRandomizer(steps, power)

    prev_index = None
    for curve in curves:
        stroke_curve = get_stroke_node(curve)
        attr = stroke_curve.attr(id_attr)
        if prev_index is None:
            rand_index = random.randint(0, last)
        else:
            rand_index = (prev_index + randomizer.random()) % num
        resource_id = r_indices[rand_index]
        try:
            attr.set(resource_id)
            if set_key:
                attr.setKey(value=resource_id)
            prev_index = rand_index  # only set prev is we successfully set the resource
        except RuntimeError:
            pm.warning("Skipping locked attribute %s" % attr)


def do_random_existing(
        curves,
        orig_ids,
        r_indices,
        id_attr,
        steps,
        power,
        set_key):

    num = len(r_indices)
    if not num:
        return

    randomizer = StepRandomizer(steps, power)

    curve_packs = zip(curves, orig_ids)

    for c in curve_packs:
        curve, orig_index = c
        stroke_curve = get_stroke_node(curve)
        attr = stroke_curve.attr(id_attr)
        rand_index = (orig_index + randomizer.random()) % num
        resource_id = r_indices[rand_index]
        try:
            attr.set(resource_id)
            if set_key:
                attr.setKey(value=resource_id)

        except RuntimeError:
            pm.warning("Skipping locked attribute %s" % attr)


def _assign_random_resource(
        curves,
        orig_ids,
        resource_attr,
        id_attr,
        random_params,
        set_key):

    painting = pm.listHistory(curves[0], future=True, type="painting")[0]
    r_indices = painting.attr(resource_attr).getArrayIndices()
    spec, steps, power = random_params
    # mode can be "random_all", "random_existing", "random_last"
    # mode = None

    try:
        seq = Sequence.create(spec)
        r_indices = list(seq.intersection(r_indices))
        if steps < 1:  # mode = random_all
            do_random_all(curves, r_indices, id_attr, set_key)
        else:
            do_random_last(curves, r_indices, id_attr, set_key)
    except TypeError:
        if steps < 1:  # mode = random_all
            do_random_all(curves, r_indices, id_attr, set_key)
        else:
            do_random_existing(
                curves,
                orig_ids,
                r_indices,
                id_attr,
                steps,
                power,
                set_key)


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
        curves, d=True, s=False, type="curveStroke")

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
        curves, d=True, s=False, type="curveStroke")
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
        stroke_curve = get_stroke_node(curve)
        extent = get_extent(node, stroke_curve, curve, "outer")
        # print "extent %s - max %s" % (extent, max_extent)
        vis = not (extent > max_extent)
        tf.attr("visibility").set(vis)
        if set_key:
            tf.attr("visibility").setKey(value=vis)


def _randomize(
        curve_packs,
        random_paint_params,
        random_brush_params,
        max_extent,
        spacing,
        spacing_type,
        paint_ids,
        brush_ids,
        do_keys):

    curves, brush_ids, paint_ids = zip(*curve_packs)

    if random_paint_params:
        _assign_random_resource(
            curves,
            paint_ids,
            "paints",
            "paintId",
            random_paint_params,
            do_keys)
    if random_brush_params:
        _assign_random_resource(
            curves,
            brush_ids,
            "brushes",
            "brushId",
            random_brush_params,
            do_keys)

    if spacing_type == "spine":
        arrange_rings_spine(curves, spacing, do_keys)
    elif spacing_type == "gap":  # gap
        arrange_rings_gap(curves, spacing, do_keys)

    if max_extent is not None:
        set_max_extent_visibility(curves, max_extent, do_keys)


def duplicate_into_gaps(curves):

    painting = pm.listHistory(curves[0], future=True, type="painting")[0]

    for curve_pair in zip(curves, curves[1:]):
        curve_tf, curve_shape, stroke_curve = duplicate_curve_to_painting(
            curve_pair[0], painting)
        inner_stroke_curve = get_stroke_node(curve_pair[0])
        outer_stroke_curve = get_stroke_node(curve_pair[1])

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


def propagate_ramp_attribute(att, auxAtt):
    nodes = pm.ls(selection=True)
    stroke_nodes = get_stroke_nodes(nodes)

    last_node = stroke_nodes[-1]
    if last_node:
        auxVal = last_node.attr(auxAtt).get()
        indices = last_node.attr(att).getArrayIndices()
        for stroke_node in stroke_nodes[:-1]:
            indices_to_clear = stroke_node.attr(att).getArrayIndices()
            for itc in indices_to_clear:
                pm.removeMultiInstance(
                    stroke_node.attr(
                        "%s[%d]" %
                        (att, itc)), b=True)
            stroke_node.attr(auxAtt).set(auxVal)
            for index in indices:
                for suffix in ["Position", "FloatValue", "Interp"]:
                    att_str = "%s[%d].%s_%s" % (att, index, att, suffix)
                    val = last_node.attr(att_str).get()
                    stroke_node.attr(att_str).set(val)


def set_ramp(attribute, values, interp=1):
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
        stroke_curve = get_stroke_node(curve)

        if subcurve_factor is not None:
            subcurvemin = stroke_curve.attr(
                "liftLength").get() * subcurve_factor
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
        brush = Brush.brush_at_index(painting, brushId)
        tip_height = brush.tip

        if lift:
            stroke_curve.attr("liftLength").set(tip_height * lift[0])
            stroke_curve.attr("liftHeight").set(tip_height * lift[1])
            stroke_curve.attr("liftBias").set(tip_height * lift[2])

        # now set twist, maybe only on flat brushes
        if twist:
            flat_only = twist[2]
            is_flat = Brush.brush_at_index(painting, brushId).is_flat()
            if is_flat or not flat_only:
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
            profile_range_atts = (
                "strokeProfileScaleMin",
                "strokeProfileScaleMax")
            stroke_curve.attr(profile_range_atts[0]).set(0)
            stroke_curve.attr(profile_range_atts[1]).set(1)

            profile_stop_pos = _clamp(0.0, (profile[1] / strokelength), 1.0)
            profile_stop_val = _clamp(0.0, (profile[0] * tip_height), 1.0)

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
            power = density[2]
            val = (radius - radmin) / (radmax - radmin)
            val = _clamp(0, val, 1)
            val = math.pow(val, power)
            val = denmin + (val * (denmax - denmin))
            stroke_curve.attr("pointDensity").set(val)


def connect_to_containment(curve, containment):
    curveStroke = get_stroke_node(curve)
    index = sfu.get_index(containment, "curve.", connect_to="next_available")
    curve.attr("worldSpace[0]") >> containment.attr("curve[%d]" % index)
    containment.attr("outLength[%d]" % index) >> curveStroke.attr("subcurve")


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


def curve_vis_active_connection(curves, connect=True):
    for curve in curves:
        try:
            stroke_curve = get_stroke_node(curve)
            if connect:
                curve.attr("visibility") >> stroke_curve.attr("active")
            else:
                curve.attr("visibility") // stroke_curve.attr("active")
        except BaseException as ex:
            print ex


def hide_objects(obs):
    for o in obs:
        if o.type() == "curveStroke":
            xf = get_curve(o).getParent()
        elif o.type() == "nurbsCurve":
            xf = o.getParent()
        elif o.type() == "transform":
            xf = o
        else:
            pm.warning("Cant get transform")
            return
        try:
            xf.attr("visibility").set(0)
        except RuntimeError:
            pm.warning("Can't change visibility for %s" % xf)


def show_objects(obs):
    for o in obs:
        if o.type() == "curveStroke":
            xf = get_curve(o).getParent()
        elif o.type() == "nurbsCurve":
            xf = o.getParent()
        elif o.type() == "transform":
            xf = o
        else:
            pm.warning("Cant get transform")
            return
        try:
            xf.attr("visibility").set(1)
        except RuntimeError:
            pm.warning("Can't change visibility for %s" % xf)



# brushes = pm.listRelatives("brushNodes", c=True)

# def create_stroke_curves(brush, grp, suffix):
#     attr = "outDipBrush" if suffix == "dip" else "outWipeBrush"
#     result = []
#     parts = brush.name().split("_")
#     curves = pm.ls(grp, dag=True, leaf=True, type="nurbsCurve")
#     print curves
#     stroke_curves = pm.listConnections(curves, d=True, s=False, type="curveStroke")
#     print stroke_curves

#     dups = pm.duplicate(stroke_curves)
#     for i,dup in enumerate(dups):
#         dup.rename("cs_bpx_{}_{}_{:02d}_{}".format(parts[1], parts[4], i, suffix))
#     for   pair in zip(curves, dups):
#         result.append(
#             [pair[0].attr("ws"), pair[1].attr("curves[0]")]
#         )
#         result.append(
#             [brush.attr(attr), pair[1].attr("brush")]
#         )

#     return result


# connection_pairs = [

# ]

# for brush in brushes:
#     print "-" * 30

#     brush_shape = pm.listRelatives(brush, shapes=True)[0]
#     print brush_shape
#     parts = brush.name().split("_")
#     model = parts[4]

#     dip_grp = pm.PyNode("grpDW_dip_{}".format(model))
#     connection_pairs +=  create_stroke_curves(brush, dip_grp, "dip")


#     wipe_grp = pm.PyNode("grpDW_wipe_{}".format(model))
#     connection_pairs +=   create_stroke_curves(brush, wipe_grp, "wipe")

#     water_wipe_grp = pm.PyNode("grpWater_wipe")
#     connection_pairs +=  create_stroke_curves(brush, water_wipe_grp, "waterWipe")

# print connection_pairs


# for p in connection_pairs:
#     print p[0], " ---> ",p[1]



# brushes = pm.listRelatives("brushNodes", c=True)
# for brush in brushes:
#     print "-" * 30
#     parts = brush.name().split("_")
#     for which in ["waterWipe", "wipe"]:
#         wildcard = "cs_bpx_{}_{}_*_{}".format(parts[1], parts[4], which)
#         nodes = sorted(pm.ls(wildcard))
#         collector = pm.createNode("collectStrokes")
#         collector.rename("collect_bpx_{}_{}_{}".format(parts[1], parts[4], which))
#         for i, node in enumerate(nodes):
#             node.attr("output") >> collector.attr("strokes")[i]






# for hole in range(20):
#     for which in ["dip", "wipe"]:
#         paintings = pm.ls("rack|holes|holeRot_{:02d}|holeTrans|{}_loc".format(hole, which), dag=True, leaf=True, type="painting")
    
#         print "-" * 10
#         for painting in paintings:
#             brush_number = painting.split("|")[3][1:3]
            
#             newname = "ptg_{}_b{:02d}_p{:02d}".format(which, brush_number, hole)
#             print "Rename: {} --> {}".format(painting, newname)
            


# for hole in range(20):
 
#     paintings = pm.ls("rack|holes|holeRot_{:02d}|holeTrans|wipe_loc".format(hole), dag=True, leaf=True, type="painting")
#     for painting in paintings:
#         pm.PyNode("DWTransform").attr("worldMatrix[0]") >> painting.attr("canvasMatrix")
        
        
# select pm.ls("rack|holes|holeRot_|holeTrans|wipe_loc".format(hole), dag=True, leaf=True, type="painting")

# pm.PyNode("cs_bpx_13_davinci30_00_dip").connections(d=True, type="painting")



# import pymel.core as pm

# for hole in range(20):
#     paintings = pm.ls("rack|holes|holeRot_{:02d}|holeTrans|dip_loc".format(hole), dag=True, leaf=True, type="painting")
#     for painting in paintings:
#         brush_number = painting.split("|")[3][1:3]
        
#         dip_node = pm.ls("cs_bpx_{}_*_*_dip".format(brush_number))[0]    
 
#         dip_node.attr("output") >> painting.attr("strokes")[0]
# pm.select(pm.ls(type="curveStroke"))







# pm.PyNode("board_BL").attr("t").set(1783.31 / 10, 7.36 / 10, 461.64 / 10)
# pm.PyNode("board_TL").attr("t").set(1787.15 / 10, 13.89 / 10, 1382.69 / 10)
# pm.PyNode("board_TR").attr("t").set(1778.98 / 10, -900.69 / 10, 1391.56 / 10)