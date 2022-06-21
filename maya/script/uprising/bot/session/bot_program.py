from uprising.bot.session.bot_painting import BotPainting
import pymel.core as pm
from uprising import robo
from uprising.common.session.program import Program
from uprising.bot.session.pick_place_program import PickProgram, PlaceProgram
from uprising.bot.session.dip_wipe_program import DipWipeProgram, WaterProgram, RetardantProgram

from uprising import progress
from uprising.bot.session import configurator
from uprising import const as k
from robolink import (
    INSTRUCTION_COMMENT,
    INSTRUCTION_CALL_PROGRAM,
    INSTRUCTION_INSERT_CODE,
    INSTRUCTION_SHOW_MESSAGE,
)


def _log_line(*vals):
    return "{}\n".format("\t".join(vals))


class BotProgram(Program):
    def __init__(self, name, **kw):
        super(BotProgram, self).__init__(name)
        self.painting = BotPainting()
        self.event_log = _log_line(
            "ClusterId", "Red", "Green", "Blue", "", "Brush Id", "Paint Id", "Pot Id"
        )

    def configure(self):
        for cluster in self.painting.clusters:
            for stroke in cluster.strokes:
                try:
                    configurator.solve(stroke, cluster.brush)
                except BaseException:
                    stroke.ignore = True

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

        self.program.setRounding(self.painting.rounding)
        self.program.setSpeed(self.painting.linear_speed)
        self.program.setSpeedJoints(self.painting.angular_speed)

        last_cluster = None if start == 0 else self.painting.clusters[(start - 1)]

        if is_first_chunk:
            self.send_open_gripper()

        sub_programs = self._send_program_body(start, end, last_cluster, is_last_chunk)
        return sub_programs

    def _send_program_body(self, start, end, last_cluster, is_last_chunk):
        subprograms = set()
        cluster = None

        for cluster in self.painting.clusters[start:end]:

            (
                did_change_pot,
                did_change_paint,
                did_change_tool,
                did_change_brush,
                did_end_last_brush,
                did_change_layer,
            ) = cluster.get_flow_info(last_cluster)

            # Add an Identifier
            if did_change_pot or did_change_brush:
                self.push_to_event_log(cluster)

            num_dips = 1

            if (did_change_layer and did_change_pot) or did_change_brush:
                subprograms |= self._on_end_tool(last_cluster)
                self._add_halts(cluster)
                subprograms |= self._on_start_tool(cluster)
                num_dips = max(cluster.brush.initial_dips, 1)

            subprograms |= self._write_dip_and_cluster(cluster, num_dips)

            if did_change_tool:
                last_cluster = cluster

        if is_last_chunk and cluster:
            subprograms |= self._on_end_tool(cluster)
            self.program.addMoveJ(robo.home_approach)

        return sorted(list(subprograms))

    def _add_halts(self, cluster):
        brush = cluster.brush

        wait = brush.initial_wait
        if wait > 0:
            self.program.RunInstruction(
                "Program waiting - Brush:{} Pot:{}".format(brush.id, cluster.pot_id),
                INSTRUCTION_COMMENT,
            )
            self.program.RunInstruction("WAIT SEC {:d}".format(wait), INSTRUCTION_INSERT_CODE)
        elif wait == -1:
            self.program.RunInstruction(
                "Program halted for Brush:{} pot:{}".format(brush.id, cluster.pot_id),
                INSTRUCTION_COMMENT,
            )
            self.program.RunInstruction("HALT", INSTRUCTION_INSERT_CODE)

    def _on_start_tool(self, cluster):
        result = set()
        brush = cluster.brush
        bid = brush.id

        pick_program_name = PickProgram.generate_program_name(bid)

        self.program.RunInstruction(pick_program_name, INSTRUCTION_CALL_PROGRAM)
        result.add(pick_program_name)
        # Initial Wait

        # Initial Water
        if brush.initial_water:
            water_program_name = WaterProgram.generate_program_name(bid)

            self.program.RunInstruction(water_program_name, INSTRUCTION_CALL_PROGRAM)
            result.add(water_program_name)
        return result

    def _on_end_tool(self, cluster):
        result = set()
        if not cluster:
            return result
        bid = cluster.brush.id
        if cluster.brush.retardant:
            retardant_program_name = RetardantProgram.generate_program_name(bid)

            self.program.RunInstruction(retardant_program_name, INSTRUCTION_CALL_PROGRAM)
            result.add(retardant_program_name)

        place_program_name = PlaceProgram.generate_program_name(bid)

        self.program.RunInstruction(place_program_name, INSTRUCTION_CALL_PROGRAM)
        result.add(place_program_name)
        return result

    def _write_dip_and_cluster(self, cluster, num_dips):
        result = set()
        dip_program_name = DipWipeProgram.generate_program_name(cluster.pot_id, cluster.brush.id)

        for repeat in range(num_dips):
            self.program.RunInstruction(dip_program_name, INSTRUCTION_CALL_PROGRAM)
        result.add(dip_program_name)

        self.program.addMoveJ(robo.dip_approach)
        cluster.send(self.program, self.frame)
        self.program.addMoveJ(robo.dip_approach)
        return result

    def push_to_event_log(self, cluster):
        paint = cluster.paint
        clusterid = "Cluster:{:06d}".format(cluster.id)
        brushid = "{:02d}".format(cluster.brush.id)
        paintid = "{:02d}".format(paint.id)
        potid = "{:02d}".format(cluster.pot_id)
        self.program.RunInstruction(
            "{} BrushId:{} PaintId:{} PotId:{}".format(clusterid, brushid, paintid, potid),
            INSTRUCTION_COMMENT,
        )
        self.event_log += _log_line(
            clusterid,
            "{:d}".format(int(paint.color[0] * 255)),
            "{:d}".format(int(paint.color[1] * 255)),
            "{:d}".format(int(paint.color[2] * 255)),
            "",
            brushid,
            paintid,
            potid,
        )


class BotRetryProgram(Program):
    def __init__(self, name):
        super(BotRetryProgram, self).__init__(name)
        self.painting = BotPainting(pm.PyNode(k.PAINTING_NAME))

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
            cluster.send(self.program, self.frame)

    def send_open_gripper(self):
        self.program.RunInstruction("Gripper opens here", INSTRUCTION_SHOW_MESSAGE)
        self.program.RunInstruction("$OUT[2]=FALSE", INSTRUCTION_INSERT_CODE)
        self.program.RunInstruction("$OUT[1]=TRUE", INSTRUCTION_INSERT_CODE)
        self.program.RunInstruction("WAIT FOR ($IN[2])", INSTRUCTION_INSERT_CODE)
