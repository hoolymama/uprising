import pymel.core as pm
from robolink import INSTRUCTION_COMMENT


from uprising.bot.session.bot_stroke import BotStroke
from uprising import robo


class ClusterError(Exception):
    pass


class Cluster(object):
    def __init__(self, painting, _id, brush, paint):
        self.painting = painting
        self.id = _id
        self.brush = brush
        self.paint = paint
        self.strokes = []

        node = self.painting.node
        self.travel = pm.paintingQuery(node, clusterIndex=_id, clusterTravel=True)
        self.reason = pm.paintingQuery(node, clusterIndex=_id, clusterReason=True)
        num_strokes = pm.paintingQuery(node, clusterIndex=_id, strokeCount=True)
        self.pot_id = pm.paintingQuery(node, clusterIndex=_id, clusterPotId=True)

        for stroke_index in range(num_strokes):
            stroke = BotStroke(self, stroke_index)
            self.strokes.append(stroke)

    def name(self, program):
        return "{}_c{}".format(program.Name(), self.id)

    def send(self, program, frame):
        link = robo.link()
        tool = link.Item(self.brush.name)
        if not tool.Valid():
            raise ClusterError("SERIOUS RISK OF DAMAGE! Can't find valid tool!")

        program.setPoseTool(tool)
        cluster_name = self.name(program)

        program.RunInstruction("Cluster %s" % cluster_name, INSTRUCTION_COMMENT)

        for i, stroke in enumerate(self.strokes):
            if stroke.ignore:
                # the stroke is ignored, so we don't send it.
                # 
                # However, if there's a next stroke we need to go to a safe place (canvas approach) since arrival
                # targets are for the wrong preceding stroke. Then we need to reconfigure the
                # next stroke.
                if i < len(self.strokes) - 1:
                    self.program.addMoveJ(robo.canvas_approach)
                    next_stroke = self.strokes[i + 1]
                    next_stroke.arrivals = next_stroke.arrivals[:-1]
            else:
                stroke.send(cluster_name, program, frame)

    def get_flow_info(self, last_cluster):
        last_paint_id = last_cluster.paint.id if last_cluster else None
        last_pot_id = last_cluster.pot_id if last_cluster else None

        last_layer_id = last_cluster.strokes[-1].layerId if last_cluster else None
        this_layer_id = self.strokes[0].layerId if self.strokes else "XXXX"
        did_change_layer = last_layer_id != this_layer_id

        this_paint_id = self.paint.id
        did_change_paint = last_paint_id != this_paint_id
        did_change_pot = last_pot_id != self.pot_id

        last_brush_id = last_cluster.brush.id if last_cluster else None
        this_brush_id = self.brush.id
        did_change_tool = self.reason == "tool"
        did_change_brush = did_change_tool and (last_brush_id != this_brush_id)
        did_end_last_brush = did_change_brush and (last_brush_id is not None)
        return (
            did_change_pot,
            did_change_paint,
            did_change_tool,
            did_change_brush,
            did_end_last_brush,
            did_change_layer,
        )
