class Paint(object):
    def __init__(self, the_id, name, color, opacity, travel):
        self.id = the_id
        self.color = color
        self.opacity = opacity,
        self.travel = travel



    @classmethod
    def paint_at_index(cls, node, index):
        vals = [index]
        conns = node.attr(
                 "paints[%d].paintTravel" %index).connections(
                source=True,
                destination=False)
        vals.append(str(conns[0]))

        for att in ["paintColor","paintOpacity","paintTravel"]:
            vals.append(node.attr("paints[%d].%s" % (index, att)).get())

        return Paint(*vals)

    @classmethod
    def paints(cls, node):
        result = {}
        for paint_id in node.attr("paints").getArrayIndices():
            result[paint_id] = Paint.paint_at_index(node, paint_id)
        return result