
from robolink import INSTRUCTION_INSERT_CODE
from uprising.common.session.target import Target
from uprising import robo


class PovTarget(Target):
    def __init__(self, target_id, position, rotation, color):
        super(PovTarget, self).__init__(target_id, position, rotation)
        self.color = color*10

    def send(self, prefix, program, frame, last_color):
        link = robo.link()
        robot = robo.robot()
        target_name = self.name(prefix)
        rdk_target = link.AddTarget(target_name, frame, robot)
        rdk_target.setPose(self.tool_pose)
        rdk_target.setJoints(self.joint_pose)

        if self.linear:
            program.addMoveL(rdk_target)
        else:
            program.addMoveJ(rdk_target)
        self.send_color(program, last_color=last_color)

    def send_color(self, program, last_color=None):
        if not last_color or last_color.r != self.color.r:
            program.RunInstruction("TRIGGER WHEN DISTANCE=0 DELAY=0 DO $ANOUT[1]={:.3f}".format(
                self.color.r), INSTRUCTION_INSERT_CODE)

        if not last_color or last_color.g != self.color.g:
            program.RunInstruction("TRIGGER WHEN DISTANCE=0 DELAY=0 DO $ANOUT[2]={:.3f}".format(
                self.color.g), INSTRUCTION_INSERT_CODE)

        if not last_color or last_color.b != self.color.b:
            program.RunInstruction("TRIGGER WHEN DISTANCE=0 DELAY=0 DO $ANOUT[3]={:.3f}".format(
                self.color.b), INSTRUCTION_INSERT_CODE)

        if not last_color or last_color.a != self.color.a:
            program.RunInstruction("TRIGGER WHEN DISTANCE=0 DELAY=0 DO $ANOUT[4]={:.3f}".format(
                self.color.a), INSTRUCTION_INSERT_CODE)
