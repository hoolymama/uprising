from robolink import (
    INSTRUCTION_COMMENT,
    INSTRUCTION_SHOW_MESSAGE,
    INSTRUCTION_INSERT_CODE
)
from uprising import robo
from uprising.utils import ClusterError
from uprising.pov.session.pov_target import PovTarget
import pymel.core as pm

from uprising import utils
from uprising.stroke import Stroke


class PovStroke(Stroke):

    def __init__(self,  stroke_id, brush, node):
        super(PovStroke, self).__init__(stroke_id, brush, node)

    def build_targets(self):

        positions = self.query_positions()
        rotations = self.query_rotations()
        colors = self.query_colors()


        self.targets = self._build_target_array(
            positions, rotations, colors, self.brush, self.id)

    @classmethod
    def _build_target_array(cls, positions, rotations, colors, brush, stroke_id):
        result = []
        
        num_targets = len(positions)
        if not (num_targets == len(rotations) and num_targets == len(colors)):
            raise utils.StrokeError(
                "Length mismatch: positions, rotations, colors")

        for i, (p, r, c) in enumerate(zip(positions, rotations, colors)):
            try:
                if i == 0:
                    tg = PovTarget(i, (p * 10), r, pm.dt.Color(0.0,0.0,0.0,0.0), brush)
                    result.append(tg)
                tg = PovTarget(i, (p * 10), r, c, brush)
            except utils.StrokeError:
                print "Target Position:", p
                print "StrokeId:", stroke_id
                raise

            result.append(tg)
        return result

    def configure_targets(self):

        try:
            config = self.best_config()
        except utils.StrokeError:
            configs = self.all_configs()
            print("CONFIGS FOR STROKE : {}".format(self.id))
            print(configs)
            raise

        for target in self.targets:
            target.configure(config)
        self.targets[0].linear = False  

    def send(self, prefix, program, frame, motion):
        link = robo.link()
        tool = link.Item(self.brush.name)
        if not tool.Valid():
            raise ClusterError(
                "SERIOUS RISK OF DAMAGE! Can't find valid tool!")

        stroke_name = self.name(prefix)
        program.RunInstruction("Stroke {}".format(stroke_name), INSTRUCTION_COMMENT)
        program.setPoseTool(tool)
        
        last_color = None
        for i, t in enumerate(self.targets):
            t.send(stroke_name, program, frame, last_color)
            last_color = t.color

        program.RunInstruction("End stroke {}. Set to black".format(
            stroke_name), INSTRUCTION_COMMENT)
        program.RunInstruction(
            "TRIGGER WHEN DISTANCE=0 DELAY=0 DO $ANOUT[1]=0.0", INSTRUCTION_INSERT_CODE)
        program.RunInstruction(
            "TRIGGER WHEN DISTANCE=0 DELAY=0 DO $ANOUT[2]=0.0", INSTRUCTION_INSERT_CODE)
        program.RunInstruction(
            "TRIGGER WHEN DISTANCE=0 DELAY=0 DO $ANOUT[3]=0.0", INSTRUCTION_INSERT_CODE)
        program.RunInstruction(
            "TRIGGER WHEN DISTANCE=0 DELAY=0 DO $ANOUT[4]=0.0", INSTRUCTION_INSERT_CODE)

    def query_backstroke(self):
        # print "SELF.NODE: ==------------------", self.node, "----------=="
        return pm.lightPaintingQuery(
            self.node,
            strokeIndex=self.id,
            strokeBackstroke=True,
        )

    def query_arc_length(self):
        return pm.lightPaintingQuery(
            self.node,
            strokeIndex=self.id,
            strokeArcLength=True,
        )

    def query_parent_id(self):
        return pm.lightPaintingQuery(
            self.node,
            strokeIndex=self.id,
            strokeParentIndex=True,
        )

    def query_linear_speed(self):
        return pm.lightPaintingQuery(
            self.node,
            strokeIndex=self.id,
            strokeSpeedLinear=True,
        )

    def query_angular_speed(self):
        return pm.lightPaintingQuery(
            self.node,
            strokeIndex=self.id,
            strokeSpeedAngular=True,
        )

    def query_positions(self):
        return utils.to_point_array(
            pm.lightPaintingQuery(
                self.node,
                strokeIndex=self.id,
                strokePositions=True
            )
        )

    def query_rotations(self):
        return utils.to_vector_array(
            pm.lightPaintingQuery(
                self.node,
                strokeIndex=self.id,
                strokeRotations=True,
                rotateOrder="zyx",
                rotateUnit="rad",
            )
        )

    def query_colors(self):
        return utils.to_rgba_array(
            pm.lightPaintingQuery(
                self.node,
                strokeIndex=self.id,
                strokeColors=True
            )
        )
 
        # Now we want to make thge first color black, and all the others a blend.
        # result = [pm.dt.Color(0.0,0.0,0.0,0.0)]
        # for i in range(0, len(colors)-1):
        #     result.append(colors[i])
        # return result
