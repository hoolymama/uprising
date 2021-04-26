import pymel.core as pm
import pymel.core.uitypes as gui
import time
import datetime

_time_line = ""
_start_time = None

_header = ""
_major_line = ""
_minor_line = ""
_major_bar = None
_minor_bar = None
_info_field = None
_valid = False


def create(parent):

    global _major_bar
    global _minor_bar
    global _info_field
    global _valid

    pm.setParent(parent)

    _info_field = pm.scrollField(
        wordWrap=True, text="Retries progress info", editable=False
    )

    _major_bar = pm.progressBar(isInterruptable=True)
    _minor_bar = pm.progressBar(isInterruptable=True)

    pm.setParent("..")
    _valid = True


def valid():
    global _valid
    return _valid


def update(**kw):
    global _valid
    if not _valid:
        return
    global _header
    global _major_line
    global _minor_line
    global _major_bar
    global _minor_bar
    global _info_field
    global _start_time
    global _time_line

    header = kw.get("header")
    if kw.get("header"):
        _header = header

    major_line = kw.get("major_line")
    if kw.get("major_line"):
        _major_line = major_line

    minor_line = kw.get("minor_line")
    if kw.get("minor_line"):
        _minor_line = minor_line

    major_progress = kw.get("major_progress")
    if major_progress is not None:
        pm.progressBar(_major_bar, edit=True, progress=major_progress)

        max_val = pm.progressBar(_major_bar, query=True, maxValue=True)

        if _start_time:
            time_elapsed = time.time() - _start_time
            if time_elapsed > 1 and max_val > 0 and major_progress > 0:
                fraction = major_progress / float(max_val)
                remaining_fraction = 1.0-fraction
                time_elapsed = time.time() - _start_time
                time_left = (time_elapsed / fraction) * remaining_fraction
                time_elapsed = datetime.timedelta(seconds=int(time_elapsed))
                time_left = datetime.timedelta(seconds=int(time_left))

                _time_line = "Elapsed: {} - Remaining: {}".format(
                    time_elapsed, time_left)

    if header or major_line or minor_line or major_progress:
        info = "{}\n{}\n{}\n{}".format(
            _time_line, _header, _major_line, _minor_line)
        pm.scrollField(_info_field, edit=True, text=info)

    minor_progress = kw.get("minor_progress")
    if minor_progress is not None:
        pm.progressBar(_minor_bar, edit=True, progress=minor_progress)

    major_max = kw.get("major_max")
    if major_max is not None:
        minval = pm.progressBar(_major_bar, q=True, minValue=True)
        pm.progressBar(_major_bar, edit=True,
                       maxValue=max(major_max, (minval + 1)))
        # initialize time
        _start_time = time.time()

    minor_max = kw.get("minor_max")
    if minor_max is not None:
        minval = pm.progressBar(_minor_bar, q=True, minValue=True)
        pm.progressBar(_minor_bar, edit=True,
                       maxValue=max(minor_max, (minval + 1)))
