import os
import pymel.core as pm
import palette_utils as putl
import write
import uprising.uprising_util as uutl
import props
from studio import Studio
from robolink import Robolink
import sheets
import const as k


def create():
    menu = pm.menu(label="Calibration", tearOff=True)

    pm.menuItem(label="Rack setup", subMenu=True)

    pm.menuItem(
        label="Create manual triangulation program",
        command=pm.Callback(create_manual_triangulation))

    pm.menuItem(
        label="Read rack triangulation",
        command=pm.Callback(read_rack_triangulation))

    pm.menuItem(divider=True)

    pm.menuItem(
        label="Generate paintpot calibration",
        command=pm.Callback(generate_pot_calibration))

    pm.menuItem(
        label="Read paintpot calibration",
        command=pm.Callback(read_pot_calibration))

    pm.menuItem(divider=True)

    pm.menuItem(
        label="Generate holder calibration",
        command=pm.Callback(generate_holder_calibration))

    pm.menuItem(
        label="Read holder calibration",
        command=pm.Callback(read_holder_calibration))

    pm.menuItem(divider=True)

    pm.menuItem(
        label="Generate perspex calibration",
        command=pm.Callback(generate_perspex_calibration))

    pm.menuItem(
        label="Read perspex calibration",
        command=pm.Callback(read_perspex_calibration))

    pm.setParent("..", menu=True)

    pm.menuItem(label="Board setup", subMenu=True)

    pm.menuItem(
        label="Read board triangulation",
        command=pm.Callback(read_board_triangulation))

    pm.menuItem(
        label="Generate board calibration",
        command=pm.Callback(generate_board_calibration))

    pm.menuItem(
        label="Read board calibration",
        command=pm.Callback(read_board_calibration))

    pm.setParent("..", menu=True)

    pm.menuItem(
        label="Generate Pick/Place exercise",
        command=pm.Callback(generate_pick_place_exercise))
    return menu

########################################
########################################


def generate_pick_place_exercise():

    timestamp = write.get_timestamp()
    ppdir = os.path.join(
        pm.workspace.getPath(),
        'export',
        'calibrations',
        k.PAP_EXERCISE_PROGRAM_NAME,
        timestamp)
    uutl.mkdir_p(ppdir)
    studio = Studio(do_pap_exercise=True, pick_and_place_slots="all")
    studio.write()

    write.write_program(Robolink(), ppdir,
                        k.PAP_EXERCISE_PROGRAM_NAME, timestamp)


def _read_triangulation(sheet_range):
    service = sheets._get_service()
    result = service.spreadsheets().values().get(
        spreadsheetId=sheets.SHEETS["Measurements"],
        range=sheet_range).execute()

    data = result.get('values', [])

    for row in data:
        loc_name = row[0]
        try:
            node = pm.PyNode(loc_name)
        except pm.MayaNodeError:
            node = pm.spaceLocator()
            node.rename(loc_name)

        vals = [uutl.numeric(x) * 0.1 for x in row[1:4]]
        print "{} - set {} {} {}".format(node, vals[0], vals[1], vals[2])
        node.attr("translate").set(*vals)


def _generate_calibration(which, *reference_geo):

    kw = {
        "do_perspex_triangulation": which == k.TRI_CALIBRATION_PROGRAM_NAME,
        "do_pot_calibration": which == k.POT_CALIBRATION_PROGRAM_NAME,
        "do_holder_calibration": which == k.HOLDER_CALIBRATION_PROGRAM_NAME,
        "do_board_calibration": which == k.BOARD_CALIBRATION_PROGRAM_NAME,
        "do_perspex_calibration": which == k.PERSPEX_CALIBRATION_PROGRAM_NAME,
        "pick_and_place_slots": "calibration",  # all, used, specific ids
        "pause": -1
    }

    timestamp = write.get_timestamp()
    calib_dir = os.path.join(
        pm.workspace.getPath(),
        'export',
        'calibrations',
        which,
        timestamp)
    uutl.mkdir_p(calib_dir)
    studio = Studio(**kw)
    studio.write()

    write.write_program(Robolink(), calib_dir, which, timestamp)
    props.send(reference_geo)


def read_rack_triangulation():
    _read_triangulation('Rack!A1:D3')


def read_board_triangulation():
    _read_triangulation('Board!A1:D3')

    height = (pm.PyNode("board_TL").attr("translate").get() -
              pm.PyNode("board_BL").attr("translate").get()).length()
    width = (pm.PyNode("board_TR").attr("translate").get() -
             pm.PyNode("board_TL").attr("translate").get()).length()

    pm.PyNode("canvas").attr("width").set(width)
    pm.PyNode("canvas").attr("height").set(height)


def create_manual_triangulation():
    ref_geo = pm.PyNode("rackTop")
    _generate_calibration(k.TRI_CALIBRATION_PROGRAM_NAME, ref_geo)


def generate_pot_calibration():
    ref_geo = [pm.PyNode("rackTop")]
    ref_geo += pm.ls("holes|*|holeTrans|dip_loc|pot")
    ref_geo += pm.ls("holes|*|holeTrans|wipe_loc|handle")
    _generate_calibration(k.POT_CALIBRATION_PROGRAM_NAME, *ref_geo)


def generate_holder_calibration():
    ref_geo = [pm.PyNode("rackTop")]
    ref_geo += pm.ls("holders|*|holderTrans|lowResGeo")
    _generate_calibration(k.HOLDER_CALIBRATION_PROGRAM_NAME, *ref_geo)


def generate_perspex_calibration():
    ref_geo = pm.PyNode("rackTop")
    _generate_calibration(k.PERSPEX_CALIBRATION_PROGRAM_NAME, ref_geo)


def generate_board_calibration():
    ref_geo = pm.PyNode("canvas")
    _generate_calibration(k.BOARD_CALIBRATION_PROGRAM_NAME)


def _set_precise(xf, gauge_reading, probe_height):
    new_pos = probe_height + (gauge_reading * 0.1) - 1.0
    xf.attr("tz").set(new_pos)


def read_pot_calibration():
    service = sheets._get_service()
    result = service.spreadsheets().values().get(
        spreadsheetId=sheets.SHEETS["Measurements"],
        range='Rack!A6:C25').execute()

    data = result.get('values', [])

    pots = putl.get_pots()
    handles = putl.get_handles()
    if not (len(data) == len(pots) and len(data) == len(handles)):
        raise IndexError("Sheet data and number of pots are different lengths")

    pot_depth = pm.PyNode("rack|holes").attr("calibrationPotDepth").get()
    handle_height = pm.PyNode("rack|holes").attr(
        "calibrationHandleHeight").get()

    for row in data:
        i = int(uutl.numeric(row[0]))
        _set_precise(pots[i].getParent(), uutl.numeric(row[1]), pot_depth)
        _set_precise(
            handles[i].getParent(),
            uutl.numeric(
                row[2]),
            handle_height)


def read_holder_calibration():
    service = sheets._get_service()
    result = service.spreadsheets().values().get(
        spreadsheetId=sheets.SHEETS["Measurements"],
        range='Rack!A6:G25').execute()

    data = result.get('values', [])

    holders = pm.ls("RACK1_CONTEXT|j1|rack|holders|holderRot*|holderTrans")
    if not len(data) == len(holders):
        raise IndexError("Sheet data and number of holders are different")

    for row in data:
        i = int(uutl.numeric(row[0]))
        # Setting Z (height)
        _set_precise(holders[i], uutl.numeric(row[3]), k.RACK_HOLDER_HEIGHT)

        holders[i].attr("ty").set(uutl.numeric(row[4]) * 0.1)
        holders[i].attr("tx").set(
            (uutl.numeric(row[5]) * 0.1) + k.RACK_HOLDER_DISTANCE)


def read_perspex_calibration():
    service = sheets._get_service()
    result = service.spreadsheets().values().get(
        spreadsheetId=sheets.SHEETS["Measurements"],
        range='Rack!A6:G17').execute()

    packs = putl.get_perspex_packs()
    pack_len = len(packs)
    data = result.get('values', [])

    for pack in packs:
        index = pack["index"]
    # for row in data:
        # i = int(uutl.numeric(row[0]))
        # if i < pack_len:
        # pack = packs[i]
        row = data[index]
        print "ROW:", row
        if len(row) > 6 and row[6]:
            dup = pm.duplicate(pack["base"])
            dup = dup[0]
            _set_precise(dup, uutl.numeric(row[6]), 0)
            dup.rename("Calib_{}".format(pack["name"]))
            pm.parent(dup, world=True)


def read_board_calibration():
    service = sheets._get_service()
    result = service.spreadsheets().values().get(
        spreadsheetId=sheets.SHEETS["Measurements"],
        range='Board!A6:B17').execute()

    data = result.get('values', [])

    verts = [item for sublist in pm.sets(
        "probePointsSet", q=True) for item in sublist]

    if not (len(data) == len(verts)):
        raise IndexError(
            "Sheet data and number of verts are different lengths")

    # with uutl.zero_position(pm.PyNode("mainPaintingGroup")):
    for val, vtx in zip(zip(*data)[1], verts):
        pos = vtx.getPosition(space="world")
        pos.z = (uutl.numeric(val) * 0.1) - 1.0
        vtx.setPosition(pos, space="world")
