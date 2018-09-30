import errno
import os
import sys
import datetime
import pymel.core as pm
import uprising_util as uutl

from robolink import (Robolink, ITEM_TYPE_STATION)
from studio import Studio

import setup_dip

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


# class Publisher(object):

#     def __init__():


def publish_sequence(export_dir, painting_node, dip_node, description, frame_range, maya_only=False):
    RL = Robolink()
    for station in RL.getOpenStations():
        station.Delete()
    RL.AddFile(CLEAN_FILE)


    session_dir = os.path.join(export_dir, "sessions")
    mkdir_p(session_dir)
    
    for frame in range(frame_range[0], frame_range[1]+1):
        pm.currentTime(frame)
        timestamp = get_timestamp(frame)
        ts_dir = get_ts_dir(export_dir, timestamp)
        mkdir_p(ts_dir)
        description, notes = split_desc(description.replace("#f", str(frame)))
        
        write_log( painting_node, dip_node, ts_dir, timestamp, description, notes, frame)
        write_maya_scene(ts_dir, timestamp)
        write_ref_image(ts_dir, timestamp)
        write_csv(export_dir, timestamp, description)

        if not maya_only:
            setup_dip.doit()
            with uutl.minimize_robodk():
                studio = Studio(painting_node, dip_node)
                studio.write()
            write_station(RL, ts_dir, timestamp)
            write_program(RL, ts_dir, timestamp)


# def export_maya_package_only(export_dir, description, suffix=None):

#     timestamp = get_timestamp(suffix)
#     ts_dir = get_ts_dir(export_dir, timestamp)

#     session_dir = os.path.join(ts_dir, "sessions")
#     mkdir_p(session_dir)

#     write_log(ts_dir, timestamp, description)
#     write_maya_scene(ts_dir, timestamp)
#     write_ref_image(ts_dir, timestamp)


# def export_package(export_dir, description, suffix=None):
#     RL = Robolink()

#     timestamp = get_timestamp(suffix)
#     ts_dir = get_ts_dir(export_dir, timestamp)

#     session_dir = os.path.join(ts_dir, "sessions")
#     mkdir_p(session_dir)

#     write_log(ts_dir, timestamp, description)
#     write_maya_scene(ts_dir, timestamp)
#     write_ref_image(ts_dir, timestamp)
#     write_station(RL, ts_dir, timestamp)
#     write_program(RL, ts_dir, timestamp)
#     write_csv(export_dir, timestamp, description)



# def export_to_robodk(self, painting_node, dip_node):
#     RL = Robolink()
#     RL.setWindowState(-1)
#     try:
#         studio = Studio(painting_node, dip_node)
#         studio.write()
#     except Exception:
#         t, v, tb = sys.exc_info()
#         RL.setWindowState(2)
#         raise t, v, tb
#     RL.setWindowState(2)


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


def get_timestamp(suffix):
    timestamp = datetime.datetime.now().strftime('%y%m%d_%H%M')

    if suffix:
        timestamp = "%s_%s" % (timestamp, suffix)
    return timestamp


def get_ts_dir(export_dir, timestamp):
    return os.path.join(export_dir, timestamp)



def write_csv(export_dir, timestamp, description):
    fn = os.path.join(export_dir, "session_entries.csv")
    export_root = os.path.join(pm.workspace.getPath(), 'export')
    folder = export_dir.replace(export_root,"")

    line = (",").join([folder,
                       "ID",
                       timestamp,
                       description,
                       "Gloss",
                       "Dibond",
                       "DATERUN",
                       "LINK",
                       "Waiting",
                       "NOTES"])

    with open(fn, 'a+') as the_file:
        the_file.write(line)
        the_file.write("\n")


def write_ref_image(ts_dir, timestamp):

    # TODO - make sure its active panel and
    f = pm.currentTime(q=True)
    fn = os.path.join(ts_dir, timestamp)
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
        widthHeight=(1024, 1024))


def write_program(RL, ts_dir, timestamp):
    prog_filename = "PX_%s" % timestamp
    program = RL.Item("px")
    program.setName(prog_filename)
    program.MakeProgram(ts_dir)
    program.setName("px")


def write_station(RL, ts_dir, timestamp):
    station = RL.Item("", ITEM_TYPE_STATION)
    station.setName(timestamp)
    RL.Save(os.path.join(ts_dir, "%s.rdk" % timestamp))


def write_maya_scene(ts_dir, timestamp):
    new_name = os.path.join(ts_dir, "%s.ma" % timestamp)
    orig_sn = pm.sceneName()
    pm.saveAs(new_name)
    pm.renameFile(orig_sn)


def _painting_stats(node):

    cluster_count = pm.paintingQuery(node, cc=True)
    stroke_count = 0
    for i in range(cluster_count):
        stroke_count += pm.paintingQuery(node, ci=i, sc=True)

    strokes_per_cluster = stroke_count / float(cluster_count)

    result = {
        "Cluster count": cluster_count,
        "Stroke count": stroke_count,
        "Strokes per cluster": strokes_per_cluster
    }
    return result


def write_log(painting_node, dip_node, ts_dir, timestamp, description, notes, frame):

 
    dip_combos = setup_dip.dip_combinations(painting_node)

    painting_stats = _painting_stats(painting_node)
    dip_stats = _painting_stats(dip_node)

    log_file = os.path.join(ts_dir, "log.txt")
    with open(log_file, 'w') as the_file:
        the_file.write("Description:\n")
        the_file.write("%s\n\n" % description)
        the_file.write("Notes:\n")
        the_file.write("%s\n\n" % notes)
        
        the_file.write("Timestamp: %s\n\n" % timestamp)

        the_file.write("Frame number: %s\n\n" % frame)

        the_file.write("Painting node stats:")
        for k in painting_stats:
            the_file.write("%s: %s\n" % (k, painting_stats[k]))

        the_file.write("\n")
        the_file.write("Dip node stats:")
        for k in dip_stats:
            the_file.write("%s: %s\n" % (k, dip_stats[k]))

        the_file.write("Dip combinations:")
        for k in dip_combos:
            the_file.write("%s\n" % k)








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
#     session_dir = os.path.join(ts_dir, "sessions")
#     mkdir_p(session_dir)

#     write_maya_scene(ts_dir, timestamp)
#     write_ref_image(ts_dir, timestamp)
#     write_log(ts_dir, timestamp, description)
#     write_station(RL, ts_dir, timestamp)
#     write_program(RL, ts_dir, timestamp)
