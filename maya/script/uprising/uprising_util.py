import errno
import json
import os
from contextlib import contextmanager

import pymel.core as pm

PI = 3.14159265359

 
UPRISING_PROJECT_PATH = os.path.dirname(pm.getModulePath(moduleName="uprising"))
CLEAN_FILE = os.path.join(UPRISING_PROJECT_PATH, "robodk", "clean.rdk")


@contextmanager
def at_value(attr, value):
    old = attr.get()
    attr.set(value)
    yield
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
    yield
    node.attr("tz").set(old)


@contextmanager
def at_position(node, x, y, z):
    old = node.attr("t").get()
    node.attr("t").set(x, y, z)
    yield
    node.attr("t").set(old)


@contextmanager
def final_position(*nodes):
    remember = []
    for node in nodes:
        asy = assembly(node)
        remember.append([asy, asy.attr("zeroPosition").get()])
        asy.attr("zeroPosition").set(False)
    yield
    for asy, rem in remember:
        asy.attr("zeroPosition").set(rem)


@contextmanager
def zero_position(node):
    asy = assembly(node)
    zpos = asy.attr("zeroPosition").get()
    asy.attr("zeroPosition").set(True)
    yield
    asy.attr("zeroPosition").set(zpos)


@contextmanager
def filters_off(*nodes):
    curr_vals = [node.attr("applyFilters").get() for node in nodes]
    for node in nodes:
        node.attr("applyFilters").set(False)
    yield
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
    yield
    cb = pm.checkBox(label=label, value=state)

    children = pm.formLayout(form, query=True, childArray=True)

    ctrl = children[0]
    pm.checkBox(
        cb, edit=True, changeCommand=pm.Callback(_on_active_cb_change, ctrl, cb)
    )

    form.attachForm(cb, "top", 2)
    form.attachForm(cb, "bottom", 2)
    form.attachForm(cb, "right", 2)
    form.attachNone(cb, "left")

    form.attachForm(ctrl, "left", 2)
    form.attachForm(ctrl, "top", 2)
    form.attachForm(ctrl, "bottom", 2)
    form.attachControl(ctrl, "right", 2, cb)

    _on_active_cb_change(ctrl, cb)

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
    out_len = in_len / 3
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
    out_len = in_len / 3
    if out_len * 3 != in_len:
        raise ValueError("Input array must be multiple of 3")
    result = []

    for i in range(out_len):
        j = i * 3
        result.append(pm.dt.Point(arr[j], arr[j + 1], arr[j + 2]))
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


class ClusterError(Exception):
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
