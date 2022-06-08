import pymel.core as pm
import uprising.const as k
from uprising.session.calibration_program import CalibrationProgram
from uprising import utils
from robolink import (
    INSTRUCTION_COMMENT,
    INSTRUCTION_SHOW_MESSAGE
)


CAL_SHEET_FIRST_ROW = 6

CAL_APPROACH_HEIGHT = 20

DIPTYCH = "DIPTYCHShape"

PROBES_PARENT = "|mainPaintingGroup|probes"

class BoardCalibrationProgram(CalibrationProgram):
    def __init__(self, name):
        super(BoardCalibrationProgram, self).__init__(name)
        self.packs = self.get_packs()

    def send_locator_packs(self):

        last = None

        with utils.prep_for_output():
            for pack in self.packs:
                self._send_stops(
                    last, pack["approach_loc"], k.FACING_BOARD_JOINTS)
                self.program.RunInstruction(
                    ("Moving to %s" % pack["name"]), INSTRUCTION_COMMENT
                )
                self._send_one_probe(pack)
                last = pack["approach_loc"]

    def _send_one_probe(self, pack):

        base_target = self._create_a_target(
            pack["base_loc"].attr("worldMatrix[0]").get(),
            "{}_base".format(pack["name"]),
            k.FACING_BOARD_JOINTS,
        )

        approach_target = self._create_a_target(
            pack["approach_loc"].attr("worldMatrix[0]").get(),
            "{}_approach".format(pack["name"]),
            k.FACING_BOARD_JOINTS,
        )

        self.program.addMoveJ(approach_target)
        self.program.addMoveL(base_target)
        self.program.Pause()
        self.program.RunInstruction(
            "Enter {} in next cell".format(pack["name"]),
            INSTRUCTION_SHOW_MESSAGE,
        )
        self.program.addMoveL(approach_target)

    @staticmethod
    def get_packs():
        packs = []
        with utils.zero_painting():
            verts = pm.PyNode(DIPTYCH).attr("outProbePoints").get()

            parent = pm.PyNode(PROBES_PARENT)
            junk = pm.listRelatives(parent, children=True, type="transform")
            if junk:
                pm.delete(junk)

            rot = pm.dt.Vector(180, 0, -90)
            for i, vec in enumerate(verts):

                name = "board_cal_{:02}".format(i)
                pack = {"name": name, "id": i}
                pack["base_loc"] = CalibrationProgram._make_locator(
                    parent,
                    "base_{}".format(name),
                    vec,
                    relative=False,
                    rotation=rot
                )

                pack["approach_loc"] = CalibrationProgram._make_locator(
                    parent,
                    "approach_{}".format(name),
                    vec + pm.dt.Vector(0,0, CAL_APPROACH_HEIGHT),
                    relative=False,
                    rotation=rot
                )

                packs.append(pack)

        return packs
