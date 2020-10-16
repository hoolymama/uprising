
from uprising import palette_utils as putl

import pymel.core as pm

import uprising.uprising_util as uutl


from uprising.session.pick_place_exercise_session import PickPlaceExerciseSession
from uprising.session.pot_calibration_session import PotCalibrationSession
from uprising.session.holder_calibration_session import HolderCalibrationSession
from uprising.session.board_calibration_session import BoardCalibrationSession
from uprising.session.rack_calibration_session import RackCalibrationSession
from uprising.session.manual_probe_session import ManualProbeSession


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
    title = "Calibration for {}".format(
        " ".join([p.capitalize() for p in which.split("_")]))
    text = pm.text(label=title)

    tf = pm.textFieldGrp(label="Paste bin ->", w=400)
    scroll = pm.scrollField(w=400, h=400, editable=False)
    pm.textFieldGrp(tf, edit=True, changeCommand=pm.Callback(
        updateDisplay, tf, scroll))

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
    rows = list(filter(None, content.split("\n")))

    for row in rows:
        cells = list(filter(None, row.split()))

        result.append(cells)
    return result


def verify(data, nrows=-1, ncolumns=-1):
    if nrows != -1:
        if len(data) != nrows:
            raise ValueError(
                "Wrong number of rows. Should be {}".format(nrows))
    if ncolumns != -1:
        for row in data:
            if len(row) != ncolumns:
                print len(row)
                raise ValueError(
                    "Wrong number of columns. Should be {}".format(ncolumns))


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

    verts = [item for sublist in pm.sets(
        "probePointsSet", q=True) for item in sublist]

    verify(data, len(verts), 1)

    for val, vtx in zip(zip(*data)[0], verts):
        pos = vtx.getPosition(space="world")
        pos.z = (uutl.numeric(val) * 0.1) - 1.0
        vtx.setPosition(pos, space="world")


def _read_pot_calibration(content):

    COS_30 = 0.866

    data = data_matrix(content)
    pots = putl.get_pots()
    handles = putl.get_handles()
    potslen = len(pots)

    if potslen != len(handles):
        raise IndexError("handles and pots are different lengths")
    datalen = potslen*3
    verify(data, datalen)  # 60

    pot_depth = pm.PyNode("rack|holes").attr("calibrationPotDepth").get()
    handle_height = pm.PyNode("rack|holes").attr(
        "calibrationHandleHeight").get()
    handle_posx = pm.PyNode("rack|holes").attr("calibrationHandlePosX").get()

    flat = [uutl.numeric(row[0]) for row in data]
    potvals = flat[0:potslen]
    handlevals = flat[potslen:potslen*2]
    facevals = flat[potslen*2:datalen]

    for i in range(potslen):
        handle_parent = handles[i].getParent()
        pot_parent = pots[i].getParent()

        pot_height_precise = pot_depth + (potvals[i] * 0.1) - 1.0
        pot_parent.attr("tz").set(pot_height_precise)

        handle_height_precise = handle_height + (handlevals[i] * 0.1) - 1.0
        handle_parent.attr("tz").set(handle_height_precise)

        handle_face_precise = handle_posx + \
            (((facevals[i] * 0.1) - 1.0) * COS_30)
        handle_parent.attr("tx").set(handle_face_precise)


def _read_holder_calibration(content):

    data = data_matrix(content)
    holders = pm.ls("RACK1_CONTEXT|j1|rack|holders|holderRot*|holderTrans")
    holder_height = pm.PyNode("rack|holders").attr(
        "calibrationHolderHeight").get()
    holder_distance = pm.PyNode("rack|holders").attr(
        "calibrationHolderDistance").get()

    verify(data, len(holders), 3)

    for i, row in enumerate(data):
        holders[i].attr("tz").set(
            holder_height + (uutl.numeric(row[0]) * 0.1) - 1.0)
        holders[i].attr("ty").set(uutl.numeric(row[1]) * 0.1)
        holders[i].attr("tx").set(
            (uutl.numeric(row[2]) * 0.1) + holder_distance)


def _read_perspex_calibration(content):
    data = data_matrix(content)
    verify(data, 12, 1)
    packs = putl.get_perspex_packs()

    for pack in packs:
        index = pack["index"]
        row = data[index]

        val = row[0]
        if not val.startswith("X"):
            dup = pm.duplicate(pack["base"])
            dup = dup[0]
            _set_precise(dup, uutl.numeric(val), 0)
            dup.rename("Calib_{}".format(pack["name"]))
            pm.parent(dup, world=True)


def _set_precise(xf, gauge_reading, probe_height):
    new_pos = probe_height + (gauge_reading * 0.1) - 1.0
    xf.attr("tz").set(new_pos)


def generate_pick_place_exercise():
    session = PickPlaceExerciseSession()
    session.send()
    session.publish()


def generate_pot_calibration():
    session = PotCalibrationSession()
    session.send()
    session.publish()


def generate_holder_calibration():
    session = HolderCalibrationSession()
    session.send()
    session.publish()


def generate_board_calibration():
    session = BoardCalibrationSession()
    session.send()
    session.publish()


def generate_rack_calibration():
    session = RackCalibrationSession()
    session.send()
    session.publish()


def generate_manual_probe_calibration():
    session = ManualProbeSession()
    session.send()
    session.publish()


# def _generate_calibration(which, *reference_geo):

#     kw = {
#         "do_perspex_triangulation": which == k.TRI_CALIBRATION_PROGRAM_NAME,
#         "do_pot_calibration": which == k.POT_CALIBRATION_PROGRAM_NAME,
#         "do_holder_calibration": which == k.HOLDER_CALIBRATION_PROGRAM_NAME,
#         "do_board_calibration": which == k.BOARD_CALIBRATION_PROGRAM_NAME,
#         "do_perspex_calibration": which == k.PERSPEX_CALIBRATION_PROGRAM_NAME,
#         "pick_and_place_slots": "calibration",  # all, used, specific ids
#         "pause": -1
#     }

#     timestamp = write.get_timestamp()
#     directory = os.path.join(
#         pm.workspace.getPath(),
#         'export',
#         'calibrations',
#         which,
#         timestamp)
#     uutl.mkdir_p(directory)

#     robo.new()
#     studio = Studio(**kw)
#     studio.write()
#     props.send(reference_geo)
#     robo.show()
#     src_fn, rdk_fn = write.save_prog_and_station(directory, which)

#     print "Wrote", src_fn
#     subprogram_names = []
#     with uutl.final_position(pm.PyNode("RACK1_CONTEXT")):
#         for i, program in enumerate(studio.pick_place_programs):
#             name = program.program_name
#             print "Writing PP", name
#             subprogram_names.append(name)
#             write.save_prog_and_station(directory, name)

#     write.insert_external_dependencies(subprogram_names, src_fn)


# def generate_pot_holder_calibration():
#     ref_geo = [pm.PyNode("rackTop")]
#     ref_geo += pm.ls("holes|*|holeTrans|dip_loc|pot")
#     ref_geo += pm.ls("holes|*|holeTrans|wipe_loc|handle")
#     ref_geo += pm.ls("holders|*|holderTrans|lowResGeo")
#     _generate_calibration(k.POT_HOLDER_CALIBRATION_PROGRAM_NAME, *ref_geo)


# def generate_holder_calibration():
#     _generate_calibration(k.HOLDER_CALIBRATION_PROGRAM_NAME)


# def generate_pot_calibration():
#     ref_geo = [pm.PyNode("rackTop")]
#     ref_geo += pm.ls("holes|*|holeTrans|dip_loc|pot")
#     ref_geo += pm.ls("holes|*|holeTrans|wipe_loc|handle")

#     _generate_calibration(k.POT_CALIBRATION_PROGRAM_NAME, *ref_geo)


# def generate_rack_calibration():
#     ref_geo = pm.PyNode("rackTop")
#     _generate_calibration(k.PERSPEX_CALIBRATION_PROGRAM_NAME, ref_geo)


# def generate_board_calibration():
#     ref_geo = pm.PyNode("canvas")
#     _generate_calibration(k.BOARD_CALIBRATION_PROGRAM_NAME, ref_geo)
