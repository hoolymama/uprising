import pymel.core as pm
import const as k

import uprising_util as uutl

class Brush(object):
    def __init__(self, the_id, name, matrix, width, retention, shape):
        self.id = the_id
        self.matrix = uutl.maya_to_robodk_mat(matrix)
        self.width = width * 10
        self.shape = shape
        self.retention = retention
        self.name = name
               
    @classmethod
    def brush_at_index(cls, node, index):
        vals = [index]
        conns = node.attr(
                "brushes[%d].brushMatrix" %index).connections(
                source=True,
                destination=False)
        vals.append(str(conns[0]))

        for att in [
            "brushMatrix",
            "brushWidth",
            "brushShape",
            "brushRetention"]:
            vals.append(node.attr("brushes[%d].%s" % (index, att)).get())

        return Brush(*vals)

    
    @classmethod
    def brushes(cls, node):
        result = {}
        for brush_id in node.attr("brushes").getArrayIndices():
            result[brush_id] = Brush.brush_at_index(node, brush_id)
        return result

    @classmethod
    def used_brushes(cls, node):
        num_brushes = len(node.attr("brushes").getArrayIndices())
        result = {}
        found_brushes = 0
        for index in node.attr("curves").getArrayIndices():
            brush_id = node.attr("curves[%d].brushId" % index).get()
            if brush_id not in result:
                result[brush_id] = Brush.brush_at_index(node, brush_id)
                found_brushes +=1
                if found_brushes == num_brushes:
                    break
        return result