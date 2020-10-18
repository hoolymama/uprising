import pymel.core as pm
from paint import Paint
from brush import Brush
from cluster import Cluster
import progress


def paint_and_brush_name(paint, brush):
    return "%s_%d_%s_%d" % (paint.name, paint.id, brush.name, brush.id)


class Painting(object):
    def __init__(self, node):
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
            return

        progress.update(
            minor_max=num_clusters,
            minor_progress=0)
 
        for i in range(num_clusters):

            progress.update(
                minor_line="Cluster {}/{}".format(i+1, num_clusters),
                minor_progress=i)

            brush_id = pm.paintingQuery(
                self.node, clusterIndex=i, clusterBrushId=True)
            paint_id = pm.paintingQuery(
                self.node, clusterIndex=i, clusterPaintId=True)

            brush = self.brushes.get(brush_id)
            paint = self.paints.get(paint_id)

            cluster = Cluster(i, self.node, brush, paint)
            self.clusters.append(cluster)

    def send_brushes(self):
        for brush in self.brushes:
            self.brushes[brush].send()
