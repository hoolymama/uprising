import pymel.core as pm
from robolink import (Robolink, ITEM_TYPE_ROBOT)
import uprising_util as uutl

class Paint(object):
    def __init__(self, the_id, name, color, opacity, travel):
        self.id = the_id
        self.name = name
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

    @classmethod
    def write_geos(cls):
        frame = uutl.create_frame("tx_frame")
        RL = Robolink()
        node = pm.PyNode("mainPaintingShape")

        dc = pm.paintingQuery(node, dc=True)
        pids = sorted(set(dc[1::2]))
        paints = Paint.paints(node)
        for pid in pids:
            paints[pid].write_geo(frame, RL)

    def write_geo(self, frame, RL):
        tray = pm.PyNode(self.name)
        geo = pm.ls( tray.getParent().getParent(), dag=True, leaf=True, type="mesh")
 
        triangles = []
        for g in geo:
            points = g.getPoints(space='world')
            _, vert_ids = g.getTriangles()
            for vert_id in vert_ids:
                triangles.append(
                    [points[vert_id].x * 10, points[vert_id].y * 10, points[vert_id].z * 10])
 

        tray_item = RL.Item(self.name)
        if tray_item.Valid():
            tray_item.Delete()

        col = list(tray.attr("sfPaintColor").get())
        shape = RL.AddShape(triangles)
        shape.setName(self.name)
        shape.setColor(col)
        shape.setParent(frame)