import errno
import os
import datetime
import pymel.core as pm

from robolink import (Robolink, RUNMODE_MAKE_ROBOTPROG, ITEM_TYPE_ROBOT, RUNMODE_MAKE_ROBOTPROG)
RL = Robolink()


def mkdir_p(path):
    try:
        os.makedirs(path)
    except OSError as exc:
        if exc.errno == errno.EEXIST and os.path.isdir(path):
            pass
        else:
            raise


def write_program():
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
    
    prog_filename = "PX_%s" % timestamp
    station = os.path.join(ts_dir, "%s.rdk" % timestamp)
    RL.Save(station)

    # RL.Save(station)
    print "WRITE PROGRAM"
    RL.setRunMode(RUNMODE_MAKE_ROBOTPROG)
    print "setRunMode"
    RL.ProgramStart(prog_filename, ts_dir, "KUKA_KRC4_RN", robot)
    print "ProgramStart %s" % ts_dir
    RL.Render(False)
    print "Render False - now run"
    program = RL.Item("px")
    RL.RunProgram("px", False)
    print "Render True"
    RL.Render(True)
    print "DONE"
    RL.Finish()

    
