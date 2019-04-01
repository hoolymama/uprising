import pymel.core as pm
from brush import Brush
from paint import Paint
import props
from studio import Studio

def create():
    menu = pm.menu(label="Export", tearOff=True)

    pm.menuItem(label="Used Brushes",
                command=pm.Callback(Brush.write_used_brushes))

    pm.menuItem(label="Connected brushes",
                command=pm.Callback(Brush.write_connected_brushes))

    pm.menuItem(label="Used Paints",
            command=pm.Callback(Paint.write_geos))

    pm.menuItem(label="Selected geometry",
            command=pm.Callback(props.send_selected))

    pm.menuItem(label="Painting",
            command=pm.Callback(export_painting))

    pm.menuItem(label="Dips",
            command=pm.Callback(export_dips))

    pm.menuItem(label="Pick & place",
            command=pm.Callback(export_pick_and_place))

    pm.menuItem(label="Global approaches",
            command=pm.Callback(export_approaches))

    return menu
 
def export_painting():
    studio = Studio(do_painting=True)
    studio.write()

def export_dips():
    studio = Studio(do_dips=True)
    studio.write()

def export_pick_and_place():
    studio = Studio(do_auto_change=True)
    studio.write()

def export_approaches():
    studio = Studio(do_approaches=True)
    studio.write()