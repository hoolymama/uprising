import robodk as rdk

from target import Target

from uprising_util import StrokeError


def config_key(config):
    if config:
        return "%d%d%d%d" % tuple(config.list2()[0][0:4])


class Stroke(object):

    def __init__(
            self,
            robot,
            positions,
            rotations,
            tangents,
            brush,
            paint,
            arc_length,
            planeNormal,
            approach,
            curve_name,
            curve_stroke_id):
        self.arc_length = arc_length
        self.common_configs = []
        self.valid_config = None
        self.targets = []
        self.curve_name = curve_name
        self.curve_stroke_id = curve_stroke_id

        ps = [(positions[0] + approach)] + \
            positions + [(positions[-1] + approach)]
        rs = [rotations[0]] + \
            rotations + [rotations[-1]]
        ts = [tangents[0]] + \
            tangents + [tangents[-1]]

        for i in range(len(ps)):
            # will throw if no solutions
            t = Target(i, (ps[i] * 10), rs[i], ts[i], robot, brush, planeNormal)
            self.targets.append(t)

    def identifier(self):
        return "%s_%d" % (self.curve_name, self.curve_stroke_id)

    def configure(self, robot, brush, ref_joint_pose):

        # print "Stroke.configure() ------------------------------- %s" % self.identifier()
        self.common_configs = self.get_common_configs()
        if not self.common_configs:
            raise StrokeError("Cant find any common configs")

        found = False
        # try the basic ref pose
        # print "1. try the basic ref pose - no flip"
        # found = self.configure_targets(
        #     robot, ref_joint_pose, allow_flip=False)

        # try start by looping through configurations

        # print self.common_configs
        # if not found:
        #     # print "2. try all configs - no flip"
        #     for config in self.common_configs:
        #         # print "Try %s " % config
        #         joint_pose = self.targets[0].configs[config][0]
        #         found = self.configure_targets(
        #             robot, joint_pose, allow_flip=False)
        #         if found:
        #             break

        # try global ref pose and allow flips
        # print "3. try the basic ref pose - with flip"

        # if not found:
        #     found = self.configure_targets(
        #         robot, ref_joint_pose, allow_flip=True)

        # try start by looping through configurations

        # if not found:
        #     # print "4. try all configs - with flip"
        #     for config in self.common_configs:
        #         joint_pose = self.targets[0].configs[config][0]
        #         found = self.configure_targets(
        #             robot, joint_pose, allow_flip=True)
        #         if found:
        #             break




        # ALTERNATIVE
        if not found:
            # print "4. try all configs - with flip"
            for config in self.common_configs:
                ref_pose = self.targets[0].configs[config][0]
                found = self.configure_targets(robot, ref_pose, config)
                if found:
                    break






        print "------------------- %s" % self.identifier()
        for t in self.targets:
            print t.configs

        if not found:
            raise StrokeError("Cannot find any stroke solution %s" % self.identifier())

    # def configure_targets(self, robot, ref_pose, **kw):
    #     allow_flip = kw["allow_flip"]
    #     last_pose = ref_pose
    #     last_mat = None
    #     for target in self.targets:
    #         try:
    #             target.configure(robot, last_mat, last_pose, allow_flip)
    #         except StrokeError:
    #             return False
    #         last_pose = target.joint_pose
    #         last_mat = target.tool_pose
    #     return True
 

    # ALTERNATIVE
    def configure_targets(self, robot, ref_pose, config):
        # allow_flip = kw["allow_flip"]
        # last_pose = ref_pose
        last_mat = None
        for target in self.targets:
            try:
                target.configure(robot, last_mat, ref_pose, config)
            except StrokeError:
                return False
            ref_pose = target.joint_pose
            last_mat = target.tool_pose
        return True
 


    def get_common_configs(self):
        result = []
        keys = self.targets[0].configs.keys()
        for key in keys:
            valid = True
            for target in self.targets:
                if key not in target.configs:
                    valid = False
                    break
            if valid:
                result.append(key)
        return result
