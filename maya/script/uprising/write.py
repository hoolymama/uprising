import errno
import os
import sys
import re
import datetime
import pymel.core as pm
import uprising_util as uutl

from robolink import (Robolink, ITEM_TYPE_STATION,ITEM_TYPE_PROGRAM)
from studio import Studio
from paint import Paint
from brush import Brush

import setup_dip
import callbacks




def split_desc(desc):
    result = [None, None]
    lines = desc.splitlines()
    n = len(lines)
    if n:
        result[0] = lines[0]
    if n > 1:
        result[1] = "\n".join(lines[1:])
    return result





def publish_proposal(
        proposals_dir,
        painting_node,
        description,
        frame_range,
        clean_top):
    maya_scenes_dir = os.path.join(proposals_dir, "maya", "scenes")
    uutl.mkdir_p(maya_scenes_dir)
    timestamp = get_timestamp()

    write_maya_scene(maya_scenes_dir, timestamp)

    media_dir = os.path.join(proposals_dir, "media", timestamp)
    uutl.mkdir_p(media_dir)
    # title, body = split_desc(description)

    write_info(
        painting_node,
        proposals_dir,
        timestamp,
        frame_range,
        description)

    write_image_sequence(
        painting_node,
        media_dir,
        timestamp,
        frame_range,
        clean_top)


def run_hook(code):
    if not code:
        return
    kw = {
        "frame": pm.currentTime(q=True),
        "painting_node": pm.PyNode("mainPaintingShape"),
        "dip_node": pm.PyNode("dipPaintingShape")
    }

    args = code.split(",")
    cmd = args[0]
    args = [uutl.numeric(a.strip()) for a in args[1:]]
    method = getattr(callbacks, cmd)
    res = method(*args, **kw)
    print res


def publish_sequence(
    export_dir,
    frame_range,
    pause,
    wait,
    save_unfiltered_snapshot
):

    # print dip_wipe_packs

    recordings_dir = os.path.join(export_dir, "recordings")
    uutl.mkdir_p(recordings_dir)
    design_dir = os.path.join(export_dir, "design")
    uutl.mkdir_p(design_dir)

    if save_unfiltered_snapshot:

        filterable_nodes = pm.ls(type=["strokeNode", "collectStrokes"])

        with uutl.filters_off(*filterable_nodes):
            ts = get_timestamp()
            write_ref_image(design_dir, ts)

    for frame in range(frame_range[0], frame_range[1] + 1):
        pm.currentTime(frame)
 
        timestamp = get_timestamp(frame)
        ts_dir = get_ts_dir(export_dir, timestamp)
        uutl.mkdir_p(ts_dir)
 
        write_maya_scene(ts_dir, timestamp)
        write_csv(export_dir, timestamp)

        write_ref_image(ts_dir, timestamp)

        use_gripper = pm.optionVar.get("upov_tool_type") == "gripper"
        RL = Robolink()
        # clean_rdk()
        studio = Studio(
            do_painting=True, 
            do_dips=True,
            do_pick_and_place=use_gripper,
            pause=pause,
            wait=wait)
    
        studio.write()
        write_program(RL, ts_dir, "px", timestamp)

        # write_log(
        #     ts_dir,
        #     timestamp,
        #     frame)

        # publish_robodk_painting( ts_dir, timestamp, auto)

 
def choose_publish_dir():
    export_dir = os.path.join(pm.workspace.getPath(), 'export')
    entries = pm.fileDialog2(caption="Choose directory", okCaption="Save",
                             dialogStyle=2, fileMode=3, dir=export_dir)
    if not entries:
        pm.displayWarning('Nothing Selected')
        return
    return entries[0]


def get_calibration_dir():
    result = os.path.join(pm.workspace.getPath(), 'export', 'calibrations')
    uutl.mkdir_p(result)
    return result


def choose_proposal_dir():
    export_dir = os.path.join(pm.workspace.getPath(), 'proposals')
    entries = pm.fileDialog2(caption="Choose directory", okCaption="Save",
                             dialogStyle=2, fileMode=3, dir=export_dir)
    if not entries:
        pm.displayWarning('Nothing Selected')
        return
    return entries[0]


def get_timestamp(suffix=None):
    timestamp = datetime.datetime.now().strftime('%y%m%d_%H%M')

    if suffix is not None:
        timestamp = "%s_%s" % (timestamp, suffix)
    return timestamp


def get_ts_dir(containing_dir, timestamp):
    return os.path.join(containing_dir, timestamp)


def write_csv(export_dir, timestamp):

    fn = os.path.join(export_dir, "session_entries.csv")
    export_root = os.path.join(pm.workspace.getPath(), 'export')
    folder = export_dir.replace(export_root, "")
    folder = folder.strip("/")
    line = (",").join([folder,
                       "ID",
                       timestamp,
                       "description",
                       "acrylic",
                       "canvas board",
                       "1",
                       "LINK",
                       "Waiting",
                       "notes"])

    with open(fn, 'a+') as the_file:
        the_file.write(line)
        the_file.write("\n")


def write_ref_image(dest_dir, timestamp, res=1024):

    # TODO - make sure its active panel and
    f = pm.currentTime(q=True)
    fn = os.path.join(dest_dir, timestamp)
    pm.playblast(
        startTime=f,
        endTime=f,
        format="image",
        filename=fn,
        sequenceTime=0,
        clearCache=True,
        viewer=False,
        showOrnaments=False,
        offScreen=True,
        fp=4,
        percent=100,
        compression="tif",
        quality=100,
        widthHeight=(res, res))


def write_image_sequence(
        painting_node,
        media_dir,
        timestamp,
        frame_range,
        clean_top,
        **kw):
    # f = pm.currentTime(q=True)
    # frame_range = kw.get("frame_range",  (f,f))
    resolution = kw.get("resolution", 1024)
    resolutionX = kw.get("resolutionX", resolution)
    resolutionY = kw.get("resolutionY", resolution)

    print "painting_node %s" % painting_node
    print "media_dir %s" % media_dir
    print "timestamp %s" % timestamp
    print "clean_top %s" % clean_top
    print "resolutionX %d" % resolutionX
    print "resolutionY %d" % resolutionY


def setup_clean_top(painting_node):
    pass


def write_program(RL, ts_dir, progname, timestamp):
    print "XXXXXXXXXXXXXXXX  TS_DIR", ts_dir
    RL = Robolink()

    sel = RL.Selection()
    for item in sel:
        print "Item:", item.Name()

 
    # print "WRITING"
    prog_filename = "%s_%s" % (progname.upper(), timestamp)
    print "prog_filename", prog_filename
    program = RL.Item(progname, ITEM_TYPE_PROGRAM)
    print "program" , program
    print "program.Name()" , program.Name()
    program.setName(prog_filename)
    program.MakeProgram(ts_dir)
    program.setName(progname)
    print "ts_dir" , ts_dir
    
    if program.Valid():
        print "VALID" 
        # 
        # program.setName(prog_filename)
        # program.MakeProgram(ts_dir)
        # program.setName(progname)
    else:
        print "INVALID" 


def write_station(RL, ts_dir, timestamp):
    station = RL.Item("", ITEM_TYPE_STATION)
    station.setName(timestamp)
    RL.Save(os.path.join(ts_dir, "%s.rdk" % timestamp))


def write_maya_scene(ts_dir, timestamp):
    new_name = os.path.join(ts_dir, "%s.ma" % timestamp)
    orig_sn = pm.sceneName()
    pm.saveAs(new_name)
    pm.renameFile(orig_sn)


def used_paints_and_brushes(painting_node):
    dc = pm.paintingQuery(painting_node, dc=True)
    bids =  dc[::2]
    pids =  dc[1::2]
    paints = Paint.paints(painting_node)
    brushes = Brush.brushes(painting_node)
    used_paints = [paints[_id] for _id in pids]
    used_brushes = [brushes[_id] for _id in bids]
    return zip(used_brushes, used_paints)


def painting_stats(node):
    cluster_count = pm.paintingQuery(node, cc=True)
    stroke_count = 0
    reason_result = {"dip": 0, "tool": 0, "tcp": 0}
    travel = []
    for i in range(cluster_count):
        stroke_count += pm.paintingQuery(node, ci=i, sc=True)
        travel.append(pm.paintingQuery(node, ci=i, clusterTravel=True))
        reason = pm.paintingQuery(node, ci=i, clusterReason=True)
        reason_result[reason] += 1
    avg_strokes_per_cluster = stroke_count / float(cluster_count)
    total_travel = sum(travel)
    avg_travel_per_cluster = total_travel / float(cluster_count)
    result = {
        "cluster_count": cluster_count,
        "stroke_count": stroke_count,
        "avg_strokes_per_cluster": avg_strokes_per_cluster,
        "tool_changes": reason_result["tool"],
        "dip_only changes": reason_result["dip"],
        "tcp_only_changes": reason_result["tcp"],
        "total_stroke_travel": total_travel,
        "avg_stroke_travel_per_cluster": avg_travel_per_cluster
    }
    return result
 
def used_brushes(painting_node):
    ids = pm.paintingQuery(painting_node, dc=True)[0::2]
    ids = sorted(set(ids))
    brushes = Brush.brushes(painting_node)
    return [brushes[_id] for _id in ids]

def used_paints(painting_node):
    ids = pm.paintingQuery(painting_node, dc=True)[1::2]
    ids = sorted(set(ids))
    paints = Paint.paints(painting_node)
    return [paints[_id] for _id in ids]


 

def write_log( ts_dir, timestamp, frame):

    painting_node = pm.PyNode("mainPaintingShape")
    dip_combos = setup_dip.dip_combinations()
 
    pnt_stats = painting_stats(painting_node)
    # dip_stats = painting_stats(dip_node)

    # paints_in_use = used_paints(painting_node)
    brush_paint_pairs = used_paints_and_brushes(painting_node)

    log_file = os.path.join(ts_dir, "log.txt")
    with open(log_file, 'w') as the_file:

        the_file.write("Timestamp: %s\n\n" % timestamp)

        the_file.write("Frame number: %s\n\n" % frame)

        the_file.write("Painting node stats:\n")
        for k in pnt_stats:
            the_file.write("%s: %s\n" % (k, pnt_stats[k]))
        the_file.write("\n")

        the_file.write("\n")
        the_file.write("Brush + aint pairs:\n")
        for brush, paint in brush_paint_pairs:
            the_file.write("{}({:02d}) + {}({:02d})".format(brush.node_name, brush.id, paint.name, paint.id))
        the_file.write("\n")

        the_file.write("\n")
        the_file.write("Used brushes:\n")
        for brush in used_brushes(painting_node):
            the_file.write("{}({:02d})".format(brush.node_name, brush.id))
 
        the_file.write("\n")
        the_file.write("Used paints:\n")
        for paint in used_paints(painting_node):
            the_file.write("{}({:02d})".format(paint.name, paint.id))
 
def write_info(
        painting_node,
        proposals_dir,
        timestamp,
        frame_range,
        description):
    info_file = os.path.join(proposals_dir, ("%s.txt" % timestamp))
    title, body = split_desc(description)

    start, end = frame_range

    with open(info_file, 'w') as the_file:
        the_file.write("%s\n\n" % title)
        the_file.write("%s\n\n" % body)
        the_file.write("Timestamp: %s\n\n" % timestamp)
        the_file.write("Frame range: %d to %d\n\n" % (start, end))


# def publish_package():
#     export_dir = os.path.join(pm.workspace.getPath(), 'export')
#     entries = pm.fileDialog2(caption="Choose package", okCaption="Publish",
#                              dialogStyle=2, fileMode=3, dir=export_dir)
#     if not entries:
#         pm.displayWarning('Nothing Selected')
#         return

#     export_dir = entries[0]

#     # timestamp = datetime.datetime.now().strftime('%y%m%d_%H%M')
#     # ts_dir = os.path.join(export_dir, timestamp)

#     # simulation = os.path.join(ts_dir, "%s.html" % timestamp)
#     recordings_dir = os.path.join(ts_dir, "sessions")
#     mkdir_p(recordings_dir)

#     write_maya_scene(ts_dir, timestamp)
#     write_ref_image(ts_dir, timestamp)
#     write_log(ts_dir, timestamp, description)
#     write_station(RL, ts_dir, timestamp)
#     write_program(RL, ts_dir, timestamp)
