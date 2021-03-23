import pymel.core as pm
from uprising.painting import Painting
from uprising.pov.session.pov_stroke import PovStroke
from uprising import progress


class PovPainting(Painting):

    def __init__(self, node):

        super(PovPainting, self).__init__(node)
        self.strokes = self._create_strokes()

        print "POV PAINTING INIT: ", self.node

    def _create_strokes(self):

        num_strokes = pm.lightPaintingQuery(self.node, strokeCount=True)
        if not num_strokes:
            return []

        brush = [self.brushes[brush] for brush in self.brushes][0]

        strokes = []
        progress.update(
            minor_max=num_strokes,
            minor_progress=0)

        for i in range(num_strokes):
            progress.update(
                minor_line="Stroke {}/{}".format(i+1, num_strokes),
                minor_progress=i)
 
            stroke = PovStroke(i, brush, self.node)
            strokes.append(stroke)
        return strokes
