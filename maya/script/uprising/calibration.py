
import os

import const as k
import palette_utils as putl
import props
import pymel.core as pm
import robo
import sheets
import uprising.uprising_util as uutl
import write
from studio import Studio





def _dismiss():
    pm.layoutDialog(dismiss="abort")


def _okay(scroll, which):

    content = pm.scrollField(scroll, query=True, text=True)
    try:
        if which == "board_triangulation":
            _read_board_triangulation(content)
        elif which == "rack_triangulation":
            _read_rack_triangulation(content)

        elif which == "board_calibration":
            _read_board_calibration(content)
        elif which == "pot_calibration":
            _read_pot_calibration(content)
        elif which == "holder_calibration":
            _read_holder_calibration(content)
        elif which == "perspex_calibration":
            _read_perspex_calibration(content)
    except:
        raise
    finally:
        pm.layoutDialog(dismiss="okay")


def updateDisplay(tf, scroll):
    content = pm.textFieldGrp(tf, q=True, text=True)
    pm.scrollField(scroll, edit=True, text=content)
    pm.textFieldGrp(tf, edit=True, text="")

def pasted_sheet_layout(which):

    form = pm.setParent(query=True)
    pm.formLayout(form, edit=True, width=300)
    title = "Calibration for {}".format(" ".join([p.capitalize() for p in which.split("_")]))
    text = pm.text(label=title)

    tf = pm.textFieldGrp(label = "Paste bin ->", w=400)
    scroll =  pm.scrollField(w=400, h=400, editable=False)
    pm.textFieldGrp(tf, edit=True, changeCommand=pm.Callback(updateDisplay, tf, scroll))

    # scroll = pm.cmdtextFieldGrpExecuter(sourceType="python")


    b1 = pm.button(label="Abort", command=pm.Callback(_dismiss))
    b2 = pm.button(label="Go", command=pm.Callback(_okay, scroll, which))


    form.attachForm(text, 'left', 2)
    form.attachForm(text, 'right', 2)
    form.attachForm(text, 'top', 2)
    form.attachNone(text, 'bottom')

    form.attachForm(tf, 'left', 2)
    form.attachForm(tf, 'right', 2)
    form.attachControl(tf, 'top', 2, text)
    form.attachNone(tf, 'bottom')

    form.attachForm(scroll, 'left', 2)
    form.attachForm(scroll, 'right', 2)
    form.attachControl(scroll, 'top', 2, tf)
    form.attachControl(scroll, 'bottom', 2, b1)

    form.attachForm(b1, 'left', 2)
    form.attachPosition(b1, 'right',  2, 50)
    form.attachNone(b1, 'top')
    form.attachForm(b1, 'bottom', 2)

    form.attachPosition(b2, 'left',  2, 50)
    form.attachForm(b2, 'right', 2)
    form.attachNone(b2, 'top')
    form.attachForm(b2, 'bottom', 2)

    pm.setParent(form)

def read_calibration(which):
    return pm.layoutDialog(ui=pm.Callback(pasted_sheet_layout, which))

def data_matrix(content):
    result = []
    rows =  list(filter(None, content.split("\n")))
    print "len(rows)", len(rows)
    for row in rows:
        cells = list(filter(None, row.split()))
        print "len(cells)", len(cells)
        result.append(cells)
    return result

def verify(data, nrows=-1, ncolumns=-1):
    if nrows != -1:
        if len(data) != nrows:
            raise ValueError("Wrong number of rows. Should be {}".format(nrows))
    if ncolumns != -1:
        for row in data:
            if len(row) != ncolumns:
                print len(row)
                raise ValueError("Wrong number of columns. Should be {}".format(ncolumns))

def _read_board_triangulation(content):
 
    _read_any_triangulation(content)

    height = (pm.PyNode("board_TL").attr("translate").get() -
            pm.PyNode("board_BL").attr("translate").get()).length()
    width = (pm.PyNode("board_TR").attr("translate").get() -
            pm.PyNode("board_TL").attr("translate").get()).length()
    pm.PyNode("canvas").attr("width").set(width)
    pm.PyNode("canvas").attr("height").set(height)

def _read_rack_triangulation(content):
    _read_any_triangulation(content)

def _read_any_triangulation(content):
    data = data_matrix(content)
    verify(data, 3, 4)

    for row in data:
        loc_name = row[0]
        try:
            node = pm.PyNode(loc_name)
        except pm.MayaNodeError:
            node = pm.spaceLocator()
            node.rename(loc_name)

        vals = [uutl.numeric(x) * 0.1 for x in row[1:4]]
        print "{} - set {} {} {}".format(node, vals[0], vals[1], vals[2])
        node.attr("translate").set(*vals)

def _read_board_calibration(content):
    data = data_matrix(content)

    verts=[item for sublist in pm.sets(
        "probePointsSet", q=True) for item in sublist]

    verify(data, len(verts), 1)

    for val, vtx in zip(zip(*data)[0], verts):
        pos=vtx.getPosition(space="world")
        pos.z=(uutl.numeric(val) * 0.1) - 1.0
        vtx.setPosition(pos, space="world")



def _read_pot_calibration(content):
 
    data = data_matrix(content)
    pots=putl.get_pots()
    handles=putl.get_handles()
    if len(pots) != len(handles):
        raise IndexError("Sheet data and number of pots are different lengths")
    verify(data, len(pots), 3)
 
    pot_depth=pm.PyNode("rack|holes").attr("calibrationPotDepth").get()
    handle_height=pm.PyNode("rack|holes").attr(
        "calibrationHandleHeight").get()

    for row in data:
        i=int(uutl.numeric(row[0]))
        _set_precise(pots[i].getParent(), uutl.numeric(row[1]), pot_depth)
        _set_precise(
            handles[i].getParent(),
            uutl.numeric(
                row[2]),
            handle_height)

def _read_holder_calibration(content):
 
    data = data_matrix(content)
    holders=pm.ls("RACK1_CONTEXT|j1|rack|holders|holderRot*|holderTrans")
    verify(data, len(holders), 3)
 
    for row in data:
        i=int(uutl.numeric(row[0]))
        _set_precise(holders[i], uutl.numeric(row[0]), k.RACK_HOLDER_HEIGHT)

        holders[i].attr("ty").set(uutl.numeric(row[1]) * 0.1)
        holders[i].attr("tx").set(
            (uutl.numeric(row[2]) * 0.1) + k.RACK_HOLDER_DISTANCE)


def _read_perspex_calibration(content):
    data = data_matrix(content)
    verify(data, 12, 1)
    packs=putl.get_perspex_packs()
    pack_len=len(packs)
 
    for pack in packs:
        index=pack["index"]
        row=data[index]
      
        val =  row[0]
        if  not val.startswith("X"):
            dup=pm.duplicate(pack["base"])
            dup=dup[0]
            _set_precise(dup, uutl.numeric( val), 0)
            dup.rename("Calib_{}".format(pack["name"]))
            pm.parent(dup, world=True)








def generate_pick_place_exercise():

    timestamp=write.get_timestamp()
    directory=os.path.join(
        pm.workspace.getPath(),
        'export',
        'calibrations',
        k.PAP_EXERCISE_PROGRAM_NAME,
        timestamp)
    uutl.mkdir_p(directory)
    robo.new()
    studio=Studio(do_pap_exercise=True, pick_and_place_slots="all")
    studio.write()

    robo.write_program(directory, k.PAP_EXERCISE_PROGRAM_NAME)
    robo.show()


def _generate_calibration(which, *reference_geo):

    kw={
        "do_perspex_triangulation": which == k.TRI_CALIBRATION_PROGRAM_NAME,
        "do_pot_calibration": which == k.POT_CALIBRATION_PROGRAM_NAME,
        "do_holder_calibration": which == k.HOLDER_CALIBRATION_PROGRAM_NAME,
        "do_board_calibration": which == k.BOARD_CALIBRATION_PROGRAM_NAME,
        "do_perspex_calibration": which == k.PERSPEX_CALIBRATION_PROGRAM_NAME,
        "pick_and_place_slots": "calibration",  # all, used, specific ids
        "pause": -1
    }

    timestamp=write.get_timestamp()
    directory=os.path.join(
        pm.workspace.getPath(),
        'export',
        'calibrations',
        which,
        timestamp)
    uutl.mkdir_p(directory)

    robo.new()
    studio=Studio(**kw)
    studio.write()
    props.send(reference_geo)
    robo.show()
    src_fn, rdk_fn=write.save_prog_and_station(directory, which)

    print "Wrote", src_fn
    subprogram_names = []
    with uutl.final_position(pm.PyNode("RACK1_CONTEXT")):
        for i, program in enumerate(studio.pick_place_programs):
            name =  program.program_name
            print "Writing PP", name
            subprogram_names.append(name)
            write.save_prog_and_station(directory, name)
    
    write.insert_external_dependencies(subprogram_names,src_fn)


def create_manual_triangulation():
    ref_geo=pm.PyNode("rackTop")
    _generate_calibration(k.TRI_CALIBRATION_PROGRAM_NAME, ref_geo)


def generate_pot_holder_calibration():
    ref_geo=[pm.PyNode("rackTop")]
    ref_geo += pm.ls("holes|*|holeTrans|dip_loc|pot")
    ref_geo += pm.ls("holes|*|holeTrans|wipe_loc|handle")
    ref_geo += pm.ls("holders|*|holderTrans|lowResGeo")
    _generate_calibration(k.POT_HOLDER_CALIBRATION_PROGRAM_NAME, *ref_geo)


def generate_holder_calibration():
    _generate_calibration(k.HOLDER_CALIBRATION_PROGRAM_NAME)

def generate_pot_calibration():
    ref_geo=[pm.PyNode("rackTop")]
    ref_geo += pm.ls("holes|*|holeTrans|dip_loc|pot")
    ref_geo += pm.ls("holes|*|holeTrans|wipe_loc|handle")

    _generate_calibration(k.POT_CALIBRATION_PROGRAM_NAME, *ref_geo)



def generate_perspex_calibration():
    ref_geo=pm.PyNode("rackTop")
    _generate_calibration(k.PERSPEX_CALIBRATION_PROGRAM_NAME, ref_geo)


def generate_board_calibration():
    ref_geo=pm.PyNode("canvas")
    _generate_calibration(k.BOARD_CALIBRATION_PROGRAM_NAME, ref_geo)


def _set_precise(xf, gauge_reading, probe_height):
    new_pos=probe_height + (gauge_reading * 0.1) - 1.0
    xf.attr("tz").set(new_pos)

