import errno
import tarfile

import os
import sys
import re
import datetime
import pymel.core as pm
import uprising_util as uutl

from robolink import (Robolink, ITEM_TYPE_STATION, ITEM_TYPE_PROGRAM)
from studio import Studio
from paint import Paint
from brush import Brush
import math




# import setup_dip
import palette_utils as putl
 
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

 

def publish_separate_files( directory, **kw):
    RL = Robolink()

    pause_gripper_ms = kw.get("pause_gripper_ms")
    first_dip_repeats = kw.get("first_dip_repeats")
    do_water_dip = kw.get("do_water_dip")
    do_retardant_dip = kw.get("do_retardant_dip")
    water_wipe_repeats = kw.get("water_wipe_repeats")
    pause_brushes = kw.get("pause_brushes")
   
    src = os.path.join(directory, "src")
    rdk = os.path.join(directory, "rdk")
    uutl.mkdir_p(src)
    uutl.mkdir_p(rdk)
 
    studio = Studio(
                do_painting=True,
                do_dips=True,
                do_water_dip=do_water_dip,
                do_retardant_dip=do_retardant_dip,
                water_wipe_repeats=water_wipe_repeats,
                pick_and_place_slots="used",
                first_dip_repeats=first_dip_repeats,
                pause=pause_gripper_ms,
                pause_brushes=pause_brushes
                )
    
    result = {"painting": [], "others": []}
    
    cluster_count = len(studio.painting_program.painting.clusters)
    cluster_chunk_size=kw.get("cluster_chunk_size", cluster_count)
    num_main_painting_chunks = int(math.ceil( cluster_count / float(cluster_chunk_size)))
    

    for i in range(num_main_painting_chunks):
        prep_clean(studio)
        program_name = studio.write_painting_program(i, cluster_chunk_size)
        save_prog_and_station(RL, src, rdk, program_name)
        result["painting"].append(program_name)

    count = len(studio.pick_place_programs)
    for i in range(count):
        prep_clean(studio)
        program_name = studio.write_pick_place_program(i)
        save_prog_and_station(RL, src, rdk, program_name)
        result["others"].append(program_name)
        print "Pick-Place {}/{}".format(i, count)

    count = len(studio.dip_programs)
    for i in range(count):
        prep_clean(studio)
        program_name = studio.write_dip_program(i)
        save_prog_and_station(RL, src, rdk, program_name)
        result["others"].append(program_name)
        print "Dips {}/{}".format(i, count)

    count = len(studio.water_programs)
    for i in range(count):
        prep_clean(studio)
        program_name = studio.write_water_program(i)
        save_prog_and_station(RL, src, rdk, program_name)
        result["others"].append(program_name)
        print "Water {}/{}".format(i, count)

    count = len(studio.retardant_programs)
    for i in range(count):
        prep_clean(studio)
        program_name = studio.write_retardant_program(i)
        save_prog_and_station(RL, src, rdk, program_name)
        result["others"].append(program_name)
        print "Retardant {}/{}".format(i, count)
    
    write_orchestrator(src, result["painting"])

    with tarfile.open("{}.tar.gz".format(src), "w:gz") as tar:
        tar.add(src, arcname=os.path.sep)


    return result

def write_orchestrator(directory, programs):
    uutl.mkdir_p(directory)
    orchestrator_file = os.path.join(directory, "main.src")
    with open(orchestrator_file, 'w') as ofile:
        ofile.write("&ACCESS RVP\n")
        ofile.write("&REL 1\n")
        ofile.write("&COMMENT Generated Uprising Robot Tools\n")
        ofile.write("&PARAM TEMPLATE = C:\\KRC\Roboter\\Template\\vorgabe\n")
        ofile.write("&PARAM EDITMASK = *\n")
        ofile.write("DEF pxMain ( )\n")
        for program in programs:
            ofile.write("{}( )\n".format(program))
        ofile.write("END\n")
    print "Wrote orchestrator file: {}".format(orchestrator_file)


def prep_clean(studio):
    uutl.clean_rdk()
    studio.write_approaches()


def save_prog_and_station(RL, src, rdk, program_name ):
    write_program(RL, src, program_name)
    write_station(RL, rdk, program_name)
    print "Wrote src and rdk: {}".format(program_name)



def publish_sequence(
    export_dir,
    frame_range,
    pause ,
    first_dip_repeats,
    do_water_dip,
    do_retardant_dip,
    **kw
):
    results = []
    painting_node = pm.PyNode("mainPaintingShape")
    recordings_dir = os.path.join(export_dir, "recordings")
    uutl.mkdir_p(recordings_dir)
    design_dir = os.path.join(export_dir, "design")
    uutl.mkdir_p(design_dir)
    prefix = kw.get("prefix", "prg")
 
    for frame in range(frame_range[0], frame_range[1] + 1):
        pm.currentTime(frame)

        name =  "{}_{:02d}".format(prefix, frame)
        # There can be an error if the painting contains no strokes.
        # In that case, we just want to skip the frame.
        try:
            pm.paintingQuery(painting_node, cc=True)
        except RuntimeError as ex:
            pm.displayWarning(ex.message)
            continue

        RL = Robolink()

        studio = Studio(
            do_painting=True,
            do_dips=True,
            do_water_dip=do_water_dip,
            do_retardant_dip=do_retardant_dip,
            water_wipe_repeats=kw.get("water_wipe_repeats"),
            pick_and_place_slots="used",
            first_dip_repeats=first_dip_repeats,
            do_rack_and_holder_geo=False,
            pause=pause,
            pause_brushes=kw.get("pause_brushes") 
            
            )

        studio.write()
        program_file = write_program(RL, export_dir, "px", name)
        results.append(program_file)

        write_station(RL, export_dir, name)

    return results

    # if first_timestamp:
    #     write_maya_scene(export_dir, first_timestamp)


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


def write_program(RL, directory, progname):
    RL = Robolink()
    program = RL.Item(progname, ITEM_TYPE_PROGRAM)
    program.MakeProgram(directory)
    return (progname,program.Valid())


def write_station(RL, directory, name):
    station = RL.Item("", ITEM_TYPE_STATION)
    station.setName(name)
    RL.Save(os.path.join(directory, "{}.rdk".format(name)))

def write_maya_scene(ts_dir, name):
    new_name = os.path.join(ts_dir, "{}.ma".format(name) )
    orig_sn = pm.sceneName()
    pm.saveAs(new_name)
    pm.renameFile(orig_sn)

def used_paints_and_brushes(painting_node):
    dc = pm.paintingQuery(painting_node, dc=True)
    bids = dc[::2]
    pids = dc[1::2]
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


def write_log(ts_dir, timestamp, frame):

    painting_node = pm.PyNode("mainPaintingShape")
    dip_combos = putl.dip_combinations()

    pnt_stats = painting_stats(painting_node)
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
            the_file.write(
                "{}({:02d}) + {}({:02d})".format(brush.node_name, brush.id, paint.name, paint.id))
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
