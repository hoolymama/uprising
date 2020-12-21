
from uprising import props
import pymel.core as pm
from uprising import robo


def create():
    menu = pm.menu(label="Props", tearOff=True)
    pm.menuItem(label="Send selected tools",
                command=pm.Callback(send_selected_tools_session))
    pm.setParent("..", menu=True)
    return menu


def send_selected_tools_session(clean=True):
    robo.new()
    if clean:
        robo.clean("kr8")
    props.send_selected_tools()
