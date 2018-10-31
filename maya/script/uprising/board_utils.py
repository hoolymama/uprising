import pymel.core as pm
import stroke_factory_utils as sfu
from brush import Brush
from robolink import (Robolink, ITEM_TYPE_ROBOT)
import uprising.uprising_util as uutl
import robodk as rdk
import sheets


# def connect_paint_to_node(paint_tf, node, connect_to="next_available"):
#     index = sfu.get_index(node, "paints.paintTravel", connect_to)
#     whitelist = ["double", "float", "short", "bool", "string","float3", "double3"]
#     atts = node.attr("paints[%d]" % index).getChildren()
#     for att in atts:
#         att_type = att.type()
#         if att_type in whitelist:
#             sfu.create_and_connect_driver(paint_tf, att)


# def delete_shaders():
#     if pm.ls("sx_*"):
#         pm.delete("sx_*")


# def set_tray_colors(tray_geos, colors):
#     tray_cols = zip(tray_geos, colors)
#     delete_shaders()
#     for geo, color in tray_cols:
#         geo.rename("tx_%s_%s" % (color["index"], color["name"]))
#         ss = pm.shadingNode('lambert', asShader=True, name=("sx_%s" % color["name"]))
#         sg = pm.sets(
#             renderable=True,
#             noSurfaceShader=True,
#             empty=True,
#             name=("sx_%s_SG" % color["name"]))
#         ss.attr('outColor') >> sg.attr('surfaceShader')
#         pm.sets(sg, edit=True, forceElement=geo)
#         geo.attr("sfPaintColor") >> ss.attr('color')
#         try:
#             geo.attr("sfPaintColor").set([color["r"],color["g"],color["b"]])
#         except RuntimeError:
#             pm.warning("Can't set tray color. Skipping")
#         try:
#             geo.attr("sfPaintCode")
#         except pm.MayaAttributeError:
#             pm.addAttr(geo, dt="string", ln="sfPaintCode")
#         code_att = geo.attr("sfPaintCode")
#         code_att.set(color["code"])


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

# def delete_paints(node):
#     indices = node.attr("paints").getArrayIndices()
#     for i in indices:
#         pm.removeMultiInstance(node.attr("paints[%d]" % i), b=True)

# def set_up_rack_from_sheet(dip_node):
#     top_node = uutl.assembly(dip_node)
#     data = get_raw_rack_data()
#     for row in data:
#         loc_name = row[0]
#         vals = [x*0.1 for x in row[1:4]]
#         pm.PyNode("%s|%s" % (top_node, loc_name) ).attr("translate").set(*vals)


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


################# SETUP ##################
def validate_board_data(data):
    if not len(data):
        raise ValueError("No paint data from Google sheets")
    for row in data:
        # if row[0].startswith("rack"):
        #     if not len(row) > 3:
        #         raise ValueError("Invalid rack corner data from Google sheets")
        # if uutl.numeric(row[0]) in range(8):
        if not len(row) > 3:
            raise ValueError("Invalid paint data from Google sheets")


def get_current_corner(locname, assembly):
    node = pm.PyNode("%s|%s" % (assembly, locname))
    return {"node": node, "current_pos": node.attr("translate").get()}


def setup_board_from_sheet(node, name, depth_only):
    # (palette_name, medium, palette) =  get_palette_by_name(palette_name)
    (name, ground, data) = sheets.get_resource_by_name(name, "Board")
    assembly = uutl.assembly(node)
    canvas = pm.PyNode("%s|jpos|canvas" % assembly)
    _, ground_att, _, _ = sfu.ensure_painting_has_notes(assembly)
    ground_att.set(ground)

    corners = {}
    # corners["BL"] = get_current_corner("BL", assembly)
    # corners["TL"] = get_current_corner("TL", assembly)
    # corners["TR"] = get_current_corner("TR", assembly)
    # corners["BR"] = get_current_corner("BR", assembly)

    validate_board_data(data)

    for row in data:
        row = [uutl.numeric(s) for s in row]
        key = row[0]
        corners[key] = get_current_corner(key, assembly)
        pos = [v * 0.1 for v in row[1:4]]
        corners[key]["new_pos"] = pm.dt.Vector(pos)

    print corners

    if not depth_only:
        for k in corners:
            corners[k]["node"].attr("translate").set(*corners[k]["new_pos"])
    else:
        # need to figure out the depth at the corners
        width = canvas.attr("width").get()
        height = canvas.attr("height").get()

        up = (corners["TL"]["current_pos"] -
              corners["BL"]["current_pos"]).normal()
        right = (
            corners["TR"]["current_pos"] -
            corners["TL"]["current_pos"]).normal()

        new_width = (
            corners["TR"]["new_pos"] -
            corners["TL"]["new_pos"]).length()
        new_height = (
            corners["TL"]["new_pos"] -
            corners["BL"]["new_pos"]).length()

        diff_width = width - new_width
        diff_height = height - new_height

        new_TL = corners["TL"]["new_pos"] + \
            (up * diff_height * 0.5) + (-right * diff_width * 0.5)
        new_BL = corners["BL"]["new_pos"] + \
            (-up * diff_height * 0.5) + (-right * diff_width * 0.5)
        new_TR = corners["TR"]["new_pos"] + \
            (up * diff_height * 0.5) + (right * diff_width * 0.5)
        new_BR = corners["BR"]["new_pos"] + \
            (-up * diff_height * 0.5) + (right * diff_width * 0.5)

        # these are now potentioally good corners - however, we only set the
        # new depths
        corners["TL"]["node"].attr("translateY").set(new_TL.y)
        corners["BL"]["node"].attr("translateY").set(new_BL.y)
        corners["TR"]["node"].attr("translateY").set(new_TR.y)
        corners["BR"]["node"].attr("translateY").set(new_BR.y)

        # tmp_locs = {}
        # tmp_locs["BL"] = pm.spaceLocator()
        # tmp_locs["BL"].rename("BL_tmp")
        # tmp_locs["TL"] = pm.spaceLocator()
        # tmp_locs["TL"].rename("TL_tmp")
        # tmp_locs["TR"] = pm.spaceLocator()
        # tmp_locs["TR"].rename("TR_tmp")
        # tmp_locs["BR"] = pm.spaceLocator()
        # tmp_locs["BR"].rename("BR_tmp")

        # for k in corners:
        #     tmp_locs[k].attr("translate").set(*corners[k]["new_pos"])
