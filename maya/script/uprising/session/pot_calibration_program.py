
import pymel.core as pm
import uprising.const as k
from uprising.session.calibration_program import CalibrationProgram
from robolink import (
    INSTRUCTION_COMMENT,
    INSTRUCTION_SHOW_MESSAGE
)
CAL_SHEET_FIRST_ROW = 6  # mm


class PotCalibrationProgram(CalibrationProgram):
    def __init__(self, name):
        super(PotCalibrationProgram, self).__init__(name)
        self.packs = self.get_packs()

    def send_locator_packs(self):
        for key in ["pot", "handle", "face"]:
            packs = self.packs[key]
            for pack in packs:
                self.program.RunInstruction(
                    "Moving to {}".format(pack["name"]), INSTRUCTION_COMMENT
                )
                self._send_one_probe(pack)

    def _send_one_probe(self, pack):

        base_target = self._create_a_target(
            pack["base"].attr("worldMatrix[0]").get(),
            "{}_base".format(pack["name"]),
            k.FACING_RACK_JOINTS,
        )

        approach_target = self._create_a_target(
            pack["approach"].attr("worldMatrix[0]").get(),
            "{}_approach".format(pack["name"]),
            k.FACING_RACK_JOINTS,
        )

        self.program.addMoveJ(approach_target)
        self.program.addMoveL(base_target)
        self.program.Pause()
        self.program.RunInstruction(
            "Enter {} in cell {}".format(pack["name"], pack["cell"]),
            INSTRUCTION_SHOW_MESSAGE,
        )
        self.program.addMoveL(approach_target)

    @staticmethod
    def get_packs():
        result = {"pot": [],  "handle": [],  "face": []}

        period = 20
        for i, p in enumerate(pm.ls("RACK1_CONTEXT|j1|rack|holes|holeRot*|holeTrans")):
            _1, _2, pot_base,  handle_base, face_base = p.getChildren(
                type="transform")

            result["pot"].append(
                {
                    "base": pot_base,
                    "approach": pot_base.getChildren(type="transform")[0],
                    "name": "pot_{:02d}".format(i),
                    "index": i,
                    "cell": "B:{}".format(k.CAL_SHEET_FIRST_ROW+i)
                }
            )
            result["handle"].append(
                {
                    "base": handle_base,
                    "approach": handle_base.getChildren(type="transform")[0],
                    "name": "handle_{:02d}".format(i),
                    "index": i,
                    "cell": "B:{}".format(k.CAL_SHEET_FIRST_ROW+i+period)
                }
            )

            result["face"].append(
                {
                    "base": face_base,
                    "approach": face_base.getChildren(type="transform")[0],
                    "name": "face_{:02d}".format(i),
                    "index": i,
                    "cell": "B:{}".format(k.CAL_SHEET_FIRST_ROW+i+period+period)
                }
            )

        return result
