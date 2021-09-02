from uprising.bot.session.bot_painting import BotPainting
import pymel.core as pm
from uprising import robo
from uprising.common.session.program import Program
from uprising.bot.session.pick_place_program import PickProgram, PlaceProgram
from uprising.bot.session.dip_wipe_program import DipWipeProgram, WaterProgram, RetardantProgram

from uprising import progress
from uprising.bot.session import configurator

from robolink import INSTRUCTION_COMMENT, INSTRUCTION_CALL_PROGRAM, INSTRUCTION_INSERT_CODE

PAINTING_NAME = "mainPaintingShape"


class BotProgram(Program):
    def __init__(self, name, **kw):
        super(BotProgram, self).__init__(name)
        self.painting = BotPainting(pm.PyNode(PAINTING_NAME))

    def configure(self):
        for cluster in self.painting.clusters:
            for stroke in cluster.strokes:
                try:
                    configurator.solve(stroke, cluster.brush)
                except BaseException:
                    stroke.ignore=True

    def send(self, **kw):
        if not self.painting.clusters:
            return
        num_clusters = len(self.painting.clusters)
        chunk_id = kw.get("chunk_id", 0)

        chunk_length = kw.get("chunk_length", num_clusters)

        with_brush_geo = kw.get("with_brush_geo", False)


        start = kw.get("chunk_start", (chunk_id * chunk_length))

        end = start + chunk_length
        end = min(end, num_clusters)
        is_last_chunk = end >= num_clusters
        is_first_chunk = chunk_id == 0

        self.bump_program_name(chunk_id)

        progress.update(
            minor_line="Writing {} clusters from {} to {}".format(self.program_name, start, end)
        )

        super(BotProgram, self).send()

        self.frame = robo.create_frame("{}_frame".format(self.program_name))

        self.painting.send_brushes(with_brush_geo)

        last_cluster = None if start == 0 else self.painting.clusters[(start - 1)]

        if is_first_chunk:
            self.send_open_gripper()

        sub_programs = self._send_program_body(start, end, last_cluster, is_last_chunk)
        return sub_programs

    def _send_program_body(self, start, end, last_cluster, is_last_chunk):
        subprograms = set()
        cluster = None

        for cluster in self.painting.clusters[start:end]:

            did_change_paint, did_change_tool, did_change_brush, did_end_last_brush = cluster.get_flow_info(
                last_cluster
            )

            num_dips = 1

            if did_end_last_brush:
                subprograms |= self._on_end_tool(last_cluster)

            if did_change_brush:
                subprograms |= self._on_start_tool(cluster)

            if did_change_paint or did_change_brush:
                num_dips = max(cluster.brush.initial_dips, 1)


            subprograms |= self._write_dip_and_cluster(cluster, num_dips)

            if did_change_tool:
                last_cluster = cluster

        if is_last_chunk and cluster:
            subprograms |= self._on_end_tool(cluster)
            self.program.addMoveJ(robo.home_approach)

        return sorted(list(subprograms))

    def _on_start_tool(self, cluster, call_program=True):
        result = set()
        brush = cluster.brush
        bid = brush.id

        if call_program:
            wait = brush.initial_wait
            if wait > 0:
                self.program.RunInstruction("WAIT SEC {:d}".format(wait), INSTRUCTION_INSERT_CODE)
            elif wait == -1:
                self.program.RunInstruction(
                    "Program halted for {}".format(brush.node_name),
                    INSTRUCTION_COMMENT,
                )
                self.program.RunInstruction("HALT", INSTRUCTION_INSERT_CODE)

        pick_program_name = PickProgram.generate_program_name(bid)
        if call_program:
            self.program.RunInstruction(pick_program_name, INSTRUCTION_CALL_PROGRAM)
        result.add(pick_program_name)
        # Initial Wait

        # Initial Water
        if brush.initial_water:
            water_program_name = WaterProgram.generate_program_name(bid)
            if call_program:
                self.program.RunInstruction(water_program_name, INSTRUCTION_CALL_PROGRAM)
            result.add(water_program_name)
        return result

    def _on_end_tool(self, cluster, call_program=True):
        result = set()
        bid = cluster.brush.id
        if cluster.brush.retardant:
            retardant_program_name = RetardantProgram.generate_program_name(bid)
            if call_program:
                self.program.RunInstruction(retardant_program_name, INSTRUCTION_CALL_PROGRAM)
            result.add(retardant_program_name)

        place_program_name = PlaceProgram.generate_program_name(bid)
        if call_program:
            self.program.RunInstruction(place_program_name, INSTRUCTION_CALL_PROGRAM)
        result.add(place_program_name)
        return result

    def _write_dip_and_cluster(self, cluster, num_dips, call_program=True):
        result = set()
        dip_program_name = DipWipeProgram.generate_program_name(cluster.paint.id, cluster.brush.id)
        if call_program:
            for repeat in range(num_dips):
                self.program.RunInstruction(dip_program_name, INSTRUCTION_CALL_PROGRAM)
        result.add(dip_program_name)

        if call_program:
            self.program.addMoveJ(robo.dip_approach)
            cluster.send(self.program, self.frame, self.painting.motion)
            self.program.addMoveJ(robo.dip_approach)
        return result


class BotRetryProgram(Program):
    def __init__(self, name):
        super(BotRetryProgram, self).__init__(name)
        self.painting = BotPainting(pm.PyNode(PAINTING_NAME))

    def configure(self):
        for cluster in self.painting.clusters:
            for stroke in cluster.strokes:
                # stroke.configure(cluster.brush)
                configurator.solve(stroke, cluster.brush)

    def send(self, **kw):
        if not self.painting.clusters:
            return
        num_clusters = len(self.painting.clusters)

        progress.update(
            minor_line="Sending {} clusters for {}".format(num_clusters, self.program_name)
        )

        super(BotRetryProgram, self).send()
        self.frame = robo.create_frame("{}_frame".format(self.program_name))
        self.painting.send_brushes()

        for cluster in self.painting.clusters:
            # cluster.brush.send()
            cluster.send(self.program, self.frame, self.painting.motion)
