import pymel.core as pm
import const as k

import uprising_util as uutl
import uprising.maya_util as mut
import robodk as rdk


class Brush(object):
    def __init__(self, the_id, name, matrix, width, retention, tip, physical_id, shape):
        self.id = the_id
        self.physical_id= physical_id
        self.matrix = uutl.maya_to_robodk_mat(matrix)
        self.width = width * 10
        self.shape = shape
        self.retention = retention
        self.tip = tip
        self.name = name

    def is_round(self):
        return self.shape == 1

    def is_flat(self):
        return self.shape == 0

    def write(self, studio):
        old_brush = studio.RL.Item(self.name)
        if old_brush.Valid():
            old_brush.Delete()

        geo = pm.PyNode(self.name).getShapes() + \
            pm.PyNode("brushes|brushBase").getShapes()
        triangles = []
        for g in geo:
            points = g.getPoints(space='world')
            _, vert_ids = g.getTriangles()
            for vert_id in vert_ids:
                triangles.append(
                    [points[vert_id].x * 10, points[vert_id].y * 10, points[vert_id].z * 10])

        color = mut.shape_color(g)
        tool_item = studio.robot.AddTool(self.matrix, self.name)
        shape = studio.RL.AddShape(triangles)
        shape.setColor(list(color))
        tool_item.AddGeometry(shape, rdk.eye())
        studio.robot.setPoseTool(tool_item)
        shape.Delete()

    @classmethod
    def brush_at_index(cls, node, index):
        vals = [index]
        conns = node.attr(
            "brushes[%d].brushMatrix" % index).connections(
            source=True,
            destination=False)
        vals.append(str(conns[0]))

        for att in [
            "brushMatrix",
            "brushWidth",
            "brushRetention",
            "brushTip",
            "brushPhysicalId",
            "brushShape"
        ]:
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
                found_brushes += 1
                if found_brushes == num_brushes:
                    break
        return result
