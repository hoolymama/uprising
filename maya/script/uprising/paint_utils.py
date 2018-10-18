import pymel.core as pm
import stroke_factory_utils as sfu
from brush import Brush
from robolink import (Robolink, ITEM_TYPE_ROBOT)
import uprising.uprising_util as uutl
import robodk as rdk
import sheets

# RL = Robolink()


# def add_all_trays_to_sf():
#     node = pm.ls(selection=True, dag=True, leaf=True, type="strokeFactory")[0]
#     paints = pm.ls(selection=True, dag=True, leaf=True, type="mesh")
#     for i, paint in enumerate(paints):
#         paint_tf = paint.getParent()
#         connect_paint_to_node(paint_tf, node, i)


def connect_paint_to_node(paint_tf, node, connect_to="next_available"):
    index = sfu.get_index(node, "paints.paintTravel", connect_to)
    whitelist = ["double", "float", "short", "bool", "string","float3", "double3"]
    atts = node.attr("paints[%d]" % index).getChildren()
    for att in atts:
        att_type = att.type()
        if att_type in whitelist:
            sfu.create_and_connect_driver(paint_tf, att)




# def send_paint_trays(dip_node):
#     RL = Robolink()
#     robot = RL.Item('', ITEM_TYPE_ROBOT)
#     p_indices = dip_node.attr("paints").getArrayIndices()
#     for i in p_indices:
#         att = sfu.input_connection(dip_node.attr("paints[%d].paintTravel" % i))
#         if att:
#             tray = att.node()
#             send_paint_tray(robot, tray)


# def send_paint_tray(robot, tray):
#     RL = Robolink()
#     geo = tray.getShapes()

#     triangles = []
#     for g in geo:
#         points = g.getPoints(space='world')
#         _, vids = g.getTriangles()
#         for vid in vids:
#             triangles.append(
#                 [points[vid].x * 10, points[vid].y * 10, points[vid].z * 10])

#     name = "tx_%s" % str(tray)

#     tray_item = RL.Item(name)
#     if tray_item.Valid():
#         tray_item.Delete()

#     cR = tray.attr("sfPaintColorR").get()
#     cG = tray.attr("sfPaintColorG").get()
#     cB = tray.attr("sfPaintColorB").get()
#     shape = RL.AddShape(triangles)
#     shape.setName(name)
#     shape.setColor([cR, cG, cB])


################# SETUP ##################
def validate_paint_data(data):
    if not len(data):
        raise ValueError("No paint data from Google sheets")
    for row in data:
        # if row[0].startswith("rack"):
        #     if not len(row) > 3:
        #         raise ValueError("Invalid rack corner data from Google sheets")
        # if uutl.numeric(row[0]) in range(8):
        if not len(row) > 9:
            raise ValueError("Invalid paint data from Google sheets")



def delete_shaders():
    if pm.ls("sx_*"):
        pm.delete("sx_*")


def set_tray_colors(tray_geos, colors):
    tray_cols = zip(tray_geos, colors)
    delete_shaders()
    for geo, color in tray_cols:
        geo.rename("tx_%s_%s" % (color["index"], color["name"]))
        ss = pm.shadingNode('lambert', asShader=True, name=("sx_%s" % color["name"]))
        sg = pm.sets(
            renderable=True,
            noSurfaceShader=True,
            empty=True,
            name=("sx_%s_SG" % color["name"]))
        ss.attr('outColor') >> sg.attr('surfaceShader')
        pm.sets(sg, edit=True, forceElement=geo)
        geo.attr("sfPaintColor") >> ss.attr('color')
        try:
            geo.attr("sfPaintColor").set([color["r"],color["g"],color["b"]])
        except RuntimeError:
            pm.warning("Can't set tray color. Skipping")
        try:
            geo.attr("sfPaintCode")
        except pm.MayaAttributeError:
            pm.addAttr(geo, dt="string", ln="sfPaintCode")
        code_att = geo.attr("sfPaintCode")
        code_att.set(color["code"])


def connect_trays(painting_node, dip_node, tray_geos):
    delete_paints(painting_node)
    delete_paints(dip_node)
    for i, geo in enumerate(tray_geos):
        connect_paint_to_node(geo, painting_node, i)
        connect_paint_to_node(geo, dip_node, i)

def set_up_trays(painting_node, dip_node, colors):
    top_node = uutl.assembly(dip_node)
    tray_geos = [x for x in pm.PyNode("%s|jpos|trays" % top_node ).getChildren() if x.startswith("tx")]
    connect_trays(painting_node, dip_node, tray_geos)
    set_tray_colors(tray_geos, colors)

def delete_paints(node):
    indices = node.attr("paints").getArrayIndices()
    for i in indices:
        pm.removeMultiInstance(node.attr("paints[%d]" % i), b=True)

def setup_paints_from_sheet(painting_node, dip_node):
    (data, medium) =  get_raw_paint_data()
    assembly= uutl.assembly(painting_node)
    notes_att, ground_att, medium_att  = sfu.ensure_painting_has_notes(assembly)
    medium_att.set(medium)

    validate_paint_data(data)
    colors = []
    for i, row in enumerate(data):
        color = {
            "index": i,
            "r": uutl.numeric(row[5]) / 255.0,
            "g": uutl.numeric(row[6]) / 255.0,
            "b": uutl.numeric(row[7]) / 255.0,
            "name": row[8],
            "code": row[9]
        }
        colors.append(color)
    set_up_trays(painting_node, dip_node, colors)

def set_up_rack_from_sheet(dip_node):
    top_node = uutl.assembly(dip_node)
    data = get_raw_rack_data()
    for row in data:
        loc_name = row[0]
        vals = [x*0.1 for x in row[1:4]]
        pm.PyNode("%s|%s" % (top_node, loc_name) ).attr("translate").set(*vals)


def get_raw_paint_data():
    service = sheets._get_service()
    result = service.spreadsheets().values().get(
        spreadsheetId=sheets.SHEETS["Measurements"],
        range='Paints!A14:J29').execute()
    values = result.get('values', [])

    medium_result = service.spreadsheets().values().get(
        spreadsheetId=sheets.SHEETS["Measurements"],
        range='Paints!J2').execute()
    medium = medium_result.get('values', [])[0][0]

    return (values, medium)



def get_raw_rack_data():
    service = sheets._get_service()
    result = service.spreadsheets().values().get(
        spreadsheetId=sheets.SHEETS["Measurements"],
        range='Rack!A1:D3').execute()
    return result.get('values', [])


