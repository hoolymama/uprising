import pymel.core as pm

from uprising.paint import Paint
from uprising.bot.session.cluster import Cluster
from uprising import progress
from uprising.common.session.painting import Painting


class BotPainting(Painting):
    def __init__(self, node):
        super(BotPainting, self).__init__(node)
        self.paints = Paint.paints(node)
        self.clusters = self._create_clusters()
        
    def _create_clusters(self):
        num_clusters = pm.paintingQuery(self.node, clusterCount=True)
        if not num_clusters:
            return []

        clusters = []
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
            clusters.append(cluster)

        return clusters
