import pymel.core as pm

from uprising.paint import Paint
from uprising.bot.session.cluster import Cluster
from uprising import progress
from uprising.common.session.painting import Painting
from uprising  import const as k


class BotPainting(Painting):
    def __init__(self, node):
        super(BotPainting, self).__init__(node)
        self.paints = Paint.paints()
        

        self.num_clusters = pm.paintingQuery(self.node, clusterCount=True)

        self.clusters = self._create_clusters()

    def _create_clusters(self):

        if not self.num_clusters:
            return []

        clusters = []
        progress.update(minor_max=self.num_clusters, minor_progress=0)

        for id in range(self.num_clusters):

            progress.update(
                minor_line="Cluster {}/{}".format(id + 1, self.num_clusters), minor_progress=id
            )

            brush_id = pm.paintingQuery(self.node, clusterIndex=id, clusterBrushId=True)
            paint_id = pm.paintingQuery(self.node, clusterIndex=id, clusterPaintId=True)

            # print("-----------")
            # print("NODE NAME", self.node.name())
            # print("self.brushes.get(brush_id)", brush_id)
            # print("self.paints.get(paint_id)", paint_id)

            brush = self.brushes.get(brush_id)
            paint = self.paints.get(paint_id)

            cluster = Cluster(self, id, brush, paint)

            clusters.append(cluster)

        return clusters
