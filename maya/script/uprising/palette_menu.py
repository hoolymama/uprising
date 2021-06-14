import pymel.core as pm
import palette_utils as putl


def create():
    menu = pm.menu(label="Palette", tearOff=True)

    pm.menuItem(label="Clean palette", command=pm.Callback(putl.clean_palette))

    return menu
