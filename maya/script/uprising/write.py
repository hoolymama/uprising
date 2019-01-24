import errno
import os
import sys
import re
import datetime
import pymel.core as pm
import uprising_util as uutl

from robolink import (Robolink, ITEM_TYPE_STATION)
from studio import Studio
from paint import Paint
from brush import Brush

import setup_dip
import callbacks


CLEAN_FILE = "/Users/julian/projects/robot/stations/clean.rdk"


def split_desc(desc):
    result = [None, None]
    lines = desc.splitlines()
    n = len(lines)
    if n:
        result[0] = lines[0]
    if n > 1:
        result[1] = "\n".join(lines[1:])
    return result


def clean_rdk():
    pass
    # RL = Robolink()
    # for station in RL.getOpenStations():
    #     station.Delete()
    # RL.AddFile(CLEAN_FILE)




def publish_proposal(
        proposals_dir,
        painting_node,
        description,
        frame_range,
        clean_top):
    maya_scenes_dir = os.path.join(proposals_dir, "maya", "scenes")
    mkdir_p(maya_scenes_dir)
    timestamp = get_timestamp()

    write_maya_scene(maya_scenes_dir, timestamp)

    media_dir = os.path.join(proposals_dir, "media", timestamp)
    mkdir_p(media_dir)
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
 

def _get_dip_wipe_packs():
    result = {}
    racks = ["rack1"]
    paint_regex = re.compile(r"^.*_(\d+)\|holeTrans$")
    brush_regex = re.compile(r"^.*\|holeTrans\|.*\|b(\d+)\|.*$")

    holes = pm.ls("rack*|holes|holeRot_*|holeTrans")
    for hole in holes:
        paint_id = "p{}".format(paint_regex.match(str(hole)).groups()[0])

        result[paint_id] = {}
        dip_shapes = pm.ls(
            "{}|dip_loc|*".format(holes[0]), dag=True, leaf=True, type="painting")
        wipe_shapes = pm.ls(
            "{}|wipe_loc|*".format(holes[0]), dag=True, leaf=True, type="painting")
        dip_wipes = zip(dip_shapes, wipe_shapes)
        for dw in dip_wipes:
            brush_id = "b{}".format(
                brush_regex.match(str(dw[0])).groups()[0])
            result[paint_id][brush_id] = {
                "dip": dw[0],
                "wipe": dw[1],
                "name": "{}_{}".format(paint_id, brush_id)
            }
    return result

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
        painting_node,
        dip_wipe_packs,
        description,
        medium,
        ground,
        frame_range,
        do_painting,
        do_verify,
        save_unfiltered_snapshot,
        pre_frame_py):
    # RL = Robolink()
    # clean_rdk()
    # print "x" * 20
    # print "painting_node: %s" % painting_node
    # print "dip_node: %s" % dip_node

    painting_node = pm.PyNode("mainPaintingShape")
    dip_wipe_packs = _get_dip_wipe_packs()


    recordings_dir = os.path.join(export_dir, "recordings")
    mkdir_p(recordings_dir)
    design_dir = os.path.join(export_dir, "design")
    mkdir_p(design_dir)

    if save_unfiltered_snapshot:

        filterable_nodes= pm.ls(type=["strokeNode", "collectStrokes"])


        with uutl.filters_off(*filterable_nodes):
            ts = get_timestamp()
            write_ref_image(design_dir, ts)


    for frame in range(frame_range[0], frame_range[1] + 1):
        pm.currentTime(frame)
        run_hook(pre_frame_py)
        timestamp = get_timestamp(frame)
        ts_dir = get_ts_dir(export_dir, timestamp)
        mkdir_p(ts_dir)
        desc, notes = split_desc(description.replace("#f", str(frame)))
        # print "Desc: %s" % desc
        # print "Notes: %s" % notes

        setup_dip.doit()
        write_maya_scene(ts_dir, timestamp)
        write_csv(export_dir, timestamp, desc, notes, medium, ground)

        write_ref_image(ts_dir, timestamp)

        write_log(
            painting_node,
            dip_wipe_packs,
            ts_dir,
            timestamp,
            desc,
            notes,
            frame)

        if do_painting or do_verify:
            kw = {}
            if do_painting:
                kw["painting_node"] = painting_node
                kw["dip_wipe_packs"] = dip_wipe_packs
            if do_verify:
                kw["verification_node"] = painting_node

            publish_robodk_painting(ts_dir, timestamp, **kw)


def publish_robodk_painting(ts_dir, timestamp, **kw):
    RL = Robolink()
    clean_rdk()

    with uutl.minimize_robodk():
        studio = Studio(**kw)
        studio.write()
    write_station(RL, ts_dir, timestamp)

    if kw.get("painting_node"):
        write_program(RL, ts_dir, "px", timestamp)
    if kw.get("verification_node"):
        write_program(RL, ts_dir, "vx", timestamp)

def publish_calibration_program(directory, node):
    RL = Robolink()
    calibration_dir = os.path.join(directory, "calibrations")
    

    clean_rdk()
    timestamp = get_timestamp()
    ts_dir = get_ts_dir(calibration_dir, timestamp)
    mkdir_p(ts_dir)
    with uutl.minimize_robodk():
        studio = Studio(calibration_node=node)
        studio.write()
    write_program(RL, ts_dir, "xx", timestamp)

def mkdir_p(path):
    try:
        os.makedirs(path)
    except OSError as exc:
        if exc.errno == errno.EEXIST and os.path.isdir(path):
            pass
        else:
            raise

def choose_publish_dir():
    export_dir = os.path.join(pm.workspace.getPath(), 'export')
    entries = pm.fileDialog2(caption="Choose directory", okCaption="Save",
                             dialogStyle=2, fileMode=3, dir=export_dir)
    if not entries:
        pm.displayWarning('Nothing Selected')
        return
    return entries[0]

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

    if suffix:
        timestamp = "%s_%s" % (timestamp, suffix)
    return timestamp

def get_ts_dir(containing_dir, timestamp):
    return os.path.join(containing_dir, timestamp)

def write_csv(export_dir, timestamp, description, notes, medium, ground):
    if description:
        description = (" ").join(description.replace(",", " - ").splitlines())
    else:
        description = "none"
    if notes:
        notes = (" ").join(notes.replace(",", " - ").splitlines())
    else:
        notes = "none"
    fn = os.path.join(export_dir, "session_entries.csv")
    export_root = os.path.join(pm.workspace.getPath(), 'export')
    folder = export_dir.replace(export_root, "")
    folder = folder.strip("/")
    line = (",").join([folder,
                       "ID",
                       timestamp,
                       description,
                       medium,
                       ground,
                       "1",
                       "LINK",
                       "Waiting",
                       notes])

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
    prog_filename = "%s_%s" % (progname.upper(), timestamp)
    program = RL.Item(progname)
    if program.Valid():
        program.setName(prog_filename)
        program.MakeProgram(ts_dir)
        program.setName(progname)


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
    bids = sorted(set(dc[::2]))
    pids = sorted(set(dc[1::2]))
    paints = Paint.paints(painting_node)
    brushes = Brush.brushes(painting_node)
    used_paints =  [paints[_id] for _id in pids]
    used_brushes =  [brushes[_id] for _id in bids]
    return  (used_paints, used_brushes)


def painting_stats(node):
    cluster_count = pm.paintingQuery(node, cc=True)
    stroke_count = 0
    reason_result = {"dip": 0, "tool": 0, "tcp": 0}
    travel = []
    for i in range(cluster_count):
        stroke_count += pm.paintingQuery(node, ci=i, sc=True)
        travel.append(pm.paintingQuery(node, ci=i, clusterTravel=True) )
        reason = pm.paintingQuery(node, ci=i, clusterReason=True)
        reason_result[reason] += 1
    avg_strokes_per_cluster = stroke_count / float(cluster_count)
    total_travel =  sum(travel)
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





# def _painting_stats(node):
#     cluster_count = pm.paintingQuery(node, cc=True)
#     stroke_count = 0
#     reason_result = {"dip": 0, "tool": 0, "tcp": 0}
#     for i in range(cluster_count):
#         stroke_count += pm.paintingQuery(node, ci=i, sc=True)
#         reason = pm.paintingQuery(node, ci=i, clusterReason=True)
#         reason_result[reason] += 1
#     strokes_per_cluster = stroke_count / float(cluster_count)

#     result = {
#         "Cluster count": cluster_count,
#         "Stroke count": stroke_count,
#         "Strokes per cluster": strokes_per_cluster,
#         "Tool changes": reason_result["tool"],
#         "Dip only changes": reason_result["dip"],
#         "Tcp only changes": reason_result["tcp"]
#     }
#     return result

def _used_paints(painting_node):
    ids = pm.paintingQuery(painting_node, dc=True)[1::2]
    ids = sorted(set(ids))
    paints = Paint.paints(painting_node)
    return [paints[_id] for _id in ids]

def write_log(
        painting_node,
        dip_node,
        ts_dir,
        timestamp,
        description,
        notes,
        frame):

    dip_combos = setup_dip.dip_combinations(painting_node)

    pnt_stats = painting_stats(painting_node)
    dip_stats = painting_stats(dip_node)

    paints_in_use = _used_paints(painting_node)
    paints_in_use, brushes_in_use = used_paints_and_brushes(painting_node)


    log_file = os.path.join(ts_dir, "log.txt")
    with open(log_file, 'w') as the_file:
        the_file.write("Description:\n")
        the_file.write("%s\n\n" % description)
        the_file.write("Notes:\n")
        the_file.write("%s\n\n" % notes)

        the_file.write("Timestamp: %s\n\n" % timestamp)

        the_file.write("Frame number: %s\n\n" % frame)

        the_file.write("Painting node stats:\n")
        for k in pnt_stats:
            the_file.write("%s: %s\n" % (k, pnt_stats[k]))
        the_file.write("\n")

        the_file.write("\n")
        the_file.write("Dip node stats:\n")
        for k in dip_stats:
            the_file.write("%s: %s\n" % (k, dip_stats[k]))
        the_file.write("\n")

        the_file.write("Paints in use:\n")
        for paint in paints_in_use:
            the_file.write("%s\t:%s\n" % (paint.id, paint.name))
        the_file.write("\n")

 
        the_file.write("Brushes in use:\n")
        for brush in brushes_in_use:
            the_file.write("%s\t:%s\n" % (brush.id, brush.name))
        the_file.write("\n")

        the_file.write("Dip combinations:\n")
        for k in dip_combos:
            the_file.write("%s\n" % k)
        the_file.write("\n")

        # err_str = "TRUE" if write_error else "FALSE"
        # the_file.write("Errors while writing: %s\n" % err_str)


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
