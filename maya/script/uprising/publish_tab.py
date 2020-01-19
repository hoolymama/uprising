
import os
import time
from contextlib import contextmanager

import pymel.core as pm
import pymel.core.uitypes as gui
import robo
import uprising_util as uutl
import write
import progress
from studio import Studio
import calibration as cal


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


class PublishTab(gui.FormLayout):
    def __init__(self):
        self.setNumberOfDivisions(100)
        pm.setParent(self)
        self.column = pm.columnLayout()
        self.column.adjustableColumn(True)

        self.retries_frame = self.create_retries_frame()
        self.export_frame = self.create_export_frame()
        self.progress_frame = self.create_progress_frame()

        pm.setParent(self)
        self.go_but = self.create_action_buttons()
        self.on_ops_change()

    def create_retries_frame(self):
        frame = pm.frameLayout(label="Retries", bv=True)

        self.retries_mode_rb = pm.radioButtonGrp(
            height=30,
            l="Retries mode",
            sl=3,
            nrb=3,
            la3=["None", "Current first", "All"],
            changeCommand=pm.Callback(self.on_ops_change),
        )

        self.read_board_calibration_cb = pm.checkBoxGrp(
            label="Read Calibration", label1="Board", value1=1
        )

        self.varying_attrib_wg = pm.textFieldGrp(label="Attribute", text="splitAngle")

        self.varying_range_wg = pm.floatFieldGrp(
            numberOfFields=2, label="Range", value1=270, value2=30.0
        )

        self.num_retries_wg = pm.intFieldGrp(
            label="Attempts", numberOfFields=1, value1=12
        )

        self.retries_scope_rb = pm.radioButtonGrp(
            height=30, l="Retries scope", sl=2, nrb=2, la2=["Selected", "All"]
        )

        pm.setParent("..")
        return frame

    def create_export_frame(self):

        frame = pm.frameLayout(label="Export", bv=True)

        self.export_ops_cb = pm.checkBoxGrp(
            numberOfCheckBoxes=2,
            label="Export",
            labelArray2=("Painting", "Subprograms"),
            valueArray2=(1, 1),
            changeCommand=pm.Callback(self.on_ops_change),
        )

        self.cluster_chunk_if = pm.intFieldGrp(
            height=30,
            label="Max clusters/file",
            annotation="Max number of clusters per painting partial program",
            numberOfFields=1,
            value1=100,
        )

        self.water_wipe_repeats_isg = pm.intSliderGrp(
            label="Water wipe repeats",
            height=30,
            field=True,
            minValue=1,
            maxValue=5,
            fieldMinValue=1,
            fieldMaxValue=5,
            value=2,
            annotation="How many times to repeat the whole wipe pattern after a water dip.",
        )
        pm.setParent("..")
        return frame

    def create_progress_frame(self):
        frame = pm.frameLayout(
            bv=True, collapse=False, labelVisible=True, label="Progress"
        )
        col = pm.columnLayout(adj=True)
        progress.create(col)
        return frame

    def on_ops_change(self):
        do_painting, do_subprograms = pm.checkBoxGrp(
            self.export_ops_cb, query=True, valueArray2=True
        )
        can_do_retries = not (do_subprograms and not do_painting)
        pm.frameLayout(self.retries_frame, edit=True, en=can_do_retries)
        pm.intFieldGrp(self.cluster_chunk_if, edit=True, en=do_painting)
        pm.intSliderGrp(self.water_wipe_repeats_isg, edit=True, en=do_subprograms)
        retries_mode = pm.radioButtonGrp(self.retries_mode_rb, query=True, sl=True)
        can_go = (
            (can_do_retries and (retries_mode > 1)) or do_painting or do_subprograms
        )
        pm.button(self.go_but, edit=True, en=can_go)

    def create_action_buttons(self):
        pm.setParent(self)  # form

        go_but = pm.button(label="Go", command=pm.Callback(self.on_go))

        self.attachForm(self.column, "left", 2)
        self.attachForm(self.column, "right", 2)
        self.attachForm(self.column, "top", 2)
        self.attachControl(self.column, "bottom", 2, go_but)

        self.attachNone(go_but, "top")
        self.attachForm(go_but, "right", 2)
        self.attachForm(go_but, "left", 2)
        self.attachForm(go_but, "bottom", 2)
        return go_but

    ##############################################################

    def on_go(self):

        do_painting, do_subprograms = pm.checkBoxGrp(
            self.export_ops_cb, query=True, valueArray2=True
        )
        retries_mode = pm.radioButtonGrp(self.retries_mode_rb, query=True, sl=True)
        retries_scope = pm.radioButtonGrp(self.retries_scope_rb, query=True, sl=True)
        retries_count = pm.intFieldGrp(self.num_retries_wg, query=True, value1=True)
        first, last = pm.floatFieldGrp(self.varying_range_wg, query=True, value=True)

        should_do_retries = (
            not (do_subprograms and not do_painting)
        ) and retries_mode > 1

        retries_attribute = pm.textFieldGrp(
            self.varying_attrib_wg, query=True, text=True
        )

        read_board_cal = pm.checkBoxGrp(
            self.read_board_calibration_cb, query=True, value1=True
        )

        water_wipe_repeats = pm.intSliderGrp(
            self.water_wipe_repeats_isg, query=True, value=True
        )

        cluster_chunk_size = pm.intFieldGrp(
            self.cluster_chunk_if, query=True, value1=True
        )

        directory = write.choose_publish_dir()
        if not directory:
            return

        timestamp = write.get_timestamp()
        directory = os.path.join(directory, timestamp)

        if read_board_cal:
            cal.read_board_calibration()

        if should_do_retries:
            nodes = (
                pm.ls(sl=True) if retries_scope == 1 else pm.ls(type="skeletonStroke")
            )
            try_existing = retries_mode == 2
            step_values = get_step_values(first, last, retries_count)
            do_retries(
                nodes,
                retries_attribute,
                try_existing,
                step_values,
                directory 
            )

        if do_painting:
            write.publish_separate_files(
                directory,
                water_wipe_repeats=water_wipe_repeats,
                cluster_chunk_size=cluster_chunk_size,
            )
 
 
def do_retries(nodes, attribute, try_existing, step_values, directory):
    timer_start = time.time()
    validate_retries_params(attribute, nodes)
    plugs = fetch_plugs(attribute, nodes)
    num_plugs = len(plugs)
    all_skels = pm.ls(type="skeletonStroke")
    results = []

    progress.update(major_max=num_plugs, header="Running {:d} Retries".format(num_plugs))

    for plug_index, plug in enumerate(plugs):
        node = plug.node()

        progress.update(major_progress=plug_index, major_line= "Retrying plug#: {:d} - {}".format(plug, (plug_index+1)))
  
        with isolate_nodes([node], all_skels):
            result = do_retries_for_plug(
                    plug_index, num_plugs, plug, step_values, try_existing
                )
            results.append(result)

    timer_end = time.time()
    result_data = {
        "results": results,
        "success": all([res["solved"] for res in results]),
        "timer": timer_end - timer_start,
    }
    uutl.show_in_window(result_data, title="Retries results")
    write.json_report(directory ,  "retries", result_data)


def do_retries_for_plug( plug_index,num_plugs, plug, step_values, try_existing):
    result = {
        "plug": str(plug),
        "attempts": -1,
        "path_results": [],
        "solved": False,
        "frame":plug_index,
        "timer":0
     }

    print "RUNNING RETRIES: {} {}".format( plug,  "*" * 20 )
    painting_node = pm.PyNode("mainPaintingShape")
    try:
        pm.paintingQuery(painting_node, cc=True)
    except RuntimeError as ex:
        pm.displaInfo(ex.message)
        result["solved"] = True
        return result

    existing = plug.get()
    if try_existing:
        values =  [existing]+[v for v in step_values if v < existing]
    else:
        values = step_values


    plug_start = time.time()
    count = len(values)

    progress.update(minor_max=count, major_line= "Retrying plug#: {:d} - {}".format(plug, plug_index))

    for i, value in enumerate(values):
        plug.set(value)
        pm.refresh()

        progress.update(minor_progress=count, minor_line="Iteration#: {:d} of {:d} value={:f}".format((i + 1), count,value))



        iter_start = time.time()

        robo.new()

        studio = Studio(do_painting=True)
        studio.write()
        path_result = studio.painting_program.validate_path() or {"status": "SUCCESS"}

        iter_end = time.time()


        metadata = {"iteration": i,  "value": value, "timer":iter_end-iter_start }
        path_result.update(metadata)
        result["path_results"].append(path_result)

        if path_result["status"] == "SUCCESS":
            result["attempts"] = i + 1
            result["solved"] = True
            break
    plug_end = time.time()
    result["timer"] = plug_end-plug_start
    return result


def validate_retries_params(attribute, nodes):
    if len(nodes) == 0:
        pm.error("No nodes selected. Aborting!")
    if any([n.type() != "skeletonStroke" for n in nodes]):
        confirm = pm.confirmDialog(
            title="Confirm",
            message="Selected nodes are not skeletonStrokes. Continue? ",
            button=["Yes", "No"],
            defaultButton="Yes",
            cancelButton="No",
            dismissString="No",
        )
        if confirm == "No":
            pm.error("User aborted")


        


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


def get_step_values(low, high, count):
    if count < 2:
        return [high]
    vals = []
    valrange = high - low
    gap = valrange / (count - 1)
    vals = [low + (gap * x) for x in range(count)]
    return vals

    def populate(self):
        pass

    def save(self):
        pass

# def publish_to_directory(self, directory, **kw):

#     try:
#         pm.paintingQuery("mainPaintingShape", cc=True)
#     except:
#         pm.warning("Something not right!! Do you have any thing selected?")
#         return

#     # wait = pm.checkBoxGrp(self.gripper_wait_cb, query=True, value1=True)
#     # pause = (
#     #     -1
#     #     if wait
#     #     else pm.intFieldGrp(self.gripper_pause_if, query=True, value1=True)
#     # )

#     water_wipe_repeats = pm.intSliderGrp(
#         self.water_wipe_repeats_isg, query=True, value=True
#     )
#     cluster_chunk_size = pm.intFieldGrp(self.cluster_chunk_if, query=True, value1=True)
#     do_separate_files = pm.checkBoxGrp(self.separate_files_cb, query=True, value1=True)

#     if do_separate_files:

#         program_files = write.publish_separate_files(
#             directory,
#             pause_gripper_ms=pause,
#             water_wipe_repeats=water_wipe_repeats,
#             cluster_chunk_size=cluster_chunk_size,
#         )
#         return program_files

#     pm.cutKey("collectStrokesMain", at=("startFrom", "endAt"), option="keys")
#     frames = self.setup_chunks()

#     program_files = write.publish_sequence(directory, frames, pause, water_wipe_repeats)

#     return program_files


# def get_retries_parameters(slf):
#     try_existing = pm.checkBoxGrp(self.try_existing_first_cb, query=True, value1=True)
#     attribute = pm.textFieldGrp(self.varying_attrib_wg, query=True, text=True)
#     count = pm.intFieldGrp(self.num_retries_wg, query=True, value1=True)
#     low, high = pm.floatFieldGrp(self.varying_range_wg, query=True, value=True)
#     step_values = get_step_values(low, high, count)

#     return {
#         "attribute": attribute,
#         "try_existing": try_existing,
#         "step_values": step_values,
#     }


# def get_pack(self):

#     result = self.get_retries_parameters()
#     result["plugs"] = []

#     nodes = pm.ls(sl=True, dag=True, leaf=True, type="skeletonStroke")

#     if len(nodes) == 0:
#         pm.warning("No nodes selected. Aborting!")
#         return result

#     if any([n.type() != "skeletonStroke" for n in nodes]):
#         confirm = pm.confirmDialog(
#             title="Confirm",
#             message="Some selected nodes are not skeletonStrokes. Continue? ",
#             button=["Yes", "No"],
#             defaultButton="Yes",
#             cancelButton="No",
#             dismissString="No",
#         )
#         if confirm == "No":
#             return result
#     result["plugs"] = fetch_plugs(result["attribute"], nodes)
#     return result

