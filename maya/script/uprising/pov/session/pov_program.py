import pymel.core as pm
from uprising import robo
from uprising.common.session.program import Program
from uprising.pov.session.pov_painting import PovPainting
from uprising.pov.session import pov_configurator, pov_lights
from robolink import PROGRAM_RUN_ON_ROBOT, RUNMODE_RUN_ROBOT
 




class PovProgram(Program):
    def __init__(self, name, run_on_robot, pause_at_end):
        super(PovProgram, self).__init__(name)
        print("INITIALIZE PovProgram")
        self.painting = PovPainting()
        print("GOT PovPainting")
        self.run_on_robot = run_on_robot
        self.pause_at_end = pause_at_end

    def configure(self):
        print("configure....")
        for stroke in self.painting.strokes:
            brush = self.painting.brushes.get(stroke.brush_id)
            if not brush:
                raise ("Invalid brush ID: {}".format(stroke.brush_id))

            try:
                pov_configurator.solve(stroke, brush)
            except BaseException:
                stroke.ignore = True
                pm.warning("IGNORING IMPOSSIBLE STROKE {}".format(stroke.name("stk")))
        print("DONE configure")

    def send(self, chunk_id, chunk_length, with_brush_geo=False):
        if not self.painting.strokes:
            return
        print(("SEND....", chunk_id))
        self.bump_program_name(chunk_id)
        super(PovProgram, self).send()
        
        self.frame = robo.create_frame("{}_frame".format(self.program_name))

        num_strokes = len(self.painting.strokes)
        start = chunk_id * chunk_length
        end = min((start + chunk_length), num_strokes)
        is_last_chunk = end >= num_strokes

        # progress.update(
        #     minor_line="Writing {} strokes from {} to {}".format(
        #         self.program_name, start, end)
        # )

        if chunk_id == 0:
            for brush in self.painting.brushes:
                dmx_id =  self.painting.brushes[brush].dmx_id
                
                pov_lights.send_lights_off(self.program, dmx_id, self.run_on_robot)
            pov_lights.send_shutter(self.program, self.run_on_robot)

        self.painting.send_brushes(with_brush_geo)

        if self.run_on_robot:
            robo.link().setRunMode(RUNMODE_RUN_ROBOT) 
            self.program.setRunType(PROGRAM_RUN_ON_ROBOT)

        self.program.setRounding(self.painting.rounding)
        self.program.setSpeed(self.painting.linear_speed)
        self.program.setSpeedJoints(self.painting.angular_speed)

        
        link = robo.link()
        # tool = link.Item(self.painting.brush.name)
        # self.program.setPoseTool(tool)

        prefix = "stk"
        for stroke in self.painting.strokes[start:end]:
            if not stroke.ignore:
                brush_name = self.painting.brushes.get(stroke.brush_id).name
                tool = link.Item(brush_name)
                self.program.setPoseTool(tool)
                stroke.send(prefix, self.program, self.frame, self.run_on_robot)

        if is_last_chunk:
            pov_lights.send_shutter(self.program, self.run_on_robot)
            self.program.addMoveJ(robo.home_approach)
            # To allow the camera to send the image to the computer, we wait some time before
            # starting another painting. -1 means indefinite pause.
            if self.pause_at_end != 0:
                self.program.Pause(self.pause_at_end)
