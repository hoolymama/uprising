
 
import json
import os
import time
from contextlib import contextmanager

from robolink import Robolink

import pymel.core as pm
import pymel.core.uitypes as gui
import uprising_util as uutl
import write
from studio import Studio


@contextmanager
def isolate_nodes(show_nodes, all_nodes):
    all_vals = [n.attr("active").get() for n in all_nodes]
    vals = [n.attr("active").get() for n in show_nodes]

    for n in all_nodes:
        n.attr("active").set(False)

    for n in show_nodes:
        n.attr("active").set(True)
    yield

    for i, n in enumerate(show_nodes):
        n.attr("active").set(vals[i])

    for i, n in enumerate(all_nodes):
        n.attr("active").set(all_vals[i])


class retriesTab(gui.FormLayout):

    def __init__(self):
        self.setNumberOfDivisions(100)
        pm.setParent(self)

        self.retries_ui = self.create_retries_ui()
        pm.setParent(self)
        self.create_action_buttons()

        pm.setParent(self)
        self.publish_tab = None

        self.dry = False

    def set_publish_tab( self, publish_tab):
        self.publish_tab = publish_tab


    def create_retries_ui(self):

        col = pm.columnLayout(adj=True)

        pm.frameLayout(
            bv=True,
            collapse=False,
            labelVisible=True,
            label="Retries settings"
        )
        self.varying_attrib_wg = pm.textFieldGrp(
            label='Attribute', text='splitAngle',
            height=30)

        self.varying_range_wg = pm.floatFieldGrp(
            numberOfFields=2, label='Range', value1=270, value2=0.0,
            height=30)


        self.num_retries_wg = pm.intFieldGrp(
            label='Attempts',
            numberOfFields=1,
            value1=12,
            height=30)

        self.try_existing_first_cb = pm.checkBoxGrp(
            label='Try current first',
            value1=0,
            annotation='Before trying the set range of values, try the existing value',
            height=30)

        self.publish_cb = pm.checkBoxGrp(
            label='Publish',
            value1=1,
            annotation='Also publish if retries succeed',
            height=30)
        pm.setParent("..")
 
        pm.frameLayout(
            bv=True,
            collapse=False,
            labelVisible=True,
            label="Developer"
        )
        self.robolink_render = pm.checkBoxGrp(
            label='Robolink Render',
            value1=0,
            annotation='Render on off for profiling purposes',
            height=30)

        pm.setParent("..")
 

        pm.frameLayout(
            bv=True,
            collapse=False,
            labelVisible=True,
            label="Retries settings"
        )
 
        pm.columnLayout(adj=True)
        self.progress_info_field = pm.scrollField(
            height=70,
            wordWrap=True,
            text="Retries progress info", editable=False)

        self.progressControl = pm.progressBar()


        pm.setParent("..") # col
        pm.setParent("..")

        pm.setParent("..")
        return col
 
    def create_action_buttons(self):
        pm.setParent(self)  # form

        self.preview_but = pm.button(
            label='Preview', command=pm.Callback(
                self.on_preview))
        go_but = pm.button(label='Go', command=pm.Callback(self.on_go))


        self.attachForm(self.retries_ui, 'left', 2)
        self.attachForm(self.retries_ui, 'right', 2)
        self.attachForm(self.retries_ui, 'top', 2)
        self.attachControl(self.retries_ui, 'bottom', 2, self.preview_but)

        self.attachNone(self.preview_but, 'top')
        self.attachForm(self.preview_but, 'left', 2)
        self.attachPosition(self.preview_but, 'right', 2, 50)
        self.attachForm(self.preview_but, 'bottom', 2)

        self.attachNone(go_but, 'top')
        self.attachForm(go_but, 'right', 2)
        self.attachPosition(go_but, 'left', 2, 50)
        self.attachForm(go_but, 'bottom', 2)


    def on_preview(self):
        self.dry = True
        uutl.checkRobolink()
        pack = self.get_pack()
        # for ps in pack["passes"]:
        pack["plugs"]=[str(plug) for plug in pack["plugs"]]
        print json.dumps(pack, indent=2)
        self.on_go()

    def on_go(self ):
        uutl.checkRobolink()
        RL = Robolink()
        if pm.checkBoxGrp(self.robolink_render, query=True, value1=True):
            RL.Render(True)
            RL.ShowRoboDK()
        else:
            RL.Render(False)
            RL.HideRoboDK()


        timer_start = time.time()

        pack = self.get_pack()

        if len(pack["plugs"]) == 0:
            pm.warning("No plugs!")


        publish_mode = pm.checkBoxGrp(self.publish_cb, query=True, value1=True)

        directory=None

        if publish_mode: #
            directory = write.choose_publish_dir()
            if not directory:
                return
            timestamp = write.get_timestamp()
            directory = os.path.join(directory,timestamp)
        
        results_data = self.do_retries(pack)

        if directory:
            self.publish_tab.publish_to_directory(directory)
     
            write.json_report(directory,"retries", results_data )
            write.write_maya_scene(directory, "scene" )
        



        timer_end = time.time()
        results_data["timer"] = timer_end-timer_start

        uutl.show_in_window(results_data, title="Retries results")

        print_timer_results(results_data)


        RL.Render(True)
        RL.ShowRoboDK()

 

    def get_pack(self):
 
        result = self.get_retries_parameters()
        result["plugs"] = []
 
        nodes = pm.ls(sl=True, dag=True, leaf=True, type="skeletonStroke")

        if  len(nodes) == 0:
            pm.warning("No nodes selected. Aborting!")
            return result

        if any([n.type() != "skeletonStroke" for n in nodes]):
            confirm = pm.confirmDialog( title='Confirm', message='Some selected nodes are not skeletonStrokes. Continue? ', button=['Yes','No'], defaultButton='Yes', cancelButton='No', dismissString='No' )
            if confirm == "No":
                return result
        result["plugs"] = fetch_plugs(result["attribute"], nodes)
        return result
 
    def get_retries_parameters(self):
        try_existing = pm.checkBoxGrp(
            self.try_existing_first_cb, query=True, value1=True)
        attribute = pm.textFieldGrp(
            self.varying_attrib_wg, query=True, text=True)
        count = pm.intFieldGrp(self.num_retries_wg, query=True, value1=True)
        low, high = pm.floatFieldGrp(
            self.varying_range_wg, query=True, value=True)
        step_values = get_step_values(low, high, count)

        return {
            "attribute": attribute,
            "try_existing": try_existing,
            "step_values": step_values
            }




    def do_retries(self, pack ):
        all_skels = pm.ls(type="skeletonStroke")
        # all_results = []
        step_values = pack["step_values"]
        try_existing = pack["try_existing"]
        reset_collect_main_keys()
        plugs = pack["plugs"]
        results = []
        num_plugs = len(plugs)
        if not num_plugs:
            pm.error("No Plugs")

        pm.progressBar(self.progressControl, edit=True,  maxValue=num_plugs, isInterruptable=True)
        
        pm.scrollField(  self.progress_info_field, edit=True, text="Retrying plug {:d} of {:d}".format(0, num_plugs) )

 
        for plug_index, plug in enumerate(plugs):
            node = plug.node()
            with isolate_nodes([node], all_skels):
                if not self.dry:
                    retries_result = self.do_retries_for_plug(plug_index,num_plugs, plug, step_values, try_existing )
                else:
                    retries_result  = {"plug": str(plug), "attempts": -1, "path_results": [], "solved": False, "frame":plug_index}
            results.append(retries_result)

            # pm.progressWindow( edit=True, progress=(plug_index+1))


        pm.progressBar(self.progressControl, edit=True,  progress=0)
        
        result_data = {
            "results":results,
            "success":all([res["solved"] for res in results])
        }

        reset_collect_main_keys()

        return result_data






    def do_retries_for_plug(self, plug_index,num_plugs, plug, step_values, try_existing ):
        result = {"plug": str(plug), "attempts": -1, "path_results": [], "solved": False, "frame":plug_index, "timer":0}

        plug_start = time.time()
        print "RUNNING RETRIES: {} {}".format( plug,  "*" * 20 )
        painting_node = pm.PyNode("mainPaintingShape")
        try:
            pm.paintingQuery(painting_node, cc=True)
        except RuntimeError as ex:
            pm.displayWarning(ex.message)
            result["solved"] = True
            return result


        values = ([plug.get()] if try_existing else []) + step_values
        # with uutl.minimize_robodk():
        count = len(values)
        for i, value in enumerate(values):
            plug.set(value)
            pm.refresh()
            
            iter_start = time.time()
            studio = Studio(do_painting=True)
            before_write = time.time()
            studio.write()
            before_validate = time.time()
            path_result = studio.painting_program.validate_path() or {"status": "SUCCESS"}
            iter_end = time.time()


            metadata = {"iteration": i,  "value": value, "timer": {
                "create": before_write-iter_start,
                "write": before_validate-before_write,
                "validate": iter_end-before_validate,
                "total": iter_end-iter_start
                }
                }
            path_result.update(metadata)
            result["path_results"].append(path_result)

            attempt_number = i + 1
            print "{0} frame: {1} {3}/{4} {2} {0}".format(
                "- " * 10, plug_index, path_result["status"], attempt_number, count)


            pm.progressBar(self.progressControl, edit=True,  progress=(plug_index+1))
            msg = "Tried plug {:d} of {:d}\nAttempt {:d}/{:d} {}({}) result: {}".format((plug_index+1), num_plugs, attempt_number, count, plug, value, path_result["status"])

            pm.scrollField(  self.progress_info_field, edit=True, text=msg )

            if path_result["status"] == "SUCCESS":
                result["attempts"] = i + 1
                result["solved"] = True
                break
        plug_end = time.time()
        result["timer"] = plug_end-plug_start
        return result




def print_timer_results(result_data):
    print "Total time: {}".format(result_data["timer"]) 

    creating = 0
    writing = 0
    validating = 0
    for entry in result_data["results"]:
        print "{:02d}\t{}".format(entry["frame"],entry["timer"])
        for it in entry["path_results"]:
            print "{:02d}\t\t{}".format(it["iteration"], it["timer"]["total"])
            creating += it["timer"]["create"]
            writing += it["timer"]["write"]
            validating += it["timer"]["validate"]
            
            print  "CR\t\t{}".format(it["timer"]["create"])
            print  "WR\t\t{}".format(it["timer"]["write"])
            print  "VL\t\t{}".format(it["timer"]["validate"])
    
    print "Total creating {}".format(creating)
    print "Total writing {}".format(writing)
    print "Total validating {}".format(validating)
    





def reset_collect_main_keys():
    print "CLEAR_COLLECT_MAIN_KEYS"
    pm.cutKey("collectStrokesMain", at=("startFrom", "endAt"), option="keys")
    pm.PyNode("collectStrokesMain").attr("startFrom").set(0)
    pm.PyNode("collectStrokesMain").attr("endAt").set(-1)
    print pm.PyNode("collectStrokesMain").attr("startFrom").get(), pm.PyNode("collectStrokesMain").attr("endAt").get()


def get_step_values(low, high, count):
    if count < 2:
        return [high]
    vals = []
    valrange = high - low
    gap = valrange / (count - 1)
    vals = [low + (gap * x) for x in range(count)]
    return vals

def fetch_plugs(attribute, nodes):
    result = []
    for node in nodes:
        plug = pm.PyNode(node).attr(attribute)
        if plug.get(lock=True):
            pm.error("{} is locked. Can't adjust.")
        if plug.inputs():
            pm.error("{} has input connections. Can't adjust.")
        result.append(plug)
    return result


 
def report_results(results):
    failed = False
    some_empty = False
    for res in results:
        if not res["solved"]:
            failed = True
            print "Pack number: {} UNSOLVED".format(res["frame"])
        else:  # solved
            if res["attempts"] == -1:
                some_empty = True
    if failed:
        print "Some frames could not be resolved"
    else:
        if some_empty:
            print "All frames succeeded, some were empty"
        else:
            print "All frames succeeded"

    return not failed
