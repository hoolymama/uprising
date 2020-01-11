import pymel.core as pm
from robolink import Robolink, ITEM_TYPE_ROBOT
from paint import Paint
from brush import Brush
from cluster import Cluster


def paint_and_brush_name(paint, brush):
    return "%s_%d_%s_%d" % (paint.name, paint.id, brush.name, brush.id)


class Painting(object):
    def __init__(self, node):
        self.RL = Robolink()
        self.robot = self.RL.Item("", ITEM_TYPE_ROBOT)
        self.robot.setParam("PostProcessor", "KUKA KRC4_RN")
        self.node = node
        self.brushes = Brush.used_brushes(node)
        self.paints = Paint.paints(node)
        self.clusters = []
        self.motion = {
            "linear_speed": node.attr("linearSpeed").get() * 10,
            "angular_speed": node.attr("angularSpeed").get(),
            "rounding": node.attr("approximationDistance").get() * 10,
        }
        self._create_clusters()

    def _create_clusters(self):
        try:
            num_clusters = pm.paintingQuery(self.node, clusterCount=True)
        except RuntimeError:
            num_clusters = 0
        print "Creating {} clusters for {}".format(num_clusters, self.node)
        for i in range(num_clusters):
            if i % 10 == 0:
                print "Cluster {}/{}".format(i, num_clusters)
            brush_id = pm.paintingQuery(self.node, clusterIndex=i, clusterBrushId=True)
            paint_id = pm.paintingQuery(self.node, clusterIndex=i, clusterPaintId=True)

            brush = self.brushes.get(brush_id)
            paint = self.paints.get(paint_id)

            cluster = Cluster(i, self.node, self.robot, brush, paint)
            self.clusters.append(cluster)


    def write_brushes(self):
        for brush in self.brushes:
            self.brushes[brush].write(self.RL, self.robot)
