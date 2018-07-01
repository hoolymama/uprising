

import pymel.core as pm

from robolink import (
    Robolink,
    ITEM_TYPE_ROBOT,
    INSTRUCTION_CALL_PROGRAM,
    INSTRUCTION_SHOW_MESSAGE,
    RUNMODE_MAKE_ROBOTPROG,
    COLLISION_ON,
    COLLISION_OFF
)
import robodk as rdk

from paint import Paint
from brush import Brush
from stroke import Stroke
from cluster import Cluster

from uprising_util import StrokeError

import uprising_util as uutl
# , PaintingCluster, DipCluster

RL = Robolink()

RL.setCollisionActive(COLLISION_OFF)


def progressStart(name, maxValue):
    pm.progressWindow(
        title=name,
        progress=0,
        maxValue=maxValue,
        status="Generating: %d/%d" % (0, maxValue),
        isInterruptable=True)


def progressUpdate(amount, maxValue):
    pm.progressWindow(
        edit=True,
        progress=amount,
        status="Generating: %d/%d" % (amount, maxValue))
    if pm.progressWindow(query=True, isCancelled=True):
        raise SystemExit("Cancelled by user")


def progressEnd():
    pm.progressWindow(endProgress=1)


def paint_and_brush_name(paint, brush):
    return "%s_%d_%s_%d" % (paint.name, paint.id, brush.name, brush.id)


class Painting(object):

    def __init__(self, factory_node, is_dip=False):
        RL.setCollisionActive(0)
        self.brushes = {}
        self.paints = {}
        self.curve_names = {}
        self.is_dip = is_dip
        self.node = factory_node
        self.name = pm.listRelatives(factory_node, parent=True)[0]
        self.brushes = Brush.brushes(factory_node)
        self.paints = Paint.paints(factory_node)

        approach_dist = self.node.attr("strokeApproachDistance").get()

        planeMat = self.node.attr("outPlaneMatrixWorld").get()

        self.approach = (pm.dt.Vector(0, 0, 1) *
                         planeMat).normal() * approach_dist

        indices = factory_node.attr("curves").getArrayIndices()
        for index in indices:
            nodes = pm.listConnections(
                factory_node.attr(
                    "curves[%d].curve" %
                    index),
                source=True,
                destination=False)
            if nodes:
                self.curve_names[index] = str(nodes[0])

        self._create_clusters(is_dip)

    def _create_clusters(self, is_dip):

        robot = RL.Item('', ITEM_TYPE_ROBOT)
        positions = self.node.attr("outPosition").get()
        rotations = self.node.attr("outRotation").get()
        counts = self.node.attr("outCounts").get()
        brush_ids = self.node.attr("outBrushIds").get()
        paint_ids = self.node.attr("outPaintIds").get()
        curve_ids = self.node.attr("outCurveIds").get()
        force_dips = self.node.attr("outForceDips").get()
        arc_lengths = self.node.attr("outArcLengths").get()

        linearSpeed = self.node.attr("linearSpeed").get() * 10
        angularSpeed = self.node.attr("angularSpeed").get()
        rounding = self.node.attr("approximationDistance").get() * 10

        clusterApproachMat = uutl.mat_from_connected(self.node.attr("clusterApproachObject") , "world")
        toolChangeApproachMat = uutl.mat_from_connected(self.node.attr("toolChangeApproachObject") , "world")
        homeApproachMat = uutl.mat_from_connected(self.node.attr("homeApproachObject") , "world")



        num_strokes = len(counts)
        completed_strokes = 0
        progressStart("Generate stroke configurations", num_strokes)

        self.clusters = []
        cluster_id = 0
        start = 0
        cluster = Cluster.create(is_dip, cluster_id, "tool")
        cluster_id += 1
        curve_stroke_id = 0
        last_curve_id = -1
        for i, count in enumerate(counts):
            new_brush = self.brushes.get(brush_ids[i])
            new_paint = self.paints.get(paint_ids[i])
            curve_name = self.curve_names.get(curve_ids[i])

            if curve_ids[i] != last_curve_id:
                last_curve_id = curve_ids[i]
                curve_stroke_id = 0

            change_reason = cluster.should_change(
                new_brush, new_paint, force_dips[i])

            if change_reason:
                self.clusters.append(cluster)
                cluster = Cluster.create(is_dip, cluster_id, change_reason)
                cluster_id += 1

            if cluster.empty():
                cluster.set_tools(
                    robot,
                    new_brush,
                    new_paint,
                    linearSpeed,
                    angularSpeed,
                    rounding)

            end = start + count

            try:
                stroke = Stroke(robot,
                                positions[start:end],
                                rotations[start:end],
                                new_brush,
                                new_paint,
                                arc_lengths[i],
                                self.approach,
                                curve_name,
                                curve_stroke_id
                                )
                cluster.build_stroke(stroke)

            except StrokeError as e:
                print "Can't build stroke %s : %d" % (
                    curve_name, curve_stroke_id)
                print e.message

            curve_stroke_id += 1
            start = end
            completed_strokes += 1

            progressUpdate(completed_strokes, num_strokes)

        self.clusters.append(cluster)
        progressEnd()




    # def _create_clusters(self, is_dip):

    #     robot = RL.Item('', ITEM_TYPE_ROBOT)
    #     positions = self.node.attr("outPosition").get()
    #     rotations = self.node.attr("outRotation").get()
    #     counts = self.node.attr("outCounts").get()
    #     brush_ids = self.node.attr("outBrushIds").get()
    #     paint_ids = self.node.attr("outPaintIds").get()
    #     curve_ids = self.node.attr("outCurveIds").get()
    #     force_dips = self.node.attr("outForceDips").get()
    #     arc_lengths = self.node.attr("outArcLengths").get()

    #     linearSpeed = self.node.attr("linearSpeed").get() * 10
    #     angularSpeed = self.node.attr("angularSpeed").get()
    #     rounding = self.node.attr("approximationDistance").get() * 10

    #     self.clusters = []
    #     cluster_id = 0
    #     start = 0
    #     cluster = Cluster.create(is_dip, cluster_id, "tool")
    #     cluster_id += 1
    #     curve_stroke_id = 0
    #     last_curve_id = -1
    #     for i, count in enumerate(counts):
    #         new_brush = self.brushes.get(brush_ids[i])
    #         new_paint = self.paints.get(paint_ids[i])
    #         curve_name = self.curve_names.get(curve_ids[i])

    #         if curve_ids[i] != last_curve_id:
    #             last_curve_id = curve_ids[i]
    #             curve_stroke_id = 0

    #         change_reason = cluster.should_change(
    #             new_brush, new_paint, force_dips[i])

    #         if change_reason:
    #             self.clusters.append(cluster)
    #             cluster = Cluster.create(is_dip, cluster_id, change_reason)
    #             cluster_id += 1

    #         if cluster.empty():
    #             cluster.set_tools(
    #                 robot,
    #                 new_brush,
    #                 new_paint,
    #                 linearSpeed,
    #                 angularSpeed,
    #                 rounding)

    #         end = start + count

    #         try:
    #             stroke = Stroke(robot,
    #                             positions[start:end],
    #                             rotations[start:end],
    #                             new_brush,
    #                             new_paint,
    #                             arc_lengths[i],
    #                             self.approach,
    #                             curve_name,
    #                             curve_stroke_id
    #                             )
    #             cluster.build_stroke(stroke)

    #         except StrokeError as e:
    #             print "Can't build stroke %s : %d" % (
    #                 curve_name, curve_stroke_id)
    #             print e.message

    #         curve_stroke_id += 1
    #         start = end
       

           
    #     self.clusters.append(cluster)
    #     progressEnd()
 
 
    def create_painting_program(self):
        num_clusters = len(self.clusters)
        completed_clusters = 0
        progressStart("Create RoboDK Painting", num_clusters)

        robot = RL.Item('', ITEM_TYPE_ROBOT)

        main_program = uutl.create_program("gx")
        main_frame = uutl.create_frame("gx_frame")

        for cluster in self.clusters:
            cluster.write_program_comands(robot, main_program, main_frame)
            completed_clusters += 1
            progressUpdate(completed_clusters, num_clusters)

        main_program.addMoveJ(RL.Item('tool_change'))
        progressEnd()

    def create_dip_subroutines(self):
        # print "Creating dip subroutines"
        num_clusters = len(self.clusters)
        completed_clusters = 0
        progressStart("Create RoboDK Dips", num_clusters)

        robot = RL.Item('', ITEM_TYPE_ROBOT)

        dip_frame = uutl.create_frame("dx_frame")
        for cluster in self.clusters:
            program = uutl.create_program(cluster.name())
            cluster.write_program_comands(robot, program, dip_frame)
            completed_clusters += 1
            progressUpdate(completed_clusters, num_clusters)
        progressEnd()

    # def show(self):
    #     robot = RL.Item('', ITEM_TYPE_ROBOT)
    #     cluster_id = 0
    #     for cluster in self.clusters:
    #         print "------------ %s ------------" % cluster_id
    #         print "REASON: %s" % cluster.reason
    #         print "BRUSH %s - width: %s, id: %d" % (
    #             cluster.brush.name, cluster.brush.width, cluster.brush.id)
    #         print "PAINT %s - id: %d" % (cluster.paint.name, cluster.paint.id)
    #         print "ARCLEN: %s" % cluster.arc_length
    #         for stroke in cluster.strokes:
    #             print "STROKE ARCLEN: %s" % stroke.arc_length
    #             # print "FIRST - LAST POINTS: %s %s" % (
    #             #     stroke.positions[0], stroke.positions[-1])
    #             config = stroke.valid_config
    #             print "VALID CONFIG: %s " % config

    #             # joints = None
    #             # for t, target in enumerate(stroke.targets):
    #             #     joints = target["valid_joint_pose"]
    #             #     print "VALID JOINTS: %s" % joints

    #         print "NUM STROKES: %s" % len(cluster.strokes)

    #         # num_strokes = len(cluster.strokes)
    #         cluster_id += 1
    #     print "NUM CLUSTERS: %s" % len(self.clusters)
