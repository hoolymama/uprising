
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

        timestamp = datetime.datetime.now().strftime('%y%m%d_%H%M')
        self.directory = os.path.join(pm.workspace.getPath(
        ), 'export', 'calibrations', self.PROGRAM_NAME, timestamp)
        robo.clean("kr30")
        with uutl.final_position(pm.PyNode("RACK1_CONTEXT")):
            brush_ids = [int(n[-2:]) for n in pm.ls("RACK1_CONTEXT|j1|rack|holders|holderRot*")]
            pm.displayInfo("Creating pick_place_collection")
            self.pick_place_collection = PickPlaceCollection(brush_ids)
            pm.displayInfo("Creating exercise_program")
            self.exercise_program = PickPlaceExerciseProgram(self.PROGRAM_NAME)

    def run(self):
        with uutl.final_position(pm.PyNode("RACK1_CONTEXT")):
            self.pick_place_collection.send()
            self.exercise_program.send()
            self.send_rack_geo()
            self.send_holder_geo() 

        pm.displayInfo("Publish main_file")
        main_file = self.save_program(self.directory, self.PROGRAM_NAME)

        program_names = self.pick_place_collection.program_names()

        for program_name in program_names:
            pm.displayInfo("Publish {}".format(program_name))
            self.save_program(self.directory, program_name)

        self.insert_external_dependencies(program_names, main_file)
        pm.displayInfo("Save_station {}".format(self.PROGRAM_NAME))
        self.save_station(self.directory, self.PROGRAM_NAME)
 