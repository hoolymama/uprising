
import pymel.core as pm
 
def create():
    menu = pm.menu(label="GRIPPER", hm=True, tearOff=False)

    pm.menuItem(
        label="Bayonet",
        command=pm.Callback( on_choose_bayonet, menu))
    
    pm.menuItem(label="Gripper",
                command=pm.Callback(on_choose_gripper, menu))
 

    state = pm.optionVar.get("upov_tool_type")

    # initialize
    if state == "bayonet":
        on_choose_bayonet(menu)
    else:
        on_choose_gripper(menu)


    return menu

def on_choose_bayonet(menu):
    pm.menu(menu, edit=True, label="BAYONET")
    pm.optionVar["upov_tool_type"] = "bayonet"

 
def on_choose_gripper(menu):
    pm.menu(menu, edit=True, label="GRIPPER")
    pm.optionVar["upov_tool_type"] = "gripper"
