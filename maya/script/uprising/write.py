import errno
import os
import datetime
import pymel.core as pm


from robolink import (
    Robolink,
    RUNMODE_MAKE_ROBOTPROG,
    ITEM_TYPE_ROBOT,
    RUNMODE_MAKE_ROBOTPROG)
# RL = Robolink()

import setup_dip


def mkdir_p(path):
    try:
        os.makedirs(path)
    except OSError as exc:
        if exc.errno == errno.EEXIST and os.path.isdir(path):
            pass
        else:
            raise


def export_package(description):
    RL = Robolink()
    robot = RL.Item('', ITEM_TYPE_ROBOT)
    export_dir = os.path.join(pm.workspace.getPath(), 'export')
    entries = pm.fileDialog2(caption="Choose directory", okCaption="Save",
                             dialogStyle=2, fileMode=3, dir=export_dir)
    if not entries:
        pm.displayWarning('Nothing Selected')
        return

    export_dir = entries[0]

    timestamp = datetime.datetime.now().strftime('%y%m%d_%H%M')
    ts_dir = os.path.join(export_dir, timestamp)

    # simulation = os.path.join(ts_dir, "%s.html" % timestamp)
    session_dir = os.path.join(ts_dir, "sessions")
    mkdir_p(session_dir)

    write_log(ts_dir, timestamp, description)
    write_maya_scene(ts_dir, timestamp)
    write_ref_image(ts_dir, timestamp)
    write_station(RL, ts_dir, timestamp)
    write_program(RL, ts_dir, timestamp)


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
        widthHeight=( 1024, 1024))


def write_program(RL, ts_dir, timestamp):
    prog_filename = "PX_%s" % timestamp
    program = RL.Item("px")
    program.setName(prog_filename)
    program.MakeProgram(ts_dir)
    program.setName("px")


def write_station(RL, ts_dir, timestamp):
    station = os.path.join(ts_dir, "%s.rdk" % timestamp)
    RL.Save(station)


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
        "Cluster count" : cluster_count,
        "Stroke count": stroke_count,
        "Strokes per cluster": strokes_per_cluster
    }
    return result
 


def write_log(ts_dir, timestamp, description):

    painting_node = pm.PyNode("mainPaintingShape")
    dip_node = pm.PyNode("dipPaintingShape")
    
    dip_combos = setup_dip.dip_combinations(painting_node)

    painting_stats = _painting_stats(painting_node)
    dip_stats = _painting_stats(dip_node)


    log_file = os.path.join(ts_dir, "log.txt")
    with open(log_file, 'w') as the_file:
        the_file.write("Description:\n")
        the_file.write("%s\n" % description)
        the_file.write("Timestamp: %s\n" % timestamp)

        the_file.write("Frame number: %s\n" % pm.currentTime(q=True))

        
        the_file.write("Painting node stats:")
        for k in painting_stats:
            the_file.write("%s: %s\n" % (k, painting_stats[k]))
        
        the_file.write("\n")
        the_file.write("Dip node stats:")
        for k in dip_stats:
            the_file.write("%s: %s\n" % (k, dip_stats[k]))
        
        the_file.write("Dip combinations:")
        for k in dip_combos:
            the_file.write(k)
    
     

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


