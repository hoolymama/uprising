
import pymel.core as pm
import uprising.const as k
from uprising.session.calibration_program import CalibrationProgram
from robolink import (
    INSTRUCTION_COMMENT,
    INSTRUCTION_SHOW_MESSAGE
)
CAL_SHEET_FIRST_ROW = 6  # mm


class RackCalibrationProgram(CalibrationProgram):
    def __init__(self, name):
        super(RackCalibrationProgram, self).__init__(name)
        self.packs = self.get_packs()

    def send_locator_packs(self):
        for pack in self.packs:
            self._send_one_probe(pack)

    def _send_one_probe(self, pack):

        self.program.RunInstruction(
            "Moving to probe {} {}".format(pack["name"], pack["index"]),
            INSTRUCTION_COMMENT,
        )

        row = CAL_SHEET_FIRST_ROW + int(pack["index"])

        base_target = self._create_a_target(
            pack["base"].attr("worldMatrix[0]").get(),
            "base_{}".format(pack["name"]),
            k.FACING_RACK_JOINTS,
        )

        approach_target = self._create_a_target(
            pack["approach"].attr("worldMatrix[0]").get(),
            "approach_{}".format(pack["name"]),
            k.FACING_RACK_JOINTS,
        )

        self.program.addMoveJ(approach_target)
        self.program.addMoveL(base_target)
        self.program.RunInstruction(
            "Enter {} in cell G{}".format(pack["name"], row),
            INSTRUCTION_SHOW_MESSAGE,
        )
        self.program.Pause()
        self.program.addMoveL(approach_target)

    @staticmethod
    def get_packs():
        result = []

        for i, p in enumerate(
            pm.ls("RACK1_CONTEXT|j1|rack|probes|rackCalRot*|rackCalLocal")
        ):
            if p.attr("v").get() and p.getParent().attr("v").get():
                approach, base = [x for x in p.getChildren(type="transform")]
                name = base.split("|")[0]
                result.append(
                    {"base": base, "approach": approach, "name": name, "index": i}
                )
        return result
