
 
from uprising import robo
from uprising import progress

from robolink import (
    COLLISION_OFF,
    INSTRUCTION_SHOW_MESSAGE,
    INSTRUCTION_INSERT_CODE
)

import logging

logger = logging.getLogger("uprising")


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

    def ensure_gripper_open(self):
        self.program.RunInstruction(
            "Gripper opens here", INSTRUCTION_SHOW_MESSAGE)
        self.program.RunInstruction("$OUT[2]=FALSE", INSTRUCTION_INSERT_CODE)
        self.program.RunInstruction("$OUT[1]=TRUE", INSTRUCTION_INSERT_CODE)
        self.program.RunInstruction(
            "WAIT FOR ($IN[2])", INSTRUCTION_INSERT_CODE)

