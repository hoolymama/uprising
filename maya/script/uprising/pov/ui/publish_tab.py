
from uprising import progress
import pymel.core as pm
import pymel.core.uitypes as gui

from uprising.pov.session.pov_session import PovSession


class PovPublishTab(gui.FormLayout):
    def __init__(self):
        self.setNumberOfDivisions(100)
        pm.setParent(self)
        self.column = pm.columnLayout()
        self.column.adjustableColumn(True)

        self.export_frame = self.create_export_frame()
        self.progress_frame = self.create_progress_frame()

        pm.setParent(self)
        self.go_but = self.create_action_buttons()

    def create_export_frame(self):

        frame = pm.frameLayout(label="Export", bv=True)

        self.stroke_chunk_if = pm.intFieldGrp(
            height=30,
            label="Max strokes/file",
            annotation="Max number of strokes per painting partial program",
            numberOfFields=1,
            value1=100,
        )

        pm.setParent("..")
        return frame

    def create_progress_frame(self):
        frame = pm.frameLayout(
            bv=True, collapse=False, labelVisible=True, label="Progress"
        )
        col = pm.columnLayout(adj=True)
        progress.create(col)
        return frame

    def create_action_buttons(self):
        pm.setParent(self)  # form

        go_but = pm.button(label="Go", command=pm.Callback(self.on_go))

        self.attachForm(self.column, "left", 2)
        self.attachForm(self.column, "right", 2)
        self.attachForm(self.column, "top", 2)
        self.attachControl(self.column, "bottom", 2, go_but)

        self.attachNone(go_but, "top")
        self.attachForm(go_but, "right", 2)
        self.attachForm(go_but, "left", 2)
        self.attachForm(go_but, "bottom", 2)
        return go_but

    ##############################################################

    def on_go(self):

        stroke_chunk_size = pm.intFieldGrp(
            self.stroke_chunk_if, query=True, value1=True
        )

        # retries_session = None
        # if do_retries:
        #     stroke_nodes = find_contributing_stroke_nodes() if do_retries else []
        #     retries_session = RetriesSession(coil_delta, stroke_nodes)
        #     print retries_session.plugs
        #     retries_session.run()
        #     retries_session.show_results()
        #     retries_session.write_results()

        # if do_painting:
        #     directory = retries_session and retries_session.directory
        #     painting_session = PaintingSession(cluster_chunk_size, directory)
        #     painting_session.show_stats()
        #     painting_session.write_stats()
        #     painting_session.write_maya_scene(directory, "scene")

        pov_session = PovSession(stroke_chunk_size)
        # pov_session.show_stats()
        # pov_session.write_stats()
        # pov_session.write_maya_scene(pov_session.directory, "scene")


def find_contributing_stroke_nodes():
    pass
    # painting_node = pm.PyNode("mainPaintingShape")
    # all_skels = pm.ls(type="skeletonStroke")

    # result = [n for n in pm.ls(sl=True) if n.type() == "skeletonStroke"]
    # if result:
    #     return result

    # for node in all_skels:
    #     with isolate_nodes([node], all_skels):
    #         try:
    #             pm.paintingQuery(painting_node, cc=True)
    #         except RuntimeError:
    #             continue
    #     result.append(node)
    # print "{} of {} skeleton nodes contributing".format(
    #     len(result), len(all_skels))
    # return result
