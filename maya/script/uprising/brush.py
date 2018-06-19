class Brush(object):
    def __init__(self, the_id, width, name, lift, tcp, retention):
        self.id = the_id
        self.width = width * 10
        self.name = name
        self.lift = lift
        self.tcp = tcp * 10  # mm
        self.retention = retention

    @classmethod
    def brushes(cls, node):
        result = {}
        for index in node.attr("brushes").getArrayIndices():
            vals = node.attr("brushes[%d]" % index).get()
            result[index] = Brush(index, *vals)
        return result


