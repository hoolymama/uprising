import pymel.core as pm
import uprising.const as k
from uprising.session.calibration_program import CalibrationProgram
import uprising.uprising_util as uutl
from robolink import (
    INSTRUCTION_COMMENT,
    INSTRUCTION_SHOW_MESSAGE
)


CAL_SHEET_FIRST_ROW = 6

CAL_APPROACH_HEIGHT = 20


class BoardCalibrationProgram(CalibrationProgram):
    def __init__(self, name):
        super(BoardCalibrationProgram, self).__init__(name)
        pm.PyNode("mainPaintingGroup").attr("zeroPosition").set(1)

        self.packs = self.get_packs()

    def send_locator_packs(self):

        last = None

        with uutl.final_position(pm.PyNode("mainPaintingShape")):
            for pack in self.packs:
                self._send_stops(
                    last, pack["approach_loc"], k.FACING_BOARD_JOINTS)
                self.program.RunInstruction(
                    ("Moving to %s" % pack["name"]), INSTRUCTION_COMMENT
                )
                self._send_one_probe(pack)
                last = pack["approach_loc"]

    def _send_one_probe(self, pack):

        row = CAL_SHEET_FIRST_ROW + pack["id"]

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
            "Enter {} in cell {}B".format(pack["name"], row),
            INSTRUCTION_SHOW_MESSAGE,
        )
        self.program.addMoveL(approach_target)

    @staticmethod
    def get_packs():
        packs = []
        verts = [
            item for sublist in pm.sets("probePointsSet", q=True) for item in sublist
        ]
        parent = pm.PyNode("|mainPaintingGroup|jpos|probes")
        rot = pm.dt.Vector(180, 0, -90)
        for i, v in enumerate(verts):
            index = int(str(v).split("[")[1].split("]")[0])
            px, py, _ = v.getPosition(space="world")
            name = "board_cal_{:02}".format(index)
            pack = {"name": name, "id": i}
            pack["base_loc"] = CalibrationProgram._make_locator(
                parent,
                "base_{}".format(name),
                pm.dt.Vector(px, py, 0.0),
                relative=False,
                rotation=rot
            )

            pack["approach_loc"] = CalibrationProgram._make_locator(
                parent,
                "approach_{}".format(name),
                pm.dt.Vector(px, py, CAL_APPROACH_HEIGHT),
                relative=False,
                rotation=rot
            )

            packs.append(pack)

        return packs
