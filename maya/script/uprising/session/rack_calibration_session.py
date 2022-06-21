from uprising import utils

from uprising.session.calibration_session import CalibrationSession

from uprising.session.rack_calibration_program import RackCalibrationProgram


class RackCalibrationSession(CalibrationSession):

    PROGRAM_NAME = "rack"

    def __init__(self):
        super(RackCalibrationSession, self).__init__()
        with utils.prep_for_output():
            self.program = RackCalibrationProgram(self.PROGRAM_NAME)

    def send_props(self):
        with utils.prep_for_output():
            self.send_rack_geo()