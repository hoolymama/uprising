import pymel.core as pm
import re

import utils as uutl
import robodk as rdk
from uprising import robo
from robolink import (ITEM_TYPE_ROBOT)

class Brush(object):
    def __init__(self, index, plug):
        self.id = index
        self.plug = plug
        self.node = plug.node()
        self.node_name = self.node.name()
        self.name = str(plug).replace(".", "_")

        self.matrix = robo.maya_to_robodk_mat(
            pm.dt.Matrix(pm.brushQuery(plug, tcp=True))
        )
        self.physical_id = self.node.attr("physicalId").get()
        self.width = self.node.attr("width").get() * 10
        self.shape = self.node.attr("shape").get()
        self.retention = self.node.attr("retention").get()
        self.tip = self.node.attr("tip").get()
        self.wipe_bar_position = self.node.attr("wipeBarPosition").get()

        self.initial_wait = self.node.attr("initialWait").get()
        self.initial_water = self.node.attr("initialWater").get()
        self.initial_dips = self.node.attr("initialDips").get()
        self.retardant = self.node.attr("retardant").get()
        self.model = self.node.attr("model").get()
        self.bristleHeight = self.node.attr("bristleHeight").get()
        self.param = 0.0
        if plug.attrName(True, False) == "outDipBrush":
            self.param = self.node.attr("dipParam").get()
        elif plug.attrName(True, False) == "outWipeBrush":
            self.param = self.node.attr("wipeParam").get()
        else:
            self.param = self.node.attr("paintingParam").get()
       
        self.active_bristle = self.bristleHeight * self.param
        
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

    def send(self, with_geo=False, force=False):
        link = robo.link()
        robot = robo.robot()
        if not robot:
            robot = link.Item("", ITEM_TYPE_ROBOT) 

        old_brush = link.Item(self.name)
     
        if old_brush.Valid():
            if force:
                old_brush.Delete()
            else:
                return
        try:
            tool_item = robot.AddTool(self.matrix, self.name)
        except AttributeError:
            pm.warning("No Robot. Use robo.clean() to load a RoboDK scene with a robot.")
            raise
        if with_geo:
            triangles = uutl.to_vector_array(
                pm.brushQuery(self.plug, tri=True))
            triangles = [[t.x * 10, t.y * 10, t.z * 10] for t in triangles]
            shape = link.AddShape(triangles)
            tool_item.AddGeometry(shape, rdk.eye())
            shape.Delete()
        robot.setPoseTool(tool_item)

    @classmethod
    def send_used_brush_sets(cls):
        painting = pm.PyNode("mainPaintingShape")
        dc = pm.paintingQuery(painting, dc=True)
        bids = sorted(set(dc[::2]))
        for bid in bids:
            brush_set = Brush.brush_set_at_index(bid)
            for key in brush_set:
                brush_set[key].send()

    @classmethod
    def send_connected_brushes(cls):
        painting = pm.PyNode("mainPaintingShape")
        brushes = Brush.brushes(painting)
        for brush in brushes:
            brushes[brush].send()

    @classmethod
    def send_selected_brushes(cls, brush_atts=None):
        brushNodes = pm.ls(selection=True, dag=True,
                           leaf=True, type="brushNode")

        if not brush_atts:
            brush_atts = ["outPaintBrush", "outDipBrush", "outWipeBrush"]
        for brush_node in brushNodes:
            for brush_att in brush_atts:
                plug = brush_node.attr(brush_att)
                Brush(0, plug).send( with_geo=True, force=True)

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
            return result
        except (RuntimeError, AttributeError):

            plug = pm.PyNode(node).attr("brush")
            brush_plug = plug.connections(source=True, destination=False, plugs=True)[0]
            return {"00": Brush(0, brush_plug)}
 