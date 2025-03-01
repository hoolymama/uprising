import pymel.core as pm
from uprising import utils
from uprising import calibration as cal


def create():
    menu = pm.menu(label="Calibration", tearOff=True)

    pm.menuItem(label="Rack setup", subMenu=True)

    pm.menuItem(
        label="Create manual triangulation program",
        command=pm.Callback(cal.generate_manual_probe_calibration),
    )

    pm.menuItem(
        label="Read rack triangulation",
        command=pm.Callback(cal.read_calibration, "rack_triangulation"),
    )

    pm.menuItem(divider=True)

    pm.menuItem(
        label="Generate pot calibration",
        command=pm.Callback(cal.generate_pot_calibration),
    )

    pm.menuItem(
        label="Read pot calibration",
        command=pm.Callback(cal.read_calibration, "pot_calibration"),
    )
    pm.menuItem(
        label="Read handle height calibration",
        command=pm.Callback(cal.read_calibration, "handle_height_calibration"),
    )



    pm.menuItem(divider=True)

    pm.menuItem(
        label="Generate handle calibration",
        command=pm.Callback(cal.generate_handle_calibration),
    )

    pm.menuItem(
        label="Read handle calibration",
        command=pm.Callback(cal.read_calibration, "handle_calibration"),
    )

    pm.menuItem(divider=True)


    pm.menuItem(
        label="Generate holder calibration",
        command=pm.Callback(cal.generate_holder_calibration),
    )

    pm.menuItem(
        label="Read holder calibration",
        command=pm.Callback(cal.read_calibration, "holder_calibration"),
    )

    pm.menuItem(divider=True)

    pm.menuItem(
        label="Generate rack calibration",
        command=pm.Callback(cal.generate_rack_calibration),
    )

    pm.menuItem(
        label="Read perspex calibration",
        command=pm.Callback(cal.read_calibration, "perspex_calibration"),
    )

    pm.setParent("..", menu=True)

    pm.menuItem(label="Board setup", subMenu=True)

    pm.menuItem(
        label="Read board triangulation",
        command=pm.Callback(cal.read_calibration, "board_triangulation"),
    )

    pm.menuItem(
        label="Generate board calibration",
        command=pm.Callback(cal.generate_board_calibration),
    )

    pm.menuItem(
        label="Read board calibration",
        command=pm.Callback(cal.read_calibration, "board_calibration"),
    )

    pm.setParent("..", menu=True)

    pm.menuItem(
        label="Generate Pick/Place exercise",
        command=pm.Callback(cal.generate_pick_place_exercise),
    )

    pm.menuItem(
        label="Generate KR8 object track",
        command=pm.Callback(cal.generate_kr8_object_track),
    )
    
    return menu
