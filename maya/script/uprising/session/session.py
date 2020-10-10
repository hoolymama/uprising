import logging
import brush_utils as butl
import const as k
import palette_utils as putl
import props
import pymel.core as pm
import uprising_util as uutl
from brush import Brush
from paint import Paint
from program import (
    BoardCalibration,
    BrushHangProgram,
    DipProgram,
    HolderCalibration,
    MainProgram,
    ManualTriangulation,
    PapExerciseProgram,
    PerspexCalibration,
    PickAtHomeProgram,
    PickProgram,
    PlaceAtHomeProgram,
    PlaceProgram,
    PotCalibration,
    PotHandleExerciseProgram,
    RetardantProgram,
    WaterProgram,
)

logger = logging.getLogger("uprising")


class SessionError(Exception):
    pass

class Session(object):
    pass



def send_rack_geo():
    rack_context = pm.PyNode("RACK1_CONTEXT")
    with uutl.final_position(rack_context):
        geo = [pm.PyNode("rackTop")]
        props.send(geo)

def send_holder_geo():
    rack_context = pm.PyNode("RACK1_CONTEXT")
    with uutl.final_position(rack_context):
        geo = pm.ls("holders|*|holderTrans|lowResGeo")
        props.send(geo)

def build_all_pick_place_programs():
    return build_pick_place_programs("all"):

def build_used_pick_place_programs():
    return build_pick_place_programs("used"):
 
def build_pick_place_programs(brush_ids):
    with uutl.final_position(pm.PyNode("RACK1_CONTEXT")):
        gripper_geo = pm.PyNode("bpx_0_utility_B0_gripper_roundShape")
        gripper = Brush(0, gripper_geo.attr("outPaintBrush"))
        if not gripper:
            raise SessionError("No Gripper. Risk of damage. Can't continue.")

        packs = get_pick_place_packs(brush_ids)

        result = []
        for p in packs:
            pack = packs[p]
            pick_prg = PickProgram(gripper, pack)
            place_prg = PlaceProgram(gripper, pack)
            result += [pick_prg, place_prg]
        return result






def get_pick_place_packs(brush_ids="used"):
    result = {}
    if brush_ids == "all":
        bids = [int(n[-2:])
                for n in pm.ls("RACK1_CONTEXT|j1|rack|holders|holderRot*")]
    elif brush_ids == "used":
        painting = pm.PyNode("mainPaintingShape")
        dc = pm.paintingQuery(painting, dc=True)
        bids = set(dc[::2])
    elif brush_ids == "calibration":
        bids = [0]
    else:
        bids = brush_ids

    holders_node = pm.PyNode("RACK1_CONTEXT|j1|rack|holders")
    path_attributes = {
        "lin_speed": holders_node.attr("linearSpeed").get() * 10,
        "precision_lin_speed": holders_node.attr("precisionLinearSpeed").get() * 10,
        "ang_speed": holders_node.attr("angularSpeed").get(),
        "rounding": holders_node.attr("approximationDistance").get() * 10,
    }
    for bid in bids:
        key = "b{:02d}".format(bid)
        trans = "holderRot{:02d}|holderTrans".format(bid)
        result[key] = {
            "trans_node": pm.PyNode(trans),
            "brush_id": bid,
            "probe": pm.PyNode("{}|probe_loc".format(trans)),
            "pin": pm.PyNode("{}|pin_loc".format(trans)),
            "pin_ap": pm.PyNode("{}|pin_approach_loc".format(trans)),
            "clear": pm.PyNode("{}|clear_loc".format(trans)),
            "clear_ap": pm.PyNode("{}|clear_approach_loc".format(trans)),
        }
        result[key].update(path_attributes)
    return result
