
import logging
import os
import pymel.core as pm
import datetime
from uprising import robo

from uprising.common.session.session import Session


from uprising.session.pick_place_program import PickPlaceCollection

logger = logging.getLogger("uprising")


class CalibrationSession(Session):

    PROGRAM_NAME = "program_name"

    def __init__(self):

        self.probe_grab_drop = PickPlaceCollection("calibration")
        self.program = None

    def send(self):
        robo.new()
        # robo.hide()
        robo.clean("kr30")
        self.program.send()
        self.probe_grab_drop.send()
        self.send_props()
        robo.show()

    def publish(self):
        program_name = self.program.program_name
        timestamp = datetime.datetime.now().strftime('%y%m%d_%H%M')
        directory = os.path.join(pm.workspace.getPath(
        ), 'export', 'calibrations', program_name, timestamp)

        src_fn = self.save_program(directory, program_name)

        program_names = self.probe_grab_drop.program_names()
        for program_name in program_names:
            self.save_program(directory, program_name)

        self.insert_external_dependencies(program_names, src_fn)
