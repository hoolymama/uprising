import painting as ptg
import uprising_util as uutl

from robolink import (
    INSTRUCTION_SHOW_MESSAGE,
    INSTRUCTION_COMMENT,
    INSTRUCTION_CALL_PROGRAM)


class Program(object):
    def __init__(self, name, **kw):
        self.program_name = name

    def write(self):
        self.program = uutl.create_program(self.program_name)
        self.frame = uutl.create_frame("{}_frame".format(self.program_name))


class MainProgram(Program):
    def __init__(self, name, node, robot):
        super(MainProgram, self).__init__(name)
        self.painting = ptg.Painting(node, robot)
        # self.painting.create_clusters(robot)

    def write(self, studio):
        super(MainProgram, self).write()
        self.painting.write_brushes()

        motion = self.painting.motion
        for cluster in self.painting.clusters:

            if cluster.reason == "tool":
                self.program.addMoveJ(studio.tool_approach)
                self.program.RunInstruction(cluster.change_tool_message(),
                                            INSTRUCTION_SHOW_MESSAGE)
                self.program.Pause()

            dip_program_name = DipProgram.generate_program_name(
                cluster.paint.id, cluster.brush.id)
            
            self.program.RunInstruction(
                dip_program_name, INSTRUCTION_CALL_PROGRAM)

            cluster.write(studio, self.program, self.frame, motion)
        self.program.addMoveJ(studio.home_approach)
        self.program.ShowInstructions(False)

class DipProgram(Program):

    @staticmethod
    def generate_program_name(paint_id, brush_id):
        return "p{}_b{}".format(paint_id, brush_id)

    def __init__(self, name, dip_node, wipe_node, robot):
        super(DipProgram, self).__init__(name)


        print "DIP {}".format(dip_node)
        print "WIP {}".format(wipe_node)
        
        self.dip_painting = ptg.Painting(dip_node, robot)
        self.wipe_painting = ptg.Painting(wipe_node, robot)

    def write(self, studio):
        super(DipProgram, self).write()
        self.dip_painting.write_brushes()
        self.wipe_painting.write_brushes()

        self.program.RunInstruction(
            "Dip with tool %s" %
            self.dip_painting.clusters[0].brush.node_name, INSTRUCTION_COMMENT)

        self.program.addMoveJ(studio.dip_approach)

        for cluster in self.dip_painting.clusters:
            cluster.write(
                studio,
                self.program,
                self.frame,
                self.dip_painting.motion)

        for cluster in self.wipe_painting.clusters:
            cluster.write(
                studio,
                self.program,
                self.frame,
                self.wipe_painting.motion)

        self.program.addMoveJ(studio.dip_approach)
