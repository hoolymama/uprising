
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


class exportTab(gui.FormLayout):

    def __init__(self):
        self.setNumberOfDivisions(100)
        pm.setParent(self)

        self.simple_ui = self.create_simple_ui()
        pm.setParent(self)

        self.create_action_buttons()

        pm.setParent(self)
        # self.initialize_ui()

    def create_simple_ui(self):

        frame = pm.frameLayout(
            collapsable=False,
            collapse=False,
            en=True,
            labelVisible=False,
            label="Simple validation"
        )
        # pm.scrollLayout(childResizable=True)
        pm.columnLayout(adj=True)

        pm.frameLayout(
            collapsable=False,
            collapse=False,
            en=True,
            lv=True,
            bv=True,
            label="Painting programs"
        )
        pm.columnLayout(adj=True)
        self.painting_wg = pm.checkBoxGrp(h=30, l='Painting', v1=0)
        self.dips_wg = pm.checkBoxGrp(h=30, l='Dips', v1=0)
        self.slop_wg = pm.checkBoxGrp(h=30, l='Slop', v1=0)

        pm.setParent("..")
        pm.setParent("..")

        pm.frameLayout(
            collapsable=False,
            collapse=False,
            en=True,
            lv=True,
            bv=True,
            label="Pick & place programs"
        )
        pm.columnLayout(adj=True)
        self.pp_wg = pm.radioButtonGrp(l='Pick & place', sl=1, nrb=4,
                                       la4=['None', 'First', 'Used', 'All'])
        self.ppx_wg = pm.checkBoxGrp(h=30, l='Pick & place exercise', v1=0)
        pm.setParent("..")
        pm.setParent("..")

        pm.frameLayout(
            collapsable=False,
            collapse=False,
            en=True,
            lv=True,
            bv=True,
            label="Pick & place programs"
        )
        pm.columnLayout(adj=True)
        self.perspex_tri_wg = pm.checkBoxGrp(
            h=30, l='Perspex triangulation', v1=0)
        self.perspex_cal_wg = pm.checkBoxGrp(
            h=30, l='Perspex calibration', v1=0)
        pm.setParent("..")
        pm.setParent("..")

        pm.frameLayout(
            collapsable=False,
            collapse=False,
            en=True,
            lv=True,
            bv=True,
            label="Pot & holder calibration programs"
        )
        pm.columnLayout(adj=True)
        self.pot_cal_wg = pm.checkBoxGrp(h=30, l='Paintpot calibration', v1=0)
        self.holder_cal_wg = pm.checkBoxGrp(h=30, l='Holder calibration', v1=0)
        pm.setParent("..")
        pm.setParent("..")

        pm.frameLayout(
            collapsable=False,
            collapse=False,
            en=True,
            lv=True,
            bv=True,
            label="Board calibration programs"
        )
        pm.columnLayout(adj=True)
        self.board_cal_wg = pm.checkBoxGrp(h=30, l='Board calibration', v1=0)
        pm.setParent("..")
        pm.setParent("..")

        pm.frameLayout(
            collapsable=False,
            collapse=False,
            en=True,
            lv=True,
            bv=True,
            label="Export geometry"
        )
        pm.columnLayout(adj=True)
        self.brush_tools_wg = pm.radioButtonGrp(h=30, l='Brushes', sl=1, nrb=4,
                                                la4=['None',  'Used', 'Connected', 'Selected'])

        self.pots_geo_wg = pm.checkBoxGrp(h=30, l='Used paint pots', v1=0)

        self.sel_geo_wg = pm.checkBoxGrp(h=30, l='Selected geometry', v1=0)

        self.approaches_wg = pm.checkBoxGrp(h=30, l='Global approaches', v1=0)
        pm.setParent("..")
        pm.setParent("..")
        # pm.setParent("..")

        return frame

    def create_action_buttons(self):
        pm.setParent(self)  # form

        go_but = pm.button(label='Send to RoboDK',
                           command=pm.Callback(self.on_go))

        self.attachForm(self.simple_ui, 'left', 2)
        self.attachForm(self.simple_ui, 'right', 2)
        self.attachForm(self.simple_ui, 'top', 2)
        self.attachControl(self.simple_ui, 'bottom', 2, go_but)

        self.attachNone(go_but, 'top')
        self.attachForm(go_but, 'right', 2)
        self.attachForm(go_but, 'left', 2)
        self.attachForm(go_but, 'bottom', 2)

    def on_go(self):

        pp = pm.radioButtonGrp(self.pp_wg, query=True, sl=True)
        pp_value = [None, None, [0], "used", "all"][pp]

        studio_kwargs = {
            "do_painting": pm.checkBoxGrp(self.painting_wg, query=True, v1=True),
            "do_dips": pm.checkBoxGrp(self.dips_wg, query=True, v1=True),
            "do_slop": pm.checkBoxGrp(self.slop_wg, query=True, v1=True),

            "do_pap_exercise": pm.checkBoxGrp(self.ppx_wg, query=True, v1=True),

            "do_perspex_triangulation": pm.checkBoxGrp(self.perspex_tri_wg, query=True, v1=True),
            "do_perspex_calibration": pm.checkBoxGrp(self.perspex_cal_wg, query=True, v1=True),

            "do_pot_calibration": pm.checkBoxGrp(self.pot_cal_wg, query=True, v1=True),
            "do_holder_calibration": pm.checkBoxGrp(self.holder_cal_wg, query=True, v1=True),

            "do_board_calibration": pm.checkBoxGrp(self.board_cal_wg, query=True, v1=True),

            "do_pots_geo": pm.checkBoxGrp(self.pots_geo_wg, query=True, v1=True),
            "do_approaches": pm.checkBoxGrp(self.approaches_wg, query=True, v1=True),
            "pick_and_place_slots": pp_value
        }

        studio = Studio(**studio_kwargs)
        studio.write()

        brush_tools = pm.radioButtonGrp(
            self.brush_tools_wg, query=True, sl=True)
        if brush_tools == 2:
            Brush.write_used_brush_sets()
        elif brush_tools == 3:
            Brush.write_connected_brushes()
        elif brush_tools == 4:
            Brush.write_selected_brushes()

        if pm.checkBoxGrp(self.sel_geo_wg, query=True, v1=True):
            props.send_selected()

        if pm.checkBoxGrp(self.pots_geo_wg, query=True, v1=True):
            Paint.write_geos()

        # result = []
        # if not any(send):
        #     return
        # frames = self._get_frames()

        # try:
        #     for f in range(frames[0], (frames[1] + 1)):
        #         pm.currentTime(f)
        #         studio = Studio(**kw)
        #         studio.write()

        #         if kw.get("do_painting"):
        #             p_stats = write.painting_stats(
        #                 pm.PyNode("mainPaintingShape"))
        #             p_stats["frame"] = f
        #             validation_info = studio.painting_program.validate_path()
        #             validation_info.update(p_stats)
        #             result.append(validation_info)

        # except Exception:
        #     t, v, tb = sys.exc_info()
        #     if result:
        #         uutl.show_in_window(
        #             result, title="Simple validation results (incomplete)")
        #     raise t, v, tb
        # else:
        #     if result:
        #         uutl.show_in_window(result, title="Simple validation results")

    # uutl show_in_window(self, data, **kw):
    #     title= kw.get("title", "Validation results")
    #     result_json = json.dumps( data , indent=2)
    #     pm.window(width=600,  height=800, title="Validation results")
    #     pm.frameLayout(cll=False, lv=False)
    #     pm.scrollField(text=result_json, editable=False, wordWrap=False)
    #     pm.showWindow()

    # def get_step_values(self):
    #     count = pm.intFieldGrp(self.num_retries_wg, query=True, value1=True)
    #     step_type_idx = pm.radioButtonGrp(
    #         self.varying_step_type_wg, query=True, sl=True)
    #     step_type = ["dummy", "linear", "random"][step_type_idx]
    #     low, high = pm.floatFieldGrp(
    #         self.varying_range_wg, query=True, value=True)
    #     vals = []
    #     if step_type is "random":
    #         vals = [random.uniform(low, high) for x in range(count)]
    #     else:
    #         valrange = high - low
    #         gap = valrange / (count - 1)
    #         vals = [low + (gap * x) for x in range(count)]
    #     return vals

    # def fetch_packs(self):

    #     try_existing = pm.checkBoxGrp(
    #         self.try_existing_first_cb, query=True, value1=True)

    #     attribute = pm.textFieldGrp(
    #         self.varying_attrib_wg, query=True, text=True)

    #     result = {}
    #     forms = pm.columnLayout(self.objects_column, q=True, ca=True)

    #     for form in forms:
    #         vals = self.get_step_values()
    #         int_field, toggle = pm.formLayout(form, q=True, ca=True)
    #         frame = pm.intFieldGrp(int_field, q=True, value1=True)
    #         node = pm.intFieldGrp(int_field, q=True, label=True)
    #         do_retry = pm.checkBox(toggle, q=True, value=True)

    #         if frame not in result:
    #             result[frame] = []

    #         plug = pm.PyNode(node).attr(attribute)

    #         if plug.get(lock=True):
    #             pm.error("{} is locked. Can't adjust.")
    #         if plug.inputs():
    #             pm.error("{} has input connections. Can't adjust.")

    #         initial_values = [plug.get()] if try_existing else []

    #         result[frame].append({
    #             "plug": plug,
    #             "values": initial_values + vals,
    #             "frame": frame,
    #             "do_retry": do_retry
    #         })
    #     return result

    #     # pm.warning("Aborting")

    # def do_retries(self):
    #     # start_frame = pm.currentTime(query=True)

    #     packs = self.fetch_packs()
    #     # print packs

    #     frames = sorted(packs.keys())
    #     # print frames
    #     results = []
    #     self.deactivate_all(packs)
    #     for frame in frames:
    #         # pm.currentTime(frame)
    #         pack = packs[frame]
    #         self.activate_pack(pack)

    #         if any(entry["do_retry"] for entry in pack):
    #             retries_result = self.do_retries_for_pack(pack)
    #             results.append(retries_result)
    #         self.deactivate_pack(pack)

    #     failed = False
    #     some_empty = False
    #     for res in results:
    #         if not res["solved"]:
    #             failed = True
    #             print "Pack number: {} UNSOLVED".format(res["frame"])
    #         else:  # solved
    #             if res["attempts"] == -1:
    #                 some_empty = True

    #     if not failed:
    #         if some_empty:
    #             print "All frames succeeded, some were empty"
    #         else:
    #             print "All frames succeeded"

    #     self.activate_all(packs)

    #     # pm.currentTime(start_frame)
    #     uutl.show_in_window(results, title="Retries results")

    # def activate_all(self, packs):
    #     for k in packs:
    #         self.activate_pack(packs[k], True)

    # def deactivate_all(self, packs):
    #     for k in packs:
    #         self.deactivate_pack(packs[k])

    # def activate_pack(self, pack, everything=False):
    #     for entry in pack:
    #         if everything == True or entry["do_retry"]:
    #             entry["plug"].node().attr("active").set(True)

    # def deactivate_pack(self, pack):
    #     for entry in pack:
    #         entry["plug"].node().attr("active").set(False)

    # def do_retries_for_pack(self, pack):
    #     """"""
    #     result = {
    #         "plugs": [],
    #         "attempts": -1,
    #         "path_results": [],
    #         "solved": False,
    #         "frame": pack[0]["frame"]
    #     }

    #     # painting_node = pm.PyNode("mainPaintingShape")
    #     print "{0} RUNNING RETRIES FRAME: {1} {0}".format(
    #         "*" * 20, result["frame"])

    #     painting_node = pm.PyNode("mainPaintingShape")
    #     try:
    #         num_clusters = pm.paintingQuery(painting_node, cc=True)
    #     except RuntimeError as ex:
    #         pm.displayWarning(ex.message)
    #         result["solved"] = True
    #         return result

    #     with uutl.minimize_robodk():
    #         count = len(pack[0]["values"])
    #         for i in range(count):
    #             for item in pack:
    #                 try:
    #                     item["plug"].set(item["values"][i])
    #                 except (pm.MayaAttributeError, RuntimeError) as e:
    #                     pm.warning(
    #                         "Problem with : {} - Skipping".format(item["plug"]))
    #             pm.refresh()

    #             studio = Studio(do_painting=True)
    #             studio.write()

    #             path_result = studio.painting_program.validate_path()
    #             metadata = {"iteration": i, "plug_values": [
    #                 {"plug": str(item["plug"]), "value": item["values"][i]} for item in pack]}
    #             path_result.update(metadata)
    #             result["path_results"].append(path_result)

    #             # path_result_json = json.dumps( path_result , indent=2)
    #             # print path_result_json
    #             print "{0} frame: {1} {3}/{4} {2} {0}".format(
    #                 "- " * 10, result["frame"], path_result["status"], (i + 1), count)

    #             if path_result["status"] == "SUCCESS":
    #                 result["attempts"] = i + 1
    #                 result["solved"] = True
    #                 break

    #     return result

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
