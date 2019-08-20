import pymel.core as pm
from brush import Brush
from paint import Paint
import props
from studio import Studio


def create():
    menu = pm.menu(label="Export", tearOff=True)

    pm.menuItem(label="Used brush sets",
                command=pm.Callback(Brush.write_used_brush_sets))

    pm.menuItem(label="Connected brushes",
                command=pm.Callback(Brush.write_connected_brushes))

    pm.menuItem(label="Selected brushes",
                command=pm.Callback(Brush.write_selected_brushes))

    pm.menuItem(label="Used Paints",
                command=pm.Callback(Paint.write_geos))

    pm.menuItem(label="Selected geometry",
                command=pm.Callback(props.send_selected))

    pm.menuItem(label="Global approaches",
                command=pm.Callback(export_approaches))

    return menu

# approaches are always sent, so no need for kw


def export_approaches():
    studio = Studio()
    studio.write()


#     import pymel.core as pm
# import brush_utils as butl


# def create():
#     menu = pm.menu(label="Brush", tearOff=True)

#     pm.menuItem(
#         label="Create probe",
#         command=pm.Callback(
#             butl.setup_probe_from_sheet))

#     return menu
