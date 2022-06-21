

import pymel.core as pm
from uprising import utils

from uprising.session.calibration_session import CalibrationSession

from uprising.session.holder_calibration_program import HolderCalibrationProgram


class HolderCalibrationSession(CalibrationSession):

    PROGRAM_NAME = "holder"

    def __init__(self):
        super(HolderCalibrationSession, self).__init__()
        with utils.prep_for_output():
            self.program = HolderCalibrationProgram(self.PROGRAM_NAME)

    def send_props(self):
        with utils.prep_for_output():
            self.send_rack_geo()
            self.send_holder_geo()
