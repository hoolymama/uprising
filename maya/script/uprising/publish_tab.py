import sys
import os
import re
import pymel.core as pm
import write
import datetime
import uprising_util as uutl
import stroke_factory_utils as sfu

import pymel.core.uitypes as gui

import callbacks
import json
from itertools import groupby
from operator import itemgetter


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

        self.on_current_frame_cb_change()
        self.on_wait_cb_change()

    def create_ui(self):

        pm.setParent(self.column)
        min_frame = int(pm.playbackOptions(min=True, query=True))
        max_frame = int(pm.playbackOptions(max=True, query=True))

        self.gripper_row = pm.rowLayout(height=30,
                                        numberOfColumns=3,
                                        columnWidth3=(100, 200, 200),
                                        # adjustableColumn=1,
                                        columnAlign=(1, 'right'),
                                        columnAttach=[(1, 'both', 2), (2, 'both', 2), (2, 'both', 2)])

        pm.text(label='Gripper caution', align="left")

        self.gripper_wait_cb = pm.checkBox(
            label='Wait for user',
            value=1,
            annotation='Make the gripper wait for user confirmations before shutting',
            changeCommand=pm.Callback(self.on_wait_cb_change))

        self.gripper_pause_if = pm.intFieldGrp(
            label="Pause (ms)",
            numberOfFields=1,
            value1=200,
            annotation='Make the gripper pause before and after gripping or releasing',)

        pm.setParent('..')

        self.anim_row = pm.rowLayout(height=30,
                                     numberOfColumns=2,
                                     columnWidth2=(
                                         (390), 100),
                                     # adjustableColumn=1,
                                     columnAlign=(1, 'right'),
                                     columnAttach=[(1, 'both', 2), (2, 'both', 2)])

        self.chunk_if = pm.intFieldGrp(
            label="Max chunk size",
            numberOfFields=1,
            value1=600)

        self.current_frame_cb = pm.checkBox(
            label='Current',
            value=0,
            annotation='Use current frame only',
            changeCommand=pm.Callback(self.on_current_frame_cb_change)
        )

        pm.setParent('..')

        self.save_unfiltered_snapshot = pm.checkBoxGrp(
            height=30,
            label='Save unfiltered snapshot',
            ann="This will switch off filtering temporarily",
            value1=False)

        self.snap_size_if = pm.intFieldGrp(
            height=30,
            label='Snapshot size',
            value1=1024
        )

    def on_current_frame_cb_change(self):
        state = pm.checkBox(self.current_frame_cb, query=True, value=True)
        pm.intFieldGrp(self.chunk_if, edit=True, enable=(not state))

    def on_wait_cb_change(self):
        state = pm.checkBox(self.gripper_wait_cb, query=True, value=True)
        pm.intFieldGrp(self.gripper_pause_if, edit=True, enable=(not state))

    def create_action_buttons(self):
        pm.setParent(self)  # form

        animate_but = pm.button(
            label='Make chunks only',
            command=pm.Callback(
                self._setup_chunks))

        go_but = pm.button(
            label='Publish', command=pm.Callback(
                self.on_go))

        self.attachForm(self.column, 'left', 2)
        self.attachForm(self.column, 'right', 2)
        self.attachForm(self.column, 'top', 2)
        self.attachControl(self.column, 'bottom', 2, animate_but)

        self.attachNone(animate_but, 'top')
        self.attachForm(animate_but, 'left', 2)
        self.attachPosition(animate_but, 'right', 2, 50)
        self.attachForm(animate_but, 'bottom', 2)

        self.attachNone(go_but, 'top')
        self.attachForm(go_but, 'right', 2)
        self.attachPosition(go_but, 'left', 2, 50)
        self.attachForm(go_but, 'bottom', 2)

    def populate(self):
        pass

    def save(self):
        pass

    def _setup_chunks(self):
        current_only = pm.checkBox(
            self.current_frame_cb, query=True, value=True)

        if current_only:
            frame = int(pm.currentTime(query=True))
            return (frame, frame)

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

    def on_go(self):

        export_dir = write.choose_publish_dir()
        if not export_dir:
            return

        pause = pm.intFieldGrp(self.gripper_pause_if, query=True, value1=True)
        wait = pm.checkBox(self.gripper_wait_cb, query=True, value=True)
        frames = self._setup_chunks()

        save_unfiltered_snapshot = pm.checkBoxGrp(
            self.save_unfiltered_snapshot, query=True, value1=True)

        write.publish_sequence(
            export_dir,
            frames,
            pause,
            wait,
            save_unfiltered_snapshot
        )
