import sys
import os
import re
import pymel.core as pm
import write
import datetime
import uprising_util as uutl
import stroke_factory_utils as sfu
import pymel.core.uitypes as gui
import json
from itertools import groupby
from operator import itemgetter
from robolink import Robolink


class PublishTab(gui.FormLayout):

    def __init__(self):
        self.setNumberOfDivisions(100)
        pm.setParent(self)
        self.column = pm.columnLayout()
        self.column.adjustableColumn(True)
        self.create_ui()
        self.create_action_buttons()

        try:
            self.on_load_notes()
        except BaseException:
            pass


        self.on_wait_cb_change()
        self.on_separate_files_cb_change()
    def create_ui(self):

        pm.setParent(self.column)


        pm.frameLayout(label="Gripper behaviour", bv=True)

        self.gripper_wait_cb = pm.checkBoxGrp(
            label='Wait for user',
            height=30,
            value1=0,
            annotation='Make the gripper wait for user confirmations before shutting',
            changeCommand=pm.Callback(self.on_wait_cb_change))

        self.gripper_pause_if = pm.intFieldGrp(
            height=30,
            label="Pause (ms)",
            numberOfFields=1,
            value1=200,
            annotation='Make the gripper pause before and after gripping or releasing',)


        pm.setParent('..')

        pm.frameLayout(label="Dip/wipe behaviour", bv=True)

        self.water_wipe_repeats_isg = pm.intSliderGrp(
            label="Water wipe repeats",
            height=30,
            field=True,
            minValue=1,
            maxValue=5,
            fieldMinValue=1,
            fieldMaxValue=5,
            value=2,
            annotation="How many times to repeat the whole wipe pattern after a water dip.",
        )


        pm.setParent('..')

        pm.frameLayout(label="Program chunking", bv=True)


        self.separate_files_cb = pm.checkBoxGrp(
            label='Separate files',
            height=30,
            value1=1,
            annotation='Separate each subprogram and split the main program',
            changeCommand=pm.Callback(self.on_separate_files_cb_change))

        self.cluster_chunk_if = pm.intFieldGrp(
            height=30,
            label="Max clusters",
            annotation='Max number of clusters per painting program partial',
            numberOfFields=1,
            value1=100)

        self.chunk_if = pm.intFieldGrp(
             height=30,
            label="Max chunk size",
            numberOfFields=1,
            value1=1500)

        pm.setParent('..')

    def on_separate_files_cb_change(self):
        state = pm.checkBoxGrp(self.separate_files_cb, query=True, value1=True)
        pm.intFieldGrp(self.cluster_chunk_if, edit=True, enable=(state))
        pm.intFieldGrp(self.chunk_if, edit=True, enable=(not state))


    def on_wait_cb_change(self):
        state = pm.checkBoxGrp(self.gripper_wait_cb, query=True, value1=True)
        pm.intFieldGrp(self.gripper_pause_if, edit=True, enable=(not state))


    def create_action_buttons(self):
        pm.setParent(self)  # form

        chunks_only_but = pm.button(
            label='Make chunks only',
            command=pm.Callback(
                self.setup_chunks))

        go_but = pm.button(
            label='Publish', command=pm.Callback(
                self.on_go))

        self.attachForm(self.column, 'left', 2)
        self.attachForm(self.column, 'right', 2)
        self.attachForm(self.column, 'top', 2)
        self.attachControl(self.column, 'bottom', 2, chunks_only_but)

        self.attachNone(chunks_only_but, 'top')
        self.attachForm(chunks_only_but, 'left', 2)
        self.attachPosition(chunks_only_but, 'right', 2, 50)
        self.attachForm(chunks_only_but, 'bottom', 2)

        self.attachNone(go_but, 'top')
        self.attachForm(go_but, 'right', 2)
        self.attachPosition(go_but, 'left', 2, 50)
        self.attachForm(go_but, 'bottom', 2)

    def populate(self):
        pass

    def save(self):
        pass

    def setup_chunks(self):

        chunk_size = pm.intFieldGrp(
            self.chunk_if,
            query=True,
            value1=True)

        collect_strokes = pm.PyNode("collectStrokesMain")
        collect_strokes.attr("startFrom").set(0)
        collect_strokes.attr("endAt").set(-1)

        strokes = pm.paintingQuery("mainPaintingShape", js=True)
        pids = [json.loads(x)["parentId"] for x in strokes]

        start_from = 0

        ranges = []

        if chunk_size < 1:
            chunk_size = len(pids)

        for k, grp in groupby(enumerate(pids), lambda (i, x): x):
            num = len(map(itemgetter(1), grp))
            sentinel = start_from + num
            print "start from", start_from
            print "num", num
            print "sentinel", sentinel
            print "chunk_size", chunk_size
            print "-" * 30

            for i in xrange(start_from, sentinel, chunk_size):
                ranges.append((i, min(i+chunk_size, sentinel)))
                print i, min(i+chunk_size, sentinel)

            start_from += num

        conns = collect_strokes.attr("startFrom").connections(
            s=True, d=False, p=True, c=True)
        for c in conns:
            c[1] // c[0]
        conns = collect_strokes.attr("endAt").connections(
            s=True, d=False, p=True, c=True)
        for c in conns:
            c[1] // c[0]

        for i, r in enumerate(ranges):
            pm.setKeyframe(collect_strokes, v=r[0], at='startFrom', t=i)
            pm.setKeyframe(collect_strokes, v=r[1], at='endAt', t=i)

        return (0, len(ranges)-1)

    def publish_to_directory(self, export_dir, **kw):


        try:
            pm.paintingQuery("mainPaintingShape", cc=True)
        except:
            pm.warning("Something not right!! Do you have any thing selected?")
            return
            
        wait = pm.checkBoxGrp(self.gripper_wait_cb, query=True, value1=True)
        pause = -1 if wait else pm.intFieldGrp(
            self.gripper_pause_if, query=True, value1=True)

        water_wipe_repeats =  pm.intSliderGrp(self.water_wipe_repeats_isg, query=True, value=True)
        do_separate_files  = pm.checkBoxGrp( self.separate_files_cb, query=True, value1=True)
        cluster_chunk_size  = pm.intFieldGrp(self.cluster_chunk_if, query=True ,  value1=True)
        timestamp = datetime.datetime.now().strftime('%y%m%d_%H%M')
        directory = os.path.join(export_dir,timestamp)

        if do_separate_files:

            program_files = write.publish_separate_files( 
                directory, 
                pause_gripper_ms=pause,
                water_wipe_repeats=water_wipe_repeats,
                cluster_chunk_size=cluster_chunk_size
            )
            return program_files

        pm.cutKey("collectStrokesMain", at=(
            "startFrom", "endAt"), option="keys")
        frames = self.setup_chunks()

        program_files = write.publish_sequence( 
            directory,
            frames,
            pause,
            water_wipe_repeats
        )

        return program_files

    def on_go(self):
        uutl.checkRobolink()
        RL = Robolink()
        RL.HideRoboDK()
        export_dir = write.choose_publish_dir()
        if not export_dir:
            return
        self.publish_to_directory(export_dir)
        RL.ShowRoboDK()