
from robolink import INSTRUCTION_INSERT_CODE

import pymel.core as pm
from uprising import robo
from uprising.common.session.program import Program
from uprising.pov.session.pov_painting import PovPainting

from uprising import progress

PAINTING_NAME = "lightPaintingShape"


class PovProgram(Program):
    def __init__(self, name, **kw):
        super(PovProgram, self).__init__(name)
        print "INITIALIZE PovProgram " 
        self.painting = PovPainting(pm.PyNode(PAINTING_NAME))
        
    def send(self, chunk_id, chunk_length):
        if not self.painting.strokes:
            return

        self.bump_program_name(chunk_id)
        super(PovProgram, self).send()

        self.frame = robo.create_frame("{}_frame".format(self.program_name))

        num_strokes = len(self.painting.strokes)
        start = chunk_id*chunk_length
        end = min((start+chunk_length), num_strokes)
        is_last_chunk = (end >= num_strokes)

        progress.update(
            minor_line="Writing {} strokes from {} to {}".format(
                self.program_name, start, end)
        )

        if chunk_id == 0:
            self.send_lights_off()
            self.send_shutter()

        self.painting.send_brushes()

        self.program.setRounding(self.painting.motion["rounding"])
       
        self.program.setSpeed(
            self.painting.motion["linear_speed"], 
            self.painting.motion["angular_speed"]
            )


        for stroke in self.painting.strokes[start:end]:
            prefix = "stk"
            stroke.send(prefix, self.program, self.frame, self.painting.motion)

        if is_last_chunk:
            self.send_shutter()
            self.program.addMoveJ(robo.home_approach)

        self.program.setParam("PostProcessor", "KUKA KRC4")  # Needed??

    def send_lights_off(self):
        """Trigger a lights-off instruction."""
        self.program.RunInstruction(
            "TRIGGER WHEN DISTANCE=0 DELAY=0 DO $ANOUT[1]=0.0", INSTRUCTION_INSERT_CODE)
        self.program.RunInstruction(
            "TRIGGER WHEN DISTANCE=0 DELAY=0 DO $ANOUT[2]=0.0", INSTRUCTION_INSERT_CODE)
        self.program.RunInstruction(
            "TRIGGER WHEN DISTANCE=0 DELAY=0 DO $ANOUT[3]=0.0", INSTRUCTION_INSERT_CODE)
        self.program.RunInstruction(
            "TRIGGER WHEN DISTANCE=0 DELAY=0 DO $ANOUT[4]=0.0", INSTRUCTION_INSERT_CODE)

    def send_shutter(self):
        """
        Send a shutter instruction.

        Shutter behaves like a toggle. If it's open, it closes, and vice versa. 
        """
        self.program.RunInstruction(
            "$OUT[1]=TRUE", INSTRUCTION_INSERT_CODE)
        self.program.RunInstruction(
            "WAIT SEC 1", INSTRUCTION_INSERT_CODE)
        self.program.RunInstruction(
            "$OUT[1]=FALSE", INSTRUCTION_INSERT_CODE)
