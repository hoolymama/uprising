import pymel.core as pm
import stroke_factory_utils as sfu
from brush import Brush
from robolink import (Robolink, ITEM_TYPE_ROBOT)
import uprising.uprising_util as uput
import robodk as rdk
import sheets

# RL = Robolink()


def add_all_trays_to_sf():
    node = pm.ls(selection=True, dag=True, leaf=True, type="strokeFactory")[0]
    paints = pm.ls(selection=True, dag=True, leaf=True, type="mesh")
    for i, paint in enumerate(paints):
        paint_tf = paint.getParent()
        connect_paint_to_node(paint_tf, node, i)


def connect_paint_to_node(paint_tf, node, connect_to="next_available"):
    index = sfu.get_index(node, "paints.paintTravel", connect_to)
    whitelist = ["double", "float", "short", "bool", "string","float3", "double3"]
    atts = node.attr("paints[%d]" % index).getChildren()
    for att in atts:
        att_type = att.type()
        if att_type in whitelist:
            sfu.create_and_connect_driver(paint_tf, att)

def send_paints(factory):
    RL = Robolink()
    robot = RL.Item('', ITEM_TYPE_ROBOT)
    p_indices = factory.attr("paints").getArrayIndices()
    for i in p_indices:
        att = sfu.input_connection(factory.attr("paints[%d].paintOpacity" % i))
        if att:
            tray = att.node()
            send_paint(robot, tray)


def send_paint(robot, tray):
    RL = Robolink()
    geo = tray.getShapes()

    triangles = []
    for g in geo:
        points = g.getPoints(space='world')
        _, vids = g.getTriangles()
        for vid in vids:
            triangles.append(
                [points[vid].x * 10, points[vid].y * 10, points[vid].z * 10])

    name = "tx_%s" % str(tray)

    tray_item = RL.Item(name)
    if tray_item.Valid():
        tray_item.Delete()

    cR = tray.attr("sfPaintColorR").get()
    cG = tray.attr("sfPaintColorG").get()
    cB = tray.attr("sfPaintColorB").get()
    shape = RL.AddShape(triangles)
    shape.setName(name)
    shape.setColor([cR, cG, cB])


def validate_paint_data(data):
    if not len(data):
        raise ValueError("No paint data from Google sheets")
    for row in data:
        if row[0].startswith("rack"):
            if not len(row) > 3:
                raise ValueError("Invalid rack corner data from Google sheets")
        elif uput.numeric(row[0]) in range(8):
            if not len(row) > 7:
                raise ValueError("Invalid paint data from Google sheets")


def set_rack_position(dip_node, locators):

    top_node = sfu.assembly(dip_node)

    for key in locators:
        vals = [x*0.1 for x in locators[key]]
        pm.PyNode("%s|%s" % (top_node, key) ).attr("translate").set(*vals)

def delete_shaders():
    if pm.ls("sx_*"):
        pm.delete("sx_*")


def set_tray_colors(tray_geos, colors):

    delete_shaders()

    for i,color in enumerate(colors):
        geo = tray_geos[i]
        col = color[0:3]
        name = color[3]
        geo.rename("tx_%s_%s" % (i, name))

        ss = pm.shadingNode('lambert', asShader=True, name=("sx_%s" % name))
        sg = pm.sets(
            renderable=True,
            noSurfaceShader=True,
            empty=True,
            name=("sx_%s_SG" % name))
        ss.attr('outColor') >> sg.attr('surfaceShader')
        pm.sets(sg, edit=True, forceElement=geo)
        geo.attr("sfPaintColor") >> ss.attr('color')
        geo.attr("sfPaintColor").set(col)


def connect_trays(painting_node, dip_node, tray_geos):
    delete_paints(painting_node)
    delete_paints(dip_node)
    for i, geo in enumerate(tray_geos):
        connect_paint_to_node(geo, painting_node, i)
        connect_paint_to_node(geo, dip_node, i)

def set_up_trays(painting_node, dip_node, colors):
    top_node = sfu.assembly(dip_node)
    tray_geos = [x for x in pm.PyNode("%s|jpos|trays" % top_node ).getChildren() if x.startswith("tx")]
    connect_trays(painting_node, dip_node, tray_geos)
    set_tray_colors(tray_geos, colors)

def delete_paints(node):
    indices = node.attr("paints").getArrayIndices()
    for i in indices:
        pm.removeMultiInstance(node.attr("paints[%d]" % i), b=True)

def setup_paints_from_sheet(painting_node, dip_node):
    data =  get_raw_paint_data()
    validate_paint_data(data)
    colors = list(([],)*8)
    locators =  {}
    for row in data:
        row = [uput.numeric(s) for s in row]
        if row[0] in range(8):
            colors[int(row[0])] = row[5:9]
        elif str(row[0]).startswith("rack"):
            locators[row[0]] = row[1:4]
        if row[0] == 7:
            locators["rack_P7"] = row[1:4]

    set_rack_position(dip_node, locators)

    set_up_trays(painting_node, dip_node, colors)



 

def get_raw_paint_data():
    service = sheets._get_service()
    result = service.spreadsheets().values().get(
        spreadsheetId=sheets.SHEETS["Measurements"],
        range='Paints!A2:I11').execute()
    values = result.get('values', [])
    return values




