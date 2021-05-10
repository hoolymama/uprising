
import logging
import os
import pymel.core as pm
import uprising.utils as uutl
import datetime
from uprising import robo

from uprising.common.session.session import Session

from uprising.bot.session.pick_place_program import PickPlaceCollection
from uprising.session.brush_hang_program import BrushHangProgram


logger = logging.getLogger("uprising")


class BrushHangSession(Session):

    PROGRAM_NAME = "hang"

    def __init__(self, data):

        brush_ids = [b["id"] for b in data]

        robo.new()
        robo.clean("kr30")
        with uutl.prep_for_output():
            self.pick_place_collection = PickPlaceCollection(brush_ids)
            self.program = BrushHangProgram(self.PROGRAM_NAME, data)

    def send(self):

        with uutl.prep_for_output():
            self.program.send()
            self.pick_place_collection.send()
            self.send_rack_geo()
            self.send_holder_geo()

    def publish(self):
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
