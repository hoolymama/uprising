
import os
from contextlib import contextmanager
from uprising import progress
import pymel.core as pm
import pymel.core.uitypes as gui

import datetime
from uprising.common.session.session import Session
from uprising.bot.session.bot_painting_session import BotPaintingSession
from uprising.bot.session.retries_session import RetriesSession
from uprising import (chains, robo, utils, persist_ui)
from uprising import const as k
from uprising import tools_menu
import logging
logger = logging.getLogger(__name__)
logger.setLevel(logging.DEBUG)

ONE_PROGRAM=1
BATCHED=2


@contextmanager
def isolate_batch(batch_name, batches):
    # batch - array
    # batches - dict of arrays
    for b in batches:
        if b == batch_name:
            for skel in batches[b]:
                skel.attr("nodeState").set(0)
        else:
            for skel in batches[b]:
                skel.attr("nodeState").set(1)
    try:
        yield
    finally:
        for b in batches:
            for skel in batches[b]:
                skel.attr("nodeState").set(0)

class PublishTab(gui.FormLayout):
    def __init__(self):
        self.setNumberOfDivisions(100)
        pm.setParent(self)
        try:
            pm.PyNode(k.PAINTING_NAME)
        except pm.MayaNodeError:
            return

        self.column = pm.columnLayout()
        self.column.adjustableColumn(True)

        self.header = self.create_header()
        self.retries_frame = self.create_retries_frame()
        self.export_frame = self.create_export_frame()
        self.progress_frame = self.create_progress_frame()

        pm.setParent(self)
        self.go_but = self.create_action_buttons()

        prefix =  "upov_botpub"
        self.persistentWidgets = [
            persist_ui.factory(self, "do_components_cb", prefix, default_value=[True, True]),
            persist_ui.factory(self, "do_save_rb", prefix, default_value=ONE_PROGRAM),
            persist_ui.factory(self, "suffix_tf", prefix, default_value=""),
            persist_ui.factory(self, "coil_delta_ff", prefix, default_value=20.0),
            persist_ui.factory(self, "single_active_checkbox", prefix, default_value=False),
            persist_ui.factory(self, "single_plug_index_if", prefix, default_value=0),
            persist_ui.factory(self, "do_separate_subprograms_cb", prefix, default_value=[True]),
            persist_ui.factory(self, "cluster_chunk_if", prefix, default_value=100)
        ]




        self.populate()
        self.on_ops_change()

    def create_header(self):
 
        pm.columnLayout(adjustableColumn=True)
        self.do_components_cb = pm.checkBoxGrp(
            numberOfCheckBoxes=2,
            cw1=0,
            label="",
            height=30,
            labelArray2=("Do Retries", "Do Painting"),
            changeCommand=pm.Callback(self.on_ops_change)
        )
        self.do_save_rb = pm.radioButtonGrp(
            label='Save', 
            ann="Split the painting into multiple paintings based on the value of the batchName attribute of each skeletonStroke node",
            height=30,
            labelArray2=[
                'One program',
                'Batched'],
            numberOfRadioButtons=2,
            changeCommand=pm.Callback(self.on_ops_change))

        self.suffix_tf = pm.textFieldGrp(label="Folder Suffix", height=30)

        pm.setParent("..")

    def create_retries_frame(self):

        frame = pm.frameLayout(label="Retries", bv=True)

        self.coil_delta_ff = pm.floatFieldGrp(
            numberOfFields=1, 
            label="Coil retries delta"
            # , value1=20
        )

        with utils.activatable(state=False) as self.single_active_checkbox:

            self.do_single_row = pm.rowLayout(
                numberOfColumns=3,
                columnWidth3=(300, 280, 100),
                adjustableColumn=1,
                columnAlign=(1, "right"),
                columnAttach=[(1, "both", 2), (2, "both", 2), (3, "both", 2)],
            )

            self.single_skel_menu = pm.optionMenuGrp(
                label="Single plug",
                
                changeCommand=pm.Callback(self.configure_single_selector)
            )

            # TODO: This code goes somewhere else
            all_skels = pm.PyNode(k.PAINTING_NAME).listHistory(
                type="skeletonStroke")
            found = None
            for node in all_skels:
                pm.menuItem(label=node)
                if (not found ) and (node.attr("nodeState").get() == 0):
                    found=node
            if found:
                pm.optionMenuGrp(self.single_skel_menu, e=True, v=found.name())

            

            self.single_plug_index_if = pm.intSliderGrp(
                label="Selector",
                cw3=(60, 60, 160),
                field=True,
                minValue=-1, fieldMinValue=-1,
                maxValue=0, fieldMaxValue=0,
                # value=0
            )

            pm.button(label="Split now",
                      command=pm.Callback(self.on_split_single))

            pm.setParent("..")  # out of row

        pm.setParent("..")  # out of frame

        self.configure_single_selector()

        return frame

    def create_export_frame(self):

        frame = pm.frameLayout(label="Export", bv=True)

        self.do_separate_subprograms_cb = pm.checkBoxGrp(
            numberOfCheckBoxes=1,
            label="Separate subprograms",
            # value1=1,
            changeCommand=pm.Callback(self.on_ops_change)
        )

        self.cluster_chunk_if = pm.intFieldGrp(
            height=30,
            label="Max clusters/file",
            annotation="Max number of clusters per painting partial program",
            numberOfFields=1,
            # value1=100,
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

        pm.frameLayout(self.retries_frame, edit=True, en=(do_retries))

        pm.frameLayout(self.export_frame, edit=True, en=(do_painting))

        do_separate_subprograms = pm.checkBoxGrp(self.do_separate_subprograms_cb, q=True , value1=True)
        pm.intFieldGrp(self.cluster_chunk_if, e=True, en=do_separate_subprograms)

        pm.button(self.go_but, edit=True, en=(do_retries or do_painting))



    def create_action_buttons(self):
        pm.setParent(self)  # form

        save_but = pm.button(label='Save', command=pm.Callback(self.save))
        go_but = pm.button(label="Go", command=pm.Callback(self.on_go))

        self.attachForm(self.column, "left", 2)
        self.attachForm(self.column, "right", 2)
        self.attachForm(self.column, "top", 2)
        self.attachControl(self.column, "bottom", 2, save_but)


        self.attachNone(save_but, 'top')
        self.attachForm(save_but, 'left', 2)
        self.attachPosition(save_but, 'right', 2, 50)
        self.attachForm(save_but, 'bottom', 2)


        self.attachNone(go_but, "top")
        self.attachForm(go_but, "right", 2)
        self.attachPosition(go_but, 'left', 2, 50)
        self.attachForm(go_but, "bottom", 2)
        return go_but


    ##############################################################

    def on_split_single(self):

        node = pm.PyNode(pm.optionMenuGrp(
            self.single_skel_menu, query=True, value=True))

        chain_skel_pairs = utils.get_chain_skel_pairs(node)
        chains.chunkify_skels( chain_skel_pairs, 1 )
        self.configure_single_selector()

    def configure_single_selector(self):
        """Attach the int control to the selector attribute of the current skeleton"""
        
        node = pm.optionMenuGrp(
            self.single_skel_menu, query=True, value=True)

        if not node:
            return
        node = pm.PyNode(node)

        chain_skel_pairs = utils.get_chain_skel_pairs(node)
        numPlugs = chain_skel_pairs[0][0].attr("outputCount").get()

        pm.connectControl(self.single_plug_index_if, node.attr("selector"))

        pm.intSliderGrp(
            self.single_plug_index_if, edit=True,
            maxValue=(numPlugs-1), minValue=-1,
            fieldMaxValue=(numPlugs-1), fieldMinValue=-1)

    def on_go(self):
        self.save()
        do_single = pm.checkBox(self.single_active_checkbox, q=True, v=True)
        if do_single:
            self.do_single_retry()
            return

        do_batch = pm.radioButtonGrp(
            self.do_save_rb, 
            query=True, 
            sl=True) == 2

        if do_batch:
            self.do_batch_programs()
            return

        self.do_export_one_program()

        tools_menu.on_print_painting_flow_ss()

    def _get_ui_params(self):
        coil_delta = pm.floatFieldGrp(
            self.coil_delta_ff, query=True, value1=True)
        cluster_chunk_size = pm.intFieldGrp(
            self.cluster_chunk_if, query=True, value1=True
        )
        do_separate_subprograms = pm.checkBoxGrp(self.do_separate_subprograms_cb, q=True, value1=True)
        
        suffix = pm.textFieldGrp(self.suffix_tf, query=True, text=True).strip()
        directory = Session.choose_session_dir(suffix=suffix)
  
        do_retries, do_painting = pm.checkBoxGrp(
            self.do_components_cb, query=True, valueArray2=True
        )

        return (
            do_retries, 
            do_painting, 
            coil_delta, 
            cluster_chunk_size, 
            do_separate_subprograms, 
            directory)


    def do_batch_programs(self):
        do_retries, do_painting, coil_delta, cluster_chunk_size, do_separate_subprograms, directory = self._get_ui_params()
        if not directory:
            print("No directory - Aborted")
            return
        robo.new()
        robo.hide()

        batches = _get_active_skel_batches()
        for batch_name in batches:
            with isolate_batch(batch_name, batches):
                batch_directory = os.path.join(directory,batch_name)
                utils.mkdir_p(batch_directory)

                if do_retries:
                    retries_session = RetriesSession(
                        coil_delta,
                        None,
                        False,
                        batch_directory)

                    retries_session.run()
                    retries_session.show_results()
                    retries_session.write_results()

                if do_painting:
                    painting_session = BotPaintingSession(cluster_chunk_size, batch_directory, do_separate_subprograms)
                    painting_session.run()
                    painting_session.show_stats()
                    painting_session.write_stats()
        
        painting_session.write_maya_scene(directory, "scene")



    def do_export_one_program(self):
        do_retries, do_painting, coil_delta, cluster_chunk_size, do_separate_subprograms, directory = self._get_ui_params()
        if not directory:
            logger.warning("No directory - Aborted")
            return
        robo.new()
        robo.hide()
        
        logger.warning("Exporting to %s", directory)
        if do_retries:
            retries_session = RetriesSession(
                coil_delta,
                None,
                False,
                directory)
            logger.debug("RETRIES_SESSION.run()")
            retries_session.run()
            logger.debug("Ran RETRIES_SESSION")
            retries_session.show_results()
            retries_session.write_results()

        if do_painting:
            painting_session = BotPaintingSession(cluster_chunk_size, directory, do_separate_subprograms)
            logger.debug("PAINTING_SESSION.run()")
            painting_session.run()
            logger.debug("Ran PAINTING_SESSION")
            painting_session.show_stats()
            painting_session.write_stats()
            painting_session.write_maya_scene(directory, "scene")

        robo.show()

    def do_single_retry(self):

        coil_delta = pm.floatFieldGrp(
            self.coil_delta_ff, query=True, value1=True)

        node = pm.PyNode(pm.optionMenuGrp(
            self.single_skel_menu, query=True, value=True))
        plug = node.attr("selector")

        robo.new()
        robo.show()
        retries_session = RetriesSession(
            coil_delta,
            plug,
            False,
            None)

        retries_session.run()
        retries_session.show_results()

    def populate(self):
        for persister in self.persistentWidgets:
            persister.populate()

    def save(self):
        for persister in self.persistentWidgets:
            persister.save()




def _get_active_skel_batches():
    """Collect names of all active batches.

    Returns: dict of batch names to list of skeletons.
    """
    skels = [s for s in pm.ls(type="skeletonStroke") if s.attr("nodeState").get() == 0]
    result =  {}
    for skel in skels:
        try:
            name = skel.attr("batchName").get() or "noname"
        except pm.AttributeError:
            name = "noname"
        if name not in result:
            result[name] = []
        result[name].append(skel)
    return result


