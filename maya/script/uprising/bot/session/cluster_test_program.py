import pymel.core as pm
from uprising import robo
from uprising.common.session.program import Program
from uprising.common.session.target import Target
from robolink import (
    COLLISION_OFF
)

import copy
import robodk as rdk 

class ClusterTestProgram(Program):
    def __init__(self, name, cluster, motion, original_approach_height):

        super(ClusterTestProgram, self).__init__(name)
        self.cluster = cluster
        self.motion = motion
        self.cache = {
            "index":None,
            "arrivals": None,
            "departure": None
            }

        self.height_reduction = (original_approach_height - (1+(self.cluster.brush.active_bristle))) * 10 #mm

    @staticmethod
    def offset_target(approachTarget, boardTarget, dist):
        board_translate = pm.dt.Vector( rdk.Pose_2_TxyzRxyz(boardTarget.tool_pose)[0:3])
        approach_translate = pm.dt.Vector( rdk.Pose_2_TxyzRxyz(approachTarget.tool_pose)[0:3])
        offset = pm.dt.Vector( board_translate - approach_translate).normal() * dist
        approachTarget.tool_pose = approachTarget.tool_pose.Offset(*(list(offset)))
        return approachTarget



    def configure_internal(self, i):
        if i < 1 or i >= len(self.cluster.strokes):
            raise ValueError("Bad Stroke Index")
        self.current_index = i

        self.cache["index"] = i
        self.cache["arrivals"] = copy.deepcopy(self.cluster.strokes[i].arrivals)
        self.cache["departure"] = copy.deepcopy(self.cluster.strokes[i-1].departure)


        self.cluster.strokes[i].arrivals = self.cluster.strokes[i].arrivals[-1:]
        # now there's just one arrival
        self.cluster.strokes[i].arrivals[0].linear=True

        self.cluster.strokes[i].arrivals[0] = self.offset_target(
            self.cluster.strokes[i].arrivals[0], 
            self.cluster.strokes[i].targets[0],
            self.height_reduction
        )
        
        self.cluster.strokes[i-1].departure = self.offset_target(
           self.cluster.strokes[i-1].departure, 
           self.cluster.strokes[i-1].targets[-1],
           self.height_reduction
        )

    def undo(self):
        self.cluster.strokes[self.cache["index"]].arrivals = self.cache["arrivals"]
        self.cluster.strokes[self.cache["index"]-1].departure = self.cache["departure"]


    def send(self, **kw):
        super(ClusterTestProgram, self).send()
        frame = robo.create_frame("from_to_test_frame")
        self.cluster.brush.send()
        self.cluster.send(self.program, frame, self.motion)


    def validate_path(self):
        return super(ClusterTestProgram, self).validate_path()
