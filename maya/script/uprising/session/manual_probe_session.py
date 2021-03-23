

import pymel.core as pm
import uprising.utils as uutl

from uprising.session.calibration_session import CalibrationSession

from uprising.session.manual_probe_program import ManualProbeProgram


class ManualProbeSession(CalibrationSession):

    PROGRAM_NAME = "rack"

    def __init__(self):
        super(ManualProbeSession, self).__init__()
        self.program = ManualProbeProgram(self.PROGRAM_NAME)

    def send_props(self):
        with uutl.final_position(pm.PyNode("mainPaintingShape")):
            with uutl.final_position(pm.PyNode("RACK1_CONTEXT")):
                self.send_rack_geo()
                self.send_board_geo()
