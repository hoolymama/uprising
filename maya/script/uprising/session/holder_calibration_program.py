
import pymel.core as pm
import uprising.const as k
from uprising.session.calibration_program import CalibrationProgram
from uprising import utils
from uprising.bot.session.pick_place_program import PickPlaceCollection
from robolink import (
    INSTRUCTION_COMMENT,
    INSTRUCTION_SHOW_MESSAGE
)

RACK_HOLDER_HEIGHT = 23.0
RACK_HOLDER_DISTANCE = 164.0
CAL_SHEET_FIRST_ROW = 6


class HolderCalibrationProgram(CalibrationProgram):
    def __init__(self, name):
        super(HolderCalibrationProgram, self).__init__(name)
        brush_ids = [int(n[-2:]) for n in pm.ls("RACK1_CONTEXT|j1|rack|holders|holderRot*")]
        self.packs = PickPlaceCollection(brush_ids).packs

    def send_locator_packs(self):
        bids = sorted(self.packs.keys())
        for bid in bids:
            pack = self.packs[bid]
            with utils.at_position(
                pack["trans_node"], RACK_HOLDER_DISTANCE, 0, RACK_HOLDER_HEIGHT
            ):
                self._send_one_probe(self.packs[bid])

    def _send_one_probe(self, pack):

        self.program.RunInstruction(
            "Moving to holder {}".format(pack["brush_id"]), INSTRUCTION_COMMENT
        )

        row = CAL_SHEET_FIRST_ROW + int(pack["brush_id"])

        base_target = self._create_a_target(
            pack["probe"].attr("worldMatrix[0]").get(),
            "base_{:02d}".format(pack["brush_id"]),
            k.FACING_RACK_JOINTS,
        )

        approach_target = self._create_a_target(
            pack["pin_ap"].attr("worldMatrix[0]").get(),
            "approach_{:02d}".format(pack["brush_id"]),
            k.FACING_RACK_JOINTS,
        )

        self.program.addMoveJ(approach_target)
        self.program.addMoveL(base_target)
        self.program.RunInstruction(
            "Enter {} in cell {}D +EF by eye".format(
                pack["brush_id"], row
            ),
            INSTRUCTION_SHOW_MESSAGE,
        )
        self.program.Pause()
        self.program.addMoveL(approach_target)
