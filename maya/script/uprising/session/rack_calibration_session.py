
from uprising.session.calibration_session import CalibrationSession

from uprising.session.rack_calibration_program import RackCalibrationProgram


class RackCalibrationSession(CalibrationSession):

    PROGRAM_NAME = "rack"

    def __init__(self):
        super(RackCalibrationSession, self).__init__()
        self.program = RackCalibrationProgram(self.PROGRAM_NAME)

    def send_props(self):
        self.send_rack_geo()
