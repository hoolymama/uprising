
from uprising import props
import pymel.core as pm
from uprising import robo


def create():
    menu = pm.menu(label="Props", tearOff=True)

    pm.menuItem("clean_kr8_toggle_item", label="Clean KR8", checkBox=False)

    pm.menuItem(label="Send selected tools",
                command=pm.Callback(send_selected_tools_session))
    pm.setParent("..", menu=True)

    pm.menuItem(label="Send selected objects",
                command=pm.Callback(send_selected_objects_session))
    pm.setParent("..", menu=True)

    return menu


def send_selected_tools_session(clean=True):
    robo.new()
    if clean:
        robo.clean("kr8")
    props.send_selected_tools()


def send_selected_objects_session(clean=True):

    clean = pm.menuItem("clean_kr8_toggle_item", query=True, checkBox=True)
    print "Is Clean:", clean
    robo.new()
    if clean:
        robo.clean("kr8")
    props.send_selected()
