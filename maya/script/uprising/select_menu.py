import pymel.core as pm
import palette_utils as putl


def create():
    menu = pm.menu(label="Select", tearOff=True)

    pm.menuItem(label="All pots", command=pm.Callback(putl.select_pots))

    pm.menuItem(label="Used pots", command=pm.Callback(putl.select_used_pots))

    pm.menuItem(label="All handles", command=pm.Callback(putl.select_handles))

    pm.menuItem(label="Used handles", command=pm.Callback(
        putl.select_used_handles))

    pm.menuItem(label="Dip paintings",
                command=pm.Callback(select_paintings,"dip"))

    pm.menuItem(label="Wipe paintings",
                command=pm.Callback(select_paintings,"wipe"))


    return menu


def select_paintings(which):

    sel = "sel" if pm.ls(sl=True) else "all"

    locname = "{}_loc".format(which)
    if sel == "all":
        template =  "rack|holes|holeRot*|holeTrans|{}|*".format(locname)
        pm.select(
            pm.ls(
               template,
                dag=True,
                leaf=True,
                type="painting",
            )
        )
        return
    selected =  pm.ls(sl=True,  dag=True , type="transform")
    locs = [ n for n in selected if n.nodeName() == locname]
    pm.select(
            pm.ls(
               locs,
                dag=True,
                leaf=True,
                type="painting",
            )
        )
 