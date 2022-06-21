
import os
import pymel.core as pm
from uprising.session.handle_calibration_program import HandleCalibrationProgram
from uprising import robo
from uprising import utils
from uprising.common.session.session import Session
from uprising.bot.session.pick_place_program import PickPlaceCollection
import datetime


class HandleCalibrationSession(Session):

    PROGRAM_NAME = "handle"

    def __init__(self):
 
        robo.new()
        robo.clean("kr30", infrastructure=True)
        with utils.prep_for_output():
            self.pick_place_collection = PickPlaceCollection([0])
            self.program = HandleCalibrationProgram(self.PROGRAM_NAME)
 
    def run(self):
        with utils.prep_for_output():
            self.program.send()
            self.pick_place_collection.send()
            self.send_rack_geo()
            self.send_holder_geo()
            self.send_handle_geo()

        program_name = self.program.program_name
        timestamp = datetime.datetime.now().strftime('%y%m%d_%H%M')
        directory = os.path.join(pm.workspace.getPath(
        ), 'export', 'calibrations', program_name, timestamp)

        src_fn = self.save_program(directory, program_name)

        program_names = self.pick_place_collection.program_names()
        for program_name in program_names:
            self.save_program(directory, program_name)

        self.insert_external_dependencies(program_names, src_fn)
        self.save_station(directory, program_name)