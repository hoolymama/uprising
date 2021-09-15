
import os
from contextlib import contextmanager
import progress
import pymel.core as pm
import pymel.core.uitypes as gui

import datetime
from uprising.common.session.session import Session
from uprising.bot.session.bot_painting_session import BotPaintingSession
from uprising.bot.session.retries_session import RetriesSession
from uprising import (chains,robo, utils)


class PublishTab(gui.FormLayout):
    def __init__(self):
        self.setNumberOfDivisions(100)
        pm.setParent(self)
        try:
            pm.PyNode("mainPaintingShape")
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
        self.on_ops_change()

    def create_header(self):

        self.do_components_cb = pm.checkBoxGrp(
            numberOfCheckBoxes=4,
            label="",
            valueArray4=(1, 0, 0, 0),
            labelArray4=("Do Retries", "Do Painting", "Dry run", "Save"),
            changeCommand=pm.Callback(self.on_ops_change)
        )

    def create_retries_frame(self):

        frame = pm.frameLayout(label="Retries", bv=True)

        self.coil_delta_ff = pm.floatFieldGrp(
            numberOfFields=1, label="Coil retries delta", value1=20
        )

        

        with utils.activatable(state=False) as self.single_active_checkbox:

            self.do_single_row = pm.rowLayout(
                numberOfColumns=3,
                columnWidth3=(300, 280, 100),
                adjustableColumn=1,
                columnAlign=(1, "right"),
                columnAttach=[(1, "both", 2), (2, "both", 2), (3, "both", 2)],
            )

            all_skels = pm.PyNode("mainPaintingShape").listHistory(
                type="skeletonStroke")
            self.single_skel_menu = pm.optionMenuGrp(
                label="Single plug",
                changeCommand=pm.Callback(self.configure_single_selector)
            )
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
                value=0
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
            value1=1,
            changeCommand=pm.Callback(self.on_ops_change)
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

        do_retries, do_painting, dry_run, do_save = pm.checkBoxGrp(
            self.do_components_cb, query=True, valueArray4=True
        )

        pm.frameLayout(self.retries_frame, edit=True, en=(do_retries))

        pm.frameLayout(self.export_frame, edit=True, en=(do_painting))

        do_separate_subprograms = pm.checkBoxGrp(self.do_separate_subprograms_cb, q=True , value1=True)
        pm.intFieldGrp(self.cluster_chunk_if, e=True, en=do_separate_subprograms)
        
        if do_painting:
            do_save = True
            pm.checkBoxGrp(
                self.do_components_cb, edit=True, valueArray4=(do_retries, do_painting, dry_run, do_save)
            )

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

    def on_split_single(self):

        node = pm.PyNode(pm.optionMenuGrp(
            self.single_skel_menu, query=True, value=True))

 
        chain_skel_pairs = utils.get_chain_skel_pairs(node)

        chains.chunkify_skels( chain_skel_pairs, 1 )

        self.configure_single_selector()

    def configure_single_selector(self):

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

        do_retries, do_painting, dry_run, do_save = pm.checkBoxGrp(
            self.do_components_cb, query=True, valueArray4=True
        )

        coil_delta = pm.floatFieldGrp(
            self.coil_delta_ff, query=True, value1=True)

        # chains_per_retry = pm.intFieldGrp(
        #     self.chains_per_retry, query=True, value1=True)

        cluster_chunk_size = pm.intFieldGrp(
            self.cluster_chunk_if, query=True, value1=True
        )

        do_single = pm.checkBox(self.single_active_checkbox, q=True, v=True)

        do_separate_subprograms = pm.checkBoxGrp(self.do_separate_subprograms_cb, q=True, value1=True)

        directory = None
        if do_save:
            directory = Session.choose_session_dir()
            if not directory:
                print "Aborted"
                return

        plug = None
        if do_single:
            node = pm.PyNode(pm.optionMenuGrp(
                self.single_skel_menu, query=True, value=True))
            plug = node.attr("selector")


        if not dry_run:
            robo.new()
            if do_single:
                robo.show()
            else:
                robo.hide()

        retries_session = None
        if do_retries:
            retries_session = RetriesSession(
                coil_delta,
                plug,
                dry_run,
                directory)

            retries_session.run()
            retries_session.show_results()
            retries_session.write_results()

            # don't do painting in the below cases
            if dry_run or do_single:
                return

        if do_painting:
            painting_session = BotPaintingSession(cluster_chunk_size, directory, do_separate_subprograms)
            painting_session.run()
            painting_session.show_stats()
            painting_session.write_stats()
            painting_session.write_maya_scene(directory, "scene")

        robo.show()


# batch_export(4, 1, 0.25, 0.25, False, "batch")

def batch_export(xrepeat, yrepeat, x_offset, y_offset, do_subprograms, dirname, dry_run, return_to_start, animation=True):
    if not dry_run:
        robo.new()
    crop_node = pm.PyNode("cImgCropMainLow")
    skels = pm.PyNode("mainPaintingShape").listHistory(type="skeletonStroke")
    directory = os.path.join("/Volumes/xtr/gd/venus/export", dirname)

    identifiers = ["A", "B", "C", "D", "E", "F", "G", "H", "I", "J","K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z"]
    i = 0
    x_start = crop_node.attr("extraOffsetX").get()
    y_start = crop_node.attr("extraOffsetY").get()

    timestamp = datetime.datetime.now().strftime("%y%m%d_%H%M")
    for y in range(yrepeat):
        crop_node.attr("extraOffsetX").set(x_start)
        for x in range(xrepeat):
            identifier = identifiers[i]
            if animation:
                pm.currentTime(i+1)
            pm.refresh()
            if not dry_run:
                utils.reset_skels(utils.get_chain_skel_pairs(*skels))

                utils.mkdir_p(directory)

                robo.new()
                robo.hide()
                retries_session = RetriesSession( 20, None, False, directory)
                retries_session.run()
                # retries_session.write_results()

                prefix = "{}_px".format(identifier)
                painting_session = BotPaintingSession(100, directory, do_subprograms, program_prefix=prefix)
                painting_session.run()
                # painting_session.write_stats()

            i += 1
            crop_node.attr("extraOffsetX").set(crop_node.attr("extraOffsetX").get()+x_offset)
        crop_node.attr("extraOffsetY").set(crop_node.attr("extraOffsetY").get()+y_offset)

    crop_node.attr("extraOffsetX").set(x_start)
    if return_to_start:
        crop_node.attr("extraOffsetY").set(y_start)

    if not dry_run:
        BotPaintingSession.write_maya_scene(directory, "scene")
        robo.show()