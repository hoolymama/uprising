
import logging
import brush_utils as butl
import const as k
import palette_utils as putl
import props
import pymel.core as pm
import uprising_util as uutl
from brush import Brush
from paint import Paint
import session
import write

from program import ( PickProgram, PlaceProgram)
 
logger = logging.getLogger("uprising")

class PickPlaceExercise(session.Session):

    PROGRAM_NAME = "pap"

    def __init__(self, publish=False):
        robo.new()

        self.pick_place_programs = session.build_all_pick_place_programs()
        self.exercise_program = PapExerciseProgram(self.PROGRAM_NAME)
        self.send()
        if publish:
            self.publish()

    def send(self):
        with uutl.final_position(rack_context):
            session.send_rack_geo()
            session.send_holder_geo()
        
        self.exercise_program.write()
        for prog in self.pick_place_programs:
            prog.write()

    def publish(self):
        timestamp = datetime.datetime.now().strftime('%y%m%d_%H%M')
        directory=os.path.join( pm.workspace.getPath(), 'export', 'calibrations', self.PROGRAM_NAME, timestamp)

        src_fn,_ = write.save_prog_and_station(directory, self.PROGRAM_NAME)

        program_names = []
        for prog in self.pick_place_programs:
            program_names.append( prog.program_name)
            write.save_prog_and_station(directory, prog.program_name)

        write.insert_external_dependencies(program_names, src_fn)