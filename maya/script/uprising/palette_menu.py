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
    menu = pm.menu(label="Palette", tearOff=True)

    pm.menuItem(label="Rack setup", subMenu=True)

    pm.menuItem(
        label="Read rack triangulation",
        command=pm.Callback(read_rack_triangulation))

    pm.menuItem(
        label="Generate rack calibration",
        command=pm.Callback(generate_rack_calibration))

    pm.menuItem(
        label="Read Calibration",
        command=pm.Callback(read_rack_calibration))

    pm.setParent("..", menu=True)

    pm.menuItem(label="Select", subMenu=True)

    pm.menuItem(
        label="All pots",
        command=pm.Callback(
            putl.select_pots))

    pm.menuItem(
        label="Used pots",
        command=pm.Callback(
            putl.select_used_pots))

    pm.menuItem(
        label="All handles",
        command=pm.Callback(
            putl.select_handles))

    pm.menuItem(
        label="Used handles",
        command=pm.Callback(
            putl.select_used_handles))

    pm.setParent("..", menu=True)

    return menu


def read_rack_triangulation():
    service = sheets._get_service()
    result = service.spreadsheets().values().get(
        spreadsheetId=sheets.SHEETS["Measurements"],
        range='Rack!A1:D3').execute()

    data = result.get('values', [])

    for row in data:
        loc_name = row[0]
        vals = [uutl.numeric(x)*0.1 for x in row[1:4]]
        pm.PyNode(loc_name).attr("translate").set(*vals)


def generate_rack_calibration():

    timestamp = write.get_timestamp()
    calib_dir = os.path.join(
        pm.workspace.getPath(),
        'export',
        'calibrations',
        'rack',
        timestamp)
    uutl.mkdir_p(calib_dir)
    studio = Studio(rack_calibration=True)
    studio.write()
    write.write_program( Robolink(), calib_dir, "rx", timestamp)

    # write_station(RL, ts_dir, timestamp)

    # if kw.get("painting_node"):
    #     write_program(RL, ts_dir, "px", timestamp)


def read_rack_calibration():
    service = sheets._get_service()
    result = service.spreadsheets().values().get(
        spreadsheetId=sheets.SHEETS["Measurements"],
        range='Rack!A6:C25').execute()

    data = result.get('values', [])

    pots = putl.get_pots()
    handles = putl.get_handles()
    print "{} {} {}".format(len(data), len(pots), len(handles))
    if not (len(data) == len(pots)   and   len(data) == len(handles)):
        raise IndexError("Sheet data and number of pots are different lengths")

    for row in  data:
        i = int(uutl.numeric(row[0]))
        _set_precise(pots[i], uutl.numeric(row[1]), -k.RACK_POT_DEPTH  )
        _set_precise(handles[i], uutl.numeric(row[2]) , k.RACK_HANDLE_HEIGHT)
 

def _set_precise(geo, gauge_reading, probe_height):
    new_pos = probe_height + (gauge_reading*0.1) - 1.0
    geo.getParent().attr("tz").set(new_pos)
 