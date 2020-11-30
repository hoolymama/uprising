
from uprising import painting
import pymel.core as pm
from uprising import robo
from uprising.session.program import Program

from uprising import progress


import logging

logger = logging.getLogger("uprising")


class PovProgram(Program):
    def __init__(self, name, **kw):
        super(PovProgram, self).__init__(name)

        self.painting = painting.PovPainting(pm.PyNode("mainPaintingShape"))

    def bump_program_name(self, suffix):
        self.program_name = "{}_{:02d}".format(
            self.program_name_prefix, suffix)

    def send(self, **kw):
        if not self.painting.clusters:
            return
        num_clusters = len(self.painting.clusters)
        chunk_id = kw.get("chunk_id", 0)

        chunk_length = kw.get("chunk_length", num_clusters)

        start = kw.get("chunk_start", (chunk_id*chunk_length))

        end = start+chunk_length
        end = min(end, num_clusters)
        is_last_chunk = (end >= num_clusters)

        self.bump_program_name(chunk_id)

        progress.update(
            minor_line="Writing {} clusters from {} to {}".format(
                self.program_name, start, end)
        )

        super(PovProgram, self).send()

        self.frame = robo.create_frame("{}_frame".format(self.program_name))

        self.painting.send_brushes()

        for cluster in self.painting.clusters[start:end]:
            cluster.send(self.program, self.frame, self.painting.motion)

        if is_last_chunk and cluster:
            self.program.addMoveJ(robo.home_approach)
