
import pymel.core as pm
import stroke_factory_utils as sfu
import re


def add_curve_to_sf():
    node = pm.ls(selection=True, dag=True, leaf=True, type="strokeFactory")
    if not node:
        raise IndexError("No strokeFaactory node selected")
    curves = pm.ls(selection=True, dag=True, leaf=True, type="nurbsCurve")
    for curve in curves:
        connect_curve_to_node(curve, node[0])


def update_curve_sf():
    node = pm.ls(selection=True, dag=True, leaf=True, type="strokeFactory")
    if not node:
        raise IndexError("No strokeFaactory node selected")
    curves = pm.ls(selection=True, dag=True, leaf=True, type="nurbsCurve")
    for curve in curves:
        update_curve_connections(curve, node[0])

def connect_curve_to_node(curve, node, connect_to="next_available"):
    index = sfu.get_index(node, "curves.curve", connect_to)
    if not curve.attr("worldSpace[0]").isConnectedTo(node.attr("curves[%d].curve" % index)):
        curve.attr("worldSpace[0]") >> node.attr("curves[%d].curve" % index)
    whitelist = ["double", "short", "bool", "doubleAngle"]
    atts = node.attr("curves[%d]" % index).getChildren()
    for att in atts:
        att_type = att.type()
        if att_type in whitelist:
            sfu.create_and_connect_driver(curve, att)
        elif att_type == "double3":
            child_atts = att.getChildren()
            for c_att in child_atts:
                sfu.create_and_connect_driver(curve, c_att)


def update_curve_connections(curve, node):
    conns = curve.attr("worldSpace[0]").connections(type="strokeFactory", plugs=True)
    for conn in conns:
        found = re.compile(r'curves\[([0-9]+)].curve').search(str(conn))
        if found:
            index = int(found.group(1))
            # print "WANT TO CONNECT: %s to index %d" % (curve, index)
            connect_curve_to_node(curve, node, connect_to=index)
        else:
            print "%s not connected - use "
