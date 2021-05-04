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


    pm.menuItem(label="Vertical wipe paintings",
                command=pm.Callback(select_paintings,"wipe", vertical=True))


    return menu


def select_paintings(which, **kwargs):

    sel = "sel" if pm.ls(sl=True) else "all"

    locname = "{}_loc".format(which)

    nodes = []
    if sel == "all":
        template =  "rack|holes|holeRot*|holeTrans|{}|*".format(locname)
        nodes =  pm.ls(
               template,
                dag=True,
                leaf=True,
                type="painting",
            )
        
    else:

        selected =  pm.ls(sl=True,  dag=True , type="transform")
        locs = [ n for n in selected if n.nodeName() == locname]

        nodes = pm.ls(
                locs,
                    dag=True,
                    leaf=True,
                    type="painting",
                )
        

    if kwargs.get("vertical"):
        nodes = vertical_wipes_filter(nodes)
    
    pm.select(nodes)
        


def vertical_wipes_filter(nodes):
    result = []
    for node in nodes:
        parts = node.nodeName().split("_")
        if parts[1] != "wipe":
            continue
        brush_id = int(parts[2][1:3])
        if brush_id > 15:
            continue
        paint_id= int(parts[3][1:3])
        if paint_id == 9:
            continue
        result.append(node)
    return result
 
