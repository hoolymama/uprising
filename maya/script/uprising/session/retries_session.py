
import logging
from contextlib import contextmanager
import time
import datetime
import pymel.core as pm
import uprising.uprising_util as uutl
from uprising import robo
from uprising import progress
from uprising.session.session import Session

from uprising.session.painting_program import PaintingProgram


logger = logging.getLogger("uprising")

ATTRIBUTE = "splitAngle"


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


class RetriesSession(Session):

    def __init__(self, coil_delta, stroke_nodes, directory=None):
        validate_retries_params(stroke_nodes)
        self.directory = directory or self.choose_session_dir()
        if not directory:
            return
        self.painting_node = pm.PyNode("mainPaintingShape")
        self.all_skels = pm.ls(type="skeletonStroke")
        self.delta = coil_delta
        self.nodes = stroke_nodes
        self.plugs = [pm.PyNode(node).attr(ATTRIBUTE) for node in stroke_nodes]

        self.start_time = None

        self.result_data = {
            "plug_runs": [],
            "success": False,
            "timer": 0,
        }

    def run(self):
        results = []
        robo.new()
        robo.hide()
        num_plugs = len(self.plugs)
        progress.update(
            major_max=num_plugs, header="Running {:d} Retries".format(
                num_plugs)
        )
        timer_start = time.time()
        with uutl.final_position(pm.PyNode("mainPaintingShape")):
            with uutl.at_value(pm.PyNode("mainPaintingShape").attr("applyBrushBiases"), True):
                for plug_index, plug in enumerate(self.plugs):
                    node = plug.node()
                    with isolate_nodes([node], self.all_skels):
                        result = self.run_for_plug(plug_index, plug)
                        results.append(result)

        self.result_data = {
            "plug_runs": results,
            "success": all([res["solved"] for res in results]),
            "timer": time.time() - timer_start,
        }

        status = "SUCCESS" if self.result_data["success"] else "FAILURE"
        time_str = str(datetime.timedelta(
            seconds=int(self.result_data["timer"])))
        progress.update(
            major_line="Retries completed in {} : {}".format(
                time_str, status),
            minor_line="",
            major_progress=0,
            minor_progress=0,
        )

    def show_results(self):
        uutl.show_in_window(self.result_data, title="Retries results")

    def write_results(self):
        self.json_report(self.directory, "retries", self.result_data)

    def run_for_plug(self, plug_index, plug):
        result = self.initialize_result(plug_index, plug)
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
                    plug,   (plug_index + 1),  len(self.plugs),  i + 1,
                    (",").join(["{:.1f}".format(v) for v in values]),
                )
            )
            iter_start = time.time()
            robo.clean()
            program = PaintingProgram("retry")
            if program and program.painting and program.painting.clusters:
                program.send()
                path_result = program.validate_path() or {"status": "SUCCESS"}
                path_result.update(
                    {"iteration": i, "value": value,
                        "timer": time.time() - iter_start}
                )
                result["path_results"].append(path_result)
                if path_result["status"] == "SUCCESS":
                    result["attempts"] = i + 1
                    result["solved"] = True
                    break

                # Here if we need to try again
                i += 1
                coil = pm.PyNode(plug).node().attr("outCoil").get()
                next_val = min(coil, value) - self.delta
                if next_val < 0:
                    break
                plug.set(next_val)

        result["timer"] = time.time() - plug_start
        return result

    def initialize_result(self, plug_index, plug):
        result = {
            "plug": str(plug),
            "attempts": -1,
            "path_results": [],
            "solved": False,
            "frame": plug_index,
            "timer": 0,
            "solved": False
        }
        try:
            pm.paintingQuery(self.painting_node, cc=True)
        except RuntimeError as ex:
            pm.displayInfo(ex.message)
            result["solved"] = True
        return result


def validate_retries_params(self, nodes):

    if len(nodes) == 0:
        pm.error("No nodes selected. Aborting!")
    for node in nodes:
        plug = pm.PyNode(node).attr(ATTRIBUTE)
        if plug.get(lock=True):
            pm.error("{} is locked. Can't adjust.".format(plug))
        if plug.inputs():
            pm.error("{} has input connections. Can't adjust.".format(plug))
