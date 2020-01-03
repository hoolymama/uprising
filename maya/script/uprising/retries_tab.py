
import sys, os
import json
import pymel.core as pm
import random
from robolink import (
    Robolink,
    COLLISION_OFF)
import write
import props
import uprising_util as uutl
import curve_utils as cutl
from studio import Studio
import datetime
import pymel.core.uitypes as gui


class retriesTab(gui.FormLayout):

    def __init__(self):
        self.setNumberOfDivisions(100)
        pm.setParent(self)

        self.retries_ui = self.create_retries_ui()
        pm.setParent(self)
        self.create_action_buttons()

        pm.setParent(self)
        # self.initialize_ui()
        self.publish_tab = None
        
        self.on_publish_rb_change()

    def set_publish_tab(self, publish_tab):
        self.publish_tab = publish_tab


    def create_retries_ui(self):

        frame = pm.frameLayout(
            bv=True,
            collapse=False,
            labelVisible=True,
            label="Retries"
        )

        pm.columnLayout(adj=True)
        self.varying_attrib_wg = pm.textFieldGrp(
            label='Attribute', text='splitAngle')

        self.varying_range_wg = pm.floatFieldGrp(
            numberOfFields=2, label='Range', value1=270, value2=0.0)

        self.varying_step_type_wg = pm.radioButtonGrp(
            label='Step Type',
            sl=1,
            labelArray3=[
                'Linear',
                "Binary",
                'Random'
            ],
            numberOfRadioButtons=3
        )
        pm.setParent("..")

        self.num_retries_wg = pm.intFieldGrp(
            label='Attempts',
            numberOfFields=1,
            value1=12)

        self.try_existing_first_cb = pm.checkBoxGrp(
            label='Try current first',
            value1=1,
            annotation='Before trying the set range of values, try the existing value')



        self.publish_rb = pm.radioButtonGrp(
            l="Publish", sl=3, nrb=3, la3=["Off", "After all",  "After each pass"],
              annotation='Also publish if retries succeed',
              changeCommand=pm.Callback(self.on_publish_rb_change)
        )
 
        self.passes_wg = pm.textFieldGrp(
            label='Pass IDs', text='0,1,2,3,4')




        self.add_objs_row = pm.rowLayout( 
                                        numberOfColumns=2,
                                        columnWidth2=(200, 200),
                                        adjustableColumn=1,
                                        columnAlign=(1, 'right'),
                                        columnAttach=[(1, 'both', 2), (2, 'both', 2)])

        self.retries_add_objs_btn = pm.button(
            label='Load selected',
            command=pm.Callback(self.on_load_selected))

        self.retries_add_all_objs_btn = pm.button(
            label='Load all skeleton strokes',
            command=pm.Callback(self.on_load_all_skel))
        pm.setParent("..")

        pm.scrollLayout(bv=True)
        self.objects_column = pm.columnLayout(adj=True)
        return frame

    def on_publish_rb_change(self):
        state = pm.radioButtonGrp(self.publish_rb, query=True, sl=True)
        pm.rowLayout(self.add_objs_row, edit=True, enable=(state!=3)) 
        pm.textFieldGrp(self.passes_wg, edit=True, enable=(state==3)) 

    def on_load_selected(self):
        nodes = pm.ls(selection=True)
        self._load_retries_nodes(nodes)

    def on_load_all_skel(self):
        self._load_retries_nodes(pm.ls(type="skeletonStroke"))

    def _load_retries_nodes(self, nodes):
        self._clear_entries()
        if not nodes:
            return
        typ = type(nodes[0])
        if any(not isinstance(o, typ) for o in nodes[1:]):
            pm.error("Objects must all be the same type")

        frame = 0
        for node in nodes:
            pm.setParent(self.objects_column)
            self._create_entry(node, frame)
            frame += 1

    def _clear_entries(self):
        children = pm.columnLayout(self.objects_column, q=True, ca=True)
        if children:
            pm.deleteUI(children)

    def _create_entry(self, node, frame):
        with uutl.activatable(state=True):
            intf = pm.intFieldGrp(numberOfFields=1, label=node, value1=frame)
        return intf
 
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
        packs = self.fetch_packs()
        for pack in packs:
            for item in packs[pack]:
                item["plug"] = str(item["plug"])

        print json.dumps(packs, indent=2)

    def get_step_values(self):
        count = pm.intFieldGrp(self.num_retries_wg, query=True, value1=True)
        step_type_idx = pm.radioButtonGrp(
            self.varying_step_type_wg, query=True, sl=True)
        step_type = ["dummy", "linear", "binary", "random"][step_type_idx]
        low, high = pm.floatFieldGrp(
            self.varying_range_wg, query=True, value=True)
        vals = []
        if step_type is "random":
            vals = [random.uniform(low, high) for x in range(count)]
        else:
            valrange = high - low
            gap = valrange / (count - 1)
            vals = [low + (gap * x) for x in range(count)]
        return vals

    def fetch_packs(self):

        try_existing = pm.checkBoxGrp(
            self.try_existing_first_cb, query=True, value1=True)

        attribute = pm.textFieldGrp(
            self.varying_attrib_wg, query=True, text=True)

        result = {}
        forms = pm.columnLayout(self.objects_column, q=True, ca=True)

        for form in forms:
            vals = self.get_step_values()
            int_field, toggle = pm.formLayout(form, q=True, ca=True)
            frame = pm.intFieldGrp(int_field, q=True, value1=True)
            node = pm.intFieldGrp(int_field, q=True, label=True)
            do_retry = pm.checkBox(toggle, q=True, value=True)

            if frame not in result:
                result[frame] = []

            plug = pm.PyNode(node).attr(attribute)

            if plug.get(lock=True):
                pm.error("{} is locked. Can't adjust.")
            if plug.inputs():
                pm.error("{} has input connections. Can't adjust.")

            initial_values = [plug.get()] if try_existing else []

            result[frame].append({
                "plug": plug,
                "values": initial_values + vals,
                "frame": frame,
                "do_retry": do_retry
            })
        return result

    def on_go(self):
        uutl.checkRobolink()


        
        publish_type = pm.radioButtonGrp(self.publish_rb, query=True, sl=True)
        print "publish_type", publish_type
        # return
        if publish_type > 1: #
            export_dir = write.choose_publish_dir()
            if not export_dir:
                return
            pm.checkBoxGrp(self.publish_tab.current_cb, edit=True, value1=0)

        self.clear_collect_main_keys()
        

        if publish_type  == 3: 
            self.do_publish_passes(export_dir)

        if publish_type  == 2: 
            success = self.do_retries()
            self.publish_tab.publish_to_directory(export_dir)
            timestamp = datetime.datetime.now().strftime('%y%m%d_%H%M')
            write.write_maya_scene(export_dir, timestamp )
            self.clear_collect_main_keys()

        if publish_type  == 1: 
            success = self.do_retries()

        

        
    def clear_collect_main_keys(self):
        print "CLEAR_COLLECT_MAIN_KEYS"
        pm.cutKey("collectStrokesMain", at=("startFrom", "endAt"), option="keys")
        pm.PyNode("collectStrokesMain").attr("startFrom").set(0)
        pm.PyNode("collectStrokesMain").attr("endAt").set(-1)
        
        # if success and do_publish:
        #     self.publish_tab.publish_to_directory(export_dir)



    def do_publish_passes(self, export_dir):
        pass_ids = [int(i) for i in pm.textFieldGrp(self.passes_wg, query=True, text=True).split(",") if i is not None and i.isdigit()]
        main_collector = pm.PyNode("collectStrokesMain")
        timestamp = datetime.datetime.now().strftime('%y%m%d_%H%M')
        for pass_id in pass_ids:
            print "PASS_ID: ",pass_id
            self.clear_collect_main_keys()
            collector =main_collector.attr("strokes[{}]".format(pass_id)).connections(s=True, d=False)[0]
            ts_export_dir = os.path.join(export_dir,timestamp)
            main_collector.attr("strokeFilterList[0].strokeFilterOperand").set(pass_id)
            main_collector.attr("strokeFilterList[0].strokeFilterOperator").set(2)
            skels = collector.history(type="skeletonStroke")
            self._load_retries_nodes(skels)
            success = self.do_retries()
            print "COLLECTOR", collector[14:]
            prefix = str(collector)[14:]
            self.publish_tab.publish_to_directory(ts_export_dir, prefix=prefix)
            self.clear_collect_main_keys()
        write.write_maya_scene(ts_export_dir, str(main_collector))


    def do_retries(self):

        packs = self.fetch_packs()
        frames = sorted(packs.keys())

        results = []
        self.deactivate_all(packs)
        for frame in frames:
            pack = packs[frame]
            self.activate_pack(pack)

            if any(entry["do_retry"] for entry in pack):
                retries_result = self.do_retries_for_pack(pack)
                results.append(retries_result)
            self.deactivate_pack(pack)

        failed = False
        some_empty = False
        for res in results:
            if not res["solved"]:
                failed = True
                print "Pack number: {} UNSOLVED".format(res["frame"])
            else:  # solved
                if res["attempts"] == -1:
                    some_empty = True




        self.activate_all(packs)
        uutl.show_in_window(results, title="Retries results")

        if failed:
            print "Some frames could not be resolved"
        else:
            if some_empty:
                print "All frames succeeded, some were empty"
            else:
                print "All frames succeeded"

        return not failed


    def activate_all(self, packs):
        for k in packs:
            self.activate_pack(packs[k], True)

    def deactivate_all(self, packs):
        for k in packs:
            self.deactivate_pack(packs[k])

    def activate_pack(self, pack, everything=False):
        for entry in pack:
            if everything == True or entry["do_retry"]:
                entry["plug"].node().attr("active").set(True)

    def deactivate_pack(self, pack):
        for entry in pack:
            entry["plug"].node().attr("active").set(False)

    def do_retries_for_pack(self, pack):
        """"""
        result = {
            "plugs": [],
            "attempts": -1,
            "path_results": [],
            "solved": False,
            "frame": pack[0]["frame"]
        }

        print "{0} RUNNING RETRIES FRAME: {1} {0}".format(
            "*" * 20, result["frame"])

        painting_node = pm.PyNode("mainPaintingShape")
        try:
            num_clusters = pm.paintingQuery(painting_node, cc=True)
        except RuntimeError as ex:
            pm.displayWarning(ex.message)
            result["solved"] = True
            return result

        with uutl.minimize_robodk():
            count = len(pack[0]["values"])
            for i in range(count):
                for item in pack:
                    try:
                        item["plug"].set(item["values"][i])
                    except (pm.MayaAttributeError, RuntimeError) as e:
                        pm.warning(
                            "Problem with : {} - Skipping".format(item["plug"]))
                pm.refresh()

                studio = Studio(do_painting=True)
                studio.write()

                path_result = studio.painting_program.validate_path()
                metadata = {"iteration": i, "plug_values": [
                    {"plug": str(item["plug"]), "value": item["values"][i]} for item in pack]}
                path_result.update(metadata)
                result["path_results"].append(path_result)

                print "{0} frame: {1} {3}/{4} {2} {0}".format(
                    "- " * 10, result["frame"], path_result["status"], (i + 1), count)

                if path_result["status"] == "SUCCESS":
                    result["attempts"] = i + 1
                    result["solved"] = True
                    break

        return result
