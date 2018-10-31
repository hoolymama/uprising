import pymel.core as pm

from paint import Paint
from brush import Brush

import sheets

import uprising.uprising_util as uutl

def input_connection(attribute):
    conns = attribute.connections(source=True, destination=False, plugs=True)
    return conns and conns[0]


def input_connections(attribute):
    conns = attribute.connections(source=True, destination=False, plugs=True)
    return conns




def ensure_painting_has_notes(assembly):
    
    try:
        notes_att = assembly.attr("notes")
    except pm.MayaAttributeError:
        pm.addAttr(assembly, dt="string", ln="notes")
        notes_att = assembly.attr("notes")

    try:
        ground_att = assembly.attr("ground")
    except pm.MayaAttributeError:
        pm.addAttr(assembly, dt="string", ln="ground")
        ground_att = assembly.attr("ground")

    try:
        medium_att = assembly.attr("medium")
    except pm.MayaAttributeError:
        pm.addAttr(assembly, dt="string", ln="medium")
        medium_att = assembly.attr("medium")

    try:
        palette_att = assembly.attr("palette")
    except pm.MayaAttributeError:
        pm.addAttr(assembly, dt="string", ln="palette")
        palette_att = assembly.attr("palette")


    return (notes_att, ground_att, medium_att, palette_att)


# def ensure_node_has_att(node, att, att_type):
#     att_types = ["bool", "int", "float"];
#     data_types = ["string"];
#     try:
#         result = node.attr(att)
#     except pm.MayaAttributeError:
#         if att_type in att_types:
#             pm.addAttr(node, at=att_type, ln=att)
#         else:
#             pm.addAttr(node, dt=att_type, ln=att)
#         result = node.attr(att)
#     return result

def get_index(node, att, connect_to):

    array_att, connection_att = att.split(".")
    indices = node.attr(array_att).getArrayIndices()
    index = 0
    if connect_to == "next_available":
        if not indices:
            return 0
        while(True):
            if index not in indices:
                break
            full_att = "%s[%d]" % (array_att, index)
            if connection_att:
                full_att = "%s.%s" % (full_att, connection_att)
            elif not pm.listConnections(node.attr(full_att), source=True, destination=False):
                break
            index += 1
    elif connect_to == "at_end":
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


def create_and_connect_driver(driver_node, att):

    painting_node = att.node()
    data_atts = ["string"]
    att_type = att.type()

    name = driver_att_name(att)
    val = att.get()

    if not pm.attributeQuery(name, node=driver_node, exists=True):
        if att_type in ["float3", "double3"]:
            att_type_base = att_type[:-1]
            child_atts = att.getChildren()
            att_tail_name = att.split(".")[-1]
            iscol = pm.attributeQuery(
                att_tail_name, uac=True, node=painting_node)
            pm.addAttr(driver_node, longName=name, at=att_type, uac=iscol)
            pm.addAttr(
                driver_node,
                longName=driver_att_name(
                    child_atts[0]),
                at=att_type_base,
                parent=name)
            pm.addAttr(
                driver_node,
                longName=driver_att_name(
                    child_atts[1]),
                at=att_type_base,
                parent=name)
            pm.addAttr(
                driver_node,
                longName=driver_att_name(
                    child_atts[2]),
                at=att_type_base,
                parent=name)
        elif att_type == "enum":
            enumdict = att.getEnums()
            spec = ":".join(["%s=%s" % (k, enumdict[k]) for k in enumdict])
            pm.addAttr(driver_node, longName=name, at=att_type, enumName=spec)
        elif att_type in data_atts:
            pm.addAttr(driver_node, longName=name, dt=att_type)
        else:
            pm.addAttr(driver_node, longName=name, at=att_type)
        driver_node.attr(name).setKeyable(True)
        driver_node.attr(name).set(val)
    if not driver_node.attr(name).isConnectedTo(att):
        driver_node.attr(name) >> att


# def create_stroke_factory():
#     node = pm.createNode("strokeFactory")
#     plane = pm.createNode("sketchPlane")
#     plane_transform = plane.getParent()
#     plane_transform.rename("projection")
#     plane.attr("worldMatrix[0]") >> node.attr("planeMatrix")
#     pos_grp = pm.group(node.getParent(), name="positioner")
#     crvs_grp = pm.group(empty=True, name="curves")
#     pm.group(plane_transform, pos_grp, crvs_grp, name="strokeFactoryGroup")

# def set_board_from_sheet(node):
#     #   get top node, then find corner locators
#     assembly = uutl.assembly(node)
#     _, ground_att, _, _  =ensure_painting_has_notes(assembly)

#     top_name = assembly.name()
#     corners = {
#         "BL": {
#             "node": pm.PyNode("%s|BL" % top_name)
#         },
#         "TL": {
#             "node": pm.PyNode("%s|TL" % top_name)
#         },
#         "TR": {
#             "node": pm.PyNode("%s|TR" % top_name)
#         }
#     }

#     data, offset, ground = sheets.get_raw_board_data()
#     ground_att.set(ground)

#     validate_board_data(data)
#     offset = uutl.numeric(offset) * 0.1

#     tmp_locs = {}
#     tmp_locs["BL"] = pm.spaceLocator()
#     tmp_locs["TL"] = pm.spaceLocator()
#     tmp_locs["TR"] = pm.spaceLocator()

#     for row in data:
#         row = [uutl.numeric(s) for s in row]
#         key = row[0]
#         if key in corners:
#             pos = [v * 0.1 for v in row[1:4]]
#             corners[key]["pos"] = tuple(pos)

#     for k in corners:
#         corners[k]["node"].attr("translate").set(*corners[k]["pos"])
#         tmp_locs[k].attr("translate").set(*corners[k]["pos"])

    # move mainPainting into position and parent tmp_locs



    # zppos = assembly.attr("zeroPosition").get()
    # assembly.attr("zeroPosition").set(False)
    # for k in corners:
    #     parent_joint = "%s|jpos" % assembly
    #     pm.parent(tmp_locs[k], parent_joint)
    #     tmp_locs[k].attr("tz").set(offset)
    #     pm.parent(tmp_locs[k], world=True)
    #     off_pos = tmp_locs[k].attr("translate").get()
    #     corners[k]["node"].attr("translate").set(off_pos)
    # assembly.attr("zeroPosition").set(zppos)

    # pm.delete([tmp_locs[k] for k in tmp_locs])
 

# def validate_board_data(data):
#     if not len(data):
#         raise ValueError("No board data from Google sheets")
#     for row in data:
#         if len(row) < 4:
#             raise ValueError("Invalid board data from Google sheets")
