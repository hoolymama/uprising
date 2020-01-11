
import datetime
import json
import os
from contextlib import contextmanager

import pymel.core as pm
import pymel.core.uitypes as gui
import uprising_util as uutl
import write
from studio import Studio
from robolink import Robolink


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

        self.on_publish_rb_change()

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
        pm.setParent("..")

        pm.frameLayout(
            bv=True,
            collapse=False,
            labelVisible=True,
            label="Publish settings"
        )
        pm.columnLayout(adj=True)
        self.publish_rb = pm.radioButtonGrp(
            l="Publish", sl=2, nrb=3, la3=["Off", "After all",  "After each pass"],
                annotation='Also publish if retries succeed',
                height=30,
                changeCommand=pm.Callback(self.on_publish_rb_change)
        )

        self.passes_wg = pm.textFieldGrp(
            label='Pass IDs', text='0,1,2,3,4',
            height=30)

        pm.setParent("..")

        pm.setParent("..")
        return col

    def on_publish_rb_change(self):
        state = pm.radioButtonGrp(self.publish_rb, query=True, sl=True)
        # pm.rowLayout(self.add_objs_row, edit=True, enable=(state!=3))
        pm.textFieldGrp(self.passes_wg, edit=True, enable=(state==3))

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
        for ps in pack["passes"]:
            ps["plugs"]=[str(plug) for plug in ps["plugs"]]
        print json.dumps(pack, indent=2)
        self.on_go()

    def on_go(self ):
        uutl.checkRobolink()
        RL = Robolink()
        RL.HideRoboDK()
        pack = self.get_pack()
        publish_mode = pm.radioButtonGrp(self.publish_rb, query=True, sl=True)

        export_dir=None

        if publish_mode > 1: #
            export_dir = write.choose_publish_dir()
            if not export_dir:
                return
            timestamp = datetime.datetime.now().strftime('%y%m%d_%H%M')
            export_dir = os.path.join(export_dir,timestamp)
 
        all_results = self.do_retries(pack, export_dir)

        uutl.show_in_window(all_results, title="Retries results")
        
        all_skels = pm.ls(type="skeletonStroke")
        if export_dir:
            for s in all_skels:
                s.attr("active").set(True)
            write.write_maya_scene(export_dir, "scene" )

        RL.ShowRoboDK()

    def get_pack(self):
        publish_mode = pm.radioButtonGrp(self.publish_rb, query=True, sl=True)
        main_collector = pm.PyNode("collectStrokesMain")
        result = self.get_retries_parameters()
        result["passes"] = []
        if publish_mode  == 3:
            passes = pm.textFieldGrp(self.passes_wg, query=True, text=True)
            pass_ids = [int(i) for i in passes.split(",") if i is not None and i.isdigit()]
            for pass_id in pass_ids:
                collector = main_collector.attr("strokes[{}]".format(pass_id)).connections(s=True, d=False)[0]

                prefix = str(collector).replace("collectStrokes", "")

                nodes = collector.history(type="skeletonStroke", levels=1)
                plugs = fetch_plugs(result["attribute"], nodes)
                result["passes"].append({"prefix": prefix, "plugs": plugs, "pass_id": pass_id  })
        else:
            prefix = "all"
            nodes = pm.ls(sl=True, dag=True, leaf=True, type="skeletonStroke")
            plugs = fetch_plugs(result["attribute"], nodes)
            result["passes"]= [{"prefix": prefix, "plugs": plugs, "pass_id": -1   }]
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




    def do_retries(self, pack, export_dir  ):
        all_skels = pm.ls(type="skeletonStroke")
        all_results = []
        step_values = pack["step_values"]
        try_existing = pack["try_existing"]
   
        for pas in pack["passes"]:
            reset_collect_main_keys()
            plugs = pas["plugs"]
            results = []
            for frame, plug in enumerate(plugs):
                node = plug.node()
                with isolate_nodes([node], all_skels):
                    if not self.dry:
                        retries_result = do_retries_for_plug(frame, plug, step_values, try_existing )
                    else:
                        retries_result  = {"plug": str(plug), "attempts": -1, "path_results": [], "solved": False, "frame":frame}
                results.append(retries_result)
            success = report_results(results)

            if export_dir:
                program_files = self.publish_pass(export_dir, pas)
 
            result_data = {
                "prefix": pas["prefix"],
                "results":results,
                "success":success,
                "program_files": program_files
            }

            write.json_report(export_dir,pas["prefix"], result_data )

            reset_collect_main_keys()
            all_results.append(result_data)
 
        return all_results


    def publish_pass(self, export_dir, pas):
        program_files = []
        all_skels = pm.ls(type="skeletonStroke")
        show_nodes = [p.node() for p in pas["plugs"]]
        with isolate_nodes(show_nodes, all_skels):
            directory = os.path.join(export_dir, pas["prefix"])
            if not self.dry:
                program_files =  self.publish_tab.publish_to_directory(directory)
            else:
                print "publish_tab.publish_to_directory {}".format(directory)
                print show_nodes
        return program_files


def do_retries_for_plug(frame, plug, step_values, try_existing ):
    result = {"plug": str(plug), "attempts": -1, "path_results": [], "solved": False, "frame":frame}

    print "RUNNING RETRIES: {} {}".format( plug,  "*" * 20 )
    painting_node = pm.PyNode("mainPaintingShape")
    try:
        pm.paintingQuery(painting_node, cc=True)
    except RuntimeError as ex:
        pm.displayWarning(ex.message)
        result["solved"] = True
        return result


    values = ([plug.get()] if try_existing else []) + step_values
    with uutl.minimize_robodk():
        count = len(values)
        for i, value in enumerate(values):

            plug.set(value)

            pm.refresh()

            studio = Studio(do_painting=True)
            studio.write()

            path_result = studio.painting_program.validate_path() or {"status": "SUCCESS"}
            metadata = {"iteration": i,  "value": value}
            path_result.update(metadata)
            result["path_results"].append(path_result)

            print "{0} frame: {1} {3}/{4} {2} {0}".format(
                "- " * 10, frame, path_result["status"], (i + 1), count)

            if path_result["status"] == "SUCCESS":
                result["attempts"] = i + 1
                result["solved"] = True
                break

    return result

def reset_collect_main_keys():
    print "CLEAR_COLLECT_MAIN_KEYS"
    pm.cutKey("collectStrokesMain", at=("startFrom", "endAt"), option="keys")
    pm.PyNode("collectStrokesMain").attr("startFrom").set(0)
    pm.PyNode("collectStrokesMain").attr("endAt").set(-1)
    print pm.PyNode("collectStrokesMain").attr("startFrom").get(), pm.PyNode("collectStrokesMain").attr("endAt").get()


def get_step_values(low, high, count):
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


# def write_json_report(export_dir ,prefix, data ):
#     uutl.mkdir_p(export_dir)
#     json_file = os.path.join(export_dir, "{}.json".format(prefix))
#     with open(json_file, 'w') as outfile:
#         json.dump(data, outfile, indent=4)

