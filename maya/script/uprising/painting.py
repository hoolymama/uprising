# import pymel.core as pm
# from uprising.paint import Paint
from uprising.brush import Brush
# from uprising.cluster import Cluster
# from uprising import progress


# def paint_and_brush_name(paint, brush):
#     return "%s_%d_%s_%d" % (paint.name, paint.id, brush.name, brush.id)


class Painting(object):
    def __init__(self, node):
        self.node = node
        self.brushes = Brush.used_brushes(node)
        self.motion = {
            "linear_speed": node.attr("linearSpeed").get() * 10,
            "angular_speed": node.attr("angularSpeed").get(),
            "rounding": node.attr("approximationDistance").get() * 10,
        }

    def send_brushes(self):
        for brush in self.brushes:
            self.brushes[brush].send()


# class BotPainting(Painting):
#     def __init__(self, node):
#         super(BotPainting, self).__init__(node)

#         self.paints = Paint.paints(node)
#         self.clusters = self._create_clusters()
 
#     def _create_clusters(self):
#         num_clusters = pm.paintingQuery(self.node, clusterCount=True)
#         if not num_clusters:
#             return []

#         clusters = []
#         progress.update(
#             minor_max=num_clusters,
#             minor_progress=0)

#         for i in range(num_clusters):

#             progress.update(
#                 minor_line="Cluster {}/{}".format(i+1, num_clusters),
#                 minor_progress=i)

#             brush_id = pm.paintingQuery(
#                 self.node, clusterIndex=i, clusterBrushId=True)
#             paint_id = pm.paintingQuery(
#                 self.node, clusterIndex=i, clusterPaintId=True)

#             brush = self.brushes.get(brush_id)
#             paint = self.paints.get(paint_id)

#             cluster = Cluster(i, self.node, brush, paint)
#             clusters.append(cluster)
    
#         return clusters

