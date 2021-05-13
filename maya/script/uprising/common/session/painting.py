import pymel.core as pm
from uprising.paint import Paint
from uprising.brush import Brush
from uprising import progress


# def paint_and_brush_name(paint, brush):
#     return "%s_%d_%s_%d" % (paint.name, paint.id, brush.name, brush.id)


class Painting(object):
    def __init__(self, node):
        self.node = node
        self.brushes = Brush.used_brushes(node)
        self.motion = {
            "linear_speed": node.attr("linearSpeed").get() * 10,
            "angular_speed": node.attr("angularSpeed").get(),
            "rounding": node.attr("approximationDistance").get() * 10,
        }

    def send_brushes(self, with_geo=False):
        for brush in self.brushes:
            self.brushes[brush].send(with_geo)