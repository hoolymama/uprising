
import pymel.core as pm
import stroke_factory_utils as sfu
from paint import Paint
from brush import Brush
import curve_utils as cutl

DIP_CURVE_DEFAULTS = [
    ("sfForceDip", 1),
    ("sfFollowStroke", 0),
    ("sfActive", 1),
    ("sfStrokeLength", 1000),
    ("sfRandomOverlapFactor", 0),
    ("sfRandomLengthFactor", 0),
    ("sfRepeats", 0),
    ("sfOverlap", 0)
]


def get_dip_combinations(painting_factory):

    result = {}
    brushes = Brush.brushes(painting_factory)
    paints =  Paint.paints(painting_factory)
    indices = painting_factory.attr("curves").getArrayIndices()
    for index in indices:
        if painting_factory.attr("curves[%d].active" % index).get():
            brush_id = painting_factory.attr("curves[%d].brushId" % index).get()
            paint_id = painting_factory.attr("curves[%d].paintId" % index).get()
            key = "p%02d_b%02d" % (paint_id, brush_id)
            result[key] = {
                "brush": brushes[brush_id],
                "paint": paints[paint_id]
            }
    return result
 
def delete_if_exist(name):
    try:
        pm.delete(name)
    except BaseException:
        pass

def duplicate_and_connect_curves(curves, factory, paint_id, brush_id):
    result = pm.duplicate(curves)
    for i, curve in enumerate(result):
        name = "p%02d_b%02d_c%02d" % (paint_id, brush_id, i)
        delete_if_exist(name)
        curve.rename(name)
        shape = pm.listRelatives(curve, children=True)[0]
        first = (i == 0)
        cutl.connect_curve_to_node(shape, factory)
        set_dip_curve_defaults(shape, first)
        shape.attr("sfBrushId").set(brush_id)
        shape.attr("sfPaintId").set(paint_id)
        
    return result


def rename_as_source(curves):
    for i, curve in enumerate(curves):
        name = "source_c%02d_" % i
        shape_name = "%sShape" % name
        curve.rename(shape_name)
        curve.getParent().rename("source_c%02d" % i)

    return curves


def set_dip_curve_defaults(curve, first=True):
    for d in DIP_CURVE_DEFAULTS:
        try:
            curve.attr(d[0]).set(d[1])
        except BaseException:
            pass
    if not first:
        try:
            curve.attr("sfForceDip").set(0)
        except BaseException:
            pass


def setup_dip_factory(painting_factory, dip_factory, curves):
    if not curves:
        raise AttributeError(
            "No curves specified. Select or attach a dip curve to the dip factory")

    sfu.delete_curve_instances(dip_factory)
    source_curves = rename_as_source(curves)

    dip_combinations = get_dip_combinations(painting_factory)

    for dip in dip_combinations:
        paint_id = dip_combinations[dip]["paint"].id
        brush_id = dip_combinations[dip]["brush"].id
        duplicate_and_connect_curves(source_curves, dip_factory, paint_id, brush_id)
    
    source_parents = pm.listRelatives(source_curves, parent=True)
    print "SOURCE PARENTS"
    print source_parents
    pm.delete(source_parents)
    connect_brushes(painting_factory, dip_factory)
    connect_paints(painting_factory, dip_factory)



def input_connection(attribute):
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
            src_attribute = src.attr("brushes[%d].%s" % (index, att))
            dest_attribute = dest.attr("brushes[%d].%s" % (index, att))
            src_conn = input_connection(src_attribute)
            if src_conn:
                src_conn >> dest_attribute
            else:
                src_attribute >> dest_attribute


def connect_paints(src, dest):
    for index in dest.attr("paints").getArrayIndices():
        pm.removeMultiInstance(dest.attr("paints[%d]" % index), b=True)

    for index in src.attr("paints").getArrayIndices():
        dest.attr(  "paints[%d].paintMaxArcLength" % index).set(1000)
        for att in [
            "paintColor",
            "paintName",
            "paintOpacity"]:

            src.attr(
                "paints[%d].%s" %
                (index, att)) >> dest.attr(
                "paints[%d].%s" %
                (index, att))

