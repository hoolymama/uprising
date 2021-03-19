
import pymel.core as pm


def create():
    menu = pm.menu(label="Tools", tearOff=True)

    pm.menuItem(label="Create MeshStroke",
                command=pm.Callback(on_create_mesh_stroke, 0))

    pm.menuItem(label="Create MeshStroke option", optionBox=True,
                command=pm.Callback(on_create_mesh_stroke, 1))

    pm.menuItem(label="A sub menu", subMenu=True)
    pm.menuItem(
        label="something")

    pm.setParent("..", menu=True)

    return menu


def on_create_mesh_stroke(option):
    pass
