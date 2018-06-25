

import pymel.core as pm

from robolink import (
    Robolink,
    ITEM_TYPE_ROBOT,
    ITEM_TYPE_TARGET,
    INSTRUCTION_CALL_PROGRAM,
    INSTRUCTION_SHOW_MESSAGE,
    RUNMODE_MAKE_ROBOTPROG)
import robodk as rdk

from paint import Paint
from brush import Brush
from stroke import Stroke
from cluster import Cluster
# , PaintingCluster, DipCluster

RL = Robolink()
PI = 3.14159265359

RL.setCollisionActive(0)

def rad2deg(rad):
    return rad * (180 / PI)


def deg2rad(deg):
    return deg / (180 / PI)


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
        arcL_lengths = self.node.attr("outArcLengths").get()

        linearSpeed = self.node.attr("linearSpeed").get() * 10
        angularSpeed = self.node.attr("angularSpeed").get()
        

        num_strokes = len(counts)
        completed_strokes = 0
        progressStart("Generate stroke configurations", num_strokes)

        self.clusters = []
        cluster_id = 0
        start = 0
        cluster = Cluster.create(is_dip, cluster_id, "tool")
        cluster_id +=1
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
                cluster_id +=1

            end = start + count


            stroke = Stroke(positions[start:end],
                            rotations[start:end],
                            arcL_lengths[i],
                            self.approach,
                            curve_name,
                            curve_stroke_id
                            )


            print "curve_stroke_id is %d" % stroke.curve_stroke_id

            if cluster.empty():
                cluster.set_tools(robot, new_brush, new_paint, linearSpeed, angularSpeed)

            curve_stroke_id += 1


            cluster.build_stroke(stroke)
            start = end
            completed_strokes += 1

            progressUpdate(completed_strokes, num_strokes)

        self.clusters.append(cluster)
        progressEnd()
 
    def _create_program(self, name):
        # print "Creating Program"
        program = RL.Item(name)
        if program.Valid():
            program.Delete()
        return RL.AddProgram(name)

    def _create_frame(self, name):
        # print "Creating Frame"
        frame = RL.Item(name)
        if frame.Valid():
            frame.Delete()
        frame = RL.AddFrame(name)
        frame.setPose(rdk.eye())
        return frame

    def create_painting_program(self):
        num_clusters = len(self.clusters)
        completed_clusters = 0
        progressStart("Create RoboDK Painting", num_clusters)

        robot = RL.Item('', ITEM_TYPE_ROBOT)

        main_program = self._create_program("gx")
        main_frame = self._create_frame("gx_frame")

        last_curve_name = None
 
        for i, cluster in enumerate(self.clusters):
            cluster.write_program_comands(robot, main_program, main_frame)
            completed_clusters += 1
            progressUpdate(completed_clusters, num_clusters)

            # last_cluster = cluster
        main_program.addMoveJ(RL.Item('tool_change'))
        progressEnd()

    def create_dip_subroutines(self):
        # print "Creating subroutines"
        num_clusters = len(self.clusters)
        completed_clusters = 0
        progressStart("Create RoboDK Dips", num_clusters)

        robot = RL.Item('', ITEM_TYPE_ROBOT)

        dip_frame = self._create_frame("dx_frame")
        for i, cluster in enumerate(self.clusters):
            program = self._create_program(cluster.name())
            # program.setSpeed(self.linearSpeed, self.angularSpeed)
 
            cluster.write_program_comands(robot, program, dip_frame)
            completed_clusters += 1
            progressUpdate(completed_clusters, num_clusters)
        progressEnd()





    def show(self):
        robot = RL.Item('', ITEM_TYPE_ROBOT)
        cluster_id = 0
        for cluster in self.clusters:
            print "------------ %s ------------" % cluster_id
            print "REASON: %s" % cluster.reason
            print "BRUSH %s - width: %s, id: %d" % (
                cluster.brush.name, cluster.brush.width, cluster.brush.id)
            print "PAINT %s - id: %d" % (cluster.paint.name, cluster.paint.id)
            print "ARCLEN: %s" % cluster.arc_length
            for stroke in cluster.strokes:
                print "STROKE ARCLEN: %s" % stroke.arc_length
                # print "FIRST - LAST POINTS: %s %s" % (
                #     stroke.positions[0], stroke.positions[-1])
                config = stroke.valid_config
                print "VALID CONFIG: %s " % config

                # joints = None
                # for t, target in enumerate(stroke.targets):
                #     joints = target["valid_joint_pose"]
                #     print "VALID JOINTS: %s" % joints

            print "NUM STROKES: %s" % len(cluster.strokes)

            # num_strokes = len(cluster.strokes)
            cluster_id += 1
        print "NUM CLUSTERS: %s" % len(self.clusters)

    # def create_program_permutations(self):
    #     """for slosh painting."""
    #     RL.Render(False)
    #     frame = RL.AddFrame("SloshFrame")
    #     frame.setPose(rdk.eye())
    #     result = {}
    #     for cluster in self.clusters:
    #         paint = cluster.paint
    #         curve = cluster.create_curve(paint.name, frame)
    #         result[paint.id] = {}
    #         for bid in sorted(self.brushes.keys()):
    #             brush = self.brushes[bid]
    #             name = paint_and_brush_name(paint, brush)
    #             settings = RL.AddMillingProject("%s_settings" % name)
    #             tool = RL.Item(brush.name)
    #             result[paint.id][brush.id] = self.create_milling_program(
    #                 settings, curve, "%s_prog" % name, tool)
    #     RL.Render(True)
    #     return result

    # def create_milling_program(self, path_settings, curve, name, tool):
    #     path_settings.setPoseTool(tool)
    #     prog, status = path_settings.setMillingParameters(part=curve)
    #     prog.setName(name)
    #     # print status
    #     return prog

    # def build_painting_program(self):
    #     RL.Render(False)
    #     robot = RL.Item('', ITEM_TYPE_ROBOT)
    #     main_program_name = "painting_program"
    #     main_program = RL.Item('main_program_name')
    #     if main_program.Valid():
    #         main_program.Delete()
    #     main_program = RL.AddProgram(main_program_name)

    #     frame = RL.AddFrame("PaintFrame")
    #     frame.setPose(rdk.eye())
    #     frame.setVisible(False, visible_frame=False)
    #     # painting_programs = []

    #     home_position_target = RL.Item('home_position')
    #     tool_change_target = RL.Item('tool_change')
    #     tray_approach_target = RL.Item('tray_approach')
    #     final_position_target = RL.Item('final_position')

    #     for i, cluster in enumerate(self.clusters):
    #         curve = cluster.create_curve("C_%d_curve" % i, frame)
    #         brush = cluster.brush
    #         paint = cluster.paint

    #         new_stroke_settings = RL.AddMillingProject(
    #             "C_%d_stroke_settings" % i)

    #         tool = RL.Item(brush.name)
    #         prog = self.create_milling_program(
    #             new_stroke_settings,
    #             curve,
    #             "C_%d_stroke_prog" %
    #             i,
    #             tool)

    #         if cluster.reason is "tool":
    #             brush_id = brush.id
    #             paint_id = paint.id
    #             # go to tool change
    #             main_program.addMoveJ(tool_change_target)
    #             main_program.RunInstruction(
    #                 "Change Tool: %scm brush ID:(%d) - paint ID:(%d)" %
    #                 (cluster.brush.width, brush_id, paint_id), INSTRUCTION_SHOW_MESSAGE)
    #             main_program.Pause()

    #         # dip in the paint
    #         slosh_prog_name = "%s_prog" % paint_and_brush_name(paint, brush)
    #         main_program.addMoveJ(tray_approach_target)
    #         main_program.RunInstruction(
    #             slosh_prog_name, INSTRUCTION_CALL_PROGRAM)

    #         # paint the cluster
    #         main_program.addMoveJ(tray_approach_target)
    #         main_program.RunInstruction(prog.Name(), INSTRUCTION_CALL_PROGRAM)

    #     main_program.addMoveJ(final_position_target)
    #     RL.Render(True)
