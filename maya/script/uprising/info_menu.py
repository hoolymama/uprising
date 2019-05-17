import pymel.core as pm
import brush_utils as butl
from brush import Brush

def create():
    menu = pm.menu(label="Info", tearOff=True)

    pm.menuItem(
        label="Show brushes",
        command=pm.Callback(
             on_show_brushes))

    return menu


def on_show_brushes():
    painting_node = pm.PyNode("mainPaintingShape")

    brushes = Brush.brushes(painting_node)