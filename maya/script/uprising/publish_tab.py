import datetime
import os
import tarfile
import time
from contextlib import contextmanager

import calibration as cal
import progress
import pymel.core as pm
import pymel.core.uitypes as gui
import robo
import stats
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
            sl=4,
            nrb=4,
            la4=["None", "Current first", "All", "Coil"],
            changeCommand=pm.Callback(self.on_ops_change),
        )

        self.read_board_calibration_cb = pm.checkBoxGrp(
            label="Read Calibration", label1="Board", value1=1
        )

        # self.varying_attrib_wg = pm.textFieldGrp(label="Attribute", text="splitAngle")

        self.coil_delta_ff = pm.floatFieldGrp(
            numberOfFields=1, label="Delta", value1=20
        )


        self.varying_range_wg = pm.floatFieldGrp(
            numberOfFields=2, label="Range", value1=270, value2=0.0
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

        try:
            pm.Attribute("mainPaintingShape.subject")
        except BaseException:
            try:
                pm.PyNode("mainPaintingShape").addAttr("subject", dataType="string")
            except BaseException:
                pass

        try:
            self.subject_field = pm.attrControlGrp(
                attribute="mainPaintingShape.subject"
            )
        except BaseException:
            pass

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


        coil = (retries_mode==4)
        pm.floatFieldGrp(self.varying_range_wg, edit=True, en=(not coil))
        pm.intFieldGrp(self.num_retries_wg, edit=True, en=(not coil))
        
        pm.floatFieldGrp(self.coil_delta_ff, edit=True, en=(coil))
        

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

        retries_attribute = "splitAngle"
        delta = pm.floatFieldGrp(self.coil_delta_ff, query=True, value1=True)

        # pm.textFieldGrp(
        #     self.varying_attrib_wg, query=True, text=True
        # )

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

        uutl.mkdir_p(directory)

        if read_board_cal:
            cal.read_board_calibration()

        total_timer_start = time.time()

        if should_do_retries:
            nodes = (
                pm.ls(sl=True) if retries_scope == 1 else pm.ls(type="skeletonStroke")
            )
            # try_existing = retries_mode == 2

            step_values = get_step_values(first, last, retries_count)

            retries_result = do_retries(
                nodes, retries_attribute, retries_mode, step_values, delta
            )

            uutl.show_in_window(retries_result, title="Retries results")
            write.json_report(directory, "retries", retries_result)

            status = "SUCCESS" if retries_result["success"] else "FAILURE"
            time_str = str(datetime.timedelta(seconds=int(retries_result["timer"])))
            progress.update(
                major_line="Retries completed in {} : {}".format(time_str, status),
                minor_line="",
                major_progress=0,
                minor_progress=0,
            )

        if do_painting:
            write.publish_painting(directory, cluster_chunk_size=cluster_chunk_size)

        if do_subprograms:
            write.publish_pick_place(directory)
            write.publish_dips(directory, water_wipe_repeats)

        if do_painting or do_subprograms:
            src_folder = os.path.join(directory, "src")
            with tarfile.open("{}.tar.gz".format(src_folder), "w:gz") as tar:
                tar.add(src_folder, arcname=os.path.sep)

            progress.update(
                header="Export completed",
                major_line="",
                minor_line="",
                major_progress=0,
                minor_progress=0,
            )

        delta = int(time.time() - total_timer_start)

        total_time_str = str(datetime.timedelta(seconds=delta))
        progress.update(
            major_line="Total time: {}".format(total_time_str),
            minor_line="",
            major_progress=0,
            minor_progress=0,
        )

        ptg_stats = stats.stats()
        uutl.show_in_window(ptg_stats, title="Retries results")
        write.json_report(directory, "stats", ptg_stats)
        write.write_maya_scene(directory, "scene")
        write.session_entry(directory, timestamp, ptg_stats)


def do_retries(nodes, attribute, retries_mode, step_values, delta):
    try_existing = retries_mode == 2
    coil = retries_mode == 4

    timer_start = time.time()
    validate_retries_params(attribute, nodes)

    plugs = [pm.PyNode(node).attr(attribute) for node in nodes]

    num_plugs = len(plugs)
    all_skels = pm.ls(type="skeletonStroke")
    results = []
    robo.new()
    progress.update(
        major_max=num_plugs, header="Running {:d} Retries".format(num_plugs)
    )

    for plug_index, plug in enumerate(plugs):
        node = plug.node()

        with isolate_nodes([node], all_skels):
            if coil:
                result = do_coil_retries_for_plug(plug_index, num_plugs, plug, delta)
            else:
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

    robo.close()
    return result_data


def do_retries_for_plug(plug_index, num_plugs, plug, step_values, try_existing):
    result = {
        "plug": str(plug),
        "attempts": -1,
        "path_results": [],
        "solved": False,
        "frame": plug_index,
        "timer": 0,
    }

    # print "RUNNING RETRIES: {} {}".format( plug,  "*" * 20 )
    painting_node = pm.PyNode("mainPaintingShape")
    try:
        pm.paintingQuery(painting_node, cc=True)
    except RuntimeError as ex:
        pm.displayInfo(ex.message)
        result["solved"] = True
        return result

    if try_existing:
        existing = plug.get()
        values = [existing] + [v for v in step_values if v < existing]
    else:
        values = step_values

    plug_start = time.time()

    progress.update(major_progress=plug_index)

    num_values = len(values)
    for i, value in enumerate(values):
        plug.set(value)
        pm.refresh()

        progress.update(
            major_line="Retrying plug: {} - {:d}/{:d}, Iteration:{:d}/{:d} Value:{:f}".format(
                plug, (plug_index + 1), num_plugs, i + 1, num_values, value
            )
        )

        iter_start = time.time()

        robo.new(False)

        program = Studio(do_painting=True).painting_program

        if not program and program.painting and program.painting.clusters:
            continue

        program.write()

        path_result = program.validate_path() or {"status": "SUCCESS"}

        path_result.update(
            {"iteration": i, "value": value, "timer": time.time() - iter_start}
        )

        result["path_results"].append(path_result)

        if path_result["status"] == "SUCCESS":
            result["attempts"] = i + 1
            result["solved"] = True
            break

    result["timer"] = time.time() - plug_start
    return result


def do_coil_retries_for_plug(plug_index, num_plugs, plug, delta):
    result = {
        "plug": str(plug),
        "attempts": -1,
        "path_results": [],
        "solved": False,
        "frame": plug_index,
        "timer": 0,
    }

    # print "RUNNING RETRIES: {} {}".format( plug,  "*" * 20 )
    painting_node = pm.PyNode("mainPaintingShape")
    try:
        pm.paintingQuery(painting_node, cc=True)
    except RuntimeError as ex:
        pm.displayInfo(ex.message)
        result["solved"] = True
        return result

    plug_start = time.time()

    progress.update(major_progress=plug_index)

    i = 0
    values = []
    plug.set(360)
    while True:
        value = plug.get()
        values.append(value)

        pm.refresh()

        progress.update(
            major_line="Plug: {} - {:d}/{:d}, Attempt:{:d} Vals:{}".format(
                plug,
                (plug_index + 1),
                num_plugs,
                i + 1,
                (",").join(["{:.1f}".format(v) for v in values]),
            )
        )

        iter_start = time.time()

        robo.new(False)

        program = Studio(do_painting=True).painting_program
        if program and program.painting and program.painting.clusters:

            program.write()

            path_result = program.validate_path() or {"status": "SUCCESS"}

            path_result.update(
                {"iteration": i, "value": value, "timer": time.time() - iter_start}
            )

            result["path_results"].append(path_result)

            if path_result["status"] == "SUCCESS":
                result["attempts"] = i + 1
                result["solved"] = True
                break

            # Here if we need to try again
            i += 1
            coil = pm.PyNode(plug).node().attr("outCoil").get()
            next_val = min(coil, value) - delta
            print "Coil: {}, Value: {}, Next: {}".format(coil, value, next_val)
            if next_val < 0:
                break
            plug.set(next_val)

    result["timer"] = time.time() - plug_start
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

    for node in nodes:
        plug = pm.PyNode(node).attr(attribute)
        if plug.get(lock=True):
            pm.error("{} is locked. Can't adjust.".format(plug))
        if plug.inputs():
            pm.error("{} has input connections. Can't adjust.".format(plug))


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

