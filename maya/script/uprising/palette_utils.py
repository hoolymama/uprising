# from typing import Set
import pymel.core as pm
from uprising import stroke_factory_utils as sfu

from uprising.paint import Paint
from uprising.brush import Brush
from uprising import const as k




def brush_pot_combination_ids():
    painting_node = pm.PyNode(k.PAINTING_NAME)
    combos = pm.paintingQuery(painting_node, toolCombinations=True) or []
    s=set()
    for i in range(0, len(combos), 3):
        s.add((int(combos[i]),  int(combos[i + 2])))
    return sorted([{"brush":b,"pot":p} for (b,p) in s], key=lambda k: (k["brush"], k["pot"]))

def brush_paint_combination_ids():
    painting_node = pm.PyNode(k.PAINTING_NAME)
    combos = pm.paintingQuery(painting_node, toolCombinations=True) or []
    s=set()
    for i in range(0, len(combos), 3):
        s.add((int(combos[i]),  int(combos[i + 1])))
    return sorted([{"brush":b,"paint":p} for (b,p) in s], key=lambda k: (k["brush"], k["paint"]))


def get_pot_handle_pairs():
    return list(zip(
        pm.ls("rack|holes|holeRot*|holeTrans|dip_loc|pot*"),
        pm.ls("rack|holes|holeRot*|holeTrans|wipe_loc|handle"),
    ))


def get_used_pot_handle_pairs():
    return [
        pair for pair in get_pot_handle_pairs() if not pair[0].split("|")[-1] == "pot"
    ]


def get_used_pots():
    return list(zip(*get_used_pot_handle_pairs()))[0]


def get_pots():
    return list(zip(*get_pot_handle_pairs()))[0]


def select_used_pots():
    pm.select(get_used_pots())


def select_pots():
    pm.select(get_pots())


def get_used_handles():
    return list(zip(*get_used_pot_handle_pairs()))[1]


def get_handles():
    return list(zip(*get_pot_handle_pairs()))[1]


def select_used_handles():
    pm.select(get_used_handles())


def select_handles():
    pm.select(get_handles())




def delete_shaders():
    if pm.ls("sx_*"):
        pm.delete("sx_*")




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
