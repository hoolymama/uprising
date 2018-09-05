
from robolink import (  Robolink,  INSTRUCTION_SHOW_MESSAGE,  INSTRUCTION_COMMENT,  INSTRUCTION_CALL_PROGRAM)

import robodk as rdk

from uprising_util import StrokeError, ClusterError
from stroke import Stroke
import uprising_util as uutl
import pymel.core as pm

import const as k


import logging
logger = logging.getLogger('uprising')


class Cluster(object):

    def __init__(self, _id, node, robot, brush, paint):
        self.id = _id
        self.brush = brush
        self.paint = paint
        self.node = node
        # self.motion = motion
        self.travel = pm.paintingQuery(
            self.node, clusterIndex=_id, clusterTravel=True)
        self.robot = robot
        self.reason = pm.paintingQuery(
            self.node, clusterIndex=_id, clusterReason=True)
        self.build_strokes()

    def build_strokes(self):
        logger.debug("Cluster %d build strokes" % self.id)
        self.strokes = []
        num_strokes = pm.paintingQuery(
            self.node, clusterIndex=self.id, strokeCount=True)
        for i in range(num_strokes):
            logger.debug("Stroke %d" % i)
            stroke = Stroke(
                self.id,
                i,
                self.node,
                self.robot,
                self.brush
                )
            self.strokes.append(stroke)

    def name(self):
        raise NotImplementedError

    def write(self, studio, program, frame, motion):
        logger.debug("Write cluster")
        # program = studio.painting_program
        tool = studio.RL.Item(self.brush.name)
        if not tool.Valid():
            raise ClusterError(
                "Serious risk of damage! Can't find valid tool!")
        program.setPoseTool(tool)

        cluster_name = self.name()
        program.RunInstruction(
            "Cluster %s" %
            cluster_name,
            INSTRUCTION_COMMENT)
        program.setSpeed(
            motion["linear_speed"],
            motion["angular_speed"])
        program.setRounding(motion["rounding"])

        for i, stroke in enumerate(self.strokes):
            stroke.write(cluster_name, program,  frame, studio)


class PaintingCluster(Cluster):

    # def __init__(self, _id, node, robot, brush, paint):
    #     super(PaintingCluster, self).__init__(_id, node, robot, brush, paint)

    def name(self):
        return "px_c%s" % (self.id)

    def write(self, studio, motion):
        """Write the cluster to roboDK.

        Always do a dip before writing the strokes. If the 
        cluster is also a tool change then offer up the flange
        to the user first."""
        program = studio.painting_program
        frame = studio.painting_frame

        if self.reason == "tool":
            program.addMoveJ(studio.tool_approach)
            program.RunInstruction(
                "Change Tool: %s ID:(%d) - paint ID:(%d)" %
                (self.brush.name,
                 self.brush.id,
                 self.paint.id),
                INSTRUCTION_SHOW_MESSAGE)
            program.Pause()

        dip_program_name = DipCluster.generate_program_name(
            self.paint.id, self.brush.id)
        program.RunInstruction(
            dip_program_name, INSTRUCTION_CALL_PROGRAM)

        # program.addMoveJ(studio.dip_approach)

        super(PaintingCluster, self).write(studio, program,frame, motion)


class DipCluster(Cluster ):

    def name(self):
        return "dx_c%s" % (self.id)

    @staticmethod
    def generate_program_name(paint_id, brush_id):
        return "dip_p%02d_b%02d" % (paint_id, brush_id)


    def write(self, studio, motion):
        frame = studio.dip_frame
        dip_program_name = DipCluster.generate_program_name(
            self.paint.id, self.brush.id)
        program = uutl.create_program(dip_program_name)
 
        program.RunInstruction("Dip with tool %s" % self.brush.name, INSTRUCTION_COMMENT)
        logger.debug( "dip approach is %s" % studio.dip_approach.Name())
 
        program.addMoveJ(studio.dip_approach)
        super(DipCluster, self).write(studio, program, frame, motion)
        program.addMoveJ(studio.dip_approach)
        program.ShowInstructions(False)
    # def write(self, studio):
    #     logger.debug("Write cluster")

    #     cluster_name = self.name("dx")
    #     studio.parent_program.RunInstruction("Cluster: %s" % cluster_name, INSTRUCTION_COMMENT)
    #     studio.parent_program.setSpeed(self.motion["linear_speed"], self.motion["angular_speed"])
    #     studio.parent_program.setRounding(self.motion["rounding"])

    #     for i, stroke in enumerate(self.strokes):
    #         stroke.write(cluster_name, parent_program, parent_frame, robot, RL)



    # def write_program_comands(self, robot, parent_program, parent_frame):
    #     RL = Robolink()
    #     # we have to set the tool in each subroutine
    #     tool = RL.Item(self.brush.name)
    #     if not tool.Valid():
    #         raise ClusterError(
    #             "Serious risk of damage! Can't find valid tool!")
    #     parent_program.setPoseTool(tool)
    #     super(
    #         DipCluster,
    #         self).write(
    #         robot,
    #         parent_program,
    #         parent_frame)

    # def drawing_instruction(self, t1, t2):
    #     return None

    # def name(self):
    #     raise NotImplementedError

    # def build_stroke(self, stroke):

    #     # configure may raise a stroke error if it can't be configured
    #     stroke.configure(
    #         self.robot,
    #         self.brush,
    #         self.last_valid_joint_pose
    #     )
    #     self.strokes.append(stroke)
    #     self.last_valid_joint_pose = stroke.targets[-1].joint_pose

    # def should_change(self):
    #     raise NotImplementedError


                # if i > 1 and i < (num_targets - 3):
                #     seg_draw = self.drawing_instruction(
                #         t, stroke.targets[i + 1])
                #     if seg_draw:
                #         parent_program.RunInstruction(
                #             seg_draw, INSTRUCTION_CALL_PROGRAM)


# class PaintingCluster(Cluster ):

#     def __init__(self, **kw):
#         super(PaintingCluster, self).__init__(**kw)
#         self.reason = kw["reason"]
#         self.arc_length = 0
#         self.last_valid_joint_pose = CANVAS_HOME_JOINTS
#         self.max_paint_length = kw["paint"].max_length * kw["brush"].retention

#     def drawing_instruction(self, t1, t2):
#         color = tuple(self.paint.color)
#         geo_name = "geox_%04d" % self.id
#         args = "%.1f,%.1f,%.1f,%.1f,%.1f,%.1f,%.1f,%.1f,%.1f,%.1f,%.1f,%.1f,%.4f,%.4f,%.4f,%s" % (
#             t1.vertices[0].x,
#             t1.vertices[0].y,
#             t1.vertices[0].z,
#             t1.vertices[1].x,
#             t1.vertices[1].y,
#             t1.vertices[1].z,
#             t2.vertices[0].x,
#             t2.vertices[0].y,
#             t2.vertices[0].z,
#             t2.vertices[1].x,
#             t2.vertices[1].y,
#             t2.vertices[1].z,
#             color[0],
#             color[1],
#             color[2],
#             geo_name
#         )
#         return "draw_segment(%s)" % args

#     def name(self):
#         return "cluster_%s" % self.id

#     def should_change(self, brush, paint, force):
#         if brush is not self.brush:
#             return "tool"
#         if paint is not self.paint:
#             return "tool"
#         if (self.arc_length > self.max_paint_length):
#             return "dip"
#         if force:
#             return "dip"
#         return None

    # def write_program_comands(self, robot, parent_program, parent_frame):
    #     RL = Robolink()
    #     cluster_approach_target = RL.Item(k.CLUSTER_APPROACH)
    #     tool_change_target = RL.Item(k.TOOL_CHANGE_APPROACH)

    #     if not tool_change_target.Valid():
    #         tool_change_target = cluster_approach_target
    #     if not tool_change_target.Valid():
    #         raise ClusterError(
    #             "You must provide an object where the tool will be changed")

    #     if self.reason is "tool":
    #         tool = RL.Item(self.brush.name)
    #         parent_program.addMoveJ(tool_change_target)

    #         # parent_program.RunInstruction(
    #         #     "show_brush(%s)"% self.brush.name,
    #         #     INSTRUCTION_CALL_PROGRAM)

    #         parent_program.RunInstruction(
    #             "Change Tool: %s ID:(%d) - paint ID:(%d)" %
    #             (self.brush.name,
    #              self.brush.id,
    #              self.paint.id),
    #             INSTRUCTION_SHOW_MESSAGE)
    #         parent_program.Pause()
    #         parent_program.setPoseTool(tool)
    #     else:
    #         if cluster_approach_target.Valid():
    #             parent_program.addMoveJ(cluster_approach_target)

    #     dip_program_name = DipCluster.generate_name(
    #         self.paint.id, self.brush.id)
    #     parent_program.RunInstruction(
    #         dip_program_name, INSTRUCTION_CALL_PROGRAM)

    #     if cluster_approach_target.Valid():
    #         parent_program.addMoveJ(cluster_approach_target)

    #     super(
    #         PaintingCluster,
    #         self).write_program_comands(
    #         robot,
    #         parent_program,
    #         parent_frame)

#     def build_stroke(self, stroke):
#         try:
#             super(PaintingCluster, self).build_stroke(stroke)
#             self.arc_length += stroke.arc_length
#         except StrokeError as e:
#             print("Cannot configure stroke %s of %s" %
#                   (stroke.identifier(), self.__class__.__name__))
#             print(e.message)


# class DipCluster(Cluster):
#     def __init__(self, **kw):
#         super(DipCluster, self).__init__(**kw)
#         self.last_valid_joint_pose = TRAY_HOME_JOINTS

#     @staticmethod
#     def generate_name(paint_id, brush_id):
#         return "dip_p%02d_b%02d" % (paint_id, brush_id)

#     def name(self):
#         return DipCluster.generate_name(self.paint.id, self.brush.id)

#     def should_change(self, brush, paint, force):
#         if brush is not self.brush:
#             return True
#         if paint is not self.paint:
#             return True
#         return False

#     def write_program_comands(self, robot, parent_program, parent_frame):
#         RL = Robolink()
#         # we have to set the tool in each subroutine
#         tool = RL.Item(self.brush.name)
#         if not tool.Valid():
#             raise ClusterError(
#                 "Serious risk of damage! Can't find valid tool!")
#         parent_program.setPoseTool(tool)
#         super(
#             DipCluster,
#             self).write_program_comands(
#             robot,
#             parent_program,
#             parent_frame)

#     def build_stroke(self, stroke):
#         try:
#             super(DipCluster, self).build_stroke(stroke)
#         except StrokeError as e:
#             print("Cannot configure stroke %s of %s" %
#                   (stroke.identifier(), self.__class__.__name__))
#             print(e.message)
