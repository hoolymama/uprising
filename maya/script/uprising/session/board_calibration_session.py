

import pymel.core as pm
from uprising import utils

from uprising.session.calibration_session import CalibrationSession

from uprising.session.board_calibration_program import BoardCalibrationProgram


class BoardCalibrationSession(CalibrationSession):

    PROGRAM_NAME = "board"

    def __init__(self):
        super(BoardCalibrationSession, self).__init__()
        self.program = BoardCalibrationProgram(self.PROGRAM_NAME)

    def send_props(self):
        with utils.prep_for_output():
            self.send_board_geo()
