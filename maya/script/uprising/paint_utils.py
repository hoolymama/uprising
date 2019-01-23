import pymel.core as pm
import stroke_factory_utils as sfu
from brush import Brush
from robolink import (Robolink, ITEM_TYPE_ROBOT)
import uprising.uprising_util as uutl
import robodk as rdk
import sheets





################# SETUP ##################
def validate_paint_data(data):
    if not len(data):
        raise ValueError("No paint data from Google sheets")
    for row in data:
        if not len(row) > 4:
            raise ValueError("Invalid paint data from Google sheets")

def delete_shaders():
    if pm.ls("sx_*"):
        pm.delete("sx_*")

def delete_paints(node):
    indices = node.attr("paints").getArrayIndices()
    for i in indices:
        pm.removeMultiInstance(node.attr("paints[%d]" % i), b=True)

def set_up_rack(painting_node, rack, colors):
    pots = pm.ls("%s|holes|holeRot*|holeTrans|dip_loc|pot*" % rack)
    used_pot_cols = set_pot_colors(pots, colors)
    used_pots = zip(*used_pot_cols)[0]
    connect_pots(painting_node, used_pots)

def set_pot_colors(pots, colors):
    pot_cols = zip(pots, colors)
    delete_shaders()
    for geo, color in pot_cols:
        geo.rename("pot_%s_%s" % (color["index"], color["name"]))
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
    return pot_cols


def connect_pots(painting_node, pots):
    delete_paints(painting_node)
    for i, pot in enumerate(pots):
        connect_paint_to_node(pot, painting_node, i)
 
def connect_paint_to_node(pot, node, connect_to="next_available"):
    index = sfu.get_index(node, "paints.paintTravel", connect_to)
    whitelist = ["double", "float", "short", "bool", "string","float3", "double3"]
    atts = node.attr("paints[%d]" % index).getChildren()
    for att in atts:
        att_type = att.type()
        if att_type in whitelist:
            sfu.create_and_connect_driver(pot, att)

 
def setup_paints_from_sheet(painting_node, rack, palette_name):
    # (palette_name, medium, palette) =  get_palette_by_name(palette_name)
    resource =  sheets.get_resource_by_name(palette_name, "Paints")
    medium = resource["args"][0]
    palette_name = resource["name"]
    data =  resource["data"]

    assembly= uutl.assembly(painting_node)
    _, _, medium_att, palette_name_att = sfu.ensure_painting_has_notes(assembly)
    medium_att.set(medium)
    palette_name_att.set(palette_name)


    validate_paint_data(data)
    colors = []
    for i, row in enumerate(data):
        color = {
            "index": i,
            "r": uutl.numeric(row[0]) / 255.0,
            "g": uutl.numeric(row[1]) / 255.0,
            "b": uutl.numeric(row[2]) / 255.0,
            "name": row[3],
            "code": row[4]
        }
        colors.append(color)
    # set_up_trays(painting_node, dip_node, colors)
    set_up_rack(painting_node, rack, colors)


def set_up_rack_from_sheet(dip_node):
    top_node = uutl.assembly(dip_node)
    data = get_raw_rack_data()
    for row in data:
        loc_name = row[0]
        vals = [x*0.1 for x in row[1:4]]
        pm.PyNode("%s|%s" % (top_node, loc_name) ).attr("translate").set(*vals)

def get_raw_rack_data():
    service = sheets._get_service()
    result = service.spreadsheets().values().get(
        spreadsheetId=sheets.SHEETS["Measurements"],
        range='Rack!A1:D3').execute()
    return result.get('values', [])


# def setup_paints_from_sheet(painting_node, dip_node, palette_name):
#     # (palette_name, medium, palette) =  get_palette_by_name(palette_name)
#     (palette_name, medium, palette) = sheets.get_resource_by_name(palette_name, "Paints")
#     assembly= uutl.assembly(painting_node)
#     _, _, medium_att, palette_name_att = sfu.ensure_painting_has_notes(assembly)
#     medium_att.set(medium)
#     palette_name_att.set(palette_name)

#     validate_paint_data(palette)
#     colors = []
#     for i, row in enumerate(palette):
#         color = {
#             "index": i,
#             "r": uutl.numeric(row[0]) / 255.0,
#             "g": uutl.numeric(row[1]) / 255.0,
#             "b": uutl.numeric(row[2]) / 255.0,
#             "name": row[3],
#             "code": row[4]
#         }
#         colors.append(color)
#     set_up_trays(painting_node, dip_node, colors)




# RL = Robolink()
# def get_raw_paint_data():
#     service = sheets._get_service()

#     result = service.spreadsheets().values().get(
#         spreadsheetId=sheets.SHEETS["Measurements"],
#         range='Paints!A14:J29').execute()
#     values = result.get('values', [])

#     medium_result = service.spreadsheets().values().get(
#         spreadsheetId=sheets.SHEETS["Measurements"],
#         range='Paints!J2').execute()
#     medium = medium_result.get('values', [])[0][0]

#     return (values, medium)



# def add_all_trays_to_sf():
#     node = pm.ls(selection=True, dag=True, leaf=True, type="strokeFactory")[0]
#     paints = pm.ls(selection=True, dag=True, leaf=True, type="mesh")
#     for i, paint in enumerate(paints):
#         paint_tf = paint.getParent()
#         connect_paint_to_node(paint_tf, node, i)



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


# def get_measurements_values(cell_range, service, dimension="ROWS"):
#     result = service.spreadsheets().values().get(
#             spreadsheetId=sheets.SHEETS["Measurements"],
#             range=cell_range,majorDimension=dimension).execute()
#     return result.get('values', [])

# def get_palette_header(search_str, service):
#     batch_size = 100
#     batches = 10
#     total_rows = batch_size * batches
#     for r,x in [("Paints!A%d:A%d" % (x+1, x+batch_size) , x) for x in xrange(0,total_rows,batch_size) ]:
#         values = sheets.get_measurements_values(r,service, "COLUMNS")
#         if values:
#             for i, v in enumerate(values[0]):
#                 if v == search_str:
#                     row = (x+i+1)
#                     cell_range = "Paints!A%d:B%d"  % (row,row)
#                     header_values = sheets.get_measurements_values(cell_range,service)[0]
#                     header_values.append(row)
#                     return tuple(header_values)



# def get_palette_by_name(name):
#     service = sheets._get_service()
#     # name, medium, row = get_palette_header(name, service)

#     header =  sheets.get_named_header(name, "Paints", service)
#     print "HEADER"
#     print header
#     if header:
#         name, medium, row = header
#         cell_range = "Paints!A%d:E%d"  % (row+1,row+64)
#         palette = sheets.get_measurements_values(cell_range,service)
#         new_palette = []
#         for  entry in palette:
#             if len(entry) == 0:
#                 break
#             new_palette.append(entry)

#         # new_palette = [entry for entry in palette if len(entry) >= 5]
#         return tuple([name, medium, new_palette])
# def connect_trays(painting_node, dip_node, tray_geos):
#     delete_paints(painting_node)
#     delete_paints(dip_node)
#     for i, geo in enumerate(tray_geos):
#         connect_paint_to_node(geo, painting_node, i)
#         connect_paint_to_node(geo, dip_node, i)

# def set_up_trays(painting_node, dip_node, colors):
#     top_node = uutl.assembly(dip_node)
#     tray_geos = [x for x in pm.PyNode("%s|jpos|trays" % top_node ).getChildren() if x.startswith("tx")]
#     connect_trays(painting_node, dip_node, tray_geos)
#     set_tray_colors(tray_geos, colors)
