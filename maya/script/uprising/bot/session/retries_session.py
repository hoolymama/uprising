
"""
The goal of a retries session is to set the split angle for each set of clusters
in a skeletonStroke node to values that can be painted by the robot.
"""

from contextlib import contextmanager
import time
import datetime
import pymel.core as pm
from uprising import (utils, robo, progress)
from uprising.common.session.session import Session

from uprising.bot.session.bot_program import BotProgram, BotRetryProgram
from uprising import chains


@contextmanager
def disable_all_skels():
    all_skels = pm.PyNode("mainPaintingShape").listHistory(
        type="skeletonStroke")
    all_vals = [n.attr("nodeState").get() for n in all_skels]
    for i, n in enumerate(all_skels):
        n.attr("nodeState").set(1)
    try:
        yield
    finally:
        for i, n in enumerate(all_skels):
            n.attr("nodeState").set(all_vals[i])


@contextmanager
def activate_node(skel):
    node_state = skel.attr("nodeState").get()
    skel.attr("nodeState").set(0)
    try:
        yield
    finally:
        skel.attr("nodeState").set(node_state)


class RetriesSession(Session):

    def __init__(self,
                 coil_delta,
                 chains_per_retry,
                 selector_plug=None):

        self.delta = coil_delta
        self.chains_per_retry = chains_per_retry
        self.painting_node = pm.PyNode("mainPaintingShape")

        self.start_time = None
        self.total_num_runs = 0

        self.plug_index = None
        if selector_plug:
            # print "selector_plug.type()", selector_plug 
            # print "selector_plug.node().type()", selector_plug.node().type()

            self.chain_skel_pairs = chains.get_chain_skel_pairs(selector_plug.node())

            plug_index = selector_plug.get()
            if plug_index > -1:
                self.plug_index = plug_index
        else:
            self.chain_skel_pairs = chains.get_chain_skel_pairs()

        if not len(self.chain_skel_pairs):
            pm.error("No skel nodes. Aborting!")

        self.result_data = {
            "plug_runs": [],
            "success": False,
            "timer": 0
        }

    # def dry_run(self):

    #     chains.chunkify_skels(
    #         self.chain_skel_pairs,
    #         self.chains_per_retry
    #     )

    #     if self.plug_index is None:
    #         self.total_num_runs = chains.count_outputs(self.chain_skel_pairs)
    #     else:
    #         self.total_num_runs = 1

    #     with disable_all_skels():
    #         results = []
    #         run_id = 0
    #         with utils.final_position(pm.PyNode("mainPaintingShape")):
    #             with utils.at_value(pm.PyNode("canvasTransform").attr("applyBrushBias"), True):
    #                 for chain, skel in self.chain_skel_pairs:
    #                     with activate_node(skel):
    #                         plug_ids = [self.plug_index] if self.plug_index is not None else range(
    #                             chain.attr("outputCount").get())
    #                         for plug_index in plug_ids:
    #                             skel.attr("selector").set(plug_index)
    #                             result = self.initialize_plug_result(
    #                                 skel,  plug_index, run_id)
    #                             results.append(result)
    #                             run_id += 1
    #                         skel.attr("selector").set(-1)

    #     self.result_data = {
    #         "plug_runs": results
    #     }

    def run(self, dry_run=False):

        chains.chunkify_skels(
            self.chain_skel_pairs,
            self.chains_per_retry
        )

        if self.plug_index is None:
            self.total_num_runs = chains.count_outputs(self.chain_skel_pairs)
        else:
            self.total_num_runs = 1

        with disable_all_skels():

            results = []
            if not dry_run:
                robo.new()
                robo.hide()

            progress.update(
                major_max=self.total_num_runs,
                header="Running {:d} Retries".format(self.total_num_runs)
            )
            timer_start = time.time()
            run_id = 0
            with utils.final_position(pm.PyNode("mainPaintingShape")):
                with utils.at_value(pm.PyNode("canvasTransform").attr("applyBrushBias"), True):
                    with utils.at_value(pm.PyNode("brushLifter").attr("nodeState"), 0):
                        for chain, skel in self.chain_skel_pairs:
                            with activate_node(skel):
                                plug_ids = [self.plug_index] if self.plug_index is not None else range(
                                    chain.attr("outputCount").get())
                                print "Activate skel:", skel, "with", len(
                                    plug_ids), "outputs"
                                for plug_index in plug_ids:
                                    result = self.run_for_plug(
                                        skel, plug_index, run_id, dry_run)
                                    results.append(result)
                                    run_id += 1
                                skel.attr("selector").set(-1)

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

    # def calc_num_runs(self):

    def run_for_plug(self,  skel, plug_index, run_id, dry_run=False):
        skel.attr("selector").set(plug_index)
        result = self.initialize_plug_result(skel, plug_index, run_id, dry_run)

        if dry_run:

            return result

        if not result["cluster_count"]:
            return result

        run_start = time.time()
        progress.update(major_progress=run_id)
        attempt = 0
        values = []
        # plug.set(360)
        split_angle_plug = skel.attr("inputData")[
            plug_index].attr("splitAngle")

        initial_angle = pm.strokeQuery(skel, maxCoil=True) + self.delta
        split_angle_plug.set(initial_angle)
        print "run_for_plug - Skel: ", skel.name(),"Plug:", plug_index
        while True:
            attempt += 1
            print "attempt", attempt
            value = split_angle_plug.get()
            values.append(value)
            pm.refresh()

            self.update_progress_for_attempt(
                attempt,
                split_angle_plug, run_id, values)

            iter_start = time.time()
            robo.clean("kr30", infrastructure=False)

            ##########
            program = BotRetryProgram("retry")
            if not (program and program.painting and program.painting.clusters):
                break
            program.configure()

            program.send()
            path_result = program.validate_path() or {"status": "SUCCESS"}
            ##########

            path_result.update(
                {"attempt": attempt,
                 "value": value,
                 "timer": time.time() - iter_start
                 }
            )

            result["path_results"].append(path_result)
            if path_result["status"] == "SUCCESS":
                result["attempts"] = attempt
                result["solved"] = True
                break

            # Here if we need to try again

            # coil = pm.PyNode(plug).node().attr("outCoil").get()
            angle = pm.strokeQuery(skel, maxCoil=True)
            next_val = min(angle, value) - self.delta

            if next_val <= 0:
                break
            split_angle_plug.set(next_val)

        result["timer"] = time.time() - run_start
        return result

    def update_progress_for_attempt(self, attempt, plug, run_id,  values):
        accum_values = (",").join(["{:.1f}".format(v) for v in values])
        progress.update(
            major_line="Plug: {} - {:d}/{:d}, Attempt:{:d} Vals:{}".format(
                plug,
                (run_id + 1),
                self.total_num_runs,
                attempt,
                accum_values
            )
        )

    def initialize_plug_result(self, skel,  plug_index, run_id, dry_run=False):
        result = {
            "skel_strokes_count": pm.strokeQuery(skel, sc=True),
            "plug": skel.attr("inputData")[plug_index].name(),
            "plug_index": plug_index,
            "run_id": run_id,
            "attempts": 0,
            "path_results": [],
            "solved": False,
            "timer": 0,
            "cluster_count":  pm.paintingQuery(self.painting_node, cc=True),
            "painting_strokes_count": 0
        }
        for i in range(result["cluster_count"]):
            result["painting_strokes_count"] += pm.paintingQuery(
                self.painting_node, ci=i, sc=True)

        if not result["cluster_count"]:
            result["solved"] = True
        if dry_run:
            result["solved"] = True

        return result

    def show_results(self):
        utils.show_in_window(self.result_data, title="Retries results")

    @staticmethod
    def validate_retries_params(nodes):
        if len(nodes) == 0:
            pm.error("No nodes selected. Aborting!")
