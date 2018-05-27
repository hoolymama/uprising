

import pymel.core as pm

from robolink import (
    Robolink,
    ITEM_TYPE_ROBOT,
    ITEM_TYPE_TARGET,
    INSTRUCTION_CALL_PROGRAM,
    INSTRUCTION_SHOW_MESSAGE,
    RUNMODE_MAKE_ROBOTPROG)
import robodk as rdk

RL = Robolink()


MAX_CLUSTER_STROKE_SEPARATION = 50

class Paint(object):
    def __init__(self, the_id, color, name, opacity, max_length):
        self.id = the_id
        self.color = color
        self.name = name
        self.opacity = opacity,
        self.max_length = max_length


class Brush(object):
    def __init__(self, the_id, width, name, tcp, tip):
        self.id = the_id
        self.width = width
        self.name = name
        self.tcp = tcp
        self.tip = tip


class Stroke(object):

    def __init__(self, points, normals, arc_length):
        self.points = points
        self.normals = normals
        self.arc_length = arc_length


class Cluster(object):

    def __init__(self, reason):
        self.reason = reason
        self.strokes = []
        self.arc_length = 0
        self.brush = None
        self.paint = None

    def add_stroke(self, stroke):
        self.arc_length += stroke.arc_length
        self.strokes.append(stroke)

    def empty(self):
        return not self.strokes


    def create_curve(self, name, frame):
        curve = 0
        for i, stroke in enumerate(self.strokes):

            points = []
            for j in range(len(stroke.points)):
                p = stroke.points[j]
                n = stroke.normals[j]
                points.append([p.x * 10, p.y * 10, p.z * 10, n.x, n.y, n.z])
            curve = RL.AddCurve(points, curve, add_to_ref=True)
        curve.setParent(frame)
        curve.setName("%s_curve" % name)
        return curve


    # def create_milling_settings(self, name):
    #     path_settings = RL.AddMillingProject("%s_settings" % name)
    #     return path_settings


def paint_and_brush_name(paint, brush):
    return "%s_%d_%s_%d" % (paint.name,paint.id,brush.name,brush.id)


class Painting(object):

    def __init__(self, factoryNode):
        self.brushes = {}
        self.paints = {}
        self.slosh = None

        self.name = pm.listRelatives(factoryNode, parent=True)[0]
        for index in factoryNode.attr("brushes").getArrayIndices():
            vals = factoryNode.attr("brushes[%d]" % index).get()
            self.brushes[index] = Brush(index, *vals)

        for index in factoryNode.attr("paints").getArrayIndices():
            vals = factoryNode.attr("paints[%d]" % index).get()
            self.paints[index] = Paint(index, *vals)

        self._create_clusters(factoryNode)

    def create_milling_program(self, path_settings, curve, name, tool):
        path_settings.setPoseTool(tool)
        prog, status = path_settings.setMillingParameters(part=curve)
        prog.setName(name)
        # print status
        return prog

    def should_change_cluster(self, cluster, brush, paint, start_index, points):
        if cluster.empty():
            return (False, None)
        if brush is not cluster.brush:
            return (True, "tool")
        if paint is not cluster.paint:
            return (True, "tool")
        if cluster.arc_length > paint.max_length:
            return (True, "dip")


        if not start_index == 0:
            last_point = points[(start_index-1)]
            next_point = points[start_index]
            gap = (next_point-last_point).length()
            if gap > MAX_CLUSTER_STROKE_SEPARATION:
                return (True, "span")

        return (False, None)

    def _create_clusters(self, factoryNode):
        points = factoryNode.attr("outPointsWorld").get()
        normals = factoryNode.attr("outNormalsWorld").get()
        counts = factoryNode.attr("outCounts").get()
        brush_ids = factoryNode.attr("outBrushIds").get()
        paint_ids = factoryNode.attr("outPaintIds").get()
        arcL_lengths = factoryNode.attr("outArcLengths").get()

        self.clusters = []
        start = 0
        cluster = Cluster("tool")

        for i, count in enumerate(counts):
           
            new_brush = self.brushes.get(brush_ids[i])
            new_paint = self.paints.get(paint_ids[i])

            do_change, reason = self.should_change_cluster(
                cluster, new_brush, new_paint, start, points)

            if do_change:
                self.clusters.append(cluster)
                cluster = Cluster(reason)

            if cluster.empty():
                # add starter info
                cluster.brush = new_brush
                cluster.paint = new_paint

            end = start + count
            stroke = Stroke(points[start:end],
                            normals[start:end], arcL_lengths[i])
            cluster.add_stroke(stroke)

            start = end
        self.clusters.append(cluster)



    def create_program_permutations(self):
        """for slosh painting"""
        RL.Render(False)
        frame = RL.AddFrame("SloshFrame")
        frame.setPose(rdk.eye())
        result = {}
        for cluster in self.clusters:
            paint = cluster.paint
            curve = cluster.create_curve(paint.name, frame)
            result[paint.id] = {}
            for bid in sorted(self.brushes.keys()):
                brush = self.brushes[bid]
                name = paint_and_brush_name(paint, brush)
                settings = RL.AddMillingProject("%s_settings" % name)
                tool = RL.Item(brush.name)
                result[paint.id][brush.id] = self.create_milling_program(settings, curve, "%s_prog" % name, tool)
        RL.Render(True)
        return result


    def build_painting_program(self):
        RL.Render(False)
        robot = RL.Item('', ITEM_TYPE_ROBOT)
        main_program_name = "painting_program"
        main_program = RL.Item('main_program_name')
        if main_program.Valid():
            main_program.Delete()

        main_program = RL.AddProgram(main_program_name)

        frame = RL.AddFrame("PaintFrame")
        frame.setPose(rdk.eye())
        frame.setVisible(False, visible_frame=False)
        # painting_programs = []

        home_position_target = RL.Item('home_position')
        tool_change_target = RL.Item('tool_change')
        tray_approach_target = RL.Item('tray_approach')
        final_position_target = RL.Item('final_position')

        for i, cluster in enumerate(self.clusters):
            curve = cluster.create_curve("C_%d_curve" % i, frame)
            brush = cluster.brush
            paint = cluster.paint

            new_stroke_settings = RL.AddMillingProject("C_%d_stroke_settings" % i)

            tool = RL.Item(brush.name)
            prog = self.create_milling_program(new_stroke_settings, curve, "C_%d_stroke_prog" % i, tool)
            

            if cluster.reason is "tool":
                brush_id = brush.id
                paint_id = paint.id
                # go to tool change
                main_program.addMoveJ(tool_change_target)
                main_program.RunInstruction(
                    "Change Tool: %scm brush ID:(%d) - paint ID:(%d)" %
                    (cluster.brush.width, brush_id, paint_id), INSTRUCTION_SHOW_MESSAGE)
                main_program.Pause() 

            # dip in the paint
            slosh_prog_name = "%s_prog" % paint_and_brush_name(paint, brush)
            main_program.addMoveJ(tray_approach_target)
            main_program.RunInstruction(slosh_prog_name, INSTRUCTION_CALL_PROGRAM)
            
            # paint the cluster
            main_program.addMoveJ(tray_approach_target)
            main_program.RunInstruction(prog.Name(), INSTRUCTION_CALL_PROGRAM)
    
        main_program.addMoveJ(final_position_target)
        RL.Render(True)
        
    def show(self):
        cluster_id = 0
        for cluster in self.clusters:
            print "------------ %s ------------" % cluster_id
            print "REASON: %s" % cluster.reason
            print "BRUSH %s - width: %s, id: %d" % (cluster.brush.name, cluster.brush.width, cluster.brush.id)
            print "PAINT %s - id: %d" % ( cluster.paint.name, cluster.paint.id)
            print "ARCLEN: %s" % cluster.arc_length
            for stroke in cluster.strokes:
                print "STROKE ARCLEN: %s" % stroke.arc_length
                print "FIRST - LAST POINTS: %s %s" % (stroke.points[0], stroke.points[-1])
                


            print "NUM STROKES: %s" % len(cluster.strokes)
            # num_strokes = len(cluster.strokes)
            cluster_id += 1
        print "NUM CLUSTERS: %s" % len(self.clusters)
