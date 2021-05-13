"""
The goal of a retries session is to set the split angle for each set of chains
in a skeletonStroke node to values that can be painted by the robot.
"""

from contextlib import contextmanager
import time
import datetime
import pymel.core as pm
from uprising import utils, robo, progress
from uprising.common.session.session import Session

from uprising.bot.session.bot_program import BotProgram, BotRetryProgram
from uprising import chains


@contextmanager
def disable_all_skels():
    all_skels = pm.PyNode("mainPaintingShape").listHistory(type="skeletonStroke")
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
    def __init__(self, coil_delta, selector_plug, dry_run=False, directory=None):

        self.dry_run = dry_run
        self.directory = directory
        self.delta = coil_delta
        # self.chain_chunk_size = chain_chunk_size
        self.painting_node = pm.PyNode("mainPaintingShape")

        self.start_time = None
        self.total_num_plugs = 0
        self.current_run_id = 0
        self.plug_index = None
        if selector_plug:
            self.chain_skel_pairs = chains.get_chain_skel_pairs(selector_plug.node())

            plug_index = selector_plug.get()
            if plug_index > -1:
                self.plug_index = plug_index
        else:
            # Get all skel pairs where the skel has normal nodestate
            self.chain_skel_pairs = chains.get_chain_skel_pairs()

        if not len(self.chain_skel_pairs):
            pm.error("No skel nodes. Aborting!")

        self.result_data = {"plug_runs": [], "success": False, "timer": 0}

    def run(self):
        robo.new()
        robo.clean("kr30", infrastructure=False)
        
        chains.chunkify_skels(self.chain_skel_pairs, 1)

        if self.plug_index is None:
            self.total_num_plugs = chains.count_outputs(self.chain_skel_pairs)
        else:
            self.total_num_plugs = 1

        with disable_all_skels():
            results = []
            progress.update(
                major_max=self.total_num_plugs,
                header="Running {:d} Retries".format(self.total_num_plugs),
            )
            timer_start = time.time()
            with utils.prep_for_output():
                for pair in self.chain_skel_pairs:
                    self.run_for_node(pair)

        self.result_data["success"] = all([res["solved"] for res in self.result_data["plug_runs"]])
        self.result_data["timer"] = time.time() - timer_start

        status = "SUCCESS" if self.result_data["success"] else "FAILURE"
        time_str = str(datetime.timedelta(seconds=int(self.result_data["timer"])))
        progress.update(
            major_line="Retries completed in {} : {}".format(time_str, status),
            minor_line="",
            major_progress=0,
            minor_progress=0,
        )

    def run_for_node(self, chain_skel_pair):

        chain, skel = chain_skel_pair

        with activate_node(skel):
            plug_ids = (
                [self.plug_index]
                if self.plug_index is not None
                else range(chain.attr("outputCount").get())
            )

            for plug_index in plug_ids:
                result = self.run_for_plug(skel, plug_index)
                if result:
                    self.result_data["plug_runs"].append(result)
                    if not result["solved"]:
                        skel.attr("inputData")[plug_index].attr("active").set(False)
                        # unsolved_plug_indices.append(plug_index)
                self.current_run_id += 1
            skel.attr("selector").set(-1)
    
    # def calc_num_runs(self):

    def run_for_plug(self, skel, plug_index):
        skel.attr("selector").set(plug_index)
        if self.dry_run:
            return None
        
        split_angle_plug = skel.attr("inputData")[plug_index].attr("splitAngle")

        cluster_count = pm.paintingQuery(self.painting_node, cc=True)
        if not cluster_count:
            pm.displayInfo("Skipping plug: {}".format(split_angle_plug))
            return None

        result = self.initialize_plug_result(split_angle_plug)
 
        run_start = time.time()
        progress.update(major_progress=self.current_run_id)
        attempt = 0
        values = []
        # plug.set(360)
   
        initial_angle = pm.strokeQuery(skel, maxCoil=True) + self.delta
        split_angle_plug.set(initial_angle)

        while True:
            attempt += 1
            value = split_angle_plug.get()
            values.append(value)
            pm.refresh()

            self.update_progress_for_attempt(attempt, split_angle_plug, values)

            ##########
            program = BotRetryProgram("retry")
            if not (program and program.painting and program.painting.clusters):
                break
            program.configure()

            program.send()
            validation_result = program.validate_path() or {"status": "SUCCESS"}
            ##########

            path_result = {
                "status": validation_result["status"],
                "attempt": attempt, 
                "value": value
            }

            result["path_results"].append(path_result)
            if path_result["status"] == "SUCCESS":
                result["attempts"] = attempt
                result["solved"] = True
                break

            # Here if we need to try again

            angle = pm.strokeQuery(skel, maxCoil=True)
            next_val = min(angle, value) - self.delta

            print "split - next_val", next_val
            if next_val <= 0:
                break
            split_angle_plug.set(next_val)
        result["timer"] = time.time() - run_start
        return result

    def update_progress_for_attempt(self, attempt, plug, values, cull=False):
        accum_values = (",").join(["{:.1f}".format(v) for v in values])

        prefix = "Cull - plug" if cull else "Split - plug"
        progress.update(
            major_line="{}: {} - {:d}/{:d}, Attempt:{:d} Vals:{}".format(
                prefix, plug, (self.current_run_id + 1), self.total_num_plugs, attempt, accum_values
            )
        )

    def initialize_plug_result(self, split_angle_plug):
        result = {
            "plug": str(split_angle_plug),
            "attempts": 0,
            "path_results": [],
            "solved": False,
            "timer": 0
        }
        return result

    def show_results(self):
        utils.show_in_window(self.result_data, title="Retries results")

    def write_results(self):
        if self.directory:
            self.json_report(self.directory, "retries", self.result_data)

    @staticmethod
    def validate_retries_params(nodes):
        if len(nodes) == 0:
            pm.error("No nodes selected. Aborting!")

    # def deactivate_unsolved_chains(self, chain_skel_pair, plug_indices):
    #     print "Checking for deactivations"
    #     chains.chunkify_skels([chain_skel_pair], 1)
    #     chain, skel = chain_skel_pair
    #     for chunked_plug_index in plug_indices:
    #         start_index = chunked_plug_index * self.chain_chunk_size
    #         end_index = start_index + self.chain_chunk_size
    #         for plug_index in range(start_index, end_index):
    #             plug = skel.attr("inputData")[plug_index]
    #             conns = plug.chains.connections(s=True, d=False, type="skChainNode")
    #             if conns:
    #                 result = self.run_for_plug_simple(skel, plug_index)
    #                 if not result["solved"]:
    #                     skel.attr("inputData")[plug_index].attr("active").set(False)
    #                     print "Deactivated unsolvable plug:", plug

    # def run_for_plug_simple(self, skel, plug_index):
    #     skel.attr("selector").set(plug_index)
    #     result = self.initialize_plug_result(skel, plug_index)
    #     if self.dry_run:
    #         return result
    #     if not result["cluster_count"]:
    #         return result

    #     run_start = time.time()
    #     # progress.update(major_progress=self.current_run_id)
    #     attempt = 0
    #     values = []
    #     # plug.set(360)
    #     split_angle_plug = skel.attr("inputData")[plug_index].attr("splitAngle")

    #     initial_angle = pm.strokeQuery(skel, maxCoil=True) + self.delta
    #     split_angle_plug.set(initial_angle)

    #     while True:
    #         attempt += 1
    #         # print "attempt", attempt
    #         value = split_angle_plug.get()
    #         values.append(value)
    #         pm.refresh()

    #         self.update_progress_for_attempt(attempt, split_angle_plug, values, True)

    #         iter_start = time.time()
    #         robo.clean("kr30", infrastructure=False)

    #         ##########
    #         program = BotRetryProgram("retry")
    #         if not (program and program.painting and program.painting.clusters):
    #             break
    #         program.configure()

    #         program.send()
    #         path_result = program.validate_path() or {"status": "SUCCESS"}
    #         ##########

    #         path_result.update(
    #             {"attempt": attempt, "value": value, "timer": time.time() - iter_start}
    #         )
    #         result["path_results"].append(path_result)
    #         if path_result["status"] == "SUCCESS":
    #             result["attempts"] = attempt
    #             result["solved"] = True
    #             break

    #         # Here if we need to try again

    #         angle = pm.strokeQuery(skel, maxCoil=True)
    #         next_val = min(angle, value) - self.delta

    #         print "cull - next_val", next_val
    #         if next_val <= 0:
    #             break
    #         split_angle_plug.set(next_val)
    #     result["timer"] = time.time() - run_start
    #     return result
