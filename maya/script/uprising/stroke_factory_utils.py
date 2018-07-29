import pymel.core as pm

from paint import Paint
from brush import Brush

import sheets

import uprising.uprising_util as uput

 
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


def set_board_from_sheet(node):
    #   get top node, then find corner locators
    p = node.getParent()
    while p:
      node = p
      p =  node.getParent()
    
    top_name =  node.name()
    corners = {
        "BL": {
            "node": pm.PyNode("%s|BL" % top_name)
        },
        "TL": {
            "node": pm.PyNode("%s|TL" % top_name)
        },
        "TR": {
            "node": pm.PyNode("%s|TR" % top_name)
        }
    }

    data = sheets.get_raw_board_data()
    validate_board_data(data)


    for row in data:
        row = [uput.numeric(s) for s in row]
        key = row[0]
        if key in corners:
            print "%s is in corners" % key
            pos = [v*0.1 for v in row[1:4]]
            corners[key]["pos"] = tuple(pos)

    for k in corners:
        corners[k]["node"].attr("translate").set(*corners[k]["pos"])


def validate_board_data(data):
    if not len(data):
        raise ValueError("No board data from Google sheets")
    for row in data:
        if len(row) < 4:
            raise ValueError("Invalid board data from Google sheets")


