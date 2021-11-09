import pymel.core as pm
from uprising.common.session.painting import Painting
from uprising.pov.session.pov_stroke import PovStroke
# from uprising import progress


class PovPainting(Painting):

    def __init__(self, node):

        super(PovPainting, self).__init__(node)
        self.strokes = self._create_strokes()
        self.brush = [self.brushes[brush] for brush in self.brushes][0]
        print "POV PAINTING INIT: ", self.node

    def _create_strokes(self):

        num_strokes = pm.lightPaintingQuery(self.node, strokeCount=True)
        if not num_strokes:
            return []

        strokes = []
   
        for i in range(num_strokes):
            print("Create stroke:",i)
            stroke = PovStroke(i, self.node)
            strokes.append(stroke)
        return strokes
