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
        "do_pot_calibration": which == "pot",
        "do_holder_calibration": which == "holder",
        "do_board_calibration": which == "board",
    }
    # prefix = "rx" if which == "rack" else "bx"

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



def read_rack_triangulation():
    _read_triangulation('Rack!A1:D3')

def read_board_triangulation():
    _read_triangulation('Board!A1:D3')



def generate_pot_calibration():
    _generate_calibration("pot")

def generate_holder_calibration():
    _generate_calibration("holder")

def generate_board_calibration():
    _generate_calibration("board")
 
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

    for row in data:
        i = int(uutl.numeric(row[0]))
        _set_precise(pots[i].getParent(), uutl.numeric(row[1]), -k.RACK_POT_DEPTH)
        _set_precise(handles[i].getParent(), uutl.numeric(row[2]), k.RACK_HANDLE_HEIGHT)


def read_holder_calibration():
    service = sheets._get_service()
    result = service.spreadsheets().values().get(
        spreadsheetId=sheets.SHEETS["Measurements"],
        range='Rack!A6:D25').execute()

    data = result.get('values', [])

    holders = pm.ls("RACK1_CONTEXT|j1|rack1|holders|holderRot*|holderTrans")
    if not  len(data) == len(holders):
        raise IndexError("Sheet data and number of holders are different")

    for row in data:
        i = int(uutl.numeric(row[0]))
        _set_precise(holders[i], uutl.numeric(row[3]), k.RACK_HOLDER_HEIGHT)


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



