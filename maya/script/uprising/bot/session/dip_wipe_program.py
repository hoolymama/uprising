import pymel.core as pm
from uprising import robo
from uprising import palette_utils  
from uprising.common.session.program import Program
from uprising.bot.session.bot_painting import BotPainting
from uprising import progress
from robolink import INSTRUCTION_COMMENT
from uprising.bot.session import configurator

WATER_POT_ID = 9
RETARDANT_POT_ID = 19
WATER_REPEATS = 2


class DipWipeProgram(Program):
    @staticmethod
    def generate_program_name(pot_id, brush_id):
        return "p{:02d}_b{:02d}".format(pot_id, brush_id)

    def __init__(self, pack, **kwargs):
        self.do_dip=kwargs.get("dip", True)
        self.do_wipe=kwargs.get("wipe", True)


        print "DipWipeProgram init pack:", pack
        name = DipWipeProgram.generate_program_name(pack["pot_id"], pack["brush_id"])

        super(DipWipeProgram, self).__init__(name)
        self.dip_painting = BotPainting(pack["dip"])
        self.wipe_painting = BotPainting(pack["wipe"])

    def configure(self):
        print "DipWipeProgram CONFIGURING dip_painting.clusters and wipe_painting.clusters"
        for cluster in self.dip_painting.clusters:
            for stroke in cluster.strokes:
                configurator.solve(stroke, cluster.brush)

            for cluster in self.wipe_painting.clusters:
                for stroke in cluster.strokes:
                    configurator.solve(stroke, cluster.brush)

    def send(self):

        if not (self.dip_painting.clusters and self.wipe_painting.clusters):
            return

        super(DipWipeProgram, self).send()
        self.dip_painting.send_brushes()
        self.wipe_painting.send_brushes()
        self.program.RunInstruction(
            "Dip with tool %s" % self.dip_painting.clusters[0].brush.node_name,
            INSTRUCTION_COMMENT,
        )
        if self.do_dip:
            for cluster in self.dip_painting.clusters:
                cluster.send(self.program, robo.dips_frame, self.dip_painting.motion)
        if self.do_wipe:
            for cluster in self.wipe_painting.clusters:
                cluster.send(self.program, robo.dips_frame, self.wipe_painting.motion)


class WashProgram(Program):
    @staticmethod
    def generate_program_name(brush_id):
        raise NotImplementedError

    def __init__(self, pack, **kwargs):
        self.do_dip=kwargs.get("dip", True)
        self.do_wipe=kwargs.get("wipe", True)

        name = self.generate_program_name(pack["brush_id"])
        super(WashProgram, self).__init__(name)
        self.dip_painting = BotPainting(pack["dip"])
        self.wipe_painting = BotPainting(pack["wipe"])
        self.repeats = 0

    def configure(self):
        for cluster in self.dip_painting.clusters + self.wipe_painting.clusters:
            for stroke in cluster.strokes:
                configurator.solve(stroke, cluster.brush)

    def _valid(self):
        return self.dip_painting.clusters and self.wipe_painting.clusters

    def send(self):
        if not self._valid():
            return
        super(WashProgram, self).send()

        self.dip_painting.send_brushes()
        self.wipe_painting.send_brushes()

        self.program.RunInstruction(
            "{} dip with tool {}".format(
                self.__class__.__name__[:-7], self.dip_painting.clusters[0].brush.node_name
            ),
            INSTRUCTION_COMMENT,
        )
        if self.do_dip:
            for cluster in self.dip_painting.clusters:
                cluster.send(self.program, robo.wash_frame, self.dip_painting.motion)
        if self.do_wipe:
            for repeat in range(self.repeats):
                for cluster in self.wipe_painting.clusters:
                    cluster.send(self.program, robo.wash_frame, self.wipe_painting.motion)


class WaterProgram(WashProgram):
    @staticmethod
    def generate_program_name(brush_id):
        return "water_b{:02d}".format(brush_id)

    def __init__(self, pack, repeats, **kwargs):
        super(WaterProgram, self).__init__(pack, **kwargs)
        self.repeats = repeats

class RetardantProgram(WashProgram):
    @staticmethod
    def generate_program_name(brush_id):
        return "retardant_b{:02d}".format(brush_id)

    def __init__(self, pack, **kwargs):
        super(RetardantProgram, self).__init__(pack, **kwargs)
        # Dont wipe retardant off
        self.repeats = 0

class RackCollection(object):
    def __init__(self):
        self.painting_node = pm.PyNode("mainPaintingShape")
        self._resolve_combination_ids()
        self.packs = self.get_packs()
        self.programs = self._build()

    def get_packs(self):
        result = {}
        for combo in self.combination_ids:

            brush_key = "b{:02d}".format(combo["brush"])
            pot_key = "p{:02d}".format(combo["pot"])

            try:
                dip_ptg = pm.PyNode(
                    "ptg_dip_b{:02d}_p{:02d}".format(combo["brush"], combo["pot"])
                ).getChildren()[0]
                wipe_ptg = pm.PyNode(
                    "ptg_wipe_b{:02d}_p{:02d}".format(combo["brush"], combo["pot"])
                ).getChildren()[0]
            except IndexError:
                raise IndexError(
                    "Either dip or wipe node is missing: for {} {}".format(pot_key, brush_key)
                )

            if pot_key not in result:
                result[pot_key] = {}
            result[pot_key][brush_key] = {
                "dip": dip_ptg,
                "wipe": wipe_ptg,
                "pot_id": combo["pot"],
                "brush_id": combo["brush"],
                "name": "{}_{}".format(pot_key, brush_key),
            }

        return result

    def _resolve_combination_ids(self):
        raise NotImplementedError

    def _build(self):
        raise NotImplementedError

    def program_names(self):
        return [program.program_name for program in self.programs]

    def send(self):
        count = len(self.programs)
        progress.update(
            header="Creating {} {} programs".format(count, self.__class__.__name__),
            major_line="",
            major_max=count,
            major_progress=0,
        )
        for i, program in enumerate(self.programs):
            program.send()
            progress.update(
                major_progress=(i + 1),
                major_line="Wrote program {}  {:d}/{:d}".format(program.program_name, i + 1, count),
            )


class WaterCollection(RackCollection):
    def _resolve_combination_ids(self):
        combos = pm.paintingQuery(self.painting_node, toolCombinations=True) or []
        brush_ids = sorted(set(combos[::3]))
        self.combination_ids = [
            {"brush": int(brush_id), "pot": WATER_POT_ID} for brush_id in brush_ids
        ]

    def _build(self):
        result = []
        for pot_id in self.packs:
            pot_pack = self.packs[pot_id]
            for brush_id in pot_pack:
                pack = pot_pack[brush_id]
                result.append(WaterProgram(pack, WATER_REPEATS))
        return result


class RetardantCollection(RackCollection):
    def _resolve_combination_ids(self):
        combos = pm.paintingQuery(self.painting_node, toolCombinations=True) or []
        brush_ids = sorted(set(combos[::3]))
        self.combination_ids = [
            {"brush": int(brush_id), "pot": RETARDANT_POT_ID} for brush_id in brush_ids
        ]

    def _build(self):
        result = []
        for pot_id in self.packs:
            pot_pack = self.packs[pot_id]
            for brush_id in pot_pack:
                pack = pot_pack[brush_id]
                result.append(RetardantProgram(pack))
        return result


class DipWipeCollection(RackCollection):

    def _resolve_combination_ids(self):
        self.combination_ids = palette_utils.brush_pot_combination_ids()

    def _build(self):
        result = []
        for pot_id in self.packs:
            pot_pack = self.packs[pot_id]
            for brush_id in pot_pack:
                pack = pot_pack[brush_id]
                result.append(DipWipeProgram(pack))
        return result


class DipWipeExerciseCollection(RackCollection):
    def __init__(self, combination_ids, **kwargs):
        self.kwargs = kwargs
        self.combination_ids = combination_ids
        super(DipWipeExerciseCollection, self).__init__()

    def _resolve_combination_ids(self):
        # Already resolved in constructor
        pass

    def _build(self):
        kwargs = self.kwargs
        result = []
        for pot_id in self.packs:
            pot_pack = self.packs[pot_id]
            for brush_id in pot_pack:
                pack = pot_pack[brush_id]
                result.append(DipWipeProgram(pack,**kwargs))
        return result
