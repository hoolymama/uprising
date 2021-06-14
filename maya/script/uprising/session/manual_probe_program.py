
from uprising.brush import Brush
import uprising.utils as uutl
import pymel.core as pm
from uprising.session.calibration_program import CalibrationProgram
from robolink import (
    INSTRUCTION_COMMENT
)


class ManualProbeProgram(CalibrationProgram):

    def _get_probe_brush(self):
        geo = pm.PyNode("bpx_0_utility_B0_probe_roundShape")
        with uutl.at_value(geo.attr("paintingParam"), 0.0):
            tool = Brush(0, geo.attr("outPaintBrush"))
        return tool

    def send_locator_packs(self):
        self.program.Pause()
        self.program.RunInstruction(
            "Manually record the 3 leg centers with the brush tip", INSTRUCTION_COMMENT
        )
