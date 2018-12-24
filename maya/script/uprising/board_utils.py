import pymel.core as pm
import stroke_factory_utils as sfu
from brush import Brush
from robolink import (Robolink, ITEM_TYPE_ROBOT)
import uprising.uprising_util as uutl
import robodk as rdk
import sheets
 
################# SETUP ##################
def validate_board_data(data):
    if not len(data):
        raise ValueError("No board data from Google sheets")
    for row in data:
        # if row[0].startswith("rack"):
        #     if not len(row) > 3:
        #         raise ValueError("Invalid rack corner data from Google sheets")
        # if uutl.numeric(row[0]) in range(8):
        if not len(row) > 3:
            raise ValueError("Invalid board data from Google sheets")


def get_current_corner(locname, assembly):
    node = pm.PyNode("%s|%s" % (assembly, locname))
    return {"node": node, "current_pos": node.attr("translate").get()}


def setup_board_from_sheet(node, name, depth_only):
    # (palette_name, medium, palette) =  get_palette_by_name(palette_name)
    (name, ground, data) = sheets.get_resource_by_name(name, "Board")
    assembly = uutl.assembly(node)
    canvas = pm.PyNode("%s|jpos|canvas" % assembly)
    _, ground_att, _, _ = sfu.ensure_painting_has_notes(assembly)
    ground_att.set(ground)

    corners = {}
 
    validate_board_data(data)

    for row in data:
        row = [uutl.numeric(s) for s in row]
        key = row[0]
        corners[key] = get_current_corner(key, assembly)
        pos = [v * 0.1 for v in row[1:4]]
        corners[key]["new_pos"] = pm.dt.Vector(pos)

    # print corners

    if not depth_only:
        for k in corners:
            corners[k]["node"].attr("translate").set(*corners[k]["new_pos"])
    else:
        # need to figure out the depth at the corners
        width = canvas.attr("width").get()
        height = canvas.attr("height").get()

        up = (corners["TL"]["current_pos"] -
              corners["BL"]["current_pos"]).normal()
        right = (
            corners["TR"]["current_pos"] -
            corners["TL"]["current_pos"]).normal()

        new_width = (
            corners["TR"]["new_pos"] -
            corners["TL"]["new_pos"]).length()
        new_height = (
            corners["TL"]["new_pos"] -
            corners["BL"]["new_pos"]).length()

        diff_width = width - new_width
        diff_height = height - new_height

        new_TL = corners["TL"]["new_pos"] + \
            (up * diff_height * 0.5) + (-right * diff_width * 0.5)
        new_BL = corners["BL"]["new_pos"] + \
            (-up * diff_height * 0.5) + (-right * diff_width * 0.5)
        new_TR = corners["TR"]["new_pos"] + \
            (up * diff_height * 0.5) + (right * diff_width * 0.5)
        # new_BR = corners["BR"]["new_pos"] + \
        #     (-up * diff_height * 0.5) + (right * diff_width * 0.5)

        # these are now potentioally good corners - however, we only set the
        # new depths
        corners["TL"]["node"].attr("translateY").set(new_TL.y)
        corners["BL"]["node"].attr("translateY").set(new_BL.y)
        corners["TR"]["node"].attr("translateY").set(new_TR.y)
        # corners["BR"]["node"].attr("translateY").set(new_BR.y)

        # tmp_locs = {}
        # tmp_locs["BL"] = pm.spaceLocator()
        # tmp_locs["BL"].rename("BL_tmp")
        # tmp_locs["TL"] = pm.spaceLocator()
        # tmp_locs["TL"].rename("TL_tmp")
        # tmp_locs["TR"] = pm.spaceLocator()
        # tmp_locs["TR"].rename("TR_tmp")
        # tmp_locs["BR"] = pm.spaceLocator()
        # tmp_locs["BR"].rename("BR_tmp")

        # for k in corners:
        #     tmp_locs[k].attr("translate").set(*corners[k]["new_pos"])


def _verts(disp_mesh):
    try:
        verts = [item for sublist in  pm.sets("probePointsSet", q=True) for item in sublist]
    except pm.MayaNodeError:
        verts = disp_mesh.vtx
    return verts


def _get_probes_group(assembly):
    """Get or make the group to hold probes"""
    try:
        g = pm.PyNode("%s|jpos|probes" % assembly)
        pm.delete(g)
    except pm.MayaNodeError:
        pass
    g = pm.group(em=True)
    pm.parent(g, ("%s|jpos"% assembly), relative=True)
    g.rename("probes")
    return g

def generate_probes(node, offset, approach_dist):
    assembly = uutl.assembly(node)
    disp_mesh = pm.listConnections(node.attr("displacementMesh"), s=True, d=False)[0]
    probes_group = _get_probes_group(assembly)

    if probes_group.getChildren():
        pm.delete(probes_group.getChildren())

 
 
    with uutl.zero_position(node):
        for v in  _verts(disp_mesh):
            index = int(str(v).split("[")[1].split("]")[0])

            pos = v.getPosition(space="world")
            pos.z=0
            probe = pm.group(em=True)
            probe.rename("probe_%d" % index)
            probe.attr("t").set(pos)
            pm.parent(probe, probes_group)
            probe.attr("r").set(0,180,0)
            base = pm.spaceLocator()
            approach = pm.spaceLocator()
            actual = pm.spaceLocator()
            pm.parent(base, approach, actual, probe, relative=True)
            base.rename("base")
            approach.rename("approach")
            actual.rename("actual")
            approach.attr("tz").set(-approach_dist)
        probes_group.attr("tz").set(offset)
            
    # parent = pm.PyNode("%s|jpos|probes")
    # for o in pm.ls(sl=True):
    # if o.attr("brushId").get() == 10:
    #      o.attr("brushId").set(9)

def validate_calibration_data(data):
    if not len(data):
        raise ValueError("No calibration data from Google sheets")
    for row in data:
        if not len(row) > 1:
            raise ValueError("Invalid calibration data from Google sheets")


def set_board_vertices_from_sheets(painting_node, name):
    assembly= uutl.assembly(painting_node)
    probes_group = pm.PyNode("%s|jpos|probes" % assembly)

    probes_offset = probes_group.attr("tz").get()
    disp_mesh = pm.listConnections(painting_node.attr("displacementMesh"), s=True, d=False)[0]
    # verts = disp_mesh.vtx

    # (palette_name, medium, palette) =  get_palette_by_name(palette_name)
    calibration_name, board, data = sheets.get_resource_by_name(name, "Calibration")
    validate_calibration_data(data)

    probes = probes_group.getChildren()
    entries = [(
     int(uutl.numeric(row[0])) ,  
     (uutl.numeric(row[1]) * 0.1 )
     ) for row in data ]

    if not len(entries) == len(probes):
        raise IndexError("Number of values in sheet doesn't probes count %d <> %d" % (len(entries), len(probes)))

    for index, value in entries:
        probe =  pm.PyNode("%s|jpos|probes|probe_%d" % (assembly, index))
        vtx = disp_mesh.vtx[index]
        pos = vtx.getPosition(space="world")
        pos.z = (probes_offset + value)
        vtx.setPosition(pos, space="world")
        pm.PyNode("%s|actual" %probe).attr("tz").set(-value)


