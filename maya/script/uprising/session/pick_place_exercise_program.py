from uprising import robo
from uprising.common.session.program import Program

from uprising.session.pick_place_program import PickProgram, PlaceProgram


from robolink import (
    INSTRUCTION_COMMENT,
    INSTRUCTION_SHOW_MESSAGE,
    INSTRUCTION_CALL_PROGRAM,
    INSTRUCTION_INSERT_CODE
)


class PickPlaceExerciseProgram(Program):
    def __init__(self, name):
        super(PickPlaceExerciseProgram, self).__init__(name)

    def send(self):

        super(PickPlaceExerciseProgram, self).send()
        self.frame = robo.create_frame("{}_frame".format(self.program_name))

        last_brush_id = None
        for brush_id in range(20):
            if last_brush_id is not None:

                ############################
                self.program.Pause()
                self.program.RunInstruction(
                    "About to swap brush {:02d} for {:02d}".format(
                        last_brush_id, brush_id
                    ),
                    INSTRUCTION_SHOW_MESSAGE,
                )

                # put the last brush back
                place_program_name = PlaceProgram.generate_program_name(
                    last_brush_id
                )
                self.program.RunInstruction(
                    place_program_name, INSTRUCTION_CALL_PROGRAM
                )
                ########################

            pick_program_name = PickProgram.generate_program_name(brush_id)
            self.program.RunInstruction(
                pick_program_name, INSTRUCTION_CALL_PROGRAM)

            last_brush_id = brush_id

        ###########################
        self.program.Pause()
        self.program.RunInstruction(
            "About to replace brush {:02d} and go home.".format(last_brush_id),
            INSTRUCTION_SHOW_MESSAGE,
        )

        place_program_name = PlaceProgram.generate_program_name(last_brush_id)
        self.program.RunInstruction(
            place_program_name, INSTRUCTION_CALL_PROGRAM)
        ###########################

        self.program.addMoveJ(robo.home_approach)
