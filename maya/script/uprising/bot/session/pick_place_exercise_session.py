
import logging
import os
import pymel.core as pm
import uprising.utils as uutl
import datetime
from uprising import robo

from uprising.common.session.session import Session

from uprising.bot.session.pick_place_exercise_program import PickPlaceExerciseProgram
from uprising.bot.session.pick_place_program import PickPlaceCollection

logger = logging.getLogger("uprising")


class PickPlaceExerciseSession(Session):

    PROGRAM_NAME = "pap"

    def __init__(self):
        with uutl.final_position(pm.PyNode("RACK1_CONTEXT")):
            self.pick_place_collection = PickPlaceCollection("all")
            self.exercise_program = PickPlaceExerciseProgram(self.PROGRAM_NAME)

    def send(self):
        robo.new()
        robo.clean()
        with uutl.final_position(pm.PyNode("RACK1_CONTEXT")):
            self.exercise_program.send()
            self.pick_place_collection.send()
            self.send_rack_geo()
            self.send_holder_geo()
        robo.show()

    def publish(self):
        program_name = self.exercise_program.program_name
        timestamp = datetime.datetime.now().strftime('%y%m%d_%H%M')
        directory = os.path.join(pm.workspace.getPath(
        ), 'export', 'calibrations', program_name, timestamp)

        src_fn = self.save_program(directory, program_name)

        program_names = self.pick_place_collection.program_names()
        for program_name in program_names:
            self.save_program(directory, program_name)

        self.insert_external_dependencies(program_names, src_fn)
        self.save_station(directory, program_name)