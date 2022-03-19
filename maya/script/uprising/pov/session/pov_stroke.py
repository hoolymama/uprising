from robolink import INSTRUCTION_COMMENT

from uprising.pov.session.pov_target import PovTarget
import pymel.core as pm
from uprising import robo
from uprising import utils
from uprising.common.session.stroke import Stroke
from uprising.pov.session import pov_lights


class PovStroke(Stroke):
    def __init__(self, painting, stroke_id):
        super(PovStroke, self).__init__(painting, stroke_id)

    def _build_targets(self):

        positions = self.query_positions()
        rotations = self.query_rotations()
        colors = self.query_colors()
        waits = self.query_waits()
        self.targets = self._target_factory(positions, rotations, colors, waits, self.id)

    @classmethod
    def _target_factory(cls, positions, rotations, colors, waits, stroke_id):
        result = []
        num_targets = len(positions)
        if not (
            num_targets == len(rotations)
            and num_targets == len(colors)
            and num_targets == len(waits)
        ):
            raise utils.StrokeError("Length mismatch: positions, rotations, colors, waits")

        for i, (p, r, c, w) in enumerate(zip(positions, rotations, colors, waits)):
            try:
                if i == 0:
                    tg = PovTarget(i, (p * 10), r, pm.dt.Color(0.0, 0.0, 0.0, 0.0), w)
                    tg.linear = False
                    result.append(tg)
                target = PovTarget(i, (p * 10), r, c, w)
            except utils.StrokeError:
                print(("Target Position:", p))
                print(("StrokeId:", stroke_id))
                raise

            result.append(target)
        return result

    def send(self, prefix, program, frame, run_on_robot):

        stroke_name = self.name(prefix)
        program.RunInstruction("Stroke {}".format(stroke_name), INSTRUCTION_COMMENT)
        last_color = None

        if self.override_path_parameters:
            program.setSpeed(self.linear_speed)
            program.setSpeedJoints(self.angular_speed)
            program.setRounding(self.approximation_distance)

        print(("Sending stroke {}".format(stroke_name)))
        for i, t in enumerate(self.targets):
            t.send(stroke_name, program, frame, last_color, run_on_robot)
            last_color = t.color

        if self.override_path_parameters:
            program.setSpeed(self.painting.linear_speed)
            program.setSpeedJoints(self.painting.angular_speed)
            program.setRounding(self.painting.rounding)

        pov_lights.send_lights_off(program, run_on_robot)

    def query_arc_length(self):
        return pm.lightPaintingQuery(
            self.painting.node,
            strokeIndex=self.id,
            strokeArcLength=True,
        )

    def query_parent_id(self):
        return pm.lightPaintingQuery(
            self.painting.node,
            strokeIndex=self.id,
            strokeParentIndex=True,
        )

    def query_linear_speed(self):
        return pm.lightPaintingQuery(
            self.painting.node,
            strokeIndex=self.id,
            strokeSpeedLinear=True,
        )

    def query_angular_speed(self):
        return pm.lightPaintingQuery(
            self.painting.node,
            strokeIndex=self.id,
            strokeSpeedAngular=True,
        )

    def query_approximation_distance(self):
        return (
            pm.lightPaintingQuery(
                self.painting.node,
                strokeIndex=self.id,
                strokeApoproximationDistance=True,
            )
            * 10
        )

    def query_positions(self):
        return utils.to_point_array(
            pm.lightPaintingQuery(self.painting.node, strokeIndex=self.id, strokePositions=True)
        )

    def query_rotations(self):
        return utils.to_vector_array(
            pm.lightPaintingQuery(
                self.painting.node,
                strokeIndex=self.id,
                strokeRotations=True,
                rotateOrder="zyx",
                rotateUnit="rad",
            )
        )

    def query_colors(self):
        return utils.to_rgba_array(
            pm.lightPaintingQuery(self.painting.node, strokeIndex=self.id, strokeColors=True)
        )

    def query_layer_id(self):
        return pm.lightPaintingQuery(self.painting.node, strokeIndex=self.id, strokeLayerId=True)

    def query_waits(self):
        return pm.lightPaintingQuery(self.painting.node, strokeIndex=self.id, strokeWaits=True)

    # def configure(self):

    #     try:
    #         config = self.best_config()
    #     except utils.StrokeError:
    #         configs = self.all_configs()
    #         raise

    #     for target in self.targets:
    #         target.configure(config)
    #     self.targets[0].linear = False

    # def best_config(self):
    #     name = self.name("-")
    #     if not self.targets:
    #         print("Stroke has no targets {}".format(name))
    #         return
    #     common_configs = set([k for k in robo.ALL_KR8_CONFIGS])

    #     for target in self.targets:
    #         common_configs = common_configs.intersection(
    #             target.valid_configs())
    #         if not common_configs:
    #             raise utils.StrokeError(
    #                 "Common Config failure, can't find best config for stroke. No common configs {}".format(name))

    #     common_configs = sorted(list(common_configs))

    #     print("Stroke {} has common_configs {} -- Testing linear moves".format(name, common_configs))
    #     config = self.test_linear_moves(common_configs)
    #     if not config:
    #         raise utils.StrokeError(
    #                 "Linear move failure, can't find config where all linear moves are possible {}".format(name))

    #     return config

    # def all_configs(self):
    #     result = []
    #     for target in self.targets:
    #         result.append(target.valid_configs())
    #     return result
