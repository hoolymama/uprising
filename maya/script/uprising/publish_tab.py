
from contextlib import contextmanager

import progress
import pymel.core as pm
import pymel.core.uitypes as gui

from uprising.session.painting_session import PaintingSession
from uprising.session.retries_session import RetriesSession


@contextmanager
def isolate_nodes(show_nodes, all_nodes):
    all_vals = [n.attr("active").get() for n in all_nodes]
    vals = [n.attr("active").get() for n in show_nodes]

    for n in all_nodes:
        n.attr("active").set(False)

    for n in show_nodes:
        n.attr("active").set(True)
    yield

    for i, n in enumerate(show_nodes):
        n.attr("active").set(vals[i])

    for i, n in enumerate(all_nodes):
        n.attr("active").set(all_vals[i])


class PublishTab(gui.FormLayout):
    def __init__(self):
        self.setNumberOfDivisions(100)
        pm.setParent(self)
        self.column = pm.columnLayout()
        self.column.adjustableColumn(True)

        self.export_frame = self.create_export_frame()
        self.progress_frame = self.create_progress_frame()

        pm.setParent(self)
        self.go_but = self.create_action_buttons()
        self.on_ops_change()

    def create_export_frame(self):

        frame = pm.frameLayout(label="Export", bv=True)

        self.do_components_cb = pm.checkBoxGrp(
            numberOfCheckBoxes=2,
            label="",
            valueArray2=(1, 1),
            labelArray2=("Do Retries", "Do Painting"),
            changeCommand=pm.Callback(self.on_ops_change)
        )

        self.coil_delta_ff = pm.floatFieldGrp(
            numberOfFields=1, label="Retries delta", value1=20
        )

        self.cluster_chunk_if = pm.intFieldGrp(
            height=30,
            label="Max clusters/file",
            annotation="Max number of clusters per painting partial program",
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

    def on_ops_change(self):

        do_retries, do_painting = pm.checkBoxGrp(
            self.do_components_cb, query=True, valueArray2=True
        )

        pm.floatFieldGrp(self.coil_delta_ff, edit=True,
                         en=(do_retries))

        pm.intFieldGrp(self.cluster_chunk_if, edit=True,
                       en=(do_painting))

        pm.button(self.go_but, edit=True, en=(do_retries or do_painting))

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

        do_retries, do_painting = pm.checkBoxGrp(
            self.do_components_cb, query=True, valueArray2=True
        )

        coil_delta = pm.floatFieldGrp(
            self.coil_delta_ff, query=True, value1=True)

        cluster_chunk_size = pm.intFieldGrp(
            self.cluster_chunk_if, query=True, value1=True
        )

        retries_session = None
        if do_retries:
            stroke_nodes = find_contributing_stroke_nodes() if do_retries else []
            retries_session = RetriesSession(coil_delta, stroke_nodes)
            print retries_session.plugs
            retries_session.run()
            retries_session.show_results()
            retries_session.write_results()

        if do_painting:
            directory = retries_session and retries_session.directory
            painting_session = PaintingSession(cluster_chunk_size, directory)
            painting_session.show_stats()
            painting_session.write_stats()
            painting_session.write_maya_scene(directory, "scene")


def find_contributing_stroke_nodes():

    painting_node = pm.PyNode("mainPaintingShape")
    all_skels = pm.ls(type="skeletonStroke")

    result = [n for n in pm.ls(sl=True) if n.type() == "skeletonStroke"]
    if result:
        return result

    for node in all_skels:
        with isolate_nodes([node], all_skels):
            try:
                pm.paintingQuery(painting_node, cc=True)
            except RuntimeError:
                continue
        result.append(node)
    print "{} of {} skeleton nodes contributing".format(
        len(result), len(all_skels))
    return result
