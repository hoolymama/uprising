import pymel.core as pm
import stroke_factory_utils as sfu

from paint import Paint
from brush import Brush
 

def dip_combinations():

    painting_node = pm.PyNode("mainPaintingShape")
    result = {}

    brushes = Brush.brushes(painting_node)
    paints = Paint.paints(painting_node)

    combos = pm.paintingQuery(painting_node, dc=True)

    for i in range(0, len(combos), 2):
        brush_id = int(combos[i])
        paint_id = int(combos[i + 1])
        key = "p%02d_b%02d" % (paint_id, brush_id)

        try:
            b = brushes[brush_id]
            p = paints[paint_id]
        except KeyError:
            raise KeyError("Bad Brush or Paint ID")

        result[key] = {"brush": b, "paint": p}
    return result


def dip_combination_ids():
    painting_node = pm.PyNode("mainPaintingShape")
    result = []

    combos = pm.paintingQuery(painting_node, dc=True) or []
 
    for i in range(0, len(combos), 2):
        result.append({"brush": int(combos[i]), "paint": int(combos[i + 1])})
    return result


def get_pot_handle_pairs():
    return zip(
        pm.ls("rack|holes|holeRot*|holeTrans|dip_loc|pot*"),
        pm.ls("rack|holes|holeRot*|holeTrans|wipe_loc|handle"),
    )


def get_used_pot_handle_pairs():
    return [
        pair for pair in get_pot_handle_pairs() if not pair[0].split("|")[-1] == "pot"
    ]


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


def clean_palette():
    painting_node = pm.PyNode("mainPaintingShape")
    delete_paints(painting_node)
    for painting in pm.ls(
        "rack|holes|holeRot*|holeTrans", dag=True, leaf=True, type="painting"
    ):
        pm.delete(painting.getParent())
    for pot in pm.ls("rack|holes|holeRot*|holeTrans|dip_loc|pot*"):
        pot.rename("pot")


def delete_shaders():
    if pm.ls("sx_*"):
        pm.delete("sx_*")


def delete_paints(node):
    indices = node.attr("paints").getArrayIndices()
    for i in indices:
        pm.removeMultiInstance(node.attr("paints[%d]" % i), b=True)


def set_up_rack(colors):
    pots = pm.ls("rack|holes|holeRot*|holeTrans|dip_loc|pot*")
    used_pot_cols = set_pot_colors(pots, colors)
    used_pots = zip(*used_pot_cols)[0]
    connect_pots(used_pots)


def set_pot_colors(pots, colors):
    pot_cols = zip(pots, colors)
    delete_shaders()
    for geo, color in pot_cols:
        ss = pm.shadingNode("lambert", asShader=True,
                            name=("sx_%s" % color["name"]))
        sg = pm.sets(
            renderable=True,
            noSurfaceShader=True,
            empty=True,
            name=("sx_%s_SG" % color["name"]),
        )
        ss.attr("outColor") >> sg.attr("surfaceShader")
        pm.sets(sg, edit=True, forceElement=geo)
        geo.attr("sfPaintColor") >> ss.attr("color")
        try:
            geo.attr("sfPaintColor").set([color["r"], color["g"], color["b"]])
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
    whitelist = ["double", "float", "short",
                 "bool", "string", "float3", "double3"]
    atts = node.attr("paints[%d]" % index).getChildren()
    for att in atts:
        att_type = att.type()
        if att_type in whitelist:
            sfu.create_and_connect_driver(pot, att)


def get_perspex_packs():
    result = []

    for i, p in enumerate(
        pm.ls("RACK1_CONTEXT|j1|rack|probes|rackCalRot*|rackCalLocal")
    ):
        if p.attr("v").get() and p.getParent().attr("v").get():
            approach, base = [x for x in p.getChildren(type="transform")]
            name = base.split("|")[0]
            result.append(
                {"base": base, "approach": approach, "name": name, "index": i}
            )
    return result
