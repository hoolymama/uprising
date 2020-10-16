

from uprising.session.calibration_session import CalibrationSession
from uprising.session.pot_calibration_program import PotCalibrationProgram


class PotCalibrationSession(CalibrationSession):

    PROGRAM_NAME = "pot"

    def __init__(self):
        super(PotCalibrationSession, self).__init__()
        self.program = PotCalibrationProgram(self.PROGRAM_NAME)

    def send_props(self):
        self.send_rack_geo()
        self.send_handle_geo()
        self.send_pot_geo()
