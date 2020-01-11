import re
import pymel.core as pm
import const as k

import uprising_util as uutl
import uprising.maya_util as mut
import robodk as rdk
from robolink import Robolink, ITEM_TYPE_ROBOT


class Brush(object):
    
    def __init__( index, plug):
        self.id = index
        self.plug = plug
        self.node =  plug.node()
        self.node_name = self.node.name()
        self.name = str(plug).replace(".", "_")

        self.matrix = uutl.maya_to_robodk_mat(
            pm.dt.Matrix(pm.brushQuery(plug, tcp=True))
        )
        self.physical_id = self.node.attr("physicalId").get()
        self.width = = self.node.attr("width").get() * 10
        self.shape = self.node.attr("shape").get() 
        self.retention = self.node.attr("retention").get() 
        self.tip = self.node.attr("tip").get() 
        self.wipe_bar_position =self.node.attr("wipeBarPosition").get()

        self.initial_wait = self.node.attr("initialWait").get()
        self.initial_water = self.node.attr("initialWater").get()
        self.initial_dips = self.node.attr("initialDips").get()
        self.retardant = self.node.attr("retardant").get()

    def __str__(self):
        # Override to print a readable string
        return ", ".join(
            [
                "{key}={value}".format(key=key, value=self.__dict__.get(key))
                for key in self.__dict__
            ]
        )

    def is_round(self):
        return self.shape == 1

    def is_flat(self):
        return self.shape == 0

    def write(self, RL, robot):

        old_brush = RL.Item(self.name)
        if old_brush.Valid():
            return

        # print "WRITE BRUSH", self.name
        triangles = uutl.to_vector_array(pm.brushQuery(self.plug, tri=True))
        triangles = [[t.x * 10, t.y * 10, t.z * 10] for t in triangles]

        tool_item = robot.AddTool(self.matrix, self.name)
        shape = RL.AddShape(triangles)
        tool_item.AddGeometry(shape, rdk.eye())
        robot.setPoseTool(tool_item)
        shape.Delete()

    @classmethod
    def write_used_brush_sets(cls):
        RL = Robolink()
        robot = RL.Item("", ITEM_TYPE_ROBOT)

        painting = pm.PyNode("mainPaintingShape")
        dc = pm.paintingQuery(painting, dc=True)
        bids = sorted(set(dc[::2]))
        for bid in bids:
            brush_set = Brush.brush_set_at_index(bid)
            for key in brush_set:
                brush_set[key].write(RL, robot)

    @classmethod
    def write_connected_brushes(cls):
        RL = Robolink()
        robot = RL.Item("", ITEM_TYPE_ROBOT)
        painting = pm.PyNode("mainPaintingShape")
        brushes = Brush.brushes(painting)
        for brush in brushes:
            brushes[brush].write(RL, robot)

    @classmethod
    def write_selected_brushes(cls):
        RL = Robolink()
        robot = RL.Item("", ITEM_TYPE_ROBOT)
        brushNodes = pm.ls(selection=True, dag=True, leaf=True, type="brushNode")
        brush_atts = ["outPaintBrush", "outDipBrush", "outWipeBrush"]
        for brush_node in brushNodes:
            for brush_att in brush_atts:
                plug = brush_node.attr(brush_att)
                Brush(0, plug).write(RL, robot)

    @classmethod
    def brush_at_index(cls, node, index):

        plug = node.attr("brushes[%d]" % index).connections(
            source=True, destination=False, plugs=True
        )[0]

        return Brush(index, plug)

    @classmethod
    def brush_set_at_index(cls, index):
        """ Get paint, dip, and wipe brushes corresponding to index.
            
            Always use the painting node
        """
        node = pm.PyNode("mainPaintingShape")

        brush_node = node.attr("brushes[%d]" % index).connections(
            source=True, destination=False
        )[0]

        result = {}
        brush_atts = ["outPaintBrush", "outDipBrush", "outWipeBrush"]
        for brush_att in brush_atts:
            plug = brush_node.attr(brush_att)
            result[brush_att] = Brush(index, plug)
        return result

    @classmethod
    def find_by_regex(cls, node, regex_string):
        regex = re.compile(regex_string)
        for brush_id in node.attr("brushes").getArrayIndices():
            conns = node.attr("brushes[%d]" % brush_id).connections(
                source=True, destination=False
            )
            name = str(conns[0])
            if regex.match(name):
                return Brush.brush_at_index(node, brush_id)

    @classmethod
    def brushes(cls, node):
        result = {}
        for brush_id in node.attr("brushes").getArrayIndices():
            result[brush_id] = Brush.brush_at_index(node, brush_id)
        return result

    @classmethod
    def used_brushes(cls, node):
        result = {}
        try:
            dc = pm.paintingQuery(node, dc=True)
            bids = sorted(set(dc[::2]))
            for brush_id in bids:
                result[brush_id] = Brush.brush_at_index(node, brush_id)
        except RuntimeError:
            pass
        return result

