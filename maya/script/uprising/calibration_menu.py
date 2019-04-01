import os
import pymel.core as pm
import palette_utils as putl
import write
import uprising.uprising_util as uutl
from studio import Studio
from robolink import Robolink
import sheets
import const as k


def create():
    menu = pm.menu(label="Calibration", tearOff=True)

    pm.menuItem(label="Rack setup", subMenu=True)

    pm.menuItem(
        label="Read rack triangulation",
        command=pm.Callback(read_rack_triangulation))

    pm.menuItem(
        label="Generate rack calibration",
        command=pm.Callback(generate_rack_calibration))

    pm.menuItem(
        label="Read rack calibration",
        command=pm.Callback(read_rack_calibration))

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

    return menu

########################################
########################################


def _read_triangulation(sheet_range):
    service = sheets._get_service()
    result = service.spreadsheets().values().get(
        spreadsheetId=sheets.SHEETS["Measurements"],
        range='Board!A1:D3').execute()

    data = result.get('values', [])

    for row in data:
        loc_name = row[0]
        try:
            node = pm.PyNode(loc_name)
        except pm.MayaNodeError:
            node = pm.spaceLocator()
            node.rename(loc_name)

        vals = [uutl.numeric(x) * 0.1 for x in row[1:4]]
        node.attr("translate").set(*vals)


def _generate_calibration(which):
    kw = {
        "do_rack_calibration": which == "rack",
        "do_board_calibration": which == "board"
    }
    prefix = "rx" if which == "rack" else "bx"

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
    write.write_program(Robolink(), calib_dir, prefix, timestamp)


def _set_precise(geo, gauge_reading, probe_height):
    new_pos = probe_height + (gauge_reading * 0.1) - 1.0
    geo.getParent().attr("tz").set(new_pos)


def read_rack_triangulation():
    _read_triangulation('Rack!A1:D3')


def generate_rack_calibration():
    _generate_calibration("rack")


def read_board_triangulation():
    _read_triangulation('Board!A1:D3')


def generate_board_calibration():
    _generate_calibration("board")


def read_rack_calibration():
    service = sheets._get_service()
    result = service.spreadsheets().values().get(
        spreadsheetId=sheets.SHEETS["Measurements"],
        range='Rack!A6:C25').execute()

    data = result.get('values', [])

    pots = putl.get_pots()
    handles = putl.get_handles()
    print "{} {} {}".format(len(data), len(pots), len(handles))
    if not (len(data) == len(pots) and len(data) == len(handles)):
        raise IndexError("Sheet data and number of pots are different lengths")

    for row in data:
        i = int(uutl.numeric(row[0]))
        _set_precise(pots[i], uutl.numeric(row[1]), -k.RACK_POT_DEPTH)
        _set_precise(handles[i], uutl.numeric(row[2]), k.RACK_HANDLE_HEIGHT)


def read_board_calibration():
    service = sheets._get_service()
    result = service.spreadsheets().values().get(
        spreadsheetId=sheets.SHEETS["Measurements"],
        range='Board!A6:B17').execute()

    data = result.get('values', [])
    top_node = pm.PyNode("mainPaintingGroup")
    top_node.attr("zeroPosition").set(1)

    verts = [item for sublist in pm.sets(
        "probePointsSet", q=True) for item in sublist]

    if not (len(data) == len(verts)):
        raise IndexError("Sheet data and number of verts are different lengths")

    for val, vtx in  zip( zip(*data)[1]  , verts):
        pos = vtx.getPosition(space="world")
        pos.z = (uutl.numeric(val) * 0.1) - 1.0
        vtx.setPosition(pos, space="world")



