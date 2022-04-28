
from uprising.common.session.target import Target
from uprising.pov.session import  pov_lights

from uprising import robo
EPSILON = 0.00001

class PovTarget(Target):
    def __init__(self, target_id, position, rotation, color, wait):
        super(PovTarget, self).__init__(target_id, position, rotation)
        self.color = color
        self.wait = wait
        self.linear = True

    def send(self, prefix, program, frame, dmx_id, last_color, run_on_robot):
        link = robo.link()
        robot = robo.robot()
        target_name = self.name(prefix)
        rdk_target = link.AddTarget(target_name, frame, robot)
        rdk_target.setPose(self.tool_pose)
        rdk_target.setJoints(self.joint_pose)

        pov_lights.send_lights(program, dmx_id, run_on_robot, self.color, last_color)
        if self.linear:
            program.addMoveL(rdk_target)
        else:
            program.addMoveJ(rdk_target)

        self.send_wait(program)
 
    def send_wait(self, program):
        if self.wait < EPSILON:
            return
        program.Pause(int(self.wait*1000))
