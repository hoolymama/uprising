import pymel.core as pm

from paint import Paint
from brush import Brush


def input_connection(attribute):
    conns = attribute.connections(source=True, destination=False, plugs=True)
    return conns and conns[0]


 
def get_index(node, att, connect_to):
    
    array_att, connection_att  = att.split(".")
    indices = node.attr(array_att).getArrayIndices()
    index = 0
    if connect_to is "next_available":
        if not indices:
            return 0
        while(True):
            if index not in indices:
                break
            elif not pm.listConnections(node.attr("%s[%d].%s" % (array_att, index, connection_att)), source=True, destination=False):
                break
            index += 1
    elif connect_to is "at_end":
        if not indices:
            return 0
        index = indices[-1] + 1
    else:
        index = connect_to
    return index


def driver_att_name(att):
    name = att.longName()
    name = name.split(".")[-1]
    return "sf%s%s" % (name[0].upper(), name[1:])


def create_and_connect_driver(node, att):
    data_atts = ["string"] 
    att_type = att.type()
    name = driver_att_name(att)
    val = att.get()
    if not pm.attributeQuery(name, node=node, exists=True):
        if att_type in data_atts:
            pm.addAttr(node, longName=name, dt=att_type)
        else:
            pm.addAttr(node, longName=name, at=att_type)
        node.attr(name).setKeyable(True)
        node.attr(name).set(val)
    if not node.attr(name).isConnectedTo(att):
        node.attr(name) >> att
 

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
 