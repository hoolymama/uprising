import errno
import json
import os
from contextlib import contextmanager
# from uprising import chains
import pymel.core as pm

PI = 3.14159265359
DIPTYCH = "DIPTYCHShape"


@contextmanager
def at_value(attr, value):
    old = attr.get()
    attr.set(value)
    try:
        yield
    finally:
        attr.set(old)


def conform_activatable_checkbox(ctl):
    val = ctl.getEnable()
    form = ctl.parent()
    cb = form.getChildArray()[-1]
    pm.checkBox(cb, e=True, value=val)


def assembly(node):
    top = node
    p = node.getParent()
    while p:
        top = p
        p = p.getParent()
    return top


@contextmanager
def minimize_robodk():
    yield


@contextmanager
def at_height(node, h):
    old = node.attr("tz").get()
    node.attr("tz").set(h)
    try:
        yield
    finally:
        node.attr("tz").set(old)


@contextmanager
def at_position(node, x, y, z):
    old = node.attr("t").get()
    node.attr("t").set(x, y, z)
    try:
        yield
    finally:
        node.attr("t").set(old)

@contextmanager
def zero_painting():
    node = pm.PyNode(DIPTYCH)
    old = node.attr("pinPainting").get()
    node.attr("pinPainting").set(0)
    try:
        yield
    finally:
        node.attr("pinPainting").set(old)

@contextmanager
def pin_painting():
    node = pm.PyNode(DIPTYCH)
    old = node.attr("pinPainting").get()
    node.attr("pinPainting").set(1)
    try:
        yield
    finally:
        node.attr("pinPainting").set(old)


# @contextmanager
# def final_position(*nodes):
#     remember = []
#     for node in nodes:
#         asy = assembly(node)
#         remember.append([asy, asy.attr("zeroPosition").get()])
#         asy.attr("zeroPosition").set(False)
#     try:
#         yield
#     finally:
#         for asy, rem in remember:
#             asy.attr("zeroPosition").set(rem)


@contextmanager
def zero_position(node):
    asy = assembly(node)
    zpos = asy.attr("zeroPosition").get()
    asy.attr("zeroPosition").set(True)
    try:
        yield
    finally:
        asy.attr("zeroPosition").set(zpos)

@contextmanager
def prep_for_output():
    pm.select(cl=True)

    diptych_orig_val = pm.PyNode(DIPTYCH).attr("pinPainting").get()
    rack_zero_val =   pm.PyNode("RACK1_CONTEXT").attr("zeroPosition").get()
    
    # apply_brush_bias_val = pm.PyNode("canvasTransform").attr("applyBrushBias").get()
    apply_brush_bias_val  = pm.PyNode("mainPaintingGroup").attr("applyBrushBias").get()
    brush_lifter_ns_val = pm.PyNode("brushLifter").attr("nodeState").get()
    # displacer_ns_val = pm.PyNode("displacer").attr("nodeState").get()
    

    pm.PyNode(DIPTYCH).attr("pinPainting").set(True)
    pm.PyNode("RACK1_CONTEXT").attr("zeroPosition").set(False)
    # pm.PyNode("canvasTransform").attr("applyBrushBias").set(True)
    pm.PyNode("mainPaintingGroup").attr("applyBrushBias").set(True)
    pm.PyNode("brushLifter").attr("nodeState").set(0)
    # pm.PyNode("displacer").attr("nodeState").set(0)
    try:
        yield
    finally:
 
        pm.PyNode(DIPTYCH).attr("pinPainting").set(diptych_orig_val)
        pm.PyNode("RACK1_CONTEXT").attr("zeroPosition").set(rack_zero_val)
        # pm.PyNode("canvasTransform").attr("applyBrushBias").set(apply_brush_bias_val)
        pm.PyNode("mainPaintingGroup").attr("applyBrushBias").set(apply_brush_bias_val)
        pm.PyNode("brushLifter").attr("nodeState").set(brush_lifter_ns_val)
        # pm.PyNode("displacer").attr("nodeState").set(displacer_ns_val)

        skels = pm.PyNode("mainPaintingShape").listHistory(type="skeletonStroke")


@contextmanager
def filters_off(*nodes):
    curr_vals = [node.attr("applyFilters").get() for node in nodes]
    for node in nodes:
        node.attr("applyFilters").set(False)
    try:
        yield
    finally:
        for node, val in zip(nodes, curr_vals):
            node.attr("applyFilters").set(val)


def _on_active_cb_change(ctrl, cb_ctrl):
    state = pm.checkBox(cb_ctrl, q=True, v=True)
    pm.control(ctrl, edit=True, en=state)


@contextmanager
def activatable(**kw):
    label = kw.get("label", "Active")
    state = kw.get("state", True)
    form = pm.formLayout(nd=100)
    check_box = pm.checkBox(label=label, value=state)

    yield check_box
 
    children = pm.formLayout(form, query=True, childArray=True)

    ctrl = children[1]
    pm.checkBox(
        check_box, edit=True, changeCommand=pm.Callback(
            _on_active_cb_change, ctrl, check_box)
    )

    form.attachForm(check_box, "top", 2)
    form.attachForm(check_box, "bottom", 2)
    form.attachForm(check_box, "right", 2)
    form.attachNone(check_box, "left")

    form.attachForm(ctrl, "left", 2)
    form.attachForm(ctrl, "top", 2)
    form.attachForm(ctrl, "bottom", 2)
    form.attachControl(ctrl, "right", 2, check_box)

    _on_active_cb_change(ctrl, check_box)

    pm.setParent("..")


def mkdir_p(path):
    try:
        os.makedirs(path)
    except OSError as exc:
        if exc.errno == errno.EEXIST and os.path.isdir(path):
            pass
        else:
            raise


def to_vector_array(arr):
    if not arr:
        return []
    in_len = len(arr)
    out_len = in_len // 3
    if out_len * 3 != in_len:
        raise ValueError("Input array must be multiple of 3")
    result = []

    for i in range(out_len):
        j = i * 3
        result.append(pm.dt.Vector(arr[j], arr[j + 1], arr[j + 2]))
    return result


def to_point_array(arr):
    if not arr:
        return []
    in_len = len(arr)
    out_len = in_len // 3
    if out_len * 3 != in_len:
        raise ValueError("Input array must be multiple of 3")
    result = []

    for i in range(out_len):
        j = i * 3
        result.append(pm.dt.Point(arr[j], arr[j + 1], arr[j + 2]))
    return result


def to_rgba_array(arr):
    if not arr:
        return []
    in_len = len(arr)
    out_len = in_len // 4
    if out_len * 4 != in_len:
        raise ValueError("Input array must be multiple of 4")
    result = []

    for i in range(out_len):
        j = i * 4
        result.append(pm.dt.Color(arr[j], arr[j + 1], arr[j + 2], arr[j + 3]))
    return result


def rad2deg(rad):
    return rad * (180 / PI)


def deg2rad(deg):
    return deg / (180 / PI)


def numeric(s):
    try:
        return float(s)
    except ValueError:
        return s


class PaintingError(Exception):
    pass


class StrokeError(Exception):
    pass


def show_in_window(data, **kw):
    title = kw.get("title", "Window")
    result_json = json.dumps(data, indent=2)
    pm.window(width=600, height=800, title=title)
    pm.frameLayout(cll=False, lv=False)
    pm.scrollField(text=result_json, editable=False, wordWrap=False)
    pm.showWindow()

def toggle_skel_node_state(key=None):
    if key:
        nodes = pm.ls("skeletonStroke_*{}*".format(key))
    else:
        nodes = pm.ls(sl=True, type="skeletonStroke")
    newstate = 0 if nodes[0].attr("nodeState").get() else 1
    for n in nodes:
        n.attr("nodeState").set(newstate)
 


def reset_skels(chain_skel_pairs):
    for chain, skel in chain_skel_pairs:
        for dest, src in skel.attr("inputData").connections(s=True, d=False, c=True, p=True):
            if src.index() != 0:
                try:
                    src // dest
                except RuntimeError:
                    pass
        # remove all inactive muklti instances
        for plug in skel.attr("inputData"):
            if not plug.chains.isConnected():
                pm.removeMultiInstance(plug, b=True)

        for plug in chain.attr("outputs"):
            if not plug.isConnected():
                pm.removeMultiInstance(plug, b=True)

        chain.attr("maxChainsPerOutput").set(0)
        skel.attr("inputData")[0].attr("splitAngle").set(360)
        skel.attr("inputData")[0].attr("active").set(True)
        skel.attr("selector").set(0)


def get_chain_skel_pairs(*skels):
    result = []
    if not skels:
        skels = [n for n in pm.ls(sl=True) if n.type() == "skeletonStroke"]
    if not skels:
        skels = [
            n
            for n in pm.PyNode("mainPaintingShape").listHistory(type="skeletonStroke")
            if n.attr("nodeState").get() == 0
        ]
    for skel in skels:
        try:
            chain = skel.attr("inputData[0].chains").connections(s=True, d=False)[0]
        except IndexError:
            print("Skel name {}, error on plug [0]".format(skel))

        result.append((chain, skel))
    return result

