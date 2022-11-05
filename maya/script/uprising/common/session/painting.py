import pymel.core as pm
from uprising.paint import Paint
from uprising.brush import Brush
from uprising import progress


class Painting(object):
    def __init__(self, node):
        self.node = node
        self.brushes = Brush.used_brushes(node)

        


        self.linear_speed = node.attr("linearSpeed").get() * 10
        self.angular_speed = node.attr("angularSpeed").get()
        self.rounding = node.attr("approximationDistance").get() * 10

    def send_brushes(self, with_geo=False):
        for brush in self.brushes:
            self.brushes[brush].send(with_geo)

    def send_speeds(self, program):
        program.setRounding(self.rounding)
        program.setSpeed(self.linear_speed)
        program.setSpeedJoints(self.angular_speed)
        