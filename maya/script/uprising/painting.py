

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
from cluster import (PaintingCluster, DipCluster)
# from studio import Studio

from uprising_util import PaintingError

import uprising_util as uutl
import const as k


import logging
logger = logging.getLogger('uprising')


def paint_and_brush_name(paint, brush):
    return "%s_%d_%s_%d" % (paint.name, paint.id, brush.name, brush.id)

# should be a singleton


class Painting(object):

    def __init__(self, node):
        logger.debug("Initialize Painting")
        self.node = node
        self.brushes = Brush.brushes(node)
        self.paints = Paint.paints(node)

        self.motion = {
            "linear_speed": self.node.attr("linearSpeed").get() * 10,
            "angular_speed": self.node.attr("angularSpeed").get(),
            "rounding": self.node.attr("approximationDistance").get() * 10
        }
        logger.debug("Done initialize Painting")


    def create_clusters(self, c_type, robot):

        logger.debug("Create clusters")
        self.clusters = []

        num_clusters = pm.paintingQuery(self.node, clusterCount=True)
        logger.debug("Number of clusters: %d" % num_clusters)
        for i in range(num_clusters):

            brush_id = pm.paintingQuery(
                self.node, clusterIndex=i, clusterBrushId=True)
            paint_id = pm.paintingQuery(
                self.node, clusterIndex=i, clusterPaintId=True)

            brush = self.brushes.get(brush_id)
            paint = self.paints.get(paint_id)
            logger.debug("Cluster %d: p=%d, b=%d" % (i, paint_id, brush_id))
            if c_type == "painting":
                cluster = PaintingCluster(
                    i,
                    self.node,
                    robot,
                    brush,
                    paint)
                self.clusters.append(cluster)
            else:
                cluster = DipCluster(
                    i,
                    self.node,
                    robot,
                    brush,
                    paint)
                self.clusters.append(cluster)

    def write(self, studio):
        logger.debug("Write program")
        for brush in self.brushes:
            self.brushes[brush].write(studio)

        for cluster in self.clusters:
            cluster.write(studio, self.motion)
       




# def progressStart(name, maxValue):
#     pm.progressWindow(
#         title=name,
#         progress=0,
#         maxValue=maxValue,
#         status="Generating: %d/%d" % (0, maxValue),
#         isInterruptable=True)


# def progressUpdate(amount, maxValue):
#     pm.progressWindow(
#         edit=True,
#         progress=amount,
#         status="Generating: %d/%d" % (amount, maxValue))
#     if pm.progressWindow(query=True, isCancelled=True):
#         raise SystemExit("Cancelled by user")


# def progressEnd():
#     pm.progressWindow(endProgress=1)



     
    # def create_dip_subroutines(self):
    #     RL = Robolink()
    #     RL.Render(False)
    #     # print "Creating dip subroutines"
    #     num_clusters = len(self.clusters)
    #     completed_clusters = 0
    #     progressStart("Create RoboDK Dips", num_clusters)

    #     robot = RL.Item('', ITEM_TYPE_ROBOT)

    #     dip_frame = uutl.create_frame("dx_frame")
    #     for cluster in self.clusters:
    #         program = uutl.create_program(cluster.name())
    #         cluster.write_program_comands(robot, program, dip_frame)
    #         completed_clusters += 1
    #         progressUpdate(completed_clusters, num_clusters)
    #         program.ShowInstructions(False)
    #     progressEnd()
    #     RL.Render(True)


    # def create_approaches(self):
    #     logger.debug("Create approaches")

    #     robot = self.RL.Item('', ITEM_TYPE_ROBOT)
    #     targets_frame = uutl.create_frame("ax_frame")

    #     for approach_object in self.approach_objects:
    #         mat = self.approach_objects[approach_object]
    #         configs = uutl.config_map(mat, "000")
    #         if configs and "000" in configs:
    #             joints = configs["000"][0]

    #         target = self.RL.AddTarget(approach_object, targets_frame, self.studio.robot)
    #         # Set the target as Cartesian (default)
    #         target.setAsJointTarget()
    #         target.setJoints(joints)


# class Painting(PaintingBase):

#     NAME = "px"

#     def __init__(self, node):
#         super(Painting, self).__init__(node)
#         self.set_approach_objects()

#         # self.start_joints = k.PAINTING_START_JOINTS

#     def set_approach_objects(self):
#         self.approach_objects = {}
#         atts = [
#             k.DIP_APPROACH,
#             k.TOOL_CHANGE_APPROACH,
#             k.HOME_APPROACH]
#         for att in atts:
#             mat = uutl.mat_from_connected(self.node.attr(att), "world")
#             if mat:
#                 self.approach_objects[att] = mat


#     def write(self):
#         logger.debug("Write program")

#         # RL = Robolink()
#         # RL.Render(False)
#         num_clusters = len(self.clusters)
#         completed_clusters = 0
#         # progressStart("Create RoboDK Painting", num_clusters)


#         main_program = uutl.create_program(self.NAME)
#         main_frame = uutl.create_frame("%s_frame" % self.NAME)

#         self.create_approaches()

#         for cluster in self.clusters:
#             cluster.write(self.NAME, main_program, main_frame, self.robot, self.RL)
#             # completed_clusters += 1
#             # progressUpdate(completed_clusters, num_clusters)

        # home = RL.Item(k.HOME_APPROACH)
        # if not home.Valid():
        #     home = RL.Item(k.TOOL_CHANGE_APPROACH)
        # if home.Valid():
        #     main_program.addMoveJ(home)

        # make sure the robot is in a sensible place

        # robot.setJoints(k.HOME_JOINTS)

        # main_program.ShowInstructions(False)
        # progressEnd()
        # RL.Render(True)

    # def _create_clusters(self, is_dip):
    #     RL = Robolink()
    #     robot = RL.Item('', ITEM_TYPE_ROBOT)
    #     positions = self.node.attr("outPosition").get()

    #     if not positions:
    #         return

    #     rotations = self.node.attr("outRotation").get()
    #     counts = self.node.attr("outCounts").get()
    #     brush_ids = self.node.attr("outBrushIds").get()
    #     paint_ids = self.node.attr("outPaintIds").get()
    #     curve_ids = self.node.attr("outCurveIds").get()
    #     force_dips = self.node.attr("outForceDips").get()
    #     arc_lengths = self.node.attr("outArcLengths").get()

    #     start_approach_dists = self.node.attr("outApproachStarts").get()
    #     end_approach_dists = self.node.attr("outApproachEnds").get()

    #     linear_speed = self.node.attr("linearSpeed").get() * 10
    #     angular_speed = self.node.attr("angularSpeed").get()
    #     rounding = self.node.attr("approximationDistance").get() * 10

    #     planeMat = self.node.attr("outPlaneMatrixWorld").get()
    #     # approach_dist = self.node.attr("strokeApproachDistance").get()
    #     planeNormal = (pm.dt.Vector(0, 0, 1) * planeMat).normal()
    #     # approach = planeNormal * approach_dist

    #     tangents = [(t * planeMat).normal()
    #                 for t in self.node.attr("outTangents").get()]

    #     # clusterApproachMat = uutl.mat_from_connected(
    #     #     self.node.attr("clusterApproachObject"), "world")
    #     # toolChangeApproachMat = uutl.mat_from_connected(
    #     #     self.node.attr("toolChangeApproachObject"), "world")
    #     # homeApproachMat = uutl.mat_from_connected(
    #     #     self.node.attr("homeApproachObject"), "world")

    #     num_strokes = len(counts)
    #     completed_strokes = 0
    #     progressStart("Generate stroke configurations", num_strokes)

    #     self.clusters = []
    #     cluster_id = 0
    #     start = 0

    #     cluster = None
    #     last_curve_id = -1

    #     for i, count in enumerate(counts):
    #         new_brush = self.brushes.get(brush_ids[i])
    #         new_paint = self.paints.get(paint_ids[i])
    #         curve_name = self.curve_names.get(curve_ids[i])

    #         start_approach_dist = start_approach_dists[i]
    #         end_approach_dist = end_approach_dists[i]

    #         if curve_ids[i] != last_curve_id:
    #             last_curve_id = curve_ids[i]
    #             curve_stroke_id = 0

    #         if cluster:
    #             change_reason = cluster.should_change(
    #                 new_brush, new_paint, force_dips[i])
    #         else:
    #             change_reason = "tool"

    #         if change_reason:
    #             if cluster:
    #                 self.clusters.append(cluster)

    #             cluster = Cluster.create(
    #                 is_dip=is_dip,
    #                 cluster_id=cluster_id,
    #                 robot=robot,
    #                 brush=new_brush,
    #                 paint=new_paint,
    #                 linear_speed=linear_speed,
    #                 angular_speed=angular_speed,
    #                 rounding=rounding,
    #                 reason=change_reason)
    #             cluster_id += 1

    #         end = start + count

    #         try:
    #             stroke = Stroke(robot,
    #                             positions[start:end],
    #                             rotations[start:end],
    #                             tangents[start:end],
    #                             new_brush,
    #                             new_paint,
    #                             arc_lengths[i],
    #                             planeNormal,
    #                             start_approach_dist,
    #                             end_approach_dist,
    #                             curve_name,
    #                             curve_stroke_id
    #                             )
    #             cluster.build_stroke(stroke)

    #         except StrokeError as e:
    #             print "Can't build stroke %s : %d" % (
    #                 curve_name, curve_stroke_id)
    #             print e.message

    #         # we update the stroke id even if a stroke failed to build
    #         curve_stroke_id += 1
    #         start = end
    #         completed_strokes += 1

    #         progressUpdate(completed_strokes, num_strokes)

    #     self.clusters.append(cluster)
    #     progressEnd()

    # def create_painting_program(self):

    #     # make sure nothing is selected

    #     RL = Robolink()
    #     RL.Render(False)
    #     num_clusters = len(self.clusters)
    #     completed_clusters = 0
    #     progressStart("Create RoboDK Painting", num_clusters)

    #     robot = RL.Item('', ITEM_TYPE_ROBOT)

    #     main_program = uutl.create_program("px")
    #     main_frame = uutl.create_frame("px_frame")

    #     self.create_approaches()

    #     for cluster in self.clusters:
    #         cluster.write_program_comands(robot, main_program, main_frame)
    #         completed_clusters += 1
    #         progressUpdate(completed_clusters, num_clusters)

    #     home = RL.Item(k.HOME_APPROACH)
    #     if not home.Valid():
    #         home = RL.Item(k.TOOL_APPROACH)
    #     if home.Valid():
    #         main_program.addMoveJ(home)

    #     # make sure the robot is in a sensible place

    #     robot.setJoints(k.HOME_JOINTS)

    #     # main_program.ShowInstructions(False)
    #     progressEnd()
    #     RL.Render(True)


# class Dip(PaintingBase):
#     NAME = "dx"
#     def __init__(self, node):
#         super(Painting, self).__init__(node)
#         self.start_joints = k.DIP_START_JOINTS
