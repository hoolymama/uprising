import pymel.core as pm
from brush import Brush
from paint import Paint
import props


def create():
    menu = pm.menu(label="Export", tearOff=True)
    pm.menuItem(label="Send Used Brushes",
                command=pm.Callback(Brush.write_used_brushes))

    pm.menuItem(label="Send Used Paints",
            command=pm.Callback(Paint.write_geos))

    pm.menuItem(label="Send Geometry",
            command=pm.Callback(props.send_selected))

    return menu
 
 