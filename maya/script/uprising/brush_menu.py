import pymel.core as pm
import brush_utils as butl


def create():
    menu = pm.menu(label="Brush", tearOff=True)

    pm.menuItem(
        label="Create probe",
        command=pm.Callback(
            butl.setup_probe_from_sheet))

    return menu
