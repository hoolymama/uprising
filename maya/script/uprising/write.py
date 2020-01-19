 
import tarfile

import os
import json
import datetime
import pymel.core as pm
import uprising_util as uutl

from studio import Studio
from paint import Paint
from brush import Brush
import math
import progress

import robo 

def split_desc(desc):
    result = [None, None]
    lines = desc.splitlines()
    n = len(lines)
    if n:
        result[0] = lines[0]
    if n > 1:
        result[1] = "\n".join(lines[1:])
    return result

 

def _get_or_create_dirs(directory):

    src = os.path.join(directory, "src")
    rdk = os.path.join(directory, "rdk")
    uutl.mkdir_p(src)
    uutl.mkdir_p(rdk)

    return (src, rdk)

def publish_painting(directory, **kw):
    # pause_gripper_ms = kw.get("pause_gripper_ms", 200)
    # water_wipe_repeats = kw.get("water_wipe_repeats",1)
    src, rdk = _get_or_create_dirs(directory)
 
    robo.new()

    cluster_count = pm.paintingQuery(cc=True)
    progress.update(major_max=cluster_count, header="Creating main painting {:d} clusters".format(cluster_count))

    studio = Studio(do_painting=True)

    cluster_count = len(studio.painting_program.painting.clusters)

    cluster_chunk_size=kw.get("cluster_chunk_size", cluster_count)
    num_chunks = int(math.ceil( cluster_count / float(cluster_chunk_size)))

    progress.update(major_max=num_chunks, header="Writing {} main program chunks".format(num_chunks))

    program = studio.painting_program
    if not program:
        return
    for i in range(num_chunks):
        progress.update(major_progress=i, header="Writing {:d} of {:d} chunks".format(i+1, num_chunks))

        robo.new()
        subprograms = program.write(chunk_id=i, chunk_length=num_chunks)
        save_prog_and_station(src, rdk, program.program_name, subprograms=subprograms)

def publish_pick_place(directory):
    rack_context = pm.PyNode("RACK1_CONTEXT")
    
    src, rdk = _get_or_create_dirs(directory)
    studio = Studio( pick_and_place_slots="used" )
    count = len(studio.pick_place_programs)
    for i in range(count):
        robo.new()
        program_name = studio.write_pick_place_program(i)
        save_prog_and_station(src, rdk, program_name)



def publish_separate_files( directory, **kw):
 
    pause_gripper_ms = kw.get("pause_gripper_ms")
    water_wipe_repeats = kw.get("water_wipe_repeats")
   
    src = os.path.join(directory, "src")
    rdk = os.path.join(directory, "rdk")
    uutl.mkdir_p(src)
    uutl.mkdir_p(rdk)
    
    robo.new()

    studio = Studio(
                do_painting=True,
                do_dips=True,
                water_wipe_repeats=water_wipe_repeats,
                pick_and_place_slots="used",
                pause=pause_gripper_ms
                )
    
    result = {"painting": [], "others": []}
    
    cluster_count = len(studio.painting_program.painting.clusters)

    cluster_chunk_size=kw.get("cluster_chunk_size", cluster_count)
    num_chunks = int(math.ceil( cluster_count / float(cluster_chunk_size)))

    for i in range(num_chunks):
        robo.new()
        studio.write_approaches()
        program_name = studio.write_painting_program(i, cluster_chunk_size)
        save_prog_and_station(src, rdk, program_name)
        result["painting"].append(program_name)

    count = len(studio.pick_place_programs)
    for i in range(count):
        robo.new()
        studio.write_approaches()
        studio.write_approaches()
        program_name = studio.write_pick_place_program(i)
        save_prog_and_station(src, rdk, program_name)
        result["others"].append(program_name)
        print "Pick-Place {}/{}".format(i, count)

    count = len(studio.dip_programs)
    robo.new()
    for i in range(count):
        robo.clean()
        studio.write_approaches()
        program_name = studio.write_dip_program(i)
        save_prog_and_station(src, rdk, program_name)
        result["others"].append(program_name)
        print "Dips {}/{}".format(i, count)

    count = len(studio.water_programs)
    for i in range(count):
        robo.clean()
        studio.write_approaches()
        program_name = studio.write_water_program(i)
        save_prog_and_station(src, rdk, program_name)
        result["others"].append(program_name)
        print "Water {}/{}".format(i, count)

    count = len(studio.retardant_programs)
    for i in range(count):
        robo.clean()
        studio.write_approaches()
        program_name = studio.write_retardant_program(i)
        save_prog_and_station(src, rdk, program_name)
        result["others"].append(program_name)
        print "Retardant {}/{}".format(i, count)
    
    write_orchestrator(src, result["painting"])

    json_report(directory ,  "stats", stats()) 
    
    with tarfile.open("{}.tar.gz".format(src), "w:gz") as tar:
        tar.add(src, arcname=os.path.sep)

    return result

def stats():
    result = {}

    painting_node = pm.PyNode("mainPaintingShape") 
    brush_paint_pairs =  used_paints_and_brushes(painting_node)
    result["brush_paint_pairs"] = []
    for brush, paint in brush_paint_pairs:
        result["brush_paint_pairs"].append(
            "brush:{}({:02d})-P({}) & paint:{}({:02d})".format(brush.node_name,brush.id,brush.physical_id,paint.name, paint.id)
        )

    result["brushes_in_use"] = []
    for brush in used_brushes(painting_node):
       result["brushes_in_use"].append(
           "brush:{}({:02d})-P({})".format(brush.node_name,brush.id,brush.physical_id)
           )

    result["paints_in_use"] = []
    for paint in used_paints(painting_node):
        result["paints_in_use"].append(
            "paint:{}({:02d})".format(paint.name,paint.id)
         )

    result["painting_node"] = painting_stats(painting_node)
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


# def prep_clean(studio):
#     uutl.clean_rdk()
#     studio.write_approaches()


def save_prog_and_station(src, rdk, program_name, subprograms=None ):
    robo.write_program(src, program_name)
    robo.write_station(rdk, program_name)
    print "Wrote src and rdk: {}".format(program_name)



def publish_sequence(
    directory,
    frame_range,
    pause ,
    water_wipe_repeats
):
    results = []
    painting_node = pm.PyNode("mainPaintingShape")
    src = os.path.join(directory, "src")
    rdk = os.path.join(directory, "rdk")
    for frame in range(frame_range[0], frame_range[1] + 1):
        pm.currentTime(frame)

        program_name =  "{}_{:02d}".format("prg", frame)
        # There can be an error if the painting contains no strokes.
        # In that case, we just want to skip the frame.
        try:
            pm.paintingQuery(painting_node, cc=True)
        except RuntimeError as ex:
            pm.displayWarning(ex.message)
            continue
        studio = Studio(
            do_painting=True,
            do_dips=True,
            water_wipe_repeats=water_wipe_repeats,
            pick_and_place_slots="used",
            pause=pause
            )
        studio.write()
        save_prog_and_station(src, rdk, program_name)
        results.append(program_name)
    return results

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

def get_timestamp(suffix=None):
    timestamp = datetime.datetime.now().strftime('%y%m%d_%H%M')

    if suffix is not None:
        timestamp = "%s_%s" % (timestamp, suffix)
    return timestamp

 

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
    _used_paints = [paints[_id] for _id in pids]
    _used_brushes = [brushes[_id] for _id in bids]
    return zip(_used_brushes, _used_paints)

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


def json_report(directory ,name, data ):
    uutl.mkdir_p(directory)
    json_file = os.path.join(directory, "{}.json".format(name))
    with open(json_file, 'w') as outfile:
        json.dump(data, outfile, indent=4)


