import pymel.core as pm
import stroke_factory_utils as sfu
import uprising.uprising_util as uutl

# import sheets
from paint import Paint
from brush import Brush
import const as k


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
    try:
        combos = pm.paintingQuery(painting_node, dc=True)
    except:
        pass
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


# def validate_paint_data(data):
#     if not len(data):
#         raise ValueError("No paint data from Google sheets")
#     for row in data:
#         if not len(row) > 4:
#             raise ValueError("Invalid paint data from Google sheets")


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


# def setup_paints_from_sheet(palette_name):
#     resource = sheets.get_resource_by_name(palette_name, "Paints")
#     data = resource["data"]

#     validate_paint_data(data)
#     colors = []
#     for i, row in enumerate(data):
#         color = {
#             "index": i,
#             "r": uutl.numeric(row[0]) / 255.0,
#             "g": uutl.numeric(row[1]) / 255.0,
#             "b": uutl.numeric(row[2]) / 255.0,
#             "name": row[3],
#             "code": row[4],
#         }
#         colors.append(color)
#     set_up_rack(colors)


def get_pot_handle_packs():
    result = {
        "pot": [],
        "handle": [],
        "face": []
    }

    period = 20
    for i, p in enumerate(pm.ls("RACK1_CONTEXT|j1|rack|holes|holeRot*|holeTrans")):
        _1, _2, pot_base,  handle_base, face_base = p.getChildren(
            type="transform")

        # row = CAL_SHEET_FIRST_ROW = 6 + i  # mm
        result["pot"].append(
            {
                "base": pot_base,
                "approach": pot_base.getChildren(type="transform")[0],
                "name": "pot_{:02d}".format(i),
                "index": i,
                "cell": "B:{}".format(k.CAL_SHEET_FIRST_ROW+i)
            }
        )
        result["handle"].append(
            {
                "base": handle_base,
                "approach": handle_base.getChildren(type="transform")[0],
                "name": "handle_{:02d}".format(i),
                "index": i,
                "cell": "B:{}".format(k.CAL_SHEET_FIRST_ROW+i+period)
            }
        )

        result["face"].append(
            {
                "base": face_base,
                "approach": face_base.getChildren(type="transform")[0],
                "name": "face_{:02d}".format(i),
                "index": i,
                "cell": "B:{}".format(k.CAL_SHEET_FIRST_ROW+i+period+period)
            }
        )

    return result


def get_pick_place_packs(brush_ids="used"):
    result = {}
    painting = pm.PyNode("mainPaintingShape")
    if brush_ids == "all":
        bids = [int(n[-2:])
                for n in pm.ls("RACK1_CONTEXT|j1|rack|holders|holderRot*")]
    elif brush_ids == "used":
        dc = pm.paintingQuery(painting, dc=True)
        bids = set(dc[::2])
    elif brush_ids == "calibration":
        bids = [0]
    else:
        bids = brush_ids

    holders_node = pm.PyNode("RACK1_CONTEXT|j1|rack|holders")
    path_attributes = {
        "lin_speed": holders_node.attr("linearSpeed").get() * 10,
        "precision_lin_speed": holders_node.attr("precisionLinearSpeed").get() * 10,
        "ang_speed": holders_node.attr("angularSpeed").get(),
        "rounding": holders_node.attr("approximationDistance").get() * 10,
    }
    for bid in bids:
        key = "b{:02d}".format(bid)
        trans = "holderRot{:02d}|holderTrans".format(bid)
        result[key] = {
            "trans_node": pm.PyNode(trans),
            "brush_id": bid,
            "probe": pm.PyNode("{}|probe_loc".format(trans)),
            "pin": pm.PyNode("{}|pin_loc".format(trans)),
            "pin_ap": pm.PyNode("{}|pin_approach_loc".format(trans)),
            "clear": pm.PyNode("{}|clear_loc".format(trans)),
            "clear_ap": pm.PyNode("{}|clear_approach_loc".format(trans)),
        }
        result[key].update(path_attributes)
    return result


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


def get_dip_wipe_packs(**kw):

    result = {}
    paint_id = kw.get("paint_id")
    if paint_id != None:
        combos = []
        dc = pm.paintingQuery("mainPaintingShape", dc=True)
        bids = sorted(set(dc[::2]))
        for brush_id in bids:
            combos.append({"brush": int(brush_id), "paint": paint_id})
    elif kw.get("dip_combinations"):
        combos = kw.get("dip_combinations")
    else:
        combos = dip_combination_ids()

    for combo in combos:

        dip_ptg_path = "rack|holes|holeRot_{:02d}|holeTrans|dip_loc|b{:02d}|*".format(
            combo["paint"], combo["brush"]
        )
        wipe_ptg_path = "rack|holes|holeRot_{:02d}|holeTrans|wipe_loc|b{:02d}|*".format(
            combo["paint"], combo["brush"]
        )

        paint_key = "p{:02d}".format(combo["paint"])
        brush_key = "b{:02d}".format(combo["brush"])

        try:
            dip_ptg = pm.ls(dip_ptg_path, type="painting")[0]
            wipe_ptg = pm.ls(wipe_ptg_path, type="painting")[0]
        except IndexError:
            raise IndexError(
                "Either dip or wipe node is missing: for {} {}".format(
                    paint_key, brush_key
                )
            )

        if paint_key not in result:
            result[paint_key] = {}
        result[paint_key][brush_key] = {
            "dip": dip_ptg,
            "wipe": wipe_ptg,
            "paint_id": combo["paint"],
            "brush_id": combo["brush"],
            "name": "{}_{}".format(paint_key, brush_key),
        }

    return result
