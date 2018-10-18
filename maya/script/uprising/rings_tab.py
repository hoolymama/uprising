import sys
# import os.path
import pymel.core as pm

# import setup_dip
import curve_utils as cutl
# import brush_utils as butl
# import paint_utils as putl

# from setup_dip import setup_dip_factory

import stroke_factory_utils as sfu
import pymel.core.uitypes as gui
# import painting as pnt
from studio import Studio
from robolink import (
    Robolink
)


class RingsTab(gui.FormLayout):

    def __init__(self):
        self.setNumberOfDivisions(100)
        pm.setParent(self)
        self.column = pm.columnLayout()
        self.column.adjustableColumn(True)
        self.create_buttons()
        self.create_action_buttons()

    def create_buttons(self):
        pm.setParent(self.column)

        # pm.rowLayout(numberOfColumns=2,
        #              columnWidth2=(
        #                  (100), 100),
        #              adjustableColumn=1,
        #              columnAlign=(1, 'right'),
        #              columnAttach=[(1, 'both', 2), (2, 'both', 2)])
        # pm.button(
        #     label='Arrange rings',
        #     ann="Arrange the rings with constant gap",
        #     command=pm.Callback(self.on_arrange_rings))

        self.gap_ff = pm.floatSliderButtonGrp(
            label='Arrange const gap',
            field=True,
            minValue=-5.0,
            maxValue=5.0,
            step=0.01,
            # fieldMinValue=0,
            # fieldMaxValue=5.0,
            value=0,
            symbolButtonDisplay=False,
            buttonLabel="Go",
            buttonCommand=pm.Callback(self.on_arrange_rings_gap),
            columnWidth=(4, 60)
        )

        self.spine_ff = pm.floatSliderButtonGrp(
            label='Arrange const spines',
            field=True,
            minValue=0.0,
            maxValue=10.0,
               step=0.01,
            # fieldMinValue=0,
            # fieldMaxValue=5.0,
            value=2,
            symbolButtonDisplay=False,
            buttonLabel="Go",
            buttonCommand=pm.Callback(self.on_arrange_rings_spine),
            columnWidth=(4, 60)
        )


        # self.gap_ff = pm.floatSliderGrp(
        #     label='Gap (cm)',
        #     field=True,
        #     minValue=0,
        #     maxValue=20.0,
        #     fieldMinValue=0,
        #     fieldMaxValue=5.0,
        #     value=0,
        #     columnWidth=(1, 50),
        #     width=200)

        # pm.setParent('..')


        pm.rowLayout(numberOfColumns=2,
             columnWidth2=(
                 (390), 100),
             # adjustableColumn=1,
             columnAlign=(1, 'right'),
             columnAttach=[(1, 'both', 2), (2, 'both', 2)])

        self.random_paints_tf = pm.textFieldButtonGrp( label='Random paints', text='0-32', buttonLabel='Go' ,
            columnWidth3=(140,200,50),
            buttonCommand=pm.Callback(self.on_random_paints))

        self.random_paints_anim_cb = pm.checkBox(
            label='Animate',
            value=0,
            annotation='Animate over frame range')

        pm.setParent('..')


        pm.rowLayout(numberOfColumns=2,
             columnWidth2=(
                 (390), 100),
             # adjustableColumn=1,
             columnAlign=(1, 'right'),
             columnAttach=[(1, 'both', 2), (2, 'both', 2)])

        self.random_brushes_tf = pm.textFieldButtonGrp( label='Random brushes', text='0-32', buttonLabel='Go' ,
            columnWidth3=(140,200,50),
            buttonCommand=pm.Callback(self.on_random_brushes))

        self.random_brushes_anim_cb = pm.checkBox(
            label='Animate',
            value=0,
            annotation='Animate over frame range')

        pm.setParent('..')



        pm.button(
            label='Randomize and arrange sequence',
            command=pm.Callback(self.on_random_sequence))

        pm.button(
            label='Randomize selected curve rotations',
            command=pm.Callback(self.on_random_rotations))

        pm.button(
            label='Duplicate into gaps',
            command=pm.Callback(self.on_duplicate_into_gaps))


        pm.frameLayout(label="Auto rings")
        pm.columnLayout()

        self.min_strokes_ctl = pm.intSliderGrp(
            field=True,
            label='Minimum strokes',
            minValue=1,
            maxValue=6,
            fieldMinValue=1,
            fieldMaxValue=50,
            value=4)

        self.max_length_ctl = pm.floatSliderGrp(
            field=True,
            label='Max stroke length',
            minValue=1.0,
            maxValue=40.0,
            fieldMinValue=1.0,
            fieldMaxValue=200.0,
            value=15.0)

        self.overlap_ctl = pm.floatSliderGrp(
            field=True,
            label='Overlap length',
            minValue=0.0,
            maxValue=10.0,
            fieldMinValue=0.0,
            fieldMaxValue=20.0,
            value=2.0)

        # self.lift_length_ctl = pm.floatSliderGrp(
        #     field=True,
        #     label='Lift length',
        #     minValue=1.0,
        #     maxValue=10.0,
        #     fieldMinValue=1.0,
        #     fieldMaxValue=20.0,
        #     value=2.0)

        pm.button(
            label='Go',
            ann="Go",
            command=pm.Callback(self.on_auto_stroke_length))



    def create_action_buttons(self):
        pm.setParent(self)  # form

        cancel_but = pm.button(label='Cancel')
        go_but = pm.button(label='Go')

        self.attachForm(self.column, 'left', 2)
        self.attachForm(self.column, 'right', 2)
        self.attachForm(self.column, 'top', 2)
        self.attachControl(self.column, 'bottom', 2, cancel_but)

        self.attachNone(cancel_but, 'top')
        self.attachForm(cancel_but, 'left', 2)
        self.attachPosition(cancel_but, 'right', 2, 50)
        self.attachForm(cancel_but, 'bottom', 2)

        self.attachNone(go_but, 'top')
        self.attachForm(go_but, 'right', 2)
        self.attachPosition(go_but, 'left', 2, 50)
        self.attachForm(go_but, 'bottom', 2)



    def on_duplicate_into_gaps(self):
        curves = pm.ls(
            selection=True,
            dag=True,
            leaf=True,
            type="nurbsCurve",
            ni=True,
            ut=True)
        cutl.duplicate_into_gaps(curves)


    def on_arrange_rings_gap(self):
        curves = pm.ls(
            selection=True,
            dag=True,
            leaf=True,
            type="nurbsCurve",
            ni=True,
            ut=True)

        # painting_node = pm.PyNode("mainPaintingShape")
        gap = pm.floatSliderButtonGrp(self.gap_ff, query=True, v=True)
        cutl.arrange_rings_gap(curves, gap)

    def on_arrange_rings_spine(self):
        curves = pm.ls(
            selection=True,
            dag=True,
            leaf=True,
            type="nurbsCurve",
            ni=True,
            ut=True)
        # painting_node = pm.PyNode("mainPaintingShape")
        dist = pm.floatSliderButtonGrp(self.spine_ff, query=True, v=True)
        cutl.arrange_rings_spine(curves, dist)

    def on_random_paints(self):
        anim = pm.checkBox(self.random_paints_anim_cb, q=True, value=True)
        spec = pm.textFieldButtonGrp(self.random_paints_tf, query=True, text=True)
        curves = pm.ls(
            selection=True,
            dag=True,
            leaf=True,
            type="nurbsCurve",
            ni=True,
            ut=True)
        if (anim):
            min_frame = int(pm.playbackOptions(min=True, query=True))
            max_frame = int(pm.playbackOptions(max=True, query=True))
            for f in range(min_frame, max_frame + 1):
                pm.currentTime(f)
                cutl.assign_random_paints(curves, spec, True)
        else:
            cutl.assign_random_paints(curves, spec, False)

    def on_random_brushes(self):
        anim = pm.checkBox(self.random_brushes_tf, q=True, value=True)
        spec = pm.textFieldButtonGrp(self.random_brushes_tf, query=True, text=True)
        curves = pm.ls(
            selection=True,
            dag=True,
            leaf=True,
            type="nurbsCurve",
            ni=True,
            ut=True,
            v=True)
        if (anim):
            min_frame = int(pm.playbackOptions(min=True, query=True))
            max_frame = int(pm.playbackOptions(max=True, query=True))
            for f in range(min_frame, max_frame + 1):
                pm.currentTime(f)
                cutl.assign_random_brushes(curves, spec, True)
        else:
            cutl.assign_random_brushes(curves, spec, False)

    def on_random_sequence(self):
        paint_spec = pm.textFieldButtonGrp(self.random_paints_tf, query=True, text=True)
        brush_spec = pm.textFieldButtonGrp(self.random_brushes_tf, query=True, text=True)
        start_frame = pm.currentTime(query=True)
        painting_node = pm.PyNode("mainPaintingShape")
        min_frame = int(pm.playbackOptions(min=True, query=True))
        max_frame = int(pm.playbackOptions(max=True, query=True))
        gap = pm.floatSliderButtonGrp(self.gap_ff, query=True, v=True)
        for f in range(min_frame, max_frame + 1):
            print "Randomizing frame: %s" % f
            pm.currentTime(f)
            cutl.assign_random_paints(painting_node, paint_spec, True)
            cutl.assign_random_brushes(painting_node, brush_spec, True)
            cutl.arrange_rings(painting_node, gap, True)
        pm.currentTime(start_frame)

    def on_random_rotations(self):
        curves = pm.ls(
            selection=True,
            dag=True,
            leaf=True,
            type="nurbsCurve",
            ni=True,
            ut=True,
            v=True)
        cutl.randomize_ring_rotation(curves)

    def on_auto_stroke_length(self):
        min_strokes = pm.intSliderGrp(
            self.min_strokes_ctl, q=True, v=True)
        max_length = pm.floatSliderGrp(
            self.max_length_ctl, q=True, v=True)
        overlap = pm.floatSliderGrp(
            self.overlap_ctl, q=True, v=True)
        # lift_length = pm.floatSliderGrppm.floatSliderGrp(
        #     self.lift_length_ctl, q=True, v=True)
        curves = pm.ls(
            selection=True,
            dag=True,
            leaf=True,
            type="nurbsCurve",
            ni=True,
            ut=True,
            v=True)
        if not curves:
            raise pm.MayaNodeError("Must select curves")
        cutl.auto_set_rings(curves, min_strokes, max_length, overlap)
