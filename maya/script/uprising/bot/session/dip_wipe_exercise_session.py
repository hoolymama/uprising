

# 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19

import logging
import os
import pymel.core as pm
import uprising.utils as uutl
import datetime
from uprising import robo

from uprising.common.session.session import Session

from uprising.bot.session.pick_place_program import PickPlaceCollection
from uprising.bot.session.dip_wipe_program import DipWipeExerciseCollection
from uprising.bot.session.dip_wipe_exercise_program import DipWipeExerciseProgram

logger = logging.getLogger("uprising")

class DipWipeExerciseSession(Session):

    PROGRAM_NAME = "dwex"

    def __init__(self, combinations, **kwargs):

        timestamp = datetime.datetime.now().strftime('%y%m%d_%H%M')
        self.directory = os.path.join(pm.workspace.getPath(
        ), 'export', 'calibrations', self.PROGRAM_NAME, timestamp)

        brush_ids = list(set([c["brush"] for c in combinations]))
        robo.clean("kr30")

        with uutl.final_position(pm.PyNode("RACK1_CONTEXT")):
            pm.displayInfo("Creating pick_place_collection")
            self.pick_place_collection = PickPlaceCollection(brush_ids)
            pm.displayInfo("Creating dip_wipe_ex_collection")
            self.dip_wipe_ex_collection = DipWipeExerciseCollection(combinations, **kwargs)
            pm.displayInfo("Creating exercise_program")
            self.exercise_program = DipWipeExerciseProgram(self.PROGRAM_NAME, combinations)


    def run(self):
        # configure dw collection programs
        for program in self.dip_wipe_ex_collection.programs:
            program.configure()

        with uutl.final_position(pm.PyNode("RACK1_CONTEXT")):
            # send programs
            pm.displayInfo("Sending dip_wipe_ex_collection")
            self.dip_wipe_ex_collection.send()
            pm.displayInfo("Sending exercise_program")
            self.exercise_program.send()
            pm.displayInfo("Sending pick_place_collection")
            self.pick_place_collection.send()

            # send geo
            pm.displayInfo("Sending rack_geo")
            self.send_rack_geo()
            pm.displayInfo("Sending holder_geo")
            self.send_holder_geo()
            pm.displayInfo("Sending pot_geo")
            self.send_pot_geo()
            pm.displayInfo("Sending handle_geo")
            self.send_handle_geo()

        self._publish()

    def _publish(self):
        # save the exercise program
        pm.displayInfo("Publish main_file")
        main_file = self.save_program(self.directory, self.PROGRAM_NAME)

        # save the sub programs
        program_names = self.pick_place_collection.program_names() + self.dip_wipe_ex_collection.program_names()
        for program_name in program_names:
            pm.displayInfo("Publish {}".format(program_name))
            self.save_program(self.directory, program_name)

        self.insert_external_dependencies(program_names, main_file)
        pm.displayInfo("Save_station {}".format(self.PROGRAM_NAME))
        self.save_station(self.directory, self.PROGRAM_NAME)


 