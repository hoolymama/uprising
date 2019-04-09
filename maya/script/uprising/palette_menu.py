
import pymel.core as pm
import palette_utils as putl

def create():
    menu = pm.menu(label="Palette", tearOff=True)

    pm.menuItem(label="Select", subMenu=True)

    pm.menuItem(
        label="All pots",
        command=pm.Callback(
            putl.select_pots))

    pm.menuItem(
        label="Used pots",
        command=pm.Callback(
            putl.select_used_pots))

    pm.menuItem(
        label="All handles",
        command=pm.Callback(
            putl.select_handles))

    pm.menuItem(
        label="Used handles",
        command=pm.Callback(
            putl.select_used_handles))

    pm.setParent("..", menu=True)

    pm.menuItem(
        label="Clean palette",
        command=pm.Callback(
            putl.clean_palette))

    return menu

