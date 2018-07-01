
import pymel.core as pm
import stroke_factory_utils as sfu
from paint import Paint
from brush import Brush

import curve_utils as cutl
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

    sfu.delete_curve_instances(dip_factory)

    for dip in dips:
        paint_id = dips[dip]["paint"].id
        brush_id = dips[dip]["brush"].id
        curve = pm.duplicate(source_dip_curve)[0]
        curve.rename("p%02d_b%02d" % (paint_id, brush_id))
        curve.attr("sfPaintId").set(paint_id)
        curve.attr("sfBrushId").set(brush_id)
        curve = pm.listRelatives(curve, children=True)[0]
        cutl.connect_curve_to_node(curve, dip_factory)

    pm.delete(source_dip_curve.getParent())
    connect_brushes(painting_factory, dip_factory)
    connect_paints(painting_factory, dip_factory)






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

def inputConnection(attribute):
    conns = attribute.connections(source=True, destination=False, plugs=True)
    return conns and conns[0]

def connect_brushes(src, dest):
    for index in dest.attr("brushes").getArrayIndices():
        pm.removeMultiInstance(dest.attr("brushes[%d]" % index), b=True)

    for index in src.attr("brushes").getArrayIndices():
        atts = [
            "brushWidth",
            "brushLift",
            "brushMatrix",
            "brushRetention"]
        for att in atts:
            src_attribute = src.attr( "brushes[%d].%s" % (index, att))
            dest_attribute = dest.attr( "brushes[%d].%s" % (index, att))
            src_conn = inputConnection(src_attribute)
            if src_conn:
                src_conn >> dest_attribute
            else:
                src_attribute >> dest_attribute

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
