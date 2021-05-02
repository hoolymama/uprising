

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

    def __init__(self, combinations):

        brush_ids = list(set([c["brush"] for c in combinations]))
        robo.clean("kr30")
        with uutl.final_position(pm.PyNode("RACK1_CONTEXT")):
            self.pick_place_collection = PickPlaceCollection(brush_ids)
            self.dip_wipe_ex_collection = DipWipeExerciseCollection(combinations)
            self.exercise_program = DipWipeExerciseProgram(self.PROGRAM_NAME, combinations)

    def configure(self):
        print self.dip_wipe_ex_collection.programs
        for program in self.dip_wipe_ex_collection.programs:
            program.configure()

    def send(self):
        with uutl.final_position(pm.PyNode("RACK1_CONTEXT")):
            self.dip_wipe_ex_collection.send()
            self.exercise_program.send()
            self.pick_place_collection.send()

            self.send_rack_geo()
            self.send_holder_geo()
            self.send_pot_geo()
            self.send_handle_geo()

    def publish(self):
        program_name = self.exercise_program.program_name
        timestamp = datetime.datetime.now().strftime('%y%m%d_%H%M')
        directory = os.path.join(pm.workspace.getPath(
        ), 'export', 'calibrations', program_name, timestamp)

        src_fn = self.save_program(directory, program_name)

        program_names = self.pick_place_collection.program_names() + self.dip_wipe_ex_collection.program_names()
        for program_name in program_names:
            self.save_program(directory, program_name)

        self.insert_external_dependencies(program_names, src_fn)
        self.save_station(directory, program_name)


 