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
from uprising.bot.session.pick_place_program import PickPlaceCollection
from uprising.bot.session.dip_wipe_program import (
    DipWipeCollection,
    WaterCollection,
    RetardantCollection,
)


class BotPaintingSession(Session):

    PROGRAM_NAME = "px"

    def __init__(self, cluster_chunk_size, directory):

        self.directory = directory

        self.painting_node = pm.PyNode("mainPaintingShape")
        self.program_names = []

        robo.new()
        robo.hide()

        self.cluster_count = pm.paintingQuery(self.painting_node, cc=True)
        self.cluster_chunk_size = min(cluster_chunk_size, self.cluster_count)

        timer_start = time.time()
        # PAINTING
        self.program = self.build_bot_program()
        
        self.program.configure()
        # for cluster in self.program.painting.clusters:
        #     for stroke in cluster.strokes:
        #         stroke.configure(cluster.brush)

        #     # cluster.configure()


        self.send_and_publish_bot_program()

        # PICK PLACE
        self.pick_place_collection = PickPlaceCollection("used")
        self.send_and_publish_pick_place_programs()

        # DIPS&WIPES, WATER, RETARDANT
        with utils.final_position(pm.PyNode("RACK1_CONTEXT")):
            self.dip_wipe_collection = DipWipeCollection()
            self.water_collection = WaterCollection()
            self.retardant_collection = RetardantCollection()

            self.dip_wipe_collection.configure()
            self.water_collection.configure()
            self.retardant_collection.configure()

            # self.send_dip_wipe_programs()
            # self.publish_dip_wipe_programs()

            robo.clean("kr30")
            self.dip_wipe_collection.send()
            self.water_collection.send()
            self.retardant_collection.send()

            program_names = self.dip_wipe_collection.program_names()
            program_names += self.water_collection.program_names()
            program_names += self.retardant_collection.program_names()

            for program_name in program_names:
                self.save_program(self.directory, program_name)
            self.save_station(self.directory, "dw")

        robo.close()

        duration = int(time.time() - timer_start)
        progress.update(
            major_line="Total time: {}".format(str(datetime.timedelta(seconds=duration))),
            minor_line="",
            major_progress=0,
            minor_progress=0,
        )

        self.stats = {"painting_stats": stats.stats(), "per_brush_stats": stats.stats_per_brush()}

    # def send_dip_wipe_programs(self):
    #     robo.clean("kr30")
    #     self.dip_wipe_collection.send()
    #     self.water_collection.send()
    #     self.retardant_collection.send()

    # def publish_dip_wipe_programs(self):
    #     program_names = self.dip_wipe_collection.program_names()
    #     program_names += self.water_collection.program_names()
    #     program_names += self.retardant_collection.program_names()

    #     for program_name in program_names:
    #         self.save_program(self.directory, program_name)
    #     self.save_station(self.directory, "dw")


    def build_bot_program(self):
        self.init_progress()
        robo.clean("kr30")
        with utils.final_position(pm.PyNode("mainPaintingShape")):
            with utils.at_value(pm.PyNode("canvasTransform").attr("applyBrushBias"), True):
                with utils.at_value(pm.PyNode("brushLifter").attr("nodeState"), 0):
                    program = BotProgram(self.PROGRAM_NAME)
        if not (program and program.painting and program.painting.clusters):
            raise ValueError("Invalid bot_program. No painting/clusters")
        return program

    def send_and_publish_bot_program(self):
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

    def send_and_publish_pick_place_programs(self):

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
