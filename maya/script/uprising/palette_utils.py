import pymel.core as pm
import stroke_factory_utils as sfu
import uprising.uprising_util as uutl

def get_pot_handle_pairs():
   return zip (
    pm.ls("rack*|holes|holeRot*|holeTrans|dip_loc|pot*"), 
    pm.ls("rack*|holes|holeRot*|holeTrans|wipe_loc|handle")
    )

def get_used_pot_handle_pairs():
    return [pair for pair in get_pot_handle_pairs() if not pair[0].split("|")[-1] == "pot"]


def get_used_pots():
    return zip(*get_used_pot_handle_pairs())[0] 

def get_pots():
   return zip(*get_pot_handle_pairs())[0] 

def select_used_pots():
    pm.select(get_used_pots())

def select_pots():
    pm.select(get_pots())


def get_used_handles():
    return zip(*get_used_pot_handle_pairs())[1] 

def get_handles():
    return zip(*get_pot_handle_pairs())[1] 

def select_used_handles():
    pm.select(get_used_handles())

def select_handles():
    pm.select(get_handles())





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

def set_up_rack(colors):
    pots = pm.ls("rack*|holes|holeRot*|holeTrans|dip_loc|pot*")
    used_pot_cols = set_pot_colors(pots, colors)
    used_pots = zip(*used_pot_cols)[0]
    connect_pots(used_pots)

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

def connect_pots(pots):
    painting_node = pm.PyNode("mainPaintingShape")
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

 
def setup_paints_from_sheet(palette_name):
    resource =  sheets.get_resource_by_name(palette_name, "Paints")
    data =  resource["data"]

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
    set_up_rack(colors)


# def generate_rack_calibration():
#     timestamp = write.get_timestamp()
#     cdir = os.path.join(pm.workspace.getPath(), 'export', 'calibrations', 'rack', timestamp)
#     uutl.mkdir_p(cdir)
#     studio = Studio(rack_calibration=True)
#     #     studio.write()
#     # write_station(RL, ts_dir, timestamp)

#     # if kw.get("painting_node"):
#     #     write_program(RL, ts_dir, "px", timestamp)



# def read_rack_calibration():
#     pass


 