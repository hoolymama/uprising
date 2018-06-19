class Paint(object):
    def __init__(self, the_id, color, name, opacity, max_length):
        self.id = the_id
        self.color = color
        self.name = name
        self.opacity = opacity,
        self.max_length = max_length


    @classmethod
    def paints(cls, node):
        result = {}
        for index in node.attr("paints").getArrayIndices():
            vals = node.attr("paints[%d]" % index).get()
            result[index] = Paint(index, *vals)
        return result
