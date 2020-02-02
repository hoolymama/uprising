
import os

import const as k
import palette_utils as putl
import props
import pymel.core as pm
import robo
import sheets
import uprising.uprising_util as uutl
import write
from studio import Studio


def generate_pick_place_exercise():

    timestamp = write.get_timestamp()
    directory = os.path.join(
        pm.workspace.getPath(),
        'export',
        'calibrations',
        k.PAP_EXERCISE_PROGRAM_NAME,
        timestamp)
    uutl.mkdir_p(directory)
    robo.new()
    studio = Studio(do_pap_exercise=True, pick_and_place_slots="all")
    studio.write()

    robo.write_program( directory, k.PAP_EXERCISE_PROGRAM_NAME)


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
    directory = os.path.join(
        pm.workspace.getPath(),
        'export',
        'calibrations',
        which,
        timestamp)
    uutl.mkdir_p(directory)

    robo.new()
    studio = Studio(**kw)
    studio.write()
    props.send(reference_geo)
    robo.show()
    src_fn,rdk_fn = write.save_prog_and_station(directory, which)


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


def generate_pot_holder_calibration():
    ref_geo = [pm.PyNode("rackTop")]
    ref_geo += pm.ls("holes|*|holeTrans|dip_loc|pot")
    ref_geo += pm.ls("holes|*|holeTrans|wipe_loc|handle")
    ref_geo += pm.ls("holders|*|holderTrans|lowResGeo")
    _generate_calibration(k.POT_HOLDER_CALIBRATION_PROGRAM_NAME, *ref_geo)


def generate_holder_calibration():
    _generate_calibration(k.HOLDER_CALIBRATION_PROGRAM_NAME)

def generate_pot_calibration():
    _generate_calibration(k.POT_CALIBRATION_PROGRAM_NAME)



def generate_perspex_calibration():
    ref_geo = pm.PyNode("rackTop")
    _generate_calibration(k.PERSPEX_CALIBRATION_PROGRAM_NAME, ref_geo)


def generate_board_calibration():
    ref_geo = pm.PyNode("canvas")
    _generate_calibration(k.BOARD_CALIBRATION_PROGRAM_NAME, ref_geo)


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
        range='Rack!A6:F25').execute()

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

    for val, vtx in zip(zip(*data)[1], verts):
        pos = vtx.getPosition(space="world")
        pos.z = (uutl.numeric(val) * 0.1) - 1.0
        vtx.setPosition(pos, space="world")
