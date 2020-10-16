import pymel.core as pm
from brush import Brush
from paint import Paint
import props
from studio import Studio
import progress
import robo
import math
import uprising_util as uutl


def create():
    menu = pm.menu(label="Export", tearOff=True)

    pm.menuItem(
        label="Used brush sets", command=pm.Callback(Brush.send_used_brush_sets)
    )

    pm.menuItem(
        label="Connected brushes", command=pm.Callback(Brush.send_connected_brushes)
    )

    pm.menuItem(
        label="Selected brushes", command=pm.Callback(Brush.send_selected_brushes)
    )

    pm.menuItem(label="Used Paints", command=pm.Callback(Paint.write_geos))

    pm.menuItem(label="Selected geometry",
                command=pm.Callback(props.send_selected))

    pm.menuItem(label="Global approaches",
                command=pm.Callback(export_approaches))

    pm.menuItem(label="Whole painting", command=pm.Callback(export_whole))

    return menu


# approaches are always sent, so no need for kw


def export_approaches():
    studio = Studio()
    studio.write()


def export_whole():

    # PAINTING

    painting_node = pm.PyNode("mainPaintingShape")
    program_names = []
    cluster_count = pm.paintingQuery(painting_node, cc=True)
    cluster_chunk_size = cluster_count
    progress.update(
        header="Creating main painting",
        major_line="{} clusters in chunks of {}".format(
            cluster_count, cluster_chunk_size
        ),
        major_progress=0,
        minor_progress=0,
    )
    robo.new()
    studio = Studio(
        do_painting=True,
        pick_and_place_slots="used",
        do_dips=True,
        water_wipe_repeats=1,
    )

    program = studio.painting_program

    cluster_count = len(program.painting.clusters)
    num_chunks = int(math.ceil(cluster_count / float(cluster_chunk_size)))

    progress.update(
        major_max=num_chunks, header="Writing {} main program chunks".format(
            num_chunks)
    )

    for i in range(num_chunks):
        progress.update(
            major_progress=i,
            major_line="Writing {:d} of {:d} chunks".format(i + 1, num_chunks),
        )

        program.write(chunk_id=i, chunk_length=cluster_chunk_size)

        program_names.append(program.program_name)

    progress.update(major_progress=num_chunks, major_line="Done")

    # PICK PLACE
    programs = studio.pick_place_programs
    count = len(programs)
    progress.update(
        header="Creating {} pick and place programs".format(count),
        major_line="",
        major_max=count,
        major_progress=0,
    )

    with uutl.final_position(pm.PyNode("RACK1_CONTEXT")):
        for i, program in enumerate(programs):
            program.write()

            progress.update(
                major_progress=(i + 1),
                major_line="Wrote program {}  {:d}/{:d}".format(
                    program.program_name, i + 1, count
                ),
            )

    # DIPS
    programs = studio.dip_programs + studio.water_programs + studio.retardant_programs
    count = len(programs)
    progress.update(
        header="Creating {} dip programs".format(count),
        major_line="",
        major_max=count,
        major_progress=0,
    )

    with uutl.final_position(pm.PyNode("RACK1_CONTEXT")):
        for i, program in enumerate(programs):
            program.write()
            progress.update(
                major_progress=(i + 1),
                major_line="Wrote program {}  {:d}/{:d}".format(
                    program.program_name, i + 1, count
                ),
            )

    # GEO

    robo.show()


#     import pymel.core as pm
# import brush_utils as butl


# def create():
#     menu = pm.menu(label="Brush", tearOff=True)

#     pm.menuItem(
#         label="Create probe",
#         command=pm.Callback(
#             butl.setup_probe_from_sheet))

#     return menu
