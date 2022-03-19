
from uprising import props
import pymel.core as pm
from uprising import robo
from uprising.brush import Brush


def create():
    menu = pm.menu(label="Props", tearOff=True)

    pm.menuItem("clean_kr8_toggle_item", label="Clean KR8", checkBox=False)

    pm.menuItem(label="Send selected brushes", subMenu=True)
    pm.menuItem(
        label="outPaintBrush",
        command=pm.Callback(
            send_selected_brushes_session,
            "outPaintBrush"))
    pm.menuItem(
        label="outDipBrush",
        command=pm.Callback(
            send_selected_brushes_session,
            "outDipBrush"))
    pm.menuItem(
        label="outWipeBrush",
        command=pm.Callback(
            send_selected_brushes_session,
            "outWipeBrush"))

    pm.setParent("..", menu=True)

    pm.menuItem(label="Send selected objects",
                command=pm.Callback(send_selected_objects_session))
    pm.setParent("..", menu=True)

    return menu


def send_selected_brushes_session(brush_att):
    clean = pm.menuItem("clean_kr8_toggle_item", query=True, checkBox=True)
    robo.new()
    if clean:
        robo.clean("kr8")
    Brush.send_selected_brushes([brush_att])

def send_selected_objects_session():

    clean = pm.menuItem("clean_kr8_toggle_item", query=True, checkBox=True)

    robo.new()
    if clean:
        robo.clean("kr8")
    props.send_selected()
