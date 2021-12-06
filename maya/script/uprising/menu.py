import pymel.core as pm

import os
import glob

from uprising import window

MAYA_PARENT_WINDOW = "MayaWindow"
HOOLY_MENU = "HoolyMenu"

def ensure_hooly_menu():
    menu = next((m for m in pm.lsUI(menus=True) if m.getLabel() == "Hooly"), None)
    if menu:
        return menu
    return pm.menu(HOOLY_MENU, label="Hooly", tearOff=True, parent=MAYA_PARENT_WINDOW)
 

class UprisingMenu(object):
    def __init__(self):
        self.hooly_menu = ensure_hooly_menu()
        pm.setParent(self.hooly_menu, menu=True)
        pm.menuItem(
            label="Robot Window", command=pm.Callback(show_robot_window)
        )
        pm.menuItem(
            divider=True
        )


def show_robot_window():
    reload(window)
    window.RobotWindow()
