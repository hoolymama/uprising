#!/usr/bin/env python

"""

"""
import os
import sys
import errno
from shutil import copy2

# /something/uprising/5.1.0
PKG_DIR =  os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
MODULE_DIR = os.path.join(PKG_DIR, "maya")
VERSION = os.path.basename(PKG_DIR)  # 5.1.0
MODULE_NAME = "uprising"
SUPPORTED_MAYA_VERSIONS = [2019]

def main():
    module_dir = os.path.expanduser("~/Library/Preferences/Autodesk/maya/modules")
    robodk_python_dir = os.path.expanduser("~/RoboDK/RoboDK.app/Contents/Python")


    ensure_directory(module_dir)
    fn = os.path.join(module_dir, "{}.mod".format(MODULE_NAME))
    with open(fn, "w") as f:
        for maya_version in SUPPORTED_MAYA_VERSIONS:
            f.write("+ MAYAVERSION:{} {} {} {}\n".format(maya_version, MODULE_NAME, VERSION, MODULE_DIR))
            f.write("PYTHONPATH+={}\n\n".format(robodk_python_dir))

    sys.stdout.write("Completed Maya Module setup!\n")


def ensure_directory(directory):
    try:
        os.makedirs(directory)
    except OSError as ex:
        if ex.errno == errno.EEXIST and os.path.isdir(directory):
            pass
        else:
            raise


if __name__ == "__main__":
    main()
