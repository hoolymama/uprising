import math
import datetime
import time
import os
import tarfile
import pymel.core as pm
from uprising import utils
from uprising import robo
from uprising import stats
from uprising import progress
from uprising.common.session.session import Session
from uprising.bot.session.bot_program import BotProgram
from uprising.bot.session.cluster_test_program import ClusterTestProgram

from uprising.bot.session.pick_place_program import PickPlaceCollection
from uprising.bot.session.dip_wipe_program import (
    DipWipeCollection,
    WaterCollection,
    RetardantCollection,
)



class BotPaintingSession(Session):

    PROGRAM_NAME = "px"

    def __init__(self, cluster_chunk_size, directory):
        self.program = None
        self.directory = directory
        self.painting_node = pm.PyNode("mainPaintingShape")
        self.program_names = []

        self.cluster_count = pm.paintingQuery(self.painting_node, cc=True)
        self.cluster_chunk_size = min(cluster_chunk_size, self.cluster_count)

    def run(self):
        timer_start = time.time()
        with utils.prep_for_output(): 

            # CONFIGURE EVERYTHING
            self.program = self._build_bot_program()
            self.program.configure()

            # DIPS&WIPES, WATER, RETARDANT
            self.dip_wipe_collection = DipWipeCollection()
            self.water_collection = WaterCollection()
            self.retardant_collection = RetardantCollection()

            for program in (
                self.dip_wipe_collection.programs
                + self.water_collection.programs
                + self.retardant_collection.programs
            ):
                program.configure()

            # PICK PLACE
            dc = pm.paintingQuery(self.painting_node, dc=True)
            brush_ids = set(dc[::2])
            self.pick_place_collection = PickPlaceCollection(brush_ids)

            # POST STEP Where we try to make linear moves between strokes and between clusters
            # and the palette.

            print "LINKAGES"
            self.configure_linkages()

            print "SEND"
            self._send_and_publish_bot_program()
            self._send_and_publish_pick_place_programs()
            self._send_and_publish_dip_wipe_wash_programs()

        # robo.close()

        duration = int(time.time() - timer_start)
        progress.update(
            major_line="Total time: {}".format(str(datetime.timedelta(seconds=duration))),
            minor_line="",
            major_progress=0,
            minor_progress=0,
        )

        self.stats = {"painting_stats": stats.stats(), "per_brush_stats": stats.stats_per_brush()}

    def _build_bot_program(self):
        self.init_progress()
        robo.clean("kr30")
        with utils.at_value(pm.PyNode("canvasTransform").attr("applyBrushBias"), True):
            with utils.at_value(pm.PyNode("brushLifter").attr("nodeState"), 0):
                program = BotProgram(self.PROGRAM_NAME)
        if not (program and program.painting and program.painting.clusters):
            raise ValueError("Invalid bot_program. No painting/clusters")
        return program

    def _send_and_publish_bot_program(self):
        cluster_count = len(self.program.painting.clusters)
        num_chunks = int(math.ceil(cluster_count / float(self.cluster_chunk_size)))

        progress.update(
            major_max=num_chunks, header="Writing {} main program chunks".format(num_chunks)
        )

        for i in range(num_chunks):
            robo.clean("kr30")
            progress.update(
                major_progress=i, major_line="Writing {:d} of {:d} chunks".format(i + 1, num_chunks)
            )

            subprograms = self.program.send(chunk_id=i, chunk_length=self.cluster_chunk_size)

            src_fn = self.save_program(self.directory, self.program.program_name)
            self.insert_external_dependencies(subprograms, src_fn)
            self.save_station(self.directory, self.program.program_name)

            self.program_names.append(self.program.program_name)

            progress.update(major_progress=num_chunks, major_line="Done")

        if len(self.program_names) > 1:
            self.orchestrate(self.directory, self.program_names)

    def _send_and_publish_pick_place_programs(self):

        program_names = self.pick_place_collection.program_names()
        count = len(program_names)

        progress.update(
            header="Creating {} pick and place programs".format(count),
            major_line="",
            major_max=count,
            major_progress=0,
        )
        robo.clean("kr30")
        self.pick_place_collection.send()

        for program_name in program_names:
            self.save_program(self.directory, program_name)

        self.save_station(self.directory, "pap")

    def _send_and_publish_dip_wipe_wash_programs(self):

        program_names = (
            self.dip_wipe_collection.program_names()
            + self.water_collection.program_names()
            + self.retardant_collection.program_names()
        )
        count = len(program_names)

        progress.update(
            header="Creating {} dip/wip/wash programs".format(count),
            major_line="",
            major_max=count,
            major_progress=0,
        )

        robo.clean("kr30")
        self.dip_wipe_collection.send()
        self.water_collection.send()
        self.retardant_collection.send()

        for program_name in program_names:
            self.save_program(self.directory, program_name)

        self.save_station(self.directory, "dw")

    def init_progress(self):
        progress.update(
            header="Creating main painting",
            major_line="{} clusters in chunks of {}".format(
                self.cluster_count, self.cluster_chunk_size
            ),
            major_progress=0,
            minor_progress=0,
        )

    def show_stats(self):
        utils.show_in_window(self.stats, title="Painting stats")

    def write_stats(self):
        self.json_report(self.directory, "stats", self.stats)

    def make_src_tar(self):
        src_folder = os.path.join(self.directory, "src")
        with tarfile.open("{}.tar.gz".format(src_folder), "w:gz") as tar:
            tar.add(src_folder, arcname=os.path.sep)

    def configure_linkages(self):

        orig_approach_height = self.painting_node.attr("approachDistanceMid").get()
        # make a program from the dip to the cluster arrival and see if it can be done linearly
        robo.clean("kr30")

        for cluster in self.program.painting.clusters:
            print "configure_linkages cluster:", cluster.id

            cluster_test_program = ClusterTestProgram(
                "cluster_test", cluster, self.program.painting.motion, orig_approach_height
            )

            num_strokes = len(cluster.strokes)
            num_links = num_strokes - 1
            num_straightened = 0
            for i in range(1, num_strokes):
                cluster_test_program.configure_internal(i)
                cluster_test_program.send()
                path_result = cluster_test_program.validate_path()
                if path_result["status"] != "SUCCESS":
                    cluster_test_program.undo()
                else:
                    num_straightened += 1

            print "configured ({}/{}) links for cluster:[{}] ".format(
                num_straightened, num_links, cluster.id
            )

    # @staticmethod
    # def configure_cluster_arrival_linkages(cluster):
    #     first_stroke = cluster.strokes[0]
    #     target = first_stroke.arrivals[-1]
    #     brush = cluster.brush
    #     cluster_test_program = LinkProgram(
    #         "linkage_test_program",
    #         robo.DIP_TARGET,  target,  brush)

    #     cluster_test_program.send()

    #     path_result = cluster_test_program.validate_path()
    #     if  path_result["status"] == "SUCCESS":
    #         if len(cluster.strokes[0].arrivals) > 1:
    #             cluster.strokes[0].arrivals = [cluster.strokes[0].arrivals[-1]]
    #         cluster.strokes[0].arrivals[0].linear=True
    #     # print "arrival:", path_result["status"]

    #     prefix2 = first_stroke.name(cluster.name(cluster_test_program.program))
    #     print "TEST {} -> {} === Status {}".format(
    #         robo.DIP_TARGET,
    #         target.name(prefix2),
    #         path_result["status"])

    # @staticmethod
    # def configure_cluster_departure_linkages(cluster):
    #     last_stroke = cluster.strokes[-1]
    #     target = last_stroke.departure
    #     brush = cluster.brush
    #     cluster_test_program = LinkProgram(
    #         "linkage_test_program", target, robo.DIP_TARGET, brush)

    #     cluster_test_program.send()

    #     path_result = cluster_test_program.validate_path()
    #     if  path_result["status"] == "SUCCESS":
    #         cluster.linear_exit = True
    #     # print "departure:", path_result["status"]

    #     prefix1 = last_stroke.name(cluster.name(cluster_test_program.program))
    #     print "TEST {} -> {} === Status {}".format(
    #         target.name(prefix1),
    #         robo.DIP_TARGET,
    #         path_result["status"])

    # @staticmethod
    # def configure_cluster_internal_linkages(cluster):
    #     print "CONFIGURE_CLUSTER_INTERNAL_LINKAGES for", cluster.id
    #     for i, stroke in enumerate(cluster.strokes[1:]):
    #         prev_stroke = cluster.strokes[i-1]

    #         target1 = prev_stroke.departure
    #         target2 = stroke.arrivals[-1]
    #         brush = cluster.brush
    #         cluster_test_program = LinkProgram(
    #             "linkage_test_program",
    #             target1,  target2,  brush)

    #         cluster_test_program.send()

    #         path_result = cluster_test_program.validate_path()
    #         if  path_result["status"] == "SUCCESS":
    #             if len(stroke.arrivals) > 1:
    #                 stroke.arrivals = [stroke.arrivals[-1]]
    #             stroke.arrivals[0].linear=True

    #         n1 = "C:{} S:{} G:{}".format(cluster.id, prev_stroke.id,  prev_stroke.global_stroke_id)
    #         n2 = "C:{} S:{} G:{}".format(cluster.id, stroke.id,  stroke.global_stroke_id)

    #         prev_stroke.name(cluster.name(cluster_test_program.program))
    #         prefix2 = stroke.name(cluster.name(cluster_test_program.program))
    #         print "TEST {} -> {} === Status {}".format(
    #             target1.name( n1),
    #             target2.name(n2),
    #              path_result["status"])

    #         # print "internal", path_result["status"]
