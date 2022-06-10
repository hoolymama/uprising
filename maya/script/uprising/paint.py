import pymel.core as pm
# import uprising.maya_util as mutil
# import robo
from uprising import const as k

class Paint(object):
    def __init__(self, the_id, name, color, opacity, travel):
        self.id = the_id
        self.name = name
        self.color = color
        self.opacity = opacity
        self.travel = travel

    @classmethod
    def paints(cls):
        paletteNode = pm.PyNode(k.PALETTE)
        result = {}
        for att in paletteNode.attr("input"):
            _id = att.index()
            result[_id] = Paint(
                _id,
                str(att),
                att.attr("color").get(),
                att.attr("opacity").get(),
                att.attr("travel").get(),
                )
        return result
