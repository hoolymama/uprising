
 
from uprising import robo
from uprising import progress

from robolink import  COLLISION_OFF

 
class ProgramError(Exception):
    pass


class Program(object):
    def __init__(self, name):
        self.program_name = name
        self.program_name_prefix = name
        self.program = None

    def send(self):
        self.program = robo.create_program(self.program_name)
        self.program.ShowInstructions(False)

    def bump_program_name(self, suffix):
        self.program_name = "{}_{:02d}".format(
            self.program_name_prefix, suffix)

    def validate_path(self):
        if self.program:
            progress.update(minor_line="Validating path")

            update_result = self.program.Update(COLLISION_OFF)
            return {
                "name": self.program_name,
                "instructions": update_result[0],
                "time": update_result[1],
                "distance": update_result[2],
                "completion": update_result[3],
                "problems": update_result[4],
                "status": "SUCCESS" if (update_result[3] == 1.0) else "FAILURE",
            }

