

class Brush(object):

    def __init__(
            self,
            index
            attack_height,
            attack_length,
            decay_height,
            decay_length):
        pass


class Cluster(object):

    def __init__(self, strokes, brush):
        self._brush = brush
        self._strokes = strokes

        pass


class Stroke(object):

    def __init__(self, points):
        self._points = points

        pass
