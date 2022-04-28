import pymel.core as pm
from uprising.common.session.painting import Painting
from uprising.pov.session.pov_stroke import PovStroke

# from uprising import progress

PAINTING_NAME = "lightPaintingShape"


class PovPainting(Painting):
    def __init__(self):
        
        super(PovPainting, self).__init__(pm.PyNode(PAINTING_NAME))
        
        # self.brush = [self.brushes[brush] for brush in self.brushes][0]
        
        self.colorGain = self.node.attr("colorGain").get()
        self.whiteGain = self.node.attr("whiteGain").get()
        self.waitGain = self.node.attr("waitGain").get()
        
        self.num_strokes = pm.lightPaintingQuery(self.node, strokeCount=True)
        
        self.strokes = [PovStroke(self, i) for i in range(self.num_strokes)]
        
        
        
# print("PovPainting init")
# print("PovPainting super")
# print("PovPainting brush")
# print("PovPainting cols")
# print("PovPainting num_strokes")
# print("PovPainting strokes")
# print(("POV PAINTING INIT: ", self.node))
