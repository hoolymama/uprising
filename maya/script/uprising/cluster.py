from robolink import (
    Robolink,
    INSTRUCTION_SHOW_MESSAGE,
    INSTRUCTION_COMMENT)
import robodk as rdk
from stroke import StrokeError
RL = Robolink()


import pymel.core as pm

CANVAS_HOME_JOINTS = [-90.000000, -90.000000, 120.000000, 0.000000, -30.000000, 0.000000]

PI = 3.14159265359

def rad2deg(rad):
    return rad * (180 / PI)

def deg2rad(deg):
    return deg / (180 / PI)


class Cluster(object):

    def __init__(self, reason):
        self.reason = reason
        self.strokes = []
        self.arc_length = 0
        self.brush = None
        self.paint = None
        self.last_valid_joint_pose = CANVAS_HOME_JOINTS

    def set_tools(self, robot, brush, paint):
        self.robot = robot
        self.brush = brush
        self.paint = paint
        self.max_paint_length = paint.max_length * brush.retention



    def build_stroke(self, stroke):
        self.arc_length += stroke.arc_length
        # stroke.cluster = self
        try:
            stroke.configure(
                self.robot,
                self.brush,
                self.last_valid_joint_pose
            )
            self.strokes.append(stroke)
            self.last_valid_joint_pose = stroke.targets[-1]["valid_joint_pose"]

        except StrokeError as e:
            print("CANNOT ADD STROKE %s" % stroke.curve_name)
            print(e.message)

    def empty(self):
        return not self.strokes

    def test_strokes(self, robot):
        pass


    def write_program_comands(self, robot, parent_program, parent_frame,
                              min_span=None,
                              max_span=None,
                              min_approach=None,
                              max_approach=None):


        tool_change_target = RL.Item('tool_change')
 

        prefix = parent_program.Name()

        # if self.reason is "tool":
        #     tool = RL.Item(self.brush.name)
        #     parent_program.addMoveJ(tool_change_target)

        # prog = self.create_milling_program(
        #     new_stroke_settings,
        #     curve,
        #     "C_%d_stroke_prog" %
        #     i,
        #     tool)

        # if cluster.reason is "tool":
        #     brush_id = brush.id
        #     paint_id = paint.id
        #     # go to tool change
        #     main_program.addMoveJ(tool_change_target)
        #     main_program.RunInstruction(
        #         "Change Tool: %scm brush ID:(%d) - paint ID:(%d)" %
        #         (cluster.brush.width, brush_id, paint_id), INSTRUCTION_SHOW_MESSAGE)
        #     main_program.Pause()

        # # dip in the paint
        # slosh_prog_name = "%s_prog" % paint_and_brush_name(paint, brush)
        # main_program.addMoveJ(tray_approach_target)
        # main_program.RunInstruction(
        #     slosh_prog_name, INSTRUCTION_CALL_PROGRAM)



        # last_target_data = None
        for i, stroke in enumerate(self.strokes):
            parent_program.RunInstruction(stroke.identifier(), INSTRUCTION_COMMENT)
            # num_targets = len(stroke.targets)
            for j, target_data in enumerate(stroke.targets):
                # print target_data["valid_joint_pose"]
                joints = target_data["valid_joint_pose"]
                pose = target_data["tool_pose"]

                tname = "%s_s_%s_t_%s" % (prefix, i, j)
                target = RL.AddTarget(tname, parent_frame, robot)
                target.setPose(pose)
                target.setJoints(joints)

                if j == 0:
                    parent_program.addMoveJ(target)
                else:
                    parent_program.addMoveL(target)











                # if j == (num_targets - 1):
                #     last_target_data = target_data

        # parent_program.RunInstruction(
        #     "Finished cluster", INSTRUCTION_SHOW_MESSAGE)





            # if last_target_data:
            #     intermediatePose = self.calcIntermediatePose(
            #         last_target_data,
            #         stroke.targets[0],
            #         min_span,
            #         max_span,
            #         min_approach,
            #         max_approach)
            #     print "intermediatePose %s" % intermediatePose
            #     if intermediatePose:
            #         tname = "%s_s_%s_inter" % (prefix, i)
            #         target = RL.AddTarget(tname, parent_frame, robot)
            #         target.setPose(intermediatePose)
            #         parent_program.addMoveJ(target)
            # print "stroke %s : %s" % (i, stroke.valid_config)



    # def calcIntermediatePose(
    #         self,
    #         last_target,
    #         target,
    #         min_span,
    #         max_span,
    #         min_approach,
    #         max_approach):

    #     min_span = min_span * 10
    #     max_span = max_span * 10
    #     min_appr = min_approach * 10
    #     max_appr = max_approach * 10

    #     print "min_span %s" % min_span
    #     print "max_span %s" % max_span
    #     print "min_appr %s" % min_appr
    #     print "max_appr %s" % max_appr

    #     # print "T TARGET %s " % target
    #     # print "L TARGET %s " % last_target

    #     v1 = pm.dt.Vector(target["tool_pose"].Pos())
    #     v2 = pm.dt.Vector(last_target["tool_pose"].Pos())
    #     print "v1 %s" % v1
    #     print "v2 %s" % v2

    #     length = (v2 - v1).length()  # mm
    #     print "length %s" % length
    #     if length < min_span:
    #         return None
    #     if length > max_span:
    #         length = max_span

    #     fraction = (length - min_span) / (max_span - min_span)
    #     print "fraction %s" % fraction
    #     halfway = (pm.dt.Vector(v1) + pm.dt.Vector(v2)) * 0.5
    #     print "halfway %s" % halfway
    #     new_pos = ((max_appr - min_appr) * fraction) + halfway
    #     print "new_pos %s" % new_pos
    #     mat = rdk.TxyzRxyz_2_Pose(
    #         [new_pos.x, new_pos.y, new_pos.z, deg2rad(-90), 0, deg2rad(90)])
    #     return mat

    # if cluster

    # def create_curve(self, name, frame):
    #     curve = 0
    #     for i, stroke in enumerate(self.strokes):

    #         points = []
    #         for j in range(len(stroke.points)):
    #             p = stroke.points[j]
    #             n = stroke.normals[j]
    #             points.append([p.x * 10, p.y * 10, p.z * 10, n.x, n.y, n.z])
    #         curve = RL.AddCurve(points, curve, add_to_ref=True)
    #     curve.setParent(frame)
    #     curve.setName("%s_curve" % name)
    #     return curve

    # def create_milling_settings(self, name):
    #     path_settings = RL.AddMillingProject("%s_settings" % name)
    #     return path_settings





    # def write_program_comands(self, robot, parent_program, parent_frame):
    #     for i, stroke in enumerate(self.strokes):
    #         for j in range(len(stroke.positions)):
    #             p = stroke.positions[j]
    #             r = stroke.rotations[j]
    #             mat = rdk.TxyzRxyz_2_Pose(
    #                 [p.x * 10, p.y * 10, p.z * 10, r.x, r.y, r.z])
    #             tname = "gx_t_%s_%s" % (i, j)
    #             target = RL.AddTarget(tname, parent_frame, robot)
    #             target.setPose(mat)
    #             if j == 0:
    #                 parent_program.addMoveJ(target)
    #             else:
    #                 parent_program.addMoveL(target)

    #     parent_program.RunInstruction(
    #         "Finished cluster", INSTRUCTION_SHOW_MESSAGE)





