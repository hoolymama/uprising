
import datetime
import fileinput
import json
import math
import os

import progress
import pymel.core as pm
import robo
import uprising_util as uutl
from studio import Studio


def split_desc(desc):
    result = [None, None]
    lines = desc.splitlines()
    n = len(lines)
    if n:
        result[0] = lines[0]
    if n > 1:
        result[1] = "\n".join(lines[1:])
    return result

def publish_painting(directory, **kw):
    painting_node = pm.PyNode("mainPaintingShape")
    program_names = []
    cluster_count = pm.paintingQuery(painting_node, cc=True)
    cluster_chunk_size=kw.get("cluster_chunk_size", cluster_count)


    progress.update(
        header="Creating main painting",
        major_line="{} clusters in chunks of {}".format(cluster_count, cluster_chunk_size),
        major_progress=0,
        minor_progress=0
    )

    robo.new()
    studio = Studio(do_painting=True)
    program = studio.painting_program
    if not program:
        return

    cluster_count = len(program.painting.clusters)
    num_chunks = int(math.ceil( cluster_count / float(cluster_chunk_size)))

    progress.update(
        major_max=num_chunks,
        header="Writing {} main program chunks".format(num_chunks)
        )

    if not program:
        return
    for i in range(num_chunks):
        progress.update(
            major_progress=i, major_line="Writig {:d} of {:d} chunks".format(i+1, num_chunks))

        robo.new()
        subprograms = program.write(chunk_id=i, chunk_length=cluster_chunk_size)

        src_fn,rdk_fn = save_prog_and_station(directory, program.program_name)

        insert_external_dependencies(subprograms, src_fn)

        program_names.append(program.program_name)

    progress.update( major_progress=num_chunks, major_line="Done")

    if len(program_names) > 1:
        orchestrator(directory, program_names)

def publish_pick_place(directory):
    studio = Studio(pick_and_place_slots="used" )
    programs = studio.pick_place_programs
    count = len(programs)
    progress.update(
        header="Creating {} pick and place programs".format(count),
        major_line="",
        major_max=count,
        major_progress=0
    )

    with uutl.final_position(pm.PyNode("RACK1_CONTEXT")):
        for i, program in enumerate(programs):
            robo.new()
            program.write()
            save_prog_and_station(directory, program.program_name)

            progress.update(
                major_progress=(i+1),
                major_line="Wrote program {}  {:d}/{:d}".format( program.program_name , i+1, count ))




def publish_dips(directory, water_wipe_repeats):
    studio = Studio(
        do_dips=True,
        water_wipe_repeats=water_wipe_repeats,
        )
    programs = studio.dip_programs+ studio.water_programs+studio.retardant_programs
    count = len(programs)
    progress.update(
        header="Creating {} dip programs".format(count),
        major_line="",
        major_max=count,
        major_progress=0
    )


    with uutl.final_position(pm.PyNode("RACK1_CONTEXT")):
        for i, program in enumerate(programs):
            robo.new()
            program.write()
            save_prog_and_station(directory, program.program_name)
            progress.update(
                major_progress=(i+1),
                major_line="Wrote program {}  {:d}/{:d}".format( program.program_name , i+1, count ))


def insert_external_dependencies(subprograms, src_fn):
    func = os.path.splitext(os.path.basename(src_fn))[0]
    func = "DEF {}".format(func)
    ext_lines = ["EXT {}()".format(s) for s in subprograms]
    search = True
    for line in fileinput.FileInput(src_fn,inplace=1) :
        print line,
        if  search and line.startswith(func):
            search = False
            for ext in ext_lines:
                print ext


def orchestrator(directory, programs):
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

def save_prog_and_station(directory, program_name):
    src = os.path.join(directory, "src")
    rdk = os.path.join(directory, "rdk")
    uutl.mkdir_p(src)
    uutl.mkdir_p(rdk)
    src_fn = robo.write_program(src, program_name)
    rdk_fn = robo.write_station(rdk, program_name)
    print "Wrote src and rdk: {}".format(program_name)
    return (src_fn,rdk_fn)

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


def write_maya_scene(directory, name):
    new_name = os.path.join(directory, "{}.ma".format(name) )
    orig_sn = pm.sceneName()
    pm.saveAs(new_name)
    pm.renameFile(orig_sn)


def json_report(directory ,name, data ):
    uutl.mkdir_p(directory)
    json_file = os.path.join(directory, "{}.json".format(name))
    with open(json_file, 'w') as outfile:
        json.dump(data, outfile, indent=4)
