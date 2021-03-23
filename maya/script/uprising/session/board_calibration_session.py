

import pymel.core as pm
import uprising.utils as uutl

from uprising.session.calibration_session import CalibrationSession

from uprising.session.board_calibration_program import BoardCalibrationProgram


class BoardCalibrationSession(CalibrationSession):

    PROGRAM_NAME = "board"

    def __init__(self):
        super(BoardCalibrationSession, self).__init__()
        self.program = BoardCalibrationProgram(self.PROGRAM_NAME)

    def send_props(self):
        with uutl.final_position(pm.PyNode("mainPaintingShape")):
            self.send_board_geo()
