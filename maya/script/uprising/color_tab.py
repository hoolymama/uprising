# import os
# import datetime
# import tarfile
# import time
# from contextlib import contextmanager

# import calibration as cal
# import progress
import pymel.core as pm
import pymel.core.uitypes as gui
import palette_utils as putl
# import robo
# import stats
# import uprising_util as uutl
# import write
# from studio import Studio
import math
import random

import uprising_util as uutl

NUM_GRAY_COLORS = 9
SPREAD_DARKER = 1
SPREAD_LIGHTER = 2
SPREAD_BOTH = 3

DO_DISPLAY = 0
DO_POTS = 1

AVAILABLE_POTS = 0

class ColorTab(gui.FormLayout):

    def __init__(self):
        self.setNumberOfDivisions(100)
        pm.setParent(self)
        self.column = pm.columnLayout()
        self.column.adjustableColumn(True)
        self.color_frame_ui = self.color_frame()
        pm.setParent(self)
        self.go_but = self.create_action_buttons()

    def color_frame(self):

        ann = {
            "cumdev": "Should the hsv spread be additive as it spreads, or added to the base each time"
        }

        frame = pm.frameLayout(label="Colors", bv=True)
        self.random_seed_wg = pm.intSliderGrp(
            label="Random Seed", field=True, value=1, minValue=0, maxValue=1000)
        self.base_color_wg = pm.colorSliderGrp(
            label="Base Color", rgb=(1, 0, 0))
        self.base_pot_id_wg = pm.intSliderGrp(
            label="Base Pot ID", field=True, value=5, minValue=0, maxValue=9)
        self.passes_wg = pm.checkBoxGrp(numberOfCheckBoxes=4, label="Passes", valueArray4=(
            0, 1, 0, 0), labelArray4=("Foreground", "Coarse", "Fine", "Detail"))
        self.spread_direction_wg = pm.radioButtonGrp(
            height=30, l="Spread direction", sl=3, nrb=3, la3=["Darker", "Lighter", "Both"])
        self.spread_steps_wg = pm.intSliderGrp(
            label="Spread Steps", field=True, value=10, minValue=0, maxValue=30)

        self.sample_probability_wg = pm.floatSliderGrp(
            label="Sample Probablility", field=True, value=1, minValue=0, maxValue=1)
        self.sample_probability_falloff_wg = pm.floatSliderGrp(
            label="Sample Probablility Falloff", field=True, value=2, minValue=0, maxValue=5)

        self.hue_widgets = make_range_widget(
            label="Hue Deviation", minValue=0, maxValue=360, step=1, value=0)
        self.sat_widgets = make_range_widget(
            label="Sat Deviation", minValue=0, maxValue=1, step=0.01, value=0)
        self.val_widgets = make_range_widget(
            label="Val Deviation", minValue=0, maxValue=1, step=0.01, value=0)
        self.cumulative_dev_wg = pm.checkBoxGrp(
            numberOfCheckBoxes=1, label="Cumulative Deviation", value1=True, ann=ann["cumdev"])

        pm.setParent("..")
        return frame

    def create_action_buttons(self):
        pm.setParent(self)  # form

        go_but = pm.button(
            label="Go", command=pm.Callback(self.on_go, DO_POTS))
        display_but = pm.button(
            label="Display", command=pm.Callback(self.on_go, DO_DISPLAY))

        self.attachForm(self.column, "left", 2)
        self.attachForm(self.column, "right", 2)
        self.attachForm(self.column, "top", 2)
        self.attachControl(self.column, "bottom", 2, display_but)

        self.attachNone(display_but, "top")
        self.attachPosition(display_but, "right", 2, 50)
        self.attachForm(display_but, "left", 2)
        self.attachForm(display_but, "bottom", 2)

        self.attachNone(go_but, "top")
        self.attachForm(go_but, "right", 2)
        self.attachControl(go_but, "left", 2, display_but)
        self.attachForm(go_but, "bottom", 2)

        return go_but

    ##############################################################

    def on_go(self, mode):

        random_seed = pm.intSliderGrp(
            self.random_seed_wg, query=True, value=True)

        base_color = pm.colorSliderGrp(
            self.base_color_wg, query=True, rgb=True)
        base_pot_id = pm.intSliderGrp(
            self.base_pot_id_wg, query=True, value=True)
        passes = pm.checkBoxGrp(self.passes_wg, query=True, valueArray4=True)

        spread_direction = pm.radioButtonGrp(
            self.spread_direction_wg, query=True,  sl=True)

        spread_steps = pm.intSliderGrp(
            self.spread_steps_wg, query=True,  value=True)

        sample_probability = pm.floatSliderGrp(
            self.sample_probability_wg, query=True,  value=True)

        sample_probability_falloff = pm.floatSliderGrp(
            self.sample_probability_falloff_wg, query=True,  value=True)

        hue_range = (pm.floatSliderGrp(self.hue_widgets[0], query=True, value=True), pm.floatSliderGrp(
            self.hue_widgets[1], query=True, value=True))
        sat_range = (pm.floatSliderGrp(self.sat_widgets[0], query=True, value=True), pm.floatSliderGrp(
            self.sat_widgets[1], query=True, value=True))
        val_range = (pm.floatSliderGrp(self.val_widgets[0], query=True, value=True), pm.floatSliderGrp(
            self.val_widgets[1], query=True, value=True))

        cumulative_dev = pm.checkBoxGrp(
            self.cumulative_dev_wg, query=True,  value1=True)

        color_packs = configure_color_packs(base_color,
                                       base_pot_id,
                                       spread_direction,
                                       spread_steps,
                                       sample_probability,
                                       sample_probability_falloff,
                                       hue_range,
                                       sat_range,
                                       val_range,
                                       cumulative_dev,
                                       random_seed)

        if mode == DO_DISPLAY:
            show_in_window(color_packs, title="Packs")
        else:
            apply_colors(color_packs, passes)


def configure_color_packs(
    base_color,
    base_pot_id,
    spread_direction,
    spread_steps,
    sample_probability,
    sample_probability_falloff,
    hue_range,
    sat_range,
    val_range,
    cumulative_dev,
    random_seed=0
):
    random.seed(random_seed)
    recip = 1.0/360.0
    hue_range = (hue_range[0]*recip, hue_range[1]*recip)

    packs = {}
    if spread_direction in [SPREAD_LIGHTER, SPREAD_BOTH]:
        packs["lighter"] = calc_colors(base_color, spread_steps, sample_probability,
                                       sample_probability_falloff,  hue_range, sat_range, val_range, cumulative_dev)
    if spread_direction in [SPREAD_DARKER, SPREAD_BOTH]:
        packs["darker"] = calc_colors(base_color, spread_steps, sample_probability,
                                      sample_probability_falloff,  hue_range, sat_range, val_range, cumulative_dev)

    packs["base"] = {
        "offset": 0,
        "color": base_color
    }

    packs = limit_length(packs)
 
    return packs

    # print "i: {}, norm:{:.2f}, rn:{:.2f}, prob:{:.2f}, Inc:{}".format(i, norm_dist,rn, prob, should_include)
def limit_length(packs):
    
    while True:
        if packs["lighter"] and not packs["lighter"][-1]["color"]:
            packs["lighter"] = packs["lighter"][:-1]
        elif packs["darker"] and not packs["darker"][-1]["color"]:
            packs["darker"] = packs["darker"][:-1]
        else:
            len_lighter = len([p for p in packs["lighter"] if p["color"]])
            len_darker = len([p for p in packs["darker"] if p["color"]])
            if len_lighter +  len_darker > AVAILABLE_POTS:
                if len(packs["lighter"]) > len(packs["darker"]):
                    packs["lighter"] = packs["lighter"][:-1]
                else:
                    packs["darker"] = packs["darker"][:-1]
            else:
                break
    return packs



def calc_colors(base_color, spread_steps, sample_probability, sample_probability_falloff,  hue_range, sat_range, val_range, cumulative_dev):

    results = []
    start_color_hsv = pm.dt.Color.rgbtohsv(base_color)

    for i in range(spread_steps):
        norm_dist = i / float(spread_steps)
        prob = sample_probability * \
            pow(1.0 - norm_dist, sample_probability_falloff)
        rn = random.random()
        should_include = rn < prob
        if should_include:
            color_hsv = calc_color(
                start_color_hsv, hue_range, sat_range, val_range)
            color_pack = {
                "offset": (i+1),
                "color": pm.dt.Color.hsvtorgb(color_hsv)
            }
            if cumulative_dev:
                start_color_hsv = color_hsv
        else:
            color_pack = {
                "offset": (i+1),
                "color": None
            }

        results.append(color_pack)
    return results


def calc_color(start_color_hsv, hue_range, sat_range, val_range):
    hue = hue_range[0] + (random.random() * (hue_range[1] - hue_range[0]))
    sat = sat_range[0] + (random.random() * (sat_range[1] - sat_range[0]))
    val = val_range[0] + (random.random() * (val_range[1] - val_range[0]))

    color_hsv_offset = pm.dt.Color(hue, sat, val)
    color_hsv = pm.dt.Color(start_color_hsv) + color_hsv_offset

    return pm.dt.Color(
        math.fmod(color_hsv[0], 1.0),
        sorted((0.0, color_hsv[1], 1.0))[1],
        sorted((0.0, color_hsv[2], 1.0))[1]
    )


def make_range_widget(label, minValue,  maxValue, step, value=0):

    form = pm.formLayout()

    label = pm.text(label=label)

    min_wg = pm.floatSliderGrp(
        field=True, value=value, minValue=minValue, maxValue=maxValue, step=step)
    max_wg = pm.floatSliderGrp(
        field=True, value=value, minValue=minValue, maxValue=maxValue, step=step)

    pm.floatSliderGrp(min_wg, edit=True, changeCommand=pm.Callback(
        on_range_widget_changed, min_wg, max_wg, "low"))
    pm.floatSliderGrp(max_wg, edit=True, changeCommand=pm.Callback(
        on_range_widget_changed,  min_wg, max_wg, "high"))

    form.attachForm(label, "top", 2)
    form.attachForm(label, "bottom", 2)
    form.attachPosition(label, "right", 2, 20)
    form.attachForm(label, "left", 2)

    form.attachForm(min_wg, "top", 2)
    form.attachForm(min_wg, "bottom", 2)
    form.attachPosition(min_wg, "right", 2, 60)
    form.attachControl(min_wg, "left", 2, label)

    form.attachForm(max_wg, "top", 2)
    form.attachForm(max_wg, "bottom", 2)
    form.attachForm(max_wg, "right", 2)
    form.attachControl(max_wg, "left", 2, min_wg)
    pm.setParent("..")

    return (min_wg, max_wg)


def on_range_widget_changed(low, high, master="low"):
    low_val = pm.floatSliderGrp(low, query=True, value=True)
    high_val = pm.floatSliderGrp(high, query=True, value=True)
    if master == "low":
        if high_val < low_val:
            pm.floatSliderGrp(high, edit=True, value=low_val)
    else:
        if low_val > high_val:
            pm.floatSliderGrp(low, edit=True, value=high_val)


def show_in_window(color_packs, **kw):
    title = "Color Packs"
    pm.window(width=600, height=1000, title=title)
    pm.scrollLayout()

    if "lighter" in color_packs:
        create_color_widgets(reversed(color_packs["lighter"]))
    create_color_widgets([color_packs["base"]])
    if "darker" in color_packs:
        create_color_widgets(color_packs["darker"])
    pm.setParent("..")
    pm.showWindow()


def create_color_widgets(color_pack):
    for color_entry in color_pack:
        if not color_entry["color"]:
            rgb = pm.dt.Color(0.5, 0.5, 0.5)
            enable = False
        else:
            rgb = color_entry["color"][0:3]
            enable = True
        pm.colorSliderGrp(
            label=str(color_entry["offset"]),    rgb=rgb, enable=enable, cw=(3, 0))


def apply_colors(color_pack, passes):

    pots = putl.get_pots()

    do_foreground, do_coarse, do_fine, do_detail = passes

    print pots
    # print passes

    print "do_foreground:", do_foreground, " - do_coarse:", do_coarse, " - do_fine:", do_fine, " - do_detail:", do_detail

    skels = []
    if do_foreground:
        skels += pm.select("skeletonStrokeForeground_*")
    if do_coarse:
        skels += pm.select("skeletonStrokeCoarse_*")
    if do_fine:
        skels += pm.select("skeletonStrokeFine_*")
    if do_detail:
        skels += pm.select("skeletonStrokeDetail_*")


    

    # do_painting, do_subprograms = pm.checkBoxGrp(
    #     self.export_ops_cb, query=True, valueArray2=True
    # )
    # retries_mode = pm.radioButtonGrp(self.retries_mode_rb, query=True, sl=True)
    # # retries_scope = pm.radioButtonGrp(self.retries_scope_rb, query=True, sl=True)
    # retries_count = pm.intFieldGrp(self.num_retries_wg, query=True, value1=True)
    # first, last = pm.floatFieldGrp(self.varying_range_wg, query=True, value=True)

    # should_do_retries = (
    #     not (do_subprograms and not do_painting)
    # ) and retries_mode > 1

    # retries_attribute = "splitAngle"
    # delta = pm.floatFieldGrp(self.coil_delta_ff, query=True, value1=True)

    # # pm.textFieldGrp(
    # #     self.varying_attrib_wg, query=True, text=True
    # # )

    # read_board_cal = pm.checkBoxGrp(
    #     self.read_board_calibration_cb, query=True, value1=True
    # )

    # water_wipe_repeats = pm.intSliderGrp(
    #     self.water_wipe_repeats_isg, query=True, value=True
    # )

    # cluster_chunk_size = pm.intFieldGrp(
    #     self.cluster_chunk_if, query=True, value1=True
    # )

    # directory = write.choose_publish_dir()
    # if not directory:
    #     return

    # timestamp = write.get_timestamp()
    # directory = os.path.join(directory, timestamp)

    # uutl.mkdir_p(directory)

    # total_timer_start = time.time()

    # if should_do_retries:

    #     if read_board_cal:
    #         cal.read_board_calibration()

    #     nodes = find_contributing_skel_nodes()

    #     step_values = get_step_values(first, last, retries_count)

    #     retries_result = do_retries(
    #         nodes, retries_attribute, retries_mode, step_values, delta
    #     )

    #     uutl.show_in_window(retries_result, title="Retries results")
    #     write.json_report(directory, "retries", retries_result)

    #     status = "SUCCESS" if retries_result["success"] else "FAILURE"
    #     time_str = str(datetime.timedelta(seconds=int(retries_result["timer"])))
    #     progress.update(
    #         major_line="Retries completed in {} : {}".format(time_str, status),
    #         minor_line="",
    #         major_progress=0,
    #         minor_progress=0,
    #     )

    # if do_painting:
    #     write.publish_painting(directory, cluster_chunk_size=cluster_chunk_size)

    # if do_subprograms:
    #     write.publish_pick_place(directory)
    #     write.publish_dips(directory, water_wipe_repeats)

    # if do_painting or do_subprograms:
    #     src_folder = os.path.join(directory, "src")
    #     with tarfile.open("{}.tar.gz".format(src_folder), "w:gz") as tar:
    #         tar.add(src_folder, arcname=os.path.sep)

    #     progress.update(
    #         header="Export completed",
    #         major_line="",
    #         minor_line="",
    #         major_progress=0,
    #         minor_progress=0,
    #     )

    # delta = int(time.time() - total_timer_start)

    # total_time_str = str(datetime.timedelta(seconds=delta))
    # progress.update(
    #     major_line="Total time: {}".format(total_time_str),
    #     minor_line="",
    #     major_progress=0,
    #     minor_progress=0,
    # )
    # ptg_stats = stats.stats()
    # all_stats = {
    #     "painting_stats":ptg_stats,
    #     "per_brush_stats": stats.stats_per_brush()
    # }
    # ptg_stats = stats.stats()
    # uutl.show_in_window(all_stats, title="Painting stats")
    # write.json_report(directory, "stats", all_stats)
    # write.write_maya_scene(directory, "scene")
    # write.session_entry(directory, timestamp, ptg_stats)

    # def do_retries(nodes, attribute, retries_mode, step_values, delta):
    #     try_existing = retries_mode == 2
    #     coil = retries_mode == 4

    #     timer_start = time.time()
    #     validate_retries_params(attribute, nodes)

    #     plugs = [pm.PyNode(node).attr(attribute) for node in nodes]

    #     num_plugs = len(plugs)
    #     all_skels = pm.ls(type="skeletonStroke")
    #     results = []
    #     robo.new()
    #     progress.update(
    #         major_max=num_plugs, header="Running {:d} Retries".format(num_plugs)
    #     )

    #     for plug_index, plug in enumerate(plugs):
    #         node = plug.node()

    #         with isolate_nodes([node], all_skels):
    #             if coil:
    #                 result = do_coil_retries_for_plug(plug_index, num_plugs, plug, delta)
    #             else:
    #                 result = do_retries_for_plug(
    #                     plug_index, num_plugs, plug, step_values, try_existing
    #                 )
    #             results.append(result)

    #     timer_end = time.time()
    #     result_data = {
    #         "results": results,
    #         "success": all([res["solved"] for res in results]),
    #         "timer": timer_end - timer_start,
    #     }

    #     robo.close()
    #     return result_data

    # def do_retries_for_plug(plug_index, num_plugs, plug, step_values, try_existing):
    #     result = {
    #         "plug": str(plug),
    #         "attempts": -1,
    #         "path_results": [],
    #         "solved": False,
    #         "frame": plug_index,
    #         "timer": 0,
    #     }

    #     # print "RUNNING RETRIES: {} {}".format( plug,  "*" * 20 )
    #     painting_node = pm.PyNode("mainPaintingShape")
    #     try:
    #         pm.paintingQuery(painting_node, cc=True)
    #     except RuntimeError as ex:
    #         pm.displayInfo(ex.message)
    #         result["solved"] = True
    #         return result

    #     if try_existing:
    #         existing = plug.get()
    #         values = [existing] + [v for v in step_values if v < existing]
    #     else:
    #         values = step_values

    #     plug_start = time.time()

    #     progress.update(major_progress=plug_index)

    #     num_values = len(values)
    #     for i, value in enumerate(values):
    #         plug.set(value)
    #         pm.refresh()

    #         progress.update(
    #             major_line="Retrying plug: {} - {:d}/{:d}, Iteration:{:d}/{:d} Value:{:f}".format(
    #                 plug, (plug_index + 1), num_plugs, i + 1, num_values, value
    #             )
    #         )

    #         iter_start = time.time()

    #         robo.new()

    #         program = Studio(do_painting=True).painting_program

    #         if not program and program.painting and program.painting.clusters:
    #             continue

    #         program.write()

    #         path_result = program.validate_path() or {"status": "SUCCESS"}

    #         path_result.update(
    #             {"iteration": i, "value": value, "timer": time.time() - iter_start}
    #         )

    #         result["path_results"].append(path_result)

    #         if path_result["status"] == "SUCCESS":
    #             result["attempts"] = i + 1
    #             result["solved"] = True
    #             break

    #     result["timer"] = time.time() - plug_start
    #     return result

    # def do_coil_retries_for_plug(plug_index, num_plugs, plug, delta):
    #     result = {
    #         "plug": str(plug),
    #         "attempts": -1,
    #         "path_results": [],
    #         "solved": False,
    #         "frame": plug_index,
    #         "timer": 0,
    #     }

    #     # print "RUNNING RETRIES: {} {}".format( plug,  "*" * 20 )
    #     painting_node = pm.PyNode("mainPaintingShape")
    #     try:
    #         pm.paintingQuery(painting_node, cc=True)
    #     except RuntimeError as ex:
    #         pm.displayInfo(ex.message)
    #         result["solved"] = True
    #         return result

    #     plug_start = time.time()

    #     progress.update(major_progress=plug_index)

    #     i = 0
    #     values = []
    #     plug.set(360)
    #     while True:
    #         value = plug.get()
    #         values.append(value)

    #         pm.refresh()

    #         progress.update(
    #             major_line="Plug: {} - {:d}/{:d}, Attempt:{:d} Vals:{}".format(
    #                 plug,
    #                 (plug_index + 1),
    #                 num_plugs,
    #                 i + 1,
    #                 (",").join(["{:.1f}".format(v) for v in values]),
    #             )
    #         )

    #         iter_start = time.time()

    #         robo.new()

    #         program = Studio(do_painting=True).painting_program
    #         if program and program.painting and program.painting.clusters:

    #             program.write()

    #             path_result = program.validate_path() or {"status": "SUCCESS"}

    #             path_result.update(
    #                 {"iteration": i, "value": value, "timer": time.time() - iter_start}
    #             )

    #             result["path_results"].append(path_result)

    #             if path_result["status"] == "SUCCESS":
    #                 result["attempts"] = i + 1
    #                 result["solved"] = True
    #                 break

    #             # Here if we need to try again
    #             i += 1
    #             coil = pm.PyNode(plug).node().attr("outCoil").get()
    #             next_val = min(coil, value) - delta
    #             print "Coil: {}, Value: {}, Next: {}".format(coil, value, next_val)
    #             if next_val < 0:
    #                 break
    #             plug.set(next_val)

    #     result["timer"] = time.time() - plug_start
    #     return result

    # def validate_retries_params(attribute, nodes):
    #     if len(nodes) == 0:
    #         pm.error("No nodes selected. Aborting!")
    #     if any([n.type() != "skeletonStroke" for n in nodes]):
    #         confirm = pm.confirmDialog(
    #             title="Confirm",
    #             message="Selected nodes are not skeletonStrokes. Continue? ",
    #             button=["Yes", "No"],
    #             defaultButton="Yes",
    #             cancelButton="No",
    #             dismissString="No",
    #         )
    #         if confirm == "No":
    #             pm.error("User aborted")

    #     for node in nodes:
    #         plug = pm.PyNode(node).attr(attribute)
    #         if plug.get(lock=True):
    #             pm.error("{} is locked. Can't adjust.".format(plug))
    #         if plug.inputs():
    #             pm.error("{} has input connections. Can't adjust.".format(plug))

    # def get_step_values(low, high, count):
    #     if count < 2:
    #         return [high]
    #     vals = []
    #     valrange = high - low
    #     gap = valrange / (count - 1)
    #     vals = [low + (gap * x) for x in range(count)]
    #     return vals

    #     def populate(self):
    #         pass

    #     def save(self):
    #         pass

    # def find_contributing_skel_nodes():

    #     painting_node = pm.PyNode("mainPaintingShape")
    #     all_skels = pm.ls(type="skeletonStroke")

    #     result = [n for n in pm.ls(sl=True) if n.type() == "skeletonStroke"]
    #     if result:
    #         return result

    #     for node in all_skels:
    #         with isolate_nodes([node], all_skels):
    #             try:
    #                 pm.paintingQuery(painting_node, cc=True)
    #             except RuntimeError:
    #                 continue
    #         result.append(node)
    #     print "{} of {} skeleton nodes contributing".format(len(result), len(all_skels))
    #     return result

    # def create_export_frame(self):

    #     frame = pm.frameLayout(label="Export", bv=True)

    #     try:
    #         pm.Attribute("mainPaintingShape.subject")
    #     except BaseException:
    #         try:
    #             pm.PyNode("mainPaintingShape").addAttr("subject", dataType="string")
    #         except BaseException:
    #             pass

    #     try:
    #         self.subject_field = pm.attrControlGrp(
    #             attribute="mainPaintingShape.subject"
    #         )
    #     except BaseException:
    #         pass

    #     self.export_ops_cb = pm.checkBoxGrp(
    #         numberOfCheckBoxes=2,
    #         label="Export",
    #         labelArray2=("Painting", "Subprograms"),
    #         valueArray2=(1, 1),
    #         changeCommand=pm.Callback(self.on_ops_change),
    #     )

    #     self.cluster_chunk_if = pm.intFieldGrp(
    #         height=30,
    #         label="Max clusters/file",
    #         annotation="Max number of clusters per painting partial program",
    #         numberOfFields=1,
    #         value1=100,
    #     )

    #     self.water_wipe_repeats_isg = pm.intSliderGrp(
    #         label="Water wipe repeats",
    #         height=30,
    #         field=True,
    #         minValue=1,
    #         maxValue=5,
    #         fieldMinValue=1,
    #         fieldMaxValue=5,
    #         value=2,
    #         annotation="How many times to repeat the whole wipe pattern after a water dip.",
    #     )
    #     pm.setParent("..")
    #     return frame

    # def create_progress_frame(self):
    #     frame = pm.frameLayout(
    #         bv=True, collapse=False, labelVisible=True, label="Progress"
    #     )
    #     col = pm.columnLayout(adj=True)
    #     progress.create(col)
    #     return frame

    # def on_ops_change(self):
    #     do_painting, do_subprograms = pm.checkBoxGrp(
    #         self.export_ops_cb, query=True, valueArray2=True
    #     )
    #     can_do_retries = not (do_subprograms and not do_painting)
    #     pm.frameLayout(self.retries_frame, edit=True, en=can_do_retries)
    #     pm.intFieldGrp(self.cluster_chunk_if, edit=True, en=do_painting)
    #     pm.intSliderGrp(self.water_wipe_repeats_isg, edit=True, en=do_subprograms)
    #     retries_mode = pm.radioButtonGrp(self.retries_mode_rb, query=True, sl=True)

    #     coil = (retries_mode==4)
    #     pm.floatFieldGrp(self.varying_range_wg, edit=True, en=do_varying_range)

    #     do_varying_range = (not coil) and can_do_retries

    #     pm.intFieldGrp(self.num_retries_wg, edit=True, en=do_varying_range)

    #     pm.floatFieldGrp(self.coil_delta_ff, edit=True, en=(coil and can_do_retries))
    #     # pm.radioButtonGrp(self.retries_scope_rb, edit=True, en=(can_do_retries))
    #     pm.checkBoxGrp(self.read_board_calibration_cb, edit=True, en=(can_do_retries))

    #     can_go = (
    #         (can_do_retries and (retries_mode>1)) or do_painting or do_subprograms
    #     )
    #     pm.button(self.go_but, edit=True, en=can_go)
