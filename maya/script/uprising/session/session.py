import logging
 
from uprising import props
from uprising import robo
import pymel.core as pm
import uprising.uprising_util as uutl
 
import fileinput
 
import os

logger = logging.getLogger("uprising")


class SessionError(Exception):
    pass


class Session(object):

    @staticmethod
    def send_rack_geo():
        rack_context = pm.PyNode("RACK1_CONTEXT")
        with uutl.final_position(rack_context):
            geo = [pm.PyNode("rackTop")]
            props.send(geo)
    @staticmethod
    def send_holder_geo():
        rack_context = pm.PyNode("RACK1_CONTEXT")
        with uutl.final_position(rack_context):
            geo = pm.ls("holders|*|holderTrans|lowResGeo")
            props.send(geo)

    @staticmethod
    def insert_external_dependencies(subprograms, src_fn):
        print "Writing Externals", subprograms
        func = os.path.splitext(os.path.basename(src_fn))[0]
        func = "DEF {}".format(func)
        ext_lines = ["EXT {}()".format(s) for s in subprograms]
        search = True
        for line in fileinput.FileInput(src_fn, inplace=1):
            print line,
            if search and line.startswith(func):
                search = False
                for ext in ext_lines:
                    print ext

    @staticmethod
    def save_program(directory, program_name):
        src = os.path.join(directory, "src")
        uutl.mkdir_p(src)
        src_fn = robo.write_program(src, program_name)
        logger.info("Wrote src: {}".format(program_name))
        return src_fn

    @staticmethod
    def save_station(directory, program_name):
        rdk = os.path.join(directory, "rdk")
        uutl.mkdir_p(rdk)
        rdk_fn = robo.write_station(rdk, program_name)
        logger.info("Wrote rdk: {}".format(program_name))
        return rdk_fn
