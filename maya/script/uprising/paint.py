import pymel.core as pm
# import uprising.maya_util as mutil
# import robo


class Paint(object):
    def __init__(self, the_id, pot_id, name, color, opacity, travel):
        self.id = the_id
        self.pot_id = pot_id
        self.name = name
        self.color = color
        self.opacity = opacity
        self.travel = travel

    @classmethod
    def paints(cls):
        paletteNode = pm.PyNode("paletteShape")
        result = {}
        for att in paletteNode.attr("input"):
            _id = att.index()
            result[_id] = Paint(
                _id,
                att.attr("potId").get(),
                str(att),
                att.attr("color").get(),
                att.attr("opacity").get(),
                att.attr("travel").get(),
                )
        return result
