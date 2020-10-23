
import logging
import pymel.core as pm
from uprising import robo
from uprising.session.program import Program
from uprising import painting
from uprising import progress
from robolink import (
    INSTRUCTION_COMMENT
)
WATER_POT_ID = 9
WATER_REPEATS = 2

RETARDANT_POT_ID = 19


logger = logging.getLogger("uprising")


class DipWipeProgram(Program):
    @staticmethod
    def generate_program_name(paint_id, brush_id):
        return "p{:02d}_b{:02d}".format(paint_id, brush_id)

    def __init__(self, pack):
        name = DipWipeProgram.generate_program_name(
            pack["paint_id"], pack["brush_id"])

        super(DipWipeProgram, self).__init__(name)
        self.dip_painting = painting.Painting(pack["dip"])
        self.wipe_painting = painting.Painting(pack["wipe"])

    def _valid(self):
        return (self.dip_painting.clusters and self.wipe_painting.clusters)

    def send(self):

        if not (self._valid()):
            return

        super(DipWipeProgram, self).send()

        self.dip_painting.send_brushes()
        self.wipe_painting.send_brushes()

        self.program.RunInstruction(
            "Dip with tool %s" % self.dip_painting.clusters[0].brush.node_name,
            INSTRUCTION_COMMENT,
        )

        for cluster in self.dip_painting.clusters:
            cluster.send(
                self.program,
                robo.dips_frame,
                self.dip_painting.motion
            )

        for cluster in self.wipe_painting.clusters:
            cluster.send(
                self.program,
                robo.dips_frame,
                self.wipe_painting.motion
            )


class WashProgram(Program):

    @staticmethod
    def generate_program_name(brush_id):
        raise NotImplementedError

    def __init__(self, pack):
        name = self.generate_program_name(pack["brush_id"])
        super(WashProgram, self).__init__(name)
        self.dip_painting = painting.Painting(pack["dip"])
        self.wipe_painting = painting.Painting(pack["wipe"])
        # self.frame = None
        self.repeats = 0

    def _valid(self):
        return (self.dip_painting.clusters and self.wipe_painting.clusters)

    def send(self):
        if not self._valid():
            return
        super(WashProgram, self).send()

        self.dip_painting.send_brushes()
        self.wipe_painting.send_brushes()

        self.program.RunInstruction(
            "{} dip with tool {}".format(self.__class__.__name__[
                                         :-7], self.dip_painting.clusters[0].brush.node_name),
            INSTRUCTION_COMMENT,
        )

        for cluster in self.dip_painting.clusters:
            cluster.send(
                self.program,
                robo.wash_frame,
                self.dip_painting.motion
            )

        for repeat in range(self.repeats):
            for cluster in self.wipe_painting.clusters:
                cluster.send(
                    self.program,
                    robo.wash_frame,
                    self.wipe_painting.motion
                )


class WaterProgram(WashProgram):
    @staticmethod
    def generate_program_name(brush_id):
        return "water_b{:02d}".format(brush_id)

    def __init__(self, pack, repeats):
        super(WaterProgram, self).__init__(pack)
        self.repeats = repeats

    # def send(self):
    #     if not self._valid:
    #         return
    #     super(WaterProgram, self).send()


class RetardantProgram(WashProgram):

    @staticmethod
    def generate_program_name(brush_id):
        return "retardant_b{:02d}".format(brush_id)

    def __init__(self, pack):
        super(RetardantProgram, self).__init__(pack)
        # Dont wipe retardant off
        self.repeats = 0

    # def send(self):
    #     if not self._valid:
    #         return
    #     super(RetardantProgram, self).send()


class RackCollection(object):
    def __init__(self):
        self.painting_node = pm.PyNode("mainPaintingShape")
        self._resolve_combination_ids()
        self.packs = self.get_packs()
        self.programs = self._build()

    def get_packs(self):
        result = {}
        for combo in self.combination_ids:

            dip_ptg_path = "rack|holes|holeRot_{:02d}|holeTrans|dip_loc|b{:02d}|*".format(
                combo["paint"], combo["brush"]
            )
            wipe_ptg_path = "rack|holes|holeRot_{:02d}|holeTrans|wipe_loc|b{:02d}|*".format(
                combo["paint"], combo["brush"]
            )

            paint_key = "p{:02d}".format(combo["paint"])
            brush_key = "b{:02d}".format(combo["brush"])

            try:
                dip_ptg = pm.ls(dip_ptg_path, type="painting")[0]
                wipe_ptg = pm.ls(wipe_ptg_path, type="painting")[0]
            except IndexError:
                raise IndexError(
                    "Either dip or wipe node is missing: for {} {}".format(
                        paint_key, brush_key
                    )
                )

            if paint_key not in result:
                result[paint_key] = {}
            result[paint_key][brush_key] = {
                "dip": dip_ptg,
                "wipe": wipe_ptg,
                "paint_id": combo["paint"],
                "brush_id": combo["brush"],
                "name": "{}_{}".format(paint_key, brush_key),
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
            header="Creating {} {} programs".format(
                count, self.__class__.__name__),
            major_line="",
            major_max=count,
            major_progress=0
        )
        for i, program in enumerate(self.programs):
            program.send()
            progress.update(
                major_progress=(i+1),
                major_line="Wrote program {}  {:d}/{:d}".format(program.program_name, i+1, count))


class WaterCollection(RackCollection):

    def _resolve_combination_ids(self):
        combos = pm.paintingQuery(self.painting_node, dc=True) or []
        brush_ids = sorted(set(combos[::2]))
        self.combination_ids = [
            {"brush": int(brush_id), "paint": WATER_POT_ID} for brush_id in brush_ids]

    def _build(self):
        result = []
        for paint_id in self.packs:
            paint_pack = self.packs[paint_id]
            for brush_id in paint_pack:
                pack = paint_pack[brush_id]
                result.append(WaterProgram(pack, WATER_REPEATS))
        return result


class RetardantCollection(RackCollection):

    def _resolve_combination_ids(self):
        combos = pm.paintingQuery(self.painting_node, dc=True) or []
        brush_ids = sorted(set(combos[::2]))
        self.combination_ids = [
            {"brush": int(brush_id), "paint": RETARDANT_POT_ID} for brush_id in brush_ids]

    def _build(self):
        result = []
        for paint_id in self.packs:
            paint_pack = self.packs[paint_id]
            for brush_id in paint_pack:
                pack = paint_pack[brush_id]
                result.append(RetardantProgram(pack))
        return result


class DipWipeCollection(RackCollection):

    def _resolve_combination_ids(self):
        result = []
        combos = pm.paintingQuery(self.painting_node, dc=True) or []

        for i in range(0, len(combos), 2):
            result.append(
                {"brush": int(combos[i]), "paint": int(combos[i + 1])})
        self.combination_ids = result

    def _build(self):
        result = []
        for paint_id in self.packs:
            paint_pack = self.packs[paint_id]
            for brush_id in paint_pack:
                pack = paint_pack[brush_id]
                result.append(DipWipeProgram(pack))
        return result


class DipWipeExerciseCollection(RackCollection):

    def __init__(self, combination_ids):
        self.combination_ids = combination_ids
        super(DipWipeExerciseCollection, self).__init__()

    def _resolve_combination_ids(self):
        # Already resolved in constructor
        pass

    def _build(self):
        result = []
        for paint_id in self.packs:
            paint_pack = self.packs[paint_id]
            for brush_id in paint_pack:
                pack = paint_pack[brush_id]
                result.append(DipWipeProgram(pack))
        return result

        self.painting_node = pm.PyNode("mainPaintingShape")
        self._resolve_combination_ids()
        self.packs = self.get_packs()
        self.programs = self._build()
