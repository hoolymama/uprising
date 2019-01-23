import re
import pymel.core as pm
import const as k

import uprising_util as uutl
import uprising.maya_util as mut
import robodk as rdk


class Brush(object):
    def __init__(self,
                 the_id,
                 plug,
                 matrix,
                 width,
                 retention,
                 tip,
                 physical_id,
                 shape):
        self.id = the_id
        self.physical_id = physical_id
        self.matrix = uutl.maya_to_robodk_mat(matrix)
        self.width = width * 10
        self.shape = shape
        self.retention = retention
        self.tip = tip
        self.name, self.attribute = plug.split(".")
        self.plug = plug

    def is_round(self):
        return self.shape == 1

    def is_flat(self):
        return self.shape == 0

    def write(self, studio):
        old_brush = studio.RL.Item(self.name)
        if old_brush.Valid():
            old_brush.Delete()

        triangles = [[t.x * 10, t.y * 10, t.z * 10]
                     for t in pm.brushQuery(self.plug, tri=True)]

        tool_item = studio.robot.AddTool(self.matrix, self.name)
        shape = studio.RL.AddShape(triangles)
        tool_item.AddGeometry(shape, rdk.eye())
        studio.robot.setPoseTool(tool_item)
        shape.Delete()

    @classmethod
    def brush_at_index(cls, node, index):

        vals = [index]

        brushPlug = node.attr(
            "brushes[%d]" % index).connections(
            source=True,
            destination=False,
            plugs=True
        )[0]

        vals.append(str(brushPlug))

        matrix = pm.dt.Matrix(pm.brushQuery(brushPlug, tcp=True))
        vals.append(matrix)

        brush_node = brushPlug.node()
        for att in [
            "width",
            "retention",
            "tip",
            "physicalId",
            "shape"
        ]:
            vals.append(brush_node.attr(att).get())
        return Brush(*vals)

    @classmethod
    def find_by_regex(cls, node, regex_string):
        regex = re.compile(regex_string)
        for brush_id in node.attr("brushes").getArrayIndices():
            conns = node.attr(
                "brushes[%d]" % brush_id).connections(
                source=True,
                destination=False)
            name = str(conns[0])
            if regex.match(name):
                return Brush.brush_at_index(node, brush_id)

    @classmethod
    def brushes(cls, node):
        result = {}
        for brush_id in node.attr("brushes").getArrayIndices():
            result[brush_id] = Brush.brush_at_index(node, brush_id)
        return result

    # @classmethod
    # def used_brushes(cls, node):
    #     num_brushes = len(node.attr("brushes").getArrayIndices())
    #     result = {}
    #     found_brushes = 0
    #     for index in node.attr("curves").getArrayIndices():
    #         brush_id = node.attr("curves[%d].brushId" % index).get()
    #         if brush_id not in result:
    #             result[brush_id] = Brush.brush_at_index(node, brush_id)
    #             found_brushes += 1
    #             if found_brushes == num_brushes:
    #                 break
    #     return result
