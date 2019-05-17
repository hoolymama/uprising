
import sys
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

import pymel.core.uitypes as gui


class ValidateTab(gui.FormLayout):

    def __init__(self):
        self.setNumberOfDivisions(100)
        pm.setParent(self)
 
        self.simple_ui = self.create_simple_ui()
        pm.setParent(self)
        self.retries_ui = self.create_retries_ui()
        pm.setParent(self)
        self.create_action_buttons()

        pm.setParent(self)
        self.initialize_ui()

    def on_collapse_change(self):
        state = pm.frameLayout(
            self.retries_ui,
            query=True,
            collapse=True)
        pm.frameLayout(self.simple_ui, edit=True, en=state)
        pm.button(self.preview_but, edit=True, en=(not state))

    def create_simple_ui(self):

        frame = pm.frameLayout(
            collapsable=False,
            collapse=False,
            en=True,
            labelVisible=True,
            label="Simple validation"
        )

        self.send_paintings_cb = pm.checkBoxGrp(
            numberOfCheckBoxes=3,
            height=30,
            label='Create components',
            labelArray3=['Painting', 'Pick & place', "Dips"],
            valueArray3=[True, True, True],
            columnWidth4=(180, 90, 90, 90)
        )

        self.anim_row = pm.rowLayout(
            numberOfColumns=2, columnWidth2=(
                (390), 100), columnAlign=(
                1, 'right'), columnAttach=[
                (1, 'both', 2), (2, 'both', 2)])

        min_frame = int(pm.playbackOptions(min=True, query=True))
        max_frame = int(pm.playbackOptions(max=True, query=True))

        self.frame_if = pm.intFieldGrp(
            label="Frames to validate",
            numberOfFields=2,
            value1=min_frame,
            value2=max_frame)

        self.current_frame_cb = pm.checkBox(
            label='Current',
            value=1,
            annotation='Use current frame only',
            changeCommand=pm.Callback(self.on_current_frame_cb_change)
        )

        return frame

    def create_retries_ui(self):

        frame = pm.frameLayout(
            bv=True,
            collapsable=True,
            collapse=False,
            labelVisible=True,
            label="Retries",
            ec=pm.Callback(self.on_collapse_change),
            cc=pm.Callback(self.on_collapse_change)
        )

        pm.columnLayout(adj=True)
        self.varying_attrib_wg = pm.textFieldGrp(
            label='Attribute', text='splitAngle')

        self.varying_range_wg = pm.floatFieldGrp(
            numberOfFields=2, label='Range', value1=0, value2=360.0)

        self.varying_step_type_wg = pm.radioButtonGrp(
            label='Step Type',
            sl=1,
            labelArray2=[
                'Linear',
                'Random'
            ],
            numberOfRadioButtons=2
        )
        pm.setParent("..")

        self.num_retries_wg = pm.intFieldGrp(
            label='Attempts',
            numberOfFields=1,
            value1=5)

        self.retries_add_objs_btn = pm.button(
            label='Load selected',
            command=pm.Callback(self.on_load_selected))

        pm.scrollLayout(bv=True)
        self.objects_column = pm.columnLayout(adj=True)

        return frame

    def on_load_selected(self):
        nodes = pm.ls(selection=True)
        if not nodes:
            pm.error("Nothing selected")

        typ = type(nodes[0])
        if any(not isinstance(o, typ) for o in nodes[1:]):
            pm.error("Objects must all be the same type")
        self._clear_entries()

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

    def on_current_frame_cb_change(self):
        state = pm.checkBox(self.current_frame_cb, query=True, value=True)
        pm.intFieldGrp(self.frame_if, edit=True, enable=(not state))

    def initialize_ui(self):
        self.on_collapse_change()
        self.on_current_frame_cb_change()

    def create_action_buttons(self):
        pm.setParent(self)  # form

        self.preview_but = pm.button(
            label='Preview', command=pm.Callback(
                self.on_preview))
        go_but = pm.button(label='Go', command=pm.Callback(self.on_go))

        self.attachForm(self.simple_ui, 'left', 2)
        self.attachForm(self.simple_ui, 'right', 2)
        self.attachForm(self.simple_ui, 'top', 2)
        self.attachNone(self.simple_ui, 'bottom')

        self.attachForm(self.retries_ui, 'left', 2)
        self.attachForm(self.retries_ui, 'right', 2)
        self.attachControl(self.retries_ui, 'top', 2, self.simple_ui)
        self.attachControl(self.retries_ui, 'bottom', 2, self.preview_but)

        self.attachNone(self.preview_but, 'top')
        self.attachForm(self.preview_but, 'left', 2)
        self.attachPosition(self.preview_but, 'right', 2, 50)
        self.attachForm(self.preview_but, 'bottom', 2)

        self.attachNone(go_but, 'top')
        self.attachForm(go_but, 'right', 2)
        self.attachPosition(go_but, 'left', 2, 50)
        self.attachForm(go_but, 'bottom', 2)

    def on_go(self):
        do_simple = pm.frameLayout(self.simple_ui, query=True, en=True)
        if do_simple:
            self.do_simple_validation()
        else:
            self.do_retries()

    def on_preview(self):
        packs = self.fetch_packs()
        for pack in packs:
            for item in packs[pack]:
                item["plug"] = str(item["plug"])

        print json.dumps(packs, indent=2)

  

    def _get_frames(self):
        current_only = pm.checkBox(
            self.current_frame_cb, query=True, value=True)
        if current_only:
            frame = int(pm.currentTime(query=True))
            frames = (frame, frame)
        else:
            frames = (
                pm.intFieldGrp(
                    self.frame_if,
                    query=True,
                    value1=True),
                pm.intFieldGrp(
                    self.frame_if,
                    query=True,
                    value2=True))
        return frames

    def do_simple_validation(self):

        send = pm.checkBoxGrp(
            self.send_paintings_cb,
            query=True,
            valueArray3=True)
        use_gripper = pm.optionVar.get("upov_tool_type") == "gripper"

        kw = {
            "do_painting": send[0],
            "do_pick_and_place": send[1] and use_gripper,
            "do_dips": send[2]
        }

        result = []
        if not any(send):
            return
        frames = self._get_frames()

        try:
            for f in range(frames[0], (frames[1] + 1)):
                pm.currentTime(f)
                studio = Studio(**kw)
                studio.write()

                if kw.get("do_painting"):
                    p_stats = write.painting_stats(
                        pm.PyNode("mainPaintingShape"))
                    p_stats["frame"] = f
                    validation_info = studio.painting_program.validate_path()
                    validation_info.update(p_stats)
                    result.append(validation_info)

        except Exception:
            t, v, tb = sys.exc_info()
            if result:
                uutl.show_in_window(result, title="Simple validation results (incomplete)")
            raise t, v, tb 
        else:
            if result:
                uutl.show_in_window(result, title="Simple validation results")

    # uutl show_in_window(self, data, **kw):
    #     title= kw.get("title", "Validation results")
    #     result_json = json.dumps( data , indent=2)
    #     pm.window(width=600,  height=800, title="Validation results")
    #     pm.frameLayout(cll=False, lv=False)
    #     pm.scrollField(text=result_json, editable=False, wordWrap=False)
    #     pm.showWindow()




    def get_step_values(self):
        count = pm.intFieldGrp(self.num_retries_wg, query=True, value1=True)
        step_type_idx = pm.radioButtonGrp(
            self.varying_step_type_wg, query=True, sl=True)
        step_type = ["dummy", "linear", "random"][step_type_idx]
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
        attribute = pm.textFieldGrp(
            self.varying_attrib_wg, query=True, text=True)

        result = {}
        forms = pm.columnLayout(self.objects_column, q=True, ca=True)

        for form in forms:
            vals = self.get_step_values()
            int_field, toggle = pm.formLayout(form, q=True, ca=True)
            frame = pm.intFieldGrp(int_field, q=True, value1=True)
            node = pm.intFieldGrp(int_field, q=True, label=True)
            active = pm.checkBox(toggle, q=True, value=True)
            if active:
                if frame not in result:
                    result[frame] = []
                    plug = pm.PyNode(node).attr(attribute)

                    if plug.get(lock=True):
                        pm.error("{} is locked. Can't adjust.")
                    if plug.inputs():
                        pm.error("{} has input connections. Can't adjust.")

                    initial_value = plug.get()
                    result[frame].append({
                        "plug": plug,
                        "values": [initial_value] + vals,
                        "frame": frame
                    })
        return result

        # pm.warning("Aborting")

    def do_retries(self):
        start_frame = pm.currentTime(query=True)

        packs = self.fetch_packs()
        # print packs

        frames = sorted(packs.keys())
        # print frames
        results = []
        for frame in frames:
            pm.currentTime(frame)
            pack = packs[frame]
            retries_result = self.do_retries_for_pack(pack)
            results.append(retries_result)

        failed = False
        for res in results:
            if not res["solved"]:
                failed = True
                print "Frame: {} UNSOLVED".format(res["frame"])

        if not failed:
            print "All frames succeeded"


        pm.currentTime(start_frame)
        uutl.show_in_window(results, title="Retries results")


    def do_retries_for_pack(self, pack):
        """"""
        result = {
            "plugs": [],
            "attempts": -1,
            "path_results": [],
            "solved": False,
            "frame": pack[0]["frame"]
        }

        # painting_node = pm.PyNode("mainPaintingShape")
        print "{0} RUNNING RETRIES FRAME: {1} {0}".format(
            "*" * 20, result["frame"])

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

                # path_result_json = json.dumps( path_result , indent=2)
                # print path_result_json
                print "{0} frame: {1} {3}/{4} {2} {0}".format(
                    "- " * 10, result["frame"], path_result["status"], (i + 1), count)

                if path_result["status"] == "SUCCESS":
                    result["attempts"] = i + 1
                    result["solved"] = True
                    break

        return result


  # def validate_path(self, program):
    #     RL = Robolink()
    #     update_result = program.Update(COLLISION_OFF)
    #     return {
    #         "instructions": update_result[0],
    #         "time": update_result[1],
    #         "distance": update_result[2],
    #         "completion": update_result[3],
    #         "problems": update_result[4],
    #         "status": "SUCCESS" if (update_result[3] == 1.0) else "FAILURE"
    #     }

#     def format_path_result(self, path_info, **metadata):
#         text = """
# ------------------------------------------
# instructions  : %d
# time          : %s
# distance      : %s
# completion    : %.3f
# problems      : %s
# status        : %s
# """ % (path_info["instructions"],
#                path_info["time"],
#                path_info["distance"],
#                path_info["completion"],
#                path_info["problems"],
#                path_info["status"])

#         if metadata:
#             for k in metadata:
#                 text += "%s : %s\n" % (k, metadata[k])
#         return text


    # def old_do_retries(self):
    #     """Repetitively retry with different values until success."""

    #     # if not self.check_retries_ui_valid():
    #     #     return

    #     count = pm.intFieldGrp(self.num_retries_wg, query=True, value=True)
    #     node_type_idx = pm.radioButtonGrp(
    #         self.varying_node_wg, query=True, sl=True)
    #     node_type = [
    #         "dummy",
    #         "selected",
    #         "shape",
    #         "stroke_curve"][node_type_idx]
    #     attribute = pm.textFieldGrp(
    #         self.varying_attrib_wg, query=True, text=True)
    #     low = pm.floatFieldGrp(
    #         self.varying_range_wg,
    #         query=True,
    #         value1=True)
    #     high = pm.floatFieldGrp(
    #         self.varying_range_wg,
    #         query=True,
    #         value2=True)
    #     step_type_idx = pm.radioButtonGrp(
    #         self.varying_step_type_wg, query=True, sl=True)
    #     step_type = ["dummy", "random", "linear"][step_type_idx]

    #     if node_type is "selected":
    #         obs = pm.ls(selection=True)
    #     elif node_type is "shape":
    #         obs = pm.ls(selection=True, dag=True, ni=True, shapes=True)
    #     else:  # stroke_curve
    #         obs = pm.ls(selection=True, type="curveStroke")

    #         curves = pm.ls(
    #             selection=True, dag=True,
    #             ni=True,
    #             shapes=True,
    #             type="nurbsCurve")
    #         conns = pm.listConnections(
    #             curves,
    #             d=True,
    #             s=False,
    #             type="curveStroke")
    #         obs += conns

    #     cutl.hide_objects(obs)

    #     results = []
    #     for o in obs:
    #         cutl.show_objects([o])
    #         attempts = self.do_retries_for_object(
    #             o, attribute, step_type, low, high, count)
    #         status = "SUCCESS" if (attempts > -1) else "FAILURE"
    #         results.append({"node": o, "status": status, "attempts": attempts})
    #         cutl.hide_objects([o])
    #     print results
    #     print "-" * 30
    #     for res in results:
    #         if res["status"] == "FAILURE":
    #             print res

    #     cutl.show_objects(obs)

    # def do_retries_for_object(self, o, attribute, step_type, low, high, count):
    #     result = -1

    #     curr = o.attr(attribute).get()

    #     if step_type is "random":
    #         vals = [curr] + [random.uniform(low, high) for x in range(count)]
    #     else:
    #         valrange = high - low
    #         gap = valrange / (count - 1)
    #         vals = [curr] + [low + (gap * x) for x in range(count)]

    #     # prepend the current val, because if it works then nothing needs to
    #     # change.

    #     painting_node = pm.PyNode("mainPaintingShape")
    #     msg = ""

    #     with uutl.minimize_robodk():
    #         for i, val in enumerate(vals):
    #             self.setParam(o, attribute, val)
    #             pm.refresh()

    #             studio = Studio(painting_node=painting_node)
    #             studio.write()
    #             path_result = self.validate_path(studio.painting_program)

    #             metadata = {
    #                 "iteration": i,
    #                 "attribute": attribute,
    #                 "value": val
    #             }
    #             msg += self.format_path_result(path_result, **metadata)
    #             if path_result["completion"] == 1.0:
    #                 result = i + 1
    #                 break
    #         print msg
    #         return result

    # def setParam(self, ob, attribute, val):
    #     try:
    #         att = ob.attr(attribute).set(val)
    #     except (pm.MayaAttributeError, RuntimeError) as e:
    #         pm.warning("Error with : %s.%s - Skipping" % (ob, attribute))
