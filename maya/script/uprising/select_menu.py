import pymel.core as pm
import palette_utils as putl


def create():
    menu = pm.menu(label="Select", tearOff=True)

    pm.menuItem(label="All pots", command=pm.Callback(putl.select_pots))

    pm.menuItem(label="Used pots", command=pm.Callback(putl.select_used_pots))

    pm.menuItem(label="All handles", command=pm.Callback(putl.select_handles))

    pm.menuItem(label="Used handles", command=pm.Callback(
        putl.select_used_handles))

    pm.menuItem(label="All dip paintings",
                command=pm.Callback(select_dip_paintings))

    pm.menuItem(label="All wipe paintings",
                command=pm.Callback(select_wipe_paintings))

    return menu


def select_dip_paintings():
    pm.select(
        pm.ls(
            "rack|holes|holeRot*|holeTrans|dip_loc|*",
            dag=True,
            leaf=True,
            type="painting",
        )
    )


def select_wipe_paintings():
    pm.select(
        pm.ls(
            "rack|holes|holeRot*|holeTrans|wipe_loc|*",
            dag=True,
            leaf=True,
            type="painting",
        )
    )

# def select_dip_stroke_nodes():
#     dip_paintings = pm.ls(
#         "rack|holes|holeRot*|holeTrans|dip_loc|*",
#         dag=True,
#         leaf=True,
#         type="painting",
#     )
#     stroke_nodes = pm.listHistory(dip_paintings, type="curveStroke")
#     pm.select(stroke_nodes)

# def select_wipe_stroke_nodes():
#     dip_paintings = pm.ls(
#         "rack|holes|holeRot*|holeTrans|dip_loc|*",
#         dag=True,
#         leaf=True,
#         type="painting",
#     )
#     stroke_nodes = pm.listHistory(dip_paintings, type="curveStroke")
#     pm.select(stroke_nodes)
