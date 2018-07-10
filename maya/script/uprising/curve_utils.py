
import pymel.core as pm
import stroke_factory_utils as sfu
import re
import random


def add_curve_to_sf():
    node = pm.ls(selection=True, dag=True, leaf=True, type="strokeFactory")
    if not node:
        raise IndexError("No strokeFactory node selected")
    curves = pm.ls(selection=True, dag=True, leaf=True, type="nurbsCurve")
    for curve in curves:
        connect_curve_to_node(curve, node[0])


def connect_curve_to_node(curve, node, connect_to="next_available"):
    index = sfu.get_index(node, "curves.curve", connect_to)
    if not curve.attr("worldSpace[0]").isConnectedTo(
            node.attr("curves[%d].curve" % index)):
        curve.attr("worldSpace[0]") >> node.attr("curves[%d].curve" % index)
    whitelist = ["double", "short", "bool", "doubleAngle"]
    atts = node.attr("curves[%d]" % index).getChildren()
    # print  "ATTS %s " % atts
    for att in atts:
        att_type = att.type()
        if att_type in whitelist:
            sfu.create_and_connect_driver(curve, att)
        elif att_type in ["double3", "double2"]:
            # print "COMPOUND %s " % att
            child_atts = att.getChildren()
            for c_att in child_atts:
                sfu.create_and_connect_driver(curve, c_att)


def update_curve_sf():
    node = pm.ls(selection=True, dag=True, leaf=True, type="strokeFactory")
    if not node:
        raise IndexError("No strokeFactory node selected")
    curves = pm.ls(selection=True, dag=True, leaf=True, type="nurbsCurve")
    for curve in curves:
        update_curve_connections(curve, node[0])


def update_curve_connections(curve, node):
    conns = curve.attr("worldSpace[0]").connections(
        type="strokeFactory", plugs=True)
    for conn in conns:
        found = re.compile(r'curves\[([0-9]+)].curve').search(str(conn))
        if found:
            index = int(found.group(1))
            # print "WANT TO CONNECT: %s to index %d" % (curve, index)
            connect_curve_to_node(curve, node, connect_to=index)
        else:
            print "%s not connected - "


def _assign_random_resource(node, resource_attr, id_attr):
    r_indices = node.attr(resource_attr).getArrayIndices()
    last = len(r_indices) -1
    c_indices = node.attr("curves").getArrayIndices()
    for c in c_indices:
        rand_int = random.randint(0, last)
        index = r_indices[rand_int]
        attribute = node.attr("curves[%d].%s" % (c, id_attr))
        src_conn = sfu.input_connection(attribute)
        if src_conn:
            attribute = src_conn
        attribute.set(index)



def assign_random_paints(node):
    _assign_random_resource(node, "paints", "paintId")


def assign_random_brushes(node):
    _assign_random_resource(node, "brushes", "brushId")
