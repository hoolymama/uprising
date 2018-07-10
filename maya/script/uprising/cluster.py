from robolink import (
    Robolink,
    INSTRUCTION_SHOW_MESSAGE,
    INSTRUCTION_COMMENT,
    INSTRUCTION_CALL_PROGRAM)
import robodk as rdk

from uprising_util import StrokeError, ClusterError


RL = Robolink()


import pymel.core as pm

CANVAS_HOME_JOINTS = [-90.000000, -90.000000,
                      120.000000, 0.000000, -30.000000, 0.000000]

TRAY_HOME_JOINTS = [-60.000000, -60.000000,
                    110.000000, 0.000000, 20.000000, 50.000000]


class Cluster(object):

    def __init__(self, **kw):
        self.strokes = []
        self.id = kw["cluster_id"]
        self.robot = kw["robot"]
        self.brush = kw["brush"]
        self.paint = kw["paint"]
        self.linear_speed = kw["linear_speed"]
        self.angular_speed = kw["angular_speed"]
        self.rounding = kw["rounding"]

    @staticmethod
    def create(**kw):
        if kw.get("is_dip"):
            return DipCluster(**kw)
        return PaintingCluster(**kw)

    def drawing_instruction(self, t1, t2):
        return None

    def name(self):
        raise NotImplementedError

    def build_stroke(self, stroke):

        # configure may raise a stroke error if it can't be configured
        stroke.configure(
            self.robot,
            self.brush,
            self.last_valid_joint_pose
        )
        self.strokes.append(stroke)
        self.last_valid_joint_pose = stroke.targets[-1].joint_pose

    def should_change(self):
        raise NotImplementedError

    def write_program_comands(self, robot, parent_program, parent_frame):
        prefix = parent_program.Name()
        parent_program.setSpeed(self.linear_speed, self.angular_speed)
        parent_program.setRounding(self.rounding)

        for i, stroke in enumerate(self.strokes):
            parent_program.RunInstruction(
                stroke.identifier(), INSTRUCTION_COMMENT)
            num_targets = len(stroke.targets)
            for i, t in enumerate(stroke.targets):

                tname = "%s_s_%s_t_%s" % (prefix, i, t.id)
                target = RL.AddTarget(tname, parent_frame, robot)
                target.setPose(t.tool_pose)
                target.setJoints(t.joint_pose)
 
                if t.linear:
                    parent_program.addMoveL(target)
                else:
                    parent_program.addMoveJ(target)
                    
                # if i > 1 and i < (num_targets - 3):
                #     seg_draw = self.drawing_instruction(
                #         t, stroke.targets[i + 1])
                #     if seg_draw:
                #         parent_program.RunInstruction(
                #             seg_draw, INSTRUCTION_CALL_PROGRAM)


class PaintingCluster(Cluster):

    def __init__(self, **kw):
        super(PaintingCluster, self).__init__(**kw)
        self.reason = kw["reason"]
        self.arc_length = 0
        self.last_valid_joint_pose = CANVAS_HOME_JOINTS
        self.max_paint_length = kw["paint"].max_length * kw["brush"].retention

    def drawing_instruction(self, t1, t2):
        color = tuple(self.paint.color)
        geo_name = "geox_%04d" % self.id
        args = "%.1f,%.1f,%.1f,%.1f,%.1f,%.1f,%.1f,%.1f,%.1f,%.1f,%.1f,%.1f,%.4f,%.4f,%.4f,%s" % (
            t1.vertices[0].x,
            t1.vertices[0].y,
            t1.vertices[0].z,
            t1.vertices[1].x,
            t1.vertices[1].y,
            t1.vertices[1].z,
            t2.vertices[0].x,
            t2.vertices[0].y,
            t2.vertices[0].z,
            t2.vertices[1].x,
            t2.vertices[1].y,
            t2.vertices[1].z,
            color[0],
            color[1],
            color[2],
            geo_name
        )
        return "draw_segment(%s)" % args

    def name(self):
        return "cluster_%s" % self.id

    def should_change(self, brush, paint, force):
        if brush is not self.brush:
            return "tool"
        if paint is not self.paint:
            return "tool"
        if (self.arc_length > self.max_paint_length):
            return "dip"
        if force:
            return "dip"
        return None

    def write_program_comands(self, robot, parent_program, parent_frame):

        tool_change_target = RL.Item('tool_change')

        if self.reason is "tool":
            tool = RL.Item(self.brush.name)
            parent_program.addMoveJ(tool_change_target)
            parent_program.RunInstruction(
                "Change Tool: %s ID:(%d) - paint ID:(%d)" %
                (self.brush.name,
                 self.brush.id,
                 self.paint.id),
                INSTRUCTION_SHOW_MESSAGE)
            parent_program.Pause()
            parent_program.setPoseTool(tool)
            # parent_program.RunInstruction(
            #                 "show_brush(%s)"% self.brush.name, 
            #                 INSTRUCTION_CALL_PROGRAM)


        dip_program_name = DipCluster.generate_name(
            self.paint.id, self.brush.id)
        parent_program.RunInstruction(
            dip_program_name, INSTRUCTION_CALL_PROGRAM)

        super(
            PaintingCluster,
            self).write_program_comands(
            robot,
            parent_program,
            parent_frame)

    def build_stroke(self, stroke):
        try:
            super(PaintingCluster, self).build_stroke(stroke)
            self.arc_length += stroke.arc_length
        except StrokeError as e:
            print("Cannot configure stroke %s of %s" %
                  (stroke.identifier(), self.__class__.__name__))
            print(e.message)


class DipCluster(Cluster):
    def __init__(self, **kw):
        super(DipCluster, self).__init__(**kw)
        self.last_valid_joint_pose = TRAY_HOME_JOINTS

    @staticmethod
    def generate_name(paint_id, brush_id):
        return "dip_p%02d_b%02d" % (paint_id, brush_id)

    def name(self):
        return DipCluster.generate_name(self.paint.id, self.brush.id)

    def should_change(self, brush, paint, force):
        if brush is not self.brush:
            return True
        if paint is not self.paint:
            return True
        return False

    def write_program_comands(self, robot, parent_program, parent_frame):

        # we have to set the tool in each subroutine
        tool = RL.Item(self.brush.name)
        if not tool.Valid():
            raise ClusterError(
                "Serious risk of damage! Can't find valid tool!")
        parent_program.setPoseTool(tool)
        super(
            DipCluster,
            self).write_program_comands(
            robot,
            parent_program,
            parent_frame)

    def build_stroke(self, stroke):
        try:
            super(DipCluster, self).build_stroke(stroke)
        except StrokeError as e:
            print("Cannot configure stroke %s of %s" %
                  (stroke.identifier(), self.__class__.__name__))
            print(e.message)
