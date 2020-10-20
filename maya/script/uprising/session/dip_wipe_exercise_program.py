from uprising import robo
from uprising.session.program import Program

from uprising.session.pick_place_program import PickProgram, PlaceProgram
from uprising.session.dip_wipe_program import DipWipeProgram

from robolink import (
    INSTRUCTION_CALL_PROGRAM
)


class DipWipeExerciseProgram(Program):
    def __init__(self, name, combinations):
        super(DipWipeExerciseProgram, self).__init__(name)
        self.combinations = combinations

    def send(self):

        super(DipWipeExerciseProgram, self).send()
        self.frame = robo.create_frame("{}_frame".format(self.program_name))

        last_brush_id = None

        brush_ids = list(set([b["brush"] for b in self.combinations]))

        for brush_id in brush_ids:
            if last_brush_id is not None:

                # put the last brush back
                place_program_name = PlaceProgram.generate_program_name(
                    last_brush_id
                )
                self.program.RunInstruction(
                    place_program_name, INSTRUCTION_CALL_PROGRAM
                )

            pick_program_name = PickProgram.generate_program_name(brush_id)
            self.program.RunInstruction(
                pick_program_name, INSTRUCTION_CALL_PROGRAM)

            last_brush_id = brush_id

            for paint_id in [p["paint"] for p in self.combinations if p["brush"] == brush_id]:
                dw_program_name = DipWipeProgram.generate_program_name(
                    paint_id, brush_id
                )
                self.program.RunInstruction(
                    dw_program_name, INSTRUCTION_CALL_PROGRAM)

        place_program_name = PlaceProgram.generate_program_name(last_brush_id)
        self.program.RunInstruction(
            place_program_name, INSTRUCTION_CALL_PROGRAM)

        self.program.addMoveJ(robo.home_approach)
        # robo.show()
