
from uprising.common.session.session import Session
from uprising import robo
import datetime
from uprising.session.kr8_track_program import Kr8TrackProgram
import os
import pymel.core as pm


class Kr8TrackSession(Session):

    PROGRAM_NAME = "track"

    def __init__(self):
        super(Kr8TrackSession, self).__init__()
        self.program = Kr8TrackProgram(self.PROGRAM_NAME)

    def send(self):
        robo.new()
        robo.clean("kr8")
        robo.show()
        self.program.send()

    def publish(self):
        program_name = self.program.program_name
        timestamp = datetime.datetime.now().strftime('%y%m%d_%H%M')
        directory = os.path.join(pm.workspace.getPath(
        ), 'export', 'calibrations', program_name, timestamp)
        self.save_program(directory, program_name)
